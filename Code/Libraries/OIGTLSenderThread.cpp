/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#include "OIGTLSenderThread.h"
#include "igtlOSUtil.h"

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
  if (socket.IsNull() || (socket.IsNotNull() && !socket->IsValid()) )
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

  if (m_initialized == true)
  {
    QLOG_ERROR() <<objectName() <<": " << "Sender already in use!" << endl;
    return false;
  }
  
  // Try to create a new client socket and if it is successful, set its parameters.
  m_clientSocket = igtl::ClientSocket::New();
  if (m_clientSocket.IsNotNull())
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
  if (m_initialized == false)
    return;

  if (!m_sendingOnSocket && !m_running)
    QLOG_INFO() <<objectName() <<": " <<"Started sender at: " <<m_port <<"\n";
  else if (!m_running)
    QLOG_INFO() <<objectName() <<": " <<"Started sender on socket" <<"\n";

  m_running = true;
  this->start();
}

void OIGTLSenderThread::stopThread(void)
{
  // Quitting thread	
  QLOG_INFO() <<objectName() <<": " << "Quitting sender thread \n";

  m_running = false;
  this->msleep(100);

  if (!m_sendingOnSocket)
  {
    int err = 0;
    QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

    m_mutex->lock();
    if (m_extSocket.IsNotNull())
    {
      err |= m_extSocket->CloseSocket();
      
      m_extSocket.operator =(NULL);
    }
    m_mutex->unlock();

    m_mutex->lock();
    if (m_clientSocket.IsNotNull())
    {
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

  if ( (!m_sendingOnSocket && m_clientSocket.IsNull()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, client socket is invalid" <<endl;
    return false;
  }

  if ( (m_sendingOnSocket && m_extSocket.IsNull()) || (m_sendingOnSocket && m_extSocket.IsNotNull() && !m_extSocket->IsValid()) )
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
  //Try to connect to remote
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
      // Perform a non-blocking connect with timeout
      m_extSocket.operator =((igtl::Socket::Pointer) m_clientSocket);
      m_extSocket->SetTimeout(m_socketTimeout);
      m_sendingOnSocket = false;
      emit connectedToRemote();
    }
  }

  // Start processing the message queue
  while (m_running == true)
  {
    if (m_sendQue.isEmpty())
    {    
      this->msleep(250);

      bool rval;
      m_mutex->lock();
      //QLOG_INFO() <<objectName() <<": " <<"Sending keepalive message" <<"\n";
      rval = m_extSocket->Poke();
      m_mutex->unlock();

      if (!rval && m_running == true)
      {
        QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: Disconnected from remote host" <<"\n";

        if (m_sendingOnSocket)
          emit disconnectedFromRemote(false);
        else
          emit disconnectedFromRemote(true);

        break;
      }
      this->msleep(250);
      continue;
    }

    //QLOG_INFO() <<objectName() <<": Messages in sendque: " << m_sendQue.count();

    // Take the oldest message in the queue and send it to the remote host
    m_queueMutex.lock();
    OIGTLMessage::Pointer msg;
    msg.operator =(m_sendQue.takeFirst());

    m_queueMutex.unlock();

    if (msg.operator!=(NULL))
    {
      igtl::MessageBase::Pointer igtMsg;
      msg->getMessagePointer(igtMsg);

      if (!m_running) break;

      if (igtMsg.IsNotNull())
      {
        int ret = 0;
        
        m_mutex->lock();
        ret = m_extSocket->Send(igtMsg->GetPackPointer(), igtMsg->GetPackSize());
        m_mutex->unlock();

        if (ret <=0)
        {
          if (m_sendingOnSocket)
            emit disconnectedFromRemote(false);
          else
            emit disconnectedFromRemote(true);

          QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: Disconnected from remote host" <<"\n";
          break;
        }

        //QLOG_INFO() <<objectName() <<": " <<"Message Sent" <<endl;
      }
      else
        QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: igtMsg is NULL" <<"\n";
    }
    else
      QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: invalid message" <<"\n";
    this->msleep(50);
  }

  // All messages were sent
  if (m_sendQue.isEmpty())
    emit sendingFinished();

  // Stopping the thread execution here, will restart when new messages arrive
  m_running = false;
  exit();

}

void OIGTLSenderThread::sendMsg(OIGTLMessage::Pointer msg)
{
  if (msg.operator ==(NULL))
  {
    QLOG_ERROR() <<objectName() <<": " <<"Invalid message arrived to send" <<"\n";
    return;
  }

  m_queueMutex.lock();
  m_sendQue.append(msg);
  m_queueMutex.unlock();

  // Start the thread. If already started then this does nothing.
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
