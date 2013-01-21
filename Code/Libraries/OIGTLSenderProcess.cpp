/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLSenderProcess.h"
#include "igtlOSUtil.h"

//-----------------------------------------------------------------------------
OIGTLSenderProcess::OIGTLSenderProcess(QObject *parent)
: OIGTLProcessBase(parent)
{
  m_sendingOnSocket = false;
  m_connectTimeout = 5;
  m_KeepAliveTimeout = 500;
  m_hostname.clear();
}


//-----------------------------------------------------------------------------
OIGTLSenderProcess::~OIGTLSenderProcess(void)
{
  m_extSocket.operator =(NULL);
  m_clientSocket.operator =(NULL);
}


//-----------------------------------------------------------------------------
bool OIGTLSenderProcess::initialize(igtl::Socket::Pointer socket, int port)
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


//-----------------------------------------------------------------------------
bool OIGTLSenderProcess::initialize(std::string &hostname, int port)
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


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::startProcess(void)
{
  if (m_initialized == false)
    return;

  if (!m_sendingOnSocket && !m_running)
    QLOG_INFO() <<objectName() <<": " <<"Started sender at: " <<m_port <<"\n";
  else if (!m_running)
    QLOG_INFO() <<objectName() <<": " <<"Started sender on socket" <<"\n";

  m_running = true;  //sets the process to running state
  m_active  = true;  //sets the indicator to active

  // Trigger the execution of the main processing loop
  connect(this, SIGNAL(startWorking()), this, SLOT(doProcessing()));
  emit startWorking();
  QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::stopProcess(void)
{
  QLOG_INFO() <<objectName() <<": " << "Attempting to stop sender process... (stopProcess()) \n";
  m_running = false;  //stops the process
}


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::terminateProcess()
{
  m_running = false; //Just in case...

  // Quitting Process	
  QLOG_INFO() <<objectName() <<": " << "Terminating sender process \n";
  QLOG_INFO() <<objectName() <<": " << "Total number of messages sent: " <<m_messageCounter;

  int err = 0;
  QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

  m_mutex->lock();
  if (m_extSocket.IsNotNull())
  {
    err |= m_extSocket->CloseSocket();
    
    m_extSocket.operator =(NULL);
  }
  m_mutex->unlock();

  if (!m_sendingOnSocket)
  {
    m_mutex->lock();
    if (m_clientSocket.IsNotNull())
    {
      err |= m_clientSocket->CloseSocket();

      m_clientSocket.operator =(NULL);
    }
    m_mutex->unlock();
  }

  if (err != 0)
    QLOG_ERROR() <<objectName() <<"CloseSocket returned with error: " <<err;
  else
    QLOG_INFO() <<objectName() <<"CloseSocket returned with error: " <<err;

  m_sendingOnSocket = false;
  m_sendQue.clear();

  disconnect(this, SIGNAL(startWorking()), this, SLOT(doProcessing()));

  m_port = -1;
  m_hostname.clear();
  m_initialized = false; 
  //QLOG_DEBUG() <<"!!!!  m_initialized = false; !!!! ";

  m_active = false;    //sets the active flag to false as the process has fully stopped

  emit shutdownHostThread();
}


//-----------------------------------------------------------------------------
bool OIGTLSenderProcess::activate(void)
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

  if (!m_sendingOnSocket && resolveHostName(QString::fromStdString(m_hostname)) == QString("UNKNOWN") )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, hostname or ip address is invalid" <<endl;
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


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::keepaliveTimeout(void)
{
  int sleepInterval = m_KeepAliveTimeout/2;

  QLOG_INFO() << objectName() <<": " << "keepaliveTimeout occured: " << m_KeepAliveTimeout << ", ms" ;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // If the sendqueue is empty we're going to perform a keepalive - send 2 bytes through the socket
  if (m_sendQue.isEmpty())
  {
    QThreadEx * p = NULL;
    try
    {
      p = dynamic_cast<QThreadEx *>(QThread::currentThread());

      QLOG_DEBUG() << objectName() << "Before keep alive check: Send queue to host " << QString::fromStdString(m_hostname) << ", port " << m_port << ", is empty, so sleeping for " << sleepInterval << " ms\n";
      p->msleepEx(sleepInterval);
    }
    catch (std::exception &e)
    {
      qDebug() <<"Type cast error. Always run this process from QThreadEx. Exception: " <<e.what();
    }


    bool rval;
    m_mutex->lock();
    rval = m_extSocket->Poke();
    m_mutex->unlock();

    // If the poke() was unsuccessful or the sender got terminated
    if (!rval && m_running == true)
    {
      QLOG_ERROR() << objectName() <<": " <<"Cannot send keep-alive message: Probably disconnected from remote host" <<"\n";

      if (m_sendingOnSocket)
        emit disconnectedFromRemote(false);
      else
        emit disconnectedFromRemote(true);

      QCoreApplication::processEvents();
      return;
    }

    QCoreApplication::processEvents();

    if (p!=NULL)
    {
      QLOG_DEBUG() << objectName() <<": " <<"After keep alive check: Send queue to host " << QString::fromStdString(m_hostname) << ", port " << m_port << ", is empty, so sleeping for " << sleepInterval << " ms\n";
      p->msleepEx(sleepInterval);
    }
  }
}


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::doProcessing(void)
{
  int sleepInterval = 25; // milliseconds. i.e. 50 fps.

  if (m_timeouter != NULL)
  {
    delete m_timeouter;
  }
  m_timeouter = new QTimer(this);
  m_timeouter->setInterval(m_KeepAliveTimeout);
  connect(m_timeouter, SIGNAL(timeout()), this, SLOT(keepaliveTimeout()));

  // Try to connect to remote
  if (!m_sendingOnSocket && m_extSocket.IsNull())
  {
    int r = m_clientSocket->ConnectToServer(m_hostname.c_str(), m_port);
    if (r != 0)
    {
      QLOG_ERROR() << objectName() <<": " << "Cannot create a sender socket, could not connect to server: " <<m_hostname.c_str() << endl;

      emit cannotConnectToRemote();
      QCoreApplication::processEvents();

      m_running = false;
      terminateProcess();
    }
    else
    {
      // Perform a non-blocking connect with timeout
      m_extSocket.operator =((igtl::Socket::Pointer) m_clientSocket);
      m_extSocket->SetTimeout(m_socketTimeout);
      m_sendingOnSocket = false;

      m_timeouter->start();

      emit connectedToRemote();
      QCoreApplication::processEvents();
    }
  }

  // Start processing the message queue
  while (m_running == true)
  {
    if (m_sendQue.isEmpty())
    { 
      QThreadEx * p = NULL;
      try
      {
        QLOG_DEBUG() << objectName() <<": " << "Send queue to host " << QString::fromStdString(m_hostname) << ", port " << m_port << ", is empty, so sleeping for " << sleepInterval << " ms\n";
        p = dynamic_cast<QThreadEx *>(QThread::currentThread());
        p->msleepEx(sleepInterval);
      }
      catch (std::exception &e)
      {
        qDebug() <<"Type cast error. Always run this process from QThreadEx. Exception: " <<e.what();
      }

      QCoreApplication::processEvents();

    }
    else
    {
      // Take the oldest message in the queue and send it to the remote host
      OIGTLMessage::Pointer msg;

      m_queueMutex.lock();
      msg.operator =(m_sendQue.takeFirst());
      m_queueMutex.unlock();

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

          QCoreApplication::processEvents();

          msg.reset();
          QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: Disconnected from remote host" <<"\n";
          break;
        }

        igtlUint32 seconds;
        igtlUint32 nanoseconds;
        m_extSocket->GetSendTimestamp()->GetTimeStamp(&seconds, &nanoseconds);

        igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
        ts->GetTime_TAI();
        ts->SetTime(seconds, nanoseconds);
        ts->toUTC();

        QLOG_INFO() <<objectName() <<": " <<"Message " << m_messageCounter << ", created=" << GetTimeInNanoSeconds(msg->getTimeCreated()) << ", sent=" << GetTimeInNanoSeconds(ts) << ", lag=" << ((double)(GetTimeInNanoSeconds(ts)) - (double)(GetTimeInNanoSeconds(msg->getTimeCreated())))/1000000000.0 << "(secs)";

        emit messageSent(GetTimeInNanoSeconds(ts));
        m_messageCounter++;

        // Reset the timer.
        m_timeouter->start(m_KeepAliveTimeout);
      }
      else
      {
        QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: igtMsg is NULL" <<"\n";
      }

      // Force delete
      msg.reset();
      
      QCoreApplication::processEvents();
    }
    
  }  //end of while

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // All messages were sent
  if (m_sendQue.isEmpty())
    emit sendingFinished();

  QCoreApplication::processEvents();

  // Stopping the Process execution here, will restart when new messages arrive
  m_running = false;
  terminateProcess();
}


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::sendMsg(OIGTLMessage::Pointer msg)
{
  // Append message to the sendqueue
  m_queueMutex.lock();
  m_sendQue.append(msg);
  m_queueMutex.unlock();
}


//-----------------------------------------------------------------------------
void OIGTLSenderProcess::setConnectTimeOut(int msec) 
{ 
  m_connectTimeout = msec; 
}


//-----------------------------------------------------------------------------
int OIGTLSenderProcess::getConnectTimeOut(void)     
{ 
  return m_connectTimeout; 
}
