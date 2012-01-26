#include "OIGTLSenderThread.h"

OIGTLSenderThread::OIGTLSenderThread(QObject *parent)
: OIGTLThreadBase(parent)
{
  m_sendingOnSocket = false;
  m_connectTimeout = 5;
  m_hostname.clear();
}

OIGTLSenderThread::~OIGTLSenderThread(void)
{
  m_extSocket.operator =(NULL);
  m_clientSocket.operator =(NULL);
}

bool OIGTLSenderThread::initialize(igtl::Socket::Pointer socket, int port)
{
  if (socket.IsNull() || !socket->IsValid())
  {
    QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, invalid external socket specified" << endl;
    return false;
  }

  if (m_initialized == true)
  {
    QLOG_ERROR() <<objectName() <<": " << "Sender already in use!" << endl;
    return false;
  }

  m_extSocket.operator =(socket);
  m_extSocket->SetTimeout(m_socketTimeout);
  m_sendingOnSocket = true;
  m_port = port;

  if (!activate())
    return false;

  return true;
}


bool OIGTLSenderThread::initialize(std::string &hostname, int port)
{
  if (port <= 0 || hostname.empty())
  {
    QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, invalid hostname or port specified" << endl;
    QLOG_ERROR() <<objectName() <<": " << "Hostname: " <<hostname.c_str() << endl;
    QLOG_ERROR() <<objectName() <<": " << "Port: " <<port << endl;
    return false;
  }

  //QLOG_ERROR() <<objectName() <<": " << "Hostname: " <<hostname << endl;
  //QLOG_ERROR() <<objectName() <<": " << "Port: " <<port << endl;

  if (m_initialized == true)
  {
    QLOG_ERROR() <<objectName() <<": " << "Sender already in use!" << endl;
    return false;
  }

  m_clientSocket = igtl::ClientSocket::New();
  if (m_clientSocket.IsNotNull())// && m_clientSocket->IsValid())
  {
    m_clientSocket->SetTimeout(m_socketTimeout);
    m_clientSocket->SetConnectionTimeout(m_connectTimeout);
    m_sendingOnSocket = false;
    m_port = port;
    m_hostname.operator =(hostname);
  }
  else
  {
    QLOG_ERROR() <<objectName() <<": " << "Failed to create socket" << endl;
    m_clientSocket.operator =(NULL);
    m_port = -1;
    m_hostname.clear();
    return false;
  }

  if (!activate())
    return false;

  return true;
}

void OIGTLSenderThread::startThread(void)
{
  if (m_initialized == false) // || m_running == true)
    return;

  m_running = true;
  this->start();

  if (!m_sendingOnSocket)
    QLOG_INFO() <<objectName() <<": " <<"Started sender at: " <<m_port <<"\n";
  else
    QLOG_INFO() <<objectName() <<": " <<"Started sender on socket" <<"\n";
}

void OIGTLSenderThread::stopThread(void)
{
  // Quitting thread	
  QLOG_INFO() <<objectName() <<": " << "Quitting sender thread \n";

  m_running = false;

  if (!m_sendingOnSocket)
  {
    int err = 0;
    QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

    m_mutex->lock();
    if (m_extSocket.IsNotNull())
    {
      if (m_extSocket->IsValid())
        err |= m_extSocket->CloseSocket();
      
      m_extSocket.operator =(NULL);
    }
    m_mutex->unlock();

    m_mutex->lock();
    if (m_clientSocket.IsNotNull())
    {
      if (m_clientSocket->IsValid())
        err |= m_clientSocket->CloseSocket();

      m_clientSocket.operator =(NULL);
    }
    m_mutex->unlock();

    if (err != 0)
      QLOG_ERROR() <<objectName() <<"CloseSocket returned with error: " <<err;
    else
      QLOG_INFO() <<objectName() <<"CloseSocket returned with error: " <<err;
  }

  m_sendingOnSocket = false;
  m_sendQue.clear();

  m_port = -1;
  m_hostname.clear();
  m_initialized = false;

  exit(0);
}

