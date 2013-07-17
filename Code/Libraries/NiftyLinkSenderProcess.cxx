/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkSenderProcess.h"
#include "igtlOSUtil.h"

//-----------------------------------------------------------------------------
NiftyLinkSenderProcess::NiftyLinkSenderProcess(QObject *parent)
  : NiftyLinkProcessBase(parent)
{
  m_SendingOnSocket = false;
  m_ConnectTimeout = 5;
  m_KeepAliveTimeout = 500;
  m_Hostname.clear();
}


//-----------------------------------------------------------------------------
NiftyLinkSenderProcess::~NiftyLinkSenderProcess(void)
{
  m_ExtSocket.operator = (NULL);
  m_ClientSocket.operator = (NULL);

  if (m_TimeOuter != NULL)
  {
    delete m_TimeOuter;
    m_TimeOuter = NULL;
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkSenderProcess::Initialize(igtl::Socket::Pointer socket, int port)
{
  if (socket.IsNull() || (socket.IsNotNull() && !socket->IsValid()) )
  {
    QLOG_ERROR() << objectName() << ": " << "Cannot create a sender socket, invalid external socket specified" << endl;
    return false;
  }

  if (m_Initialized == true)
  {
    QLOG_ERROR() << objectName() << ": " << "Sender already in use!" << endl;
    return false;
  }

  m_ExtSocket.operator = (socket);
  m_ExtSocket->SetTimeout(m_SocketTimeout);
  m_SendingOnSocket = true;
  m_Port = port;

  if (!Activate())
  {
    return false;
  }

  return true;
}


//-----------------------------------------------------------------------------
bool NiftyLinkSenderProcess::Initialize(std::string &hostname, int port)
{
  if (port <= 0 || hostname.empty())
  {
    QLOG_ERROR() << objectName() << ": " << "Cannot create a sender socket, invalid hostname or port specified" << endl;
    QLOG_ERROR() << objectName() << ": " << "Hostname: " << hostname.c_str() << endl;
    QLOG_ERROR() << objectName() << ": " << "Port: " << port << endl;
    return false;
  }

  if (m_Initialized == true)
  {
    QLOG_ERROR() << objectName() << ": " << "Sender already in use!" << endl;
    return false;
  }

  // Try to create a new client socket and if it is successful, set its parameters.
  m_ClientSocket = igtl::ClientSocket::New();
  if (m_ClientSocket.IsNotNull())
  {
    m_ClientSocket->SetTimeout(m_SocketTimeout);
    m_ClientSocket->SetConnectionTimeout(m_ConnectTimeout);
    m_SendingOnSocket = false;
    m_Port = port;
    m_Hostname.operator = (hostname);
  }
  else
  {
    QLOG_ERROR() << objectName() << ": " << "Failed to create socket" << endl;
    m_ClientSocket.operator = (NULL);
    m_Port = -1;
    m_Hostname.clear();
    return false;
  }

  if (!Activate())
  {
    return false;
  }

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::StartProcess(void)
{
  if (m_Initialized == false)
  {
    return;
  }

  if (!m_SendingOnSocket && !m_Running)
  {
    QLOG_INFO() << objectName() << ": " << "Started sender at: " << m_Port << "\n";
  }
  else if (!m_Running)
  {
    QLOG_INFO() << objectName() << ": " << "Started sender on socket" << "\n";
  }

  m_Running = true;  //sets the process to running state
  m_Active  = true;  //sets the indicator to active

  // Trigger the execution of the main processing loop
  connect(this, SIGNAL(StartWorkingSignal()), this, SLOT(DoProcessing()));
  emit StartWorkingSignal();
  QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::StopProcess(void)
{
  QLOG_INFO() << objectName() << ": " << "Attempting to stop sender process... (stopProcess()) \n";
  m_Running = false;  //stops the process
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::TerminateProcess()
{
  m_Running = false; //Just in case...

  // Quitting Process
  QLOG_INFO() << objectName() << ": " << "Terminating sender process \n";
  QLOG_INFO() << objectName() << ": " << "Total number of messages sent: " << m_MessageCounter;

  int err = 0;
  QLOG_INFO() << objectName() << ": " << "Closing socket... \n";

  m_Mutex->lock();
  if (m_ExtSocket.IsNotNull())
  {
    err |= m_ExtSocket->CloseSocket();

    m_ExtSocket.operator = (NULL);
  }
  m_Mutex->unlock();

  if (!m_SendingOnSocket)
  {
    m_Mutex->lock();
    if (m_ClientSocket.IsNotNull())
    {
      err |= m_ClientSocket->CloseSocket();

      m_ClientSocket.operator = (NULL);
    }
    m_Mutex->unlock();
  }

  if (err != 0)
  {
    QLOG_ERROR() << objectName() << "CloseSocket returned with error: " << err;
  }
  else
  {
    QLOG_INFO() << objectName() << "CloseSocket returned with error: " << err;
  }

  m_SendingOnSocket = false;
  m_SendQue.clear();

  disconnect(this, SIGNAL(StartWorkingSignal()), this, SLOT(DoProcessing()));

  m_Port = -1;
  m_Hostname.clear();
  m_Initialized = false;
  //QLOG_DEBUG() <<"!!!!  m_Initialized = false; !!!! ";

  m_Active = false;    //sets the active flag to false as the process has fully stopped

  emit ShutdownHostThreadSignal();
}


//-----------------------------------------------------------------------------
bool NiftyLinkSenderProcess::Activate(void)
{
  if (m_Mutex == NULL)
  {
    QLOG_INFO() << objectName() << ": " << "Cannot Activate sender, mutex not set" << endl;
    return false;
  }

  if ( (!m_SendingOnSocket && m_Port <= 0) || (!m_SendingOnSocket && m_Hostname.empty()) )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot Activate sender, hostname or port is invalid" << endl;
    return false;
  }

  if (!m_SendingOnSocket && ResolveHostName(QString::fromStdString(m_Hostname)) == QString("UNKNOWN") )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot Activate sender, hostname or ip address is invalid" << endl;
    return false;
  }

  if ( (!m_SendingOnSocket && m_ClientSocket.IsNull()) )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot Activate sender, client socket is invalid" << endl;
    return false;
  }

  if ( (m_SendingOnSocket && m_ExtSocket.IsNull()) || (m_SendingOnSocket && m_ExtSocket.IsNotNull() && !m_ExtSocket->IsValid()) )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot Activate listener, socket is invalid" << endl;
    return false;
  }

  m_Initialized = true;

  QLOG_INFO() << objectName() << ": " << "Sender successfully Activated" << endl;

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::OnKeepAliveTimeout(void)
{
  int sleepInterval = m_KeepAliveTimeout / 2;

  QLOG_INFO() << objectName() << ": " << "KeepAliveTimeout occured: " << m_KeepAliveTimeout << ", ms";

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // If the sendqueue is empty we're going to perform a keepalive - send 2 bytes through the socket
  if (m_SendQue.isEmpty())
  {
    QThreadEx * p = NULL;
    try
    {
      p = dynamic_cast<QThreadEx *>(QThread::currentThread());

//      QLOG_DEBUG() << objectName() << "Before keep alive check: Send queue to host "
//                   << QString::fromStdString(m_Hostname) << ", port " << m_Port << ", is empty, so sleeping for "
//                   << sleepInterval << " ms\n";

      p->MsleepEx(sleepInterval);
    }
    catch (std::exception &e)
    {
      qDebug() << "Type cast error. Always run this process from QThreadEx. Exception: " << e.what();
    }

    bool rval;
    m_Mutex->lock();
    rval = m_ExtSocket->Poke();
    m_Mutex->unlock();

    // If the poke() was unsuccessful or the sender got terminated
    if (!rval && m_Running == true)
    {
      QLOG_ERROR() << objectName() << ": " << "Cannot send keep-alive message: Probably disconnected from remote host" << "\n";

      if (m_SendingOnSocket)
      {
        emit DisconnectedFromRemoteSignal(false);
      }
      else
      {
        emit DisconnectedFromRemoteSignal(true);
      }

      QCoreApplication::processEvents();
      return;
    }

    QCoreApplication::processEvents();

    if (p != NULL)
    {
//      QLOG_DEBUG() << objectName() <<": " <<"After keep alive check: Send queue to host "
//                   << QString::fromStdString(m_Hostname) << ", port " << m_Port << ", is empty, so sleeping for "
//                   << sleepInterval << " ms\n";

      p->MsleepEx(sleepInterval);
    }
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::DoProcessing(void)
{
  //int sleepInterval = 1; // milliseconds. i.e. 50 fps.

  if (m_TimeOuter != NULL)
  {
    disconnect(m_TimeOuter, SIGNAL(timeout()), this, SLOT(OnKeepAliveTimeout()));
    delete m_TimeOuter;
  }

  m_TimeOuter = new QTimer(this);
  m_TimeOuter->setInterval(m_KeepAliveTimeout);
  connect(m_TimeOuter, SIGNAL(timeout()), this, SLOT(OnKeepAliveTimeout()));

  // Try to connect to remote
  if (!m_SendingOnSocket && m_ExtSocket.IsNull())
  {
    int r = m_ClientSocket->ConnectToServer(m_Hostname.c_str(), m_Port);
    if (r != 0)
    {
      QLOG_ERROR() << objectName() << ": " << "Cannot create a sender socket, could not connect to server: " << m_Hostname.c_str() << endl;

      emit CannotConnectToRemoteSignal();
      QCoreApplication::processEvents();

      m_Running = false;
      TerminateProcess();
    }
    else
    {
      // Perform a non-blocking connect with timeout
      m_ExtSocket.operator = ((igtl::Socket::Pointer) m_ClientSocket);
      m_ExtSocket->SetTimeout(m_SocketTimeout);
      m_SendingOnSocket = false;

      m_TimeOuter->start();

      emit ConnectedToRemoteSignal();
      QCoreApplication::processEvents();
    }
  }

  // Start processing the message queue
  while (m_Running == true)
  {
    // Need to make sure that we started the timer when sending on Socket
    if (!m_TimeOuter->isActive())
    {
      m_TimeOuter->start();
    }

    // Check the queue status and act accordingly
    if (m_SendQue.isEmpty())
    {
      /*QThreadEx * p = NULL;
      try
      {
        QLOG_DEBUG() << objectName() <<": " << "Send queue to host "
                     << QString::fromStdString(m_Hostname) << ", port " << m_Port << ", is empty, so sleeping for "
                     << sleepInterval << " ms\n";

        p = dynamic_cast<QThreadEx *>(QThread::currentThread());
        //p->MsleepEx(sleepInterval);
      }
      catch (std::exception &e)
      {
        qDebug() <<"Type cast error. Always run this process from QThreadEx. Exception: " <<e.what();
      }*/

      QCoreApplication::processEvents();

    }
    else
    {
      // Take the oldest message in the queue and send it to the remote host
      NiftyLinkMessage::Pointer msg;

      m_QueueMutex.lock();
      msg.operator = (m_SendQue.takeFirst());
      m_QueueMutex.unlock();

      igtl::MessageBase::Pointer igtMsg;
      msg->GetMessagePointer(igtMsg);

      if (!m_Running)
      {
        break;
      }

      if (igtMsg.IsNotNull())
      {
        int ret = 0;

        // This is to mark when the message leaves the socket
        igtl::TimeStamp::Pointer sendStarted = igtl::TimeStamp::New();

        m_Mutex->lock();
        sendStarted->Update();
        msg->TouchMessage("3. sendStarted", sendStarted);

        // Update the timestamp within the message itself
        //igtMsg->SetTimeStamp(sendStarted);

        ret = m_ExtSocket->Send(igtMsg->GetPackPointer(), igtMsg->GetPackSize());
        m_Mutex->unlock();

        if (ret <= 0)
        {
          if (m_SendingOnSocket)
          {
            emit DisconnectedFromRemoteSignal(false);
          }
          else
          {
            emit DisconnectedFromRemoteSignal(true);
          }

          QCoreApplication::processEvents();

          msg.reset();
          QLOG_ERROR() << objectName() << ": " << "Cannot send message: Disconnected from remote host" << "\n";
          break;
        }

        // Get the time when the message was fully transmitted
		    igtl::TimeStamp::Pointer sendFinished = m_ExtSocket->GetSendTimestamp();
        msg->TouchMessage("4. sendFinished", sendFinished);

        // Emit the time
        emit MessageSentSignal(sendFinished->GetTimeInNanoSeconds());

        // Send the full list of timestamps when the message was touched
        emit SendMessageAccessTimes(msg->GetAccessTimes());
        
        // Internal message counter
        m_MessageCounter++;

        // Reset the timer - no need for keepalive as we successfully sent a message.
        m_TimeOuter->start(m_KeepAliveTimeout);
      }
      else
      {
        QLOG_ERROR() << objectName() << ": " << "Cannot send message: igtMsg is NULL" << "\n";
      }

      // Force delete
      msg.reset();

      QCoreApplication::processEvents();
    }

  }  //end of while

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // All messages were sent
  if (m_SendQue.isEmpty())
  {
    emit SendingFinishedSignal();
  }

  QCoreApplication::processEvents();

  m_Running = false;
  m_TimeOuter->stop();
  disconnect(m_TimeOuter, SIGNAL(timeout()), this, SLOT(OnKeepAliveTimeout()));
  delete m_TimeOuter;
  m_TimeOuter = NULL;

  TerminateProcess();
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::AddMsgToSendQueue(NiftyLinkMessage::Pointer msg)
{
  // Append message to the sendqueue
  m_QueueMutex.lock();
  m_SendQue.append(msg);
  m_QueueMutex.unlock();
}


//-----------------------------------------------------------------------------
void NiftyLinkSenderProcess::SetConnectTimeOut(int msec)
{
  m_ConnectTimeout = msec;
}


//-----------------------------------------------------------------------------
int NiftyLinkSenderProcess::GetConnectTimeOut(void)
{
  return m_ConnectTimeout;
}
