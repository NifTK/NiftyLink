/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkListenerProcess.h"
#include <NiftyLinkQThread.h>
#include "igtlOSUtil.h"
#include <igtlPolyDataMessage.h>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkListenerProcess::NiftyLinkListenerProcess(QObject *parent)
  : NiftyLinkProcessBase(parent)
{
  m_ListeningOnPort = false;
  m_ClientConnected = false;
  m_ListenInterval = 1000;
}


//-----------------------------------------------------------------------------
NiftyLinkListenerProcess::~NiftyLinkListenerProcess(void)
{
  m_ServerSocket.operator = (NULL);
  m_ExtSocket.operator = (NULL);
}


//-----------------------------------------------------------------------------
bool NiftyLinkListenerProcess::Initialize(niftk::NiftyLinkSocket::Pointer socket, int port)
{
  if (socket.IsNull() || (socket.IsNotNull() && !socket->GetConnected()) )
  {
    QLOG_ERROR() << objectName() << ": " << "Cannot create a listener socket, invalid external socket specified" << endl;
    return false;
  }

  if (m_Initialized == true)
  {
    QLOG_ERROR() << objectName() << ": " << "Listener already in use!" << endl;
    return false;
  }

  m_ExtSocket.operator = (socket);
  m_ExtSocket->SetTimeout(m_SocketTimeout);

  m_ListeningOnPort = false;
  m_Port = port;

  if (!Activate())
  {
    return false;
  }

  return true;
}