bool OIGTLSenderThread::activate(void)
{
  if (m_mutex == NULL)
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, mutex not set" <<endl;
    return false;
  }

  if ( (!m_sendingOnSocket && m_port <= 0) || (!m_sendingOnSocket && m_hostname.empty()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, hostname or port is invalid" <<endl;
    return false;
  }

  if ( (!m_sendingOnSocket && m_clientSocket.IsNull()) )// || (!m_sendingOnSocket && !m_clientSocket->IsValid()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, client socket is invalid" <<endl;
    return false;
  }

  if ( (m_sendingOnSocket && m_extSocket.IsNull()) || (m_sendingOnSocket && !m_extSocket->IsValid()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, socket is invalid" <<endl;
    return false;
  }

  m_initialized = true;

  QLOG_INFO() <<objectName() <<": " <<"Sender successfully activated" <<endl;

  return true;
}

void OIGTLSenderThread::run(void)
{
  if (!m_sendingOnSocket && m_extSocket.IsNull())
  {
    int r = m_clientSocket->ConnectToServer(m_hostname.c_str(), m_port);

    if (r != 0)
    {
      QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, could not connect to server: " <<m_hostname.c_str() << endl;

      emit cannotConnectToRemote();

      stopThread();
    }
    else
    {
      m_extSocket.operator =((igtl::Socket::Pointer) m_clientSocket);
      m_extSocket->SetTimeout(m_socketTimeout);
      m_sendingOnSocket = false;

      emit connectedToRemote();
    }
  }

  while (m_running == true)
  {
    if (m_extSocket.IsNull() || !m_extSocket->IsValid())
    {
      QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: Disconnected from remote host" <<"\n";
      
      if (m_sendingOnSocket)
        emit disconnectedFromRemote(false);
      else
        emit disconnectedFromRemote(true);
      
      break;
    }

    if (m_sendQue.isEmpty())
      continue;

    QLOG_INFO() <<objectName() <<": Messages in sendque: " << m_sendQue.count();

    m_queueMutex.lock();
    OIGTLMessage::Pointer msg;
    msg.operator =(m_sendQue.takeFirst());

    m_queueMutex.unlock();

    if (msg.operator!=(NULL))
    {
      igtl::MessageBase::Pointer igtMsg;
      msg->getMessagePointer(igtMsg);

      if (igtMsg.IsNotNull())
      {
        //m_mutex->tryLock();
        m_mutex->lock();
        m_extSocket->Send(igtMsg->GetPackPointer(), igtMsg->GetPackSize());
        m_mutex->unlock();

        QLOG_INFO() <<objectName() <<": " <<"Message Sent" <<endl;
      }
      else
        QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: igtMsg is NULL" <<"\n";

      //*******************************
      // CHANGE THIS

      //delete msg;
      //*******************************
    }
    else
      QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: invalid message" <<"\n";

    //igtl::Sleep(50); // wait
  }

  if (m_sendQue.isEmpty())
    emit sendingFinished();

  m_running = false;
  exit();

}

void OIGTLSenderThread::sendMsg(OIGTLMessage::Pointer msg)
{
  QLOG_INFO() <<objectName() <<": " <<"Got new message to send, putting it to send queue.";

  if (msg.operator !=(NULL))
    QLOG_INFO() <<"MSG_ID: " <<msg->getId() <<endl;
  else
  {
    QLOG_ERROR() <<objectName() <<": " <<"Invalid message arrived to send" <<"\n";
    return;
  }

  m_queueMutex.lock();
  m_sendQue.append(msg);
  m_queueMutex.unlock();

  //igtl::Sleep(100);

  this->startThread();
}

void OIGTLSenderThread::setConnectTimeOut(int msec) 
{ 
  m_connectTimeout = msec; 
}

int OIGTLSenderThread::getConnectTimeOut(void)     
{ 
  return m_connectTimeout; 
}