//-----------------------------------------------------------------------------
bool NiftyLinkListenerProcess::Initialize(int port)
{
  //Set up a new instance of listener on a given port and create a new server socket

  if (port <= 0)
  {
    QLOG_ERROR() << objectName() << ": " << "Cannot create a listener socket, invalid socket or port specified" << endl;
    return false;
  }

  if (m_Initialized == true)
  {
    QLOG_ERROR() << objectName() << ": " << "Listener already in use!" << endl;
    return false;
  }

  niftk::NiftyLinkServerSocket::Pointer ssock = niftk::NiftyLinkServerSocket::New();
  int err = ssock->CreateServer(port);
  ssock->SetTimeout(m_SocketTimeout);

  if (err < 0)
  {
    QLOG_ERROR() << objectName() << ": " << "Error creating listener socket." << endl;
    m_Port = -1;
    return false;
  }

  m_Port = port;
  m_ServerSocket.operator = (ssock);
  m_ServerSocket->SetTimeout(m_SocketTimeout);
  m_ListeningOnPort = true;

  if (!Activate())
  {
    return false;
  }

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::StartProcess()
{
  if (m_Initialized == false || m_Running == true)
  {
    return;
  }

  m_Running = true; //sets the process into running state
  m_Active = true; //sets the flag

  if (m_ListeningOnPort)
  {
    QLOG_INFO() << objectName() << ": " << "Started listening at: " << m_Port << "\n";
  }
  else
  {
    QLOG_INFO() << objectName() << ": " << "Started listening on socket" << "\n";
  }

  // Trigger the execution of the main processing loop
  connect(this, SIGNAL(StartWorkingSignal()), this, SLOT(DoProcessing()));
  emit StartWorkingSignal();
  //QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::StopProcess()
{
  // Stopping Process
  QLOG_INFO() << objectName() << ": " << "Stopping listener process... \n";

  m_Running = false;

}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::TerminateProcess()
{
  int sleepInterval = 100; // milliseconds

  // Terminate the process
  m_Running = false; //Just in case...

  // Quitting Process
  QLOG_INFO() << objectName() << ": " << "Terminating the listener process... \n";
  QLOG_INFO() << objectName() << ": " << "Total number of messages recieved: " << m_MessageCounter;

  int err_p = 0;
  int err_s = 0;

  QLOG_INFO() << objectName() << ": " << "Closing socket... \n";

  m_Mutex->lock();
  if (m_ListeningOnPort && m_ExtSocket.IsNotNull())
  {
    err_p |= m_ExtSocket->CloseSocket();

    try
    {
      QLOG_DEBUG() << "Terminating NiftyLinkListenerProcess, waiting for " << sleepInterval << " ms for socket on port " << m_Port << " to close\n";
      dynamic_cast<NiftyLinkQThread *>(QThread::currentThread())->SleepCallingThread(sleepInterval);
    }
    catch (std::exception &e)
    {
      qDebug() << "Type cast error.Always run this process from QThreadEx. Exception: " << e.what();
    }


    m_ExtSocket.operator = (NULL);
  }
  m_Mutex->unlock();

  if (err_p != 0)
  {
    QLOG_ERROR() << objectName() << "CloseSocket returned with error: " << err_p;
  }
  else
  {
    QLOG_INFO() << objectName() << "CloseSocket succeded: " << err_p;
  }

  QLOG_INFO() << objectName() << ": " << "Closing socket... \n";

  m_Mutex->lock();
  if (m_ServerSocket.IsNotNull())
  {
    err_s |= m_ServerSocket->CloseSocket();

    try
    {
      QLOG_DEBUG() << "Terminating NiftyLinkListenerProcess, waiting for " << sleepInterval << " ms for server socket on port " << m_Port << " to close\n";
      dynamic_cast<NiftyLinkQThread *>(QThread::currentThread())->SleepCallingThread(sleepInterval);
    }
    catch (std::exception &e)
    {
      qDebug() << "Type cast error.Always run this process from QThreadEx. Exception: " << e.what();
    }

    m_ServerSocket.operator = (NULL);
  }
  m_Mutex->unlock();

  if (err_s != 0)
  {
    QLOG_ERROR() << objectName() << "CloseServerSocket returned with error: " << err_s;
  }
  else
  {
    QLOG_INFO() << objectName() << "CloseServerSocket succeeded: " << err_s;
  }

  disconnect(this, SIGNAL(StartWorkingSignal()), this, SLOT(DoProcessing()));

  m_ListeningOnPort = false;
  m_Port = -1;
  m_Initialized = false;
  m_Active = false;

  //exit(0);
  emit ShutdownHostThreadSignal();
}


//-----------------------------------------------------------------------------
bool NiftyLinkListenerProcess::Activate(void)
{
  if (m_Mutex == NULL)
  {
    QLOG_INFO() << objectName() << ": " << "Cannot activate listener, mutex not set" << endl;
    return false;
  }

  if (m_ListeningOnPort && m_Port <= 0)
  {
    QLOG_INFO() << objectName() << ": " << "Cannot activate listener, port is invalid" << endl;
    return false;
  }

  if ( (m_ListeningOnPort && m_ServerSocket.IsNull()) )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot activate listener, server socket is invalid" << endl;
    return false;
  }

  if ( (!m_ListeningOnPort && m_ExtSocket.IsNull()) || (!m_ListeningOnPort && m_ServerSocket.IsNotNull() && !m_ExtSocket->GetConnected()) )
  {
    QLOG_INFO() << objectName() << ": " << "Cannot activate listener, external socket is invalid" << endl;
    return false;
  }

  m_Initialized = true;

  QLOG_INFO() << objectName() << ": " << "Listener successfully activated" << endl;

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::DoProcessing(void)
{
  m_TimeOuter = new QTimer();
  connect(m_TimeOuter, SIGNAL(timeout()), this, SLOT(OnSocketTimeout()));

  //QCoreApplication::processEvents();

  if (!m_ListeningOnPort && m_Initialized)
  {
    m_ClientConnected = true;
    m_TimeOuter->start(2000);

    ListenOnSocket();
  }
  else if (m_ListeningOnPort && m_Initialized)
  {
    ListenOnPort();
  }

  m_TimeOuter->stop();
  delete m_TimeOuter;
  m_TimeOuter = NULL;

  TerminateProcess();
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::ListenOnSocket(void)
{
  //int sleepInterval = 1; // milliseconds

  while (m_Running == true && m_ClientConnected == true)
  {
    QCoreApplication::processEvents();

    m_Mutex->lock();
    int bytesPending = m_ExtSocket->CheckPendingData();
    m_Mutex->unlock();
    //QLOG_INFO() <<objectName() <<"Bytes received: " <<bytesPending;

    // Either there's nothing to read or it's a keepalive
    if (bytesPending <= 2)
    {
      // Handle keepalive - restart the timout clock
      if (bytesPending == 2)
      {
        QLOG_DEBUG() << objectName() << "Keepalive received, restarting the timeouter\n";
        m_TimeOuter->start(2000);
      }
      /*
      // Sleep for a bit to let some more data arrive to the socket
      try
      {
        QLOG_DEBUG() <<objectName() <<" listening with socket on port " << m_Port << ", but waiting for " << sleepInterval << " ms\n";
        //dynamic_cast<QThreadEx *>(QThread::currentThread())->MsleepEx(sleepInterval);
      }
      catch (std::exception &e)
      {
        qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
      }
      */
      continue;
    }

    // There's a lot of data, so let's read it out
    ReceiveMessage();
  }

}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::ListenOnPort(void)
{
  //int sleepInterval = 1; // milliseconds
  niftk::NiftyLinkSocket::Pointer socket;

  while (m_Running == true)
  {
    QCoreApplication::processEvents();

    //------------------------------------------------------------
    // If running a server socket wait for connection
    if (m_ServerSocket.IsNotNull() && m_Running == true)
    {
      m_Mutex->lock();
      socket = m_ServerSocket->WaitForConnection(m_ListenInterval);
      m_Mutex->unlock();

      if (!socket->GetConnected())
      {
        QLOG_INFO() << objectName() << ": " << "No client connecting\n";
        continue;
      }
      else
      {
        // Client connected, need to set socket parameters
        m_ExtSocket.operator = (socket);
        m_ExtSocket->SetTimeout(m_SocketTimeout);
        m_TimeOuter->start(2000);

        //qDebug() <<m_TimeOuter->isActive();

        emit ClientConnectedSignal();
        //QCoreApplication::processEvents();
        m_ClientConnected = true;
      }

      while (m_Running == true && m_ClientConnected == true) // if client connected
      {
        // time check needs signals being delivered
        QCoreApplication::processEvents();

        // Quickly check if there's any pending data on the socket
        m_Mutex->lock();
        int bytesPending = m_ExtSocket->CheckPendingData();
        m_Mutex->unlock();

        //QLOG_INFO() <<objectName() <<"Bytes received: " <<bytesPending;

        // Either there's nothing to read or it's a keepalive
        if (bytesPending <= 2)
        {
          // Handle keepalive - restart the timout clock
          if (bytesPending == 2)
          {
            QLOG_DEBUG() << objectName() << "Keepalive received, restarting the timeouter\n";
            m_TimeOuter->start(2000);
          }
          /*
          // Sleep for a bit to let some more data arrive to the socket
          try
          {
            QLOG_DEBUG() <<objectName() <<" listening with socket on port " << m_Port << ", but waiting for " << sleepInterval << " ms\n";
            //dynamic_cast<QThreadEx *>(QThread::currentThread())->MsleepEx(sleepInterval);
          }
          catch (std::exception &e)
          {
            qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
          }
          */
          continue;
        }

        // There's a lot of data, so let's read it out
        ReceiveMessage();

      }
    }
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkListenerProcess::ReceiveMessage()
{
  NiftyLinkMessage::Pointer msg;
  igtl::MessageBase::Pointer message;

  // Create a message buffer to receive header
  igtl::MessageHeader::Pointer msgHeader;
  msgHeader = igtl::MessageHeader::New();

  // Initialize receive buffer
  msgHeader->InitPack();

  // Receive generic header from the socket
  m_Mutex->lock();
  int r = m_ExtSocket->Receive(msgHeader->GetPackPointer(), msgHeader->GetPackSize());
  m_Mutex->unlock();

  //QLOG_INFO() <<objectName() <<"Bytes received: " <<r;

  if (r <= 0)
  {
    msgHeader.operator = (NULL);
    return false;
  }

  m_TimeOuter->start(2000);

  // Deserialize the header
  msgHeader->Unpack();

  // Interpret message and instanciate the appropriate NiftyLink message wrapper type
  if (msgHeader->GetDeviceType() == std::string("BIND"))
  {
    message = igtl::BindMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("COLORTABLE"))
  {
    message = igtl::ColorTableMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("IMAGE"))
  {
    message = igtl::ImageMessage::New();
    msg.operator = (NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
  }
  else if (msgHeader->GetDeviceType() == std::string("IMGMETA"))
  {
    message = igtl::ImageMetaMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("LBMETA"))
  {
    message = igtl::LabelMetaMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("NDARRAY"))
  {
    message = igtl::GetBindMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("POINTMSG"))
  {
    message = igtl::PointMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("POLYDATA"))
  {
    message = igtl::PolyDataMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("POSITION"))
  {
    message = igtl::PositionMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("QTDATA"))
  {
    message = igtl::QuaternionTrackingDataMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("SENSOR"))
  {
    message = igtl::SensorMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("STATUS"))
  {
    message = igtl::StatusMessage::New();
    msg.operator = (NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage()));
  }
  else if (msgHeader->GetDeviceType() == std::string("STRING"))
  {
    message = igtl::StringMessage::New();
    msg.operator = (NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage()));
  }
  else if (msgHeader->GetDeviceType() == std::string("TDATA"))
  {
    message = igtl::TrackingDataMessage::New();
    msg.operator = (NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage()));
  }
  else if (msgHeader->GetDeviceType() == std::string("TRAJ"))
  {
    message = igtl::TrajectoryMessage::New();
  }
  else if (msgHeader->GetDeviceType() == std::string("TRANSFORM"))
  {
    message = igtl::TransformMessage::New();
    msg.operator = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
  }
  else
  {
    QLOG_INFO() << objectName() << ": " << "Unknown message recieved: " << msgHeader->GetDeviceType() << endl;
    m_Mutex->lock();
    m_ExtSocket->Skip(msgHeader->GetBodySizeToRead(), 0);
    m_Mutex->unlock();

    msgHeader.operator = (NULL);
    return true;
  }

  message->SetMessageHeader(msgHeader);
  message->AllocatePack();

  // Create a new timestamp
  igtl::TimeStamp::Pointer timeReceived  = igtl::TimeStamp::New();

  if (message->GetPackBodySize() > 0)
  {
    // Receive data from the socket
    m_Mutex->lock();
    r = m_ExtSocket->Receive(message->GetPackBodyPointer(), message->GetPackBodySize());

    // Message fully received
    timeReceived->GetTime();

    m_Mutex->unlock();

    //QLOG_INFO() <<objectName() <<"Total message bytes received: " <<r;

    if (r < 0)
    {
      return false;
    }
    else
    {
      m_TimeOuter->start(2000);
    }
  }

  // Get the receive timestamp from the socket - marks when the first byte of the package arrived
  igtl::TimeStamp::Pointer timeArrived  = igtl::TimeStamp::New();
  timeArrived->SetTimeInNanoseconds(m_ExtSocket->GetReceiveTimestampInNanoseconds());
  msg->SetTimeArrived(timeArrived);
  
  // Set the time when the message was fully received
  msg->SetTimeReceived(timeReceived);

  msg->SetMessagePointer(message);
  msg->SetPort(m_Port);

  //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved: " <<msg->GetMessageType();
  m_MessageCounter++;

  emit MessageReceivedSignal(msg);
  //QCoreApplication::processEvents();

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::SetListenInterval(int msec)
{
  m_ListenInterval = msec;
}


//-----------------------------------------------------------------------------
int NiftyLinkListenerProcess::GetListenInterval(void)
{
  return m_ListenInterval;
}


//-----------------------------------------------------------------------------
void NiftyLinkListenerProcess::OnSocketTimeout(void)
{
  QLOG_INFO() << objectName() << ": " << "Client disconnected.. terminating socket." << "\n";

  if (m_ListeningOnPort)
  {
    emit ClientDisconnectedSignal(true);
  }
  else
  {
    emit ClientDisconnectedSignal(false);
  }

  //QCoreApplication::processEvents();

  m_TimeOuter->stop();
  m_ClientConnected = false;
}

} // end namespace niftk
