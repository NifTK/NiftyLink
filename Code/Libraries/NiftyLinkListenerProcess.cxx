/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkSocketObject.h"

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
bool NiftyLinkListenerProcess::Initialize(igtl::Socket::Pointer socket, int port)
{
  if (socket.IsNull() || (socket.IsNotNull() && !socket->IsValid()) )
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

  igtl::ServerSocket::Pointer ssock = igtl::ServerSocket::New();
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
  QCoreApplication::processEvents();
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
      dynamic_cast<QThreadEx *>(QThread::currentThread())->MsleepEx(sleepInterval);
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
      dynamic_cast<QThreadEx *>(QThread::currentThread())->MsleepEx(sleepInterval);
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

  if ( (!m_ListeningOnPort && m_ExtSocket.IsNull()) || (!m_ListeningOnPort && m_ServerSocket.IsNotNull() && !m_ExtSocket->IsValid()) )
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
  igtl::Socket::Pointer socket;

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

      if (!socket->IsValid())
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
        QCoreApplication::processEvents();
        m_ClientConnected = true;
      }

      while (m_Running == true && m_ClientConnected == true) // if client connected
      {
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

  //Double check if message types are mapped or not
  if (strMsgTypes.size() == 0)
  {
    InitMessageTypes(strMsgTypes);
  }

  // Interpret message and instanciate the appropriate NiftyLink message wrapper type
  switch (strMsgTypes[msgHeader->GetDeviceType()])
  {
    case BIND:
      message = igtl::BindMessage::New();
      break;
    case GET_BIND:
      message = igtl::GetBindMessage::New();
      break;
    case STT_BIND:
      message = igtl::StartBindMessage::New();
      break;
    case STP_BIND:
      message = igtl::StopBindMessage::New();
      break;
    case RTS_BIND:
      message = igtl::RTSBindMessage::New();
      break;
    case COLORTABLE:
      message = igtl::ColorTableMessage::New();
      break;
    case GET_COLORT:
      message = igtl::GetColorTableMessage::New();
      break;
    case STT_COLORT:
      message = igtl::StartColorTableMessage::New();
      break;
    case STP_COLORT:
      message = igtl::StopColorTableMessage::New();
      break;
    case RTS_COLORT:
      message = igtl::RTSColorTableMessage::New();
      break;
    case IMAGE:
      message = igtl::ImageMessage::New();
      msg.operator = (NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
      //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved" <<msgHeader->GetDeviceType();
      break;
    case GET_IMAGE:
      message = igtl::GetImageMessage::New();
      NiftyLinkImageMessage::Create_GET(msg);
      break;
    case STT_IMAGE:
      message = igtl::StartImageMessage::New();
      NiftyLinkImageMessage::Create_STT(msg);
      break;
    case STP_IMAGE:
      message = igtl::StopImageMessage::New();
      NiftyLinkImageMessage::Create_STP(msg);
      break;
    case RTS_IMAGE:
      message = igtl::RTSImageMessage::New();
      NiftyLinkImageMessage::Create_RTS(msg);
      break;
    case IMGMETA:
      message = igtl::ImageMetaMessage::New();
      break;
    case GET_IMGMETA:
      message = igtl::GetImageMetaMessage::New();
      break;
    case STT_IMGMETA:
      message = igtl::StartImageMetaMessage::New();
      break;
    case STP_IMGMETA:
      message = igtl::StopImageMetaMessage::New();
      break;
    case RTS_IMGMETA:
      message = igtl::RTSImageMetaMessage::New();
      break;
    case LBMETA:
      message = igtl::LabelMetaMessage::New();
      break;
    case GET_LBMETA:
      message = igtl::GetLabelMetaMessage::New();
      break;
    case STT_LBMETA:
      message = igtl::StartLabelMetaMessage::New();
      break;
    case STP_LBMETA:
      message = igtl::StopLabelMetaMessage::New();
      break;
    case RTS_LBMETA:
      message = igtl::RTSLabelMetaMessage::New();
      break;
    case NDARRAY:
      message = igtl::GetBindMessage::New();
      break;
    case GET_NDARRAY:
      message = igtl::GetBindMessage::New();
      break;
    case STT_NDARRAY:
      message = igtl::GetBindMessage::New();
      break;
    case STP_NDARRAY:
      message = igtl::GetBindMessage::New();
      break;
    case RTS_NDARRAY:
      message = igtl::GetBindMessage::New();
      break;
    case POINTMSG:
      message = igtl::PointMessage::New();
      break;
    case GET_POINTMSG:
      message = igtl::GetPointMessage::New();
      break;
    case STT_POINTMSG:
      message = igtl::StartPointMessage::New();
      break;
    case STP_POINTMSG:
      message = igtl::StopPointMessage::New();
      break;
    case RTS_POINTMSG:
      message = igtl::RTSPointMessage::New();
      break;
    case POLYDATA:
      message = igtl::PolyDataMessage::New();
      break;
    case GET_POLYDATA:
      message = igtl::GetPolyDataMessage::New();
      break;
    case STT_POLYDATA:
      message = igtl::StartPolyDataMessage::New();
      break;
    case STP_POLYDATA:
      message = igtl::StopPolyDataMessage::New();
      break;
    case RTS_POLYDATA:
      message = igtl::RTSPolyDataMessage::New();
      break;
    case POSITION:
      message = igtl::PositionMessage::New();
      break;
    case GET_POSITION:
      message = igtl::GetPositionMessage::New();
      break;
    case STT_POSITION:
      message = igtl::StartPositionMessage::New();
      break;
    case STP_POSITION:
      message = igtl::StopPositionMessage::New();
      break;
    case RTS_POSITION:
      message = igtl::RTSPositionMessage::New();
      break;
    case QTDATA:
      message = igtl::QuaternionTrackingDataMessage::New();
      break;
    case GET_QTDATA:
      message = igtl::GetQuaternionTrackingDataMessage::New();
      break;
    case STT_QTDATA:
      message = igtl::StartQuaternionTrackingDataMessage::New();
      break;
    case STP_QTDATA:
      message = igtl::StopQuaternionTrackingDataMessage::New();
      break;
    case RTS_QTDATA:
      message = igtl::RTSQuaternionTrackingDataMessage::New();
      break;
    case SENSOR:
      message = igtl::SensorMessage::New();
      break;
    case GET_SENSOR:
      message = igtl::GetSensorMessage::New();
      break;
    case STT_SENSOR:
      message = igtl::StartSensorMessage::New();
      break;
    case STP_SENSOR:
      message = igtl::StopSensorMessage::New();
      break;
    case RTS_SENSOR:
      message = igtl::RTSSensorMessage::New();
      break;
    case STATUS:
      message = igtl::StatusMessage::New();
      msg.operator = (NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage()));
      break;
    case GET_STATUS:
      message = igtl::GetStatusMessage::New();
      NiftyLinkStatusMessage::Create_GET(msg);
      break;
    case STT_STATUS:
      message = igtl::StartStatusMessage::New();
      NiftyLinkStatusMessage::Create_STT(msg);
      break;
    case STP_STATUS:
      message = igtl::StopStatusMessage::New();
      NiftyLinkStatusMessage::Create_STP(msg);
      break;
    case RTS_STATUS:
      message = igtl::RTSStatusMessage::New();
      NiftyLinkStatusMessage::Create_RTS(msg);
      break;
    case STRING:
      message = igtl::StringMessage::New();
      msg.operator = (NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage()));
      break;
    case GET_STRING:
      message = igtl::GetStringMessage::New();
      NiftyLinkStringMessage::Create_GET(msg);
      break;
    case STT_STRING:
      message = igtl::StartStringMessage::New();
      NiftyLinkStringMessage::Create_STT(msg);
      break;
    case STP_STRING:
      message = igtl::StopStringMessage::New();
      NiftyLinkStringMessage::Create_STP(msg);
      break;
    case RTS_STRING:
      message = igtl::RTSStringMessage::New();
      NiftyLinkStringMessage::Create_RTS(msg);
      break;
    case TDATA:
      message = igtl::TrackingDataMessage::New();
      msg.operator = (NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage()));
      break;
    case GET_TDATA:
      message = igtl::GetTrackingDataMessage::New();
      NiftyLinkTrackingDataMessage::Create_GET(msg);
      break;
    case STT_TDATA:
      message = igtl::StartTrackingDataMessage::New();
      NiftyLinkTrackingDataMessage::Create_STT(msg);
      break;
    case STP_TDATA:
      message = igtl::StopTrackingDataMessage::New();
      NiftyLinkTrackingDataMessage::Create_STP(msg);
      break;
    case RTS_TDATA:
      message = igtl::RTSTrackingDataMessage::New();
      NiftyLinkTrackingDataMessage::Create_RTS(msg);
      break;
    case TRAJ:
      message = igtl::TrajectoryMessage::New();
      break;
    case GET_TRAJ:
      message = igtl::GetTrajectoryMessage::New();
      break;
    case STT_TRAJ:
      message = igtl::StartTrajectoryMessage::New();
      break;
    case STP_TRAJ:
      message = igtl::StopTrajectoryMessage::New();
      break;
    case RTS_TRAJ:
      message = igtl::RTSTrajectoryMessage::New();
      break;
    case TRANSFORM:
      message = igtl::TransformMessage::New();
      msg.operator = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
      break;
    case GET_TRANS:
      message = igtl::GetTransformMessage::New();
      NiftyLinkTransformMessage::Create_GET(msg);
      break;
    case STT_TRANS:
      message = igtl::StartTransformMessage::New();
      NiftyLinkTransformMessage::Create_STT(msg);
      break;
    case STP_TRANS:
      message = igtl::StopTransformMessage::New();
      NiftyLinkTransformMessage::Create_STP(msg);
      break;
    case RTS_TRANS:
      message = igtl::RTSTransformMessage::New();
      NiftyLinkTransformMessage::Create_RTS(msg);
      break;
    default:
      // if the data type is unknown, skip reading.
      QLOG_INFO() << objectName() << ": " << "Unknown message recieved: " << msgHeader->GetDeviceType() << endl;

      m_Mutex->lock();
      m_ExtSocket->Skip(msgHeader->GetBodySizeToRead(), 0);
      m_Mutex->unlock();

      msgHeader.operator = (NULL);
      return true;
    }

  message->SetMessageHeader(msgHeader);
  message->AllocatePack();

  if (message->GetPackBodySize() > 0)
  {
    // Receive data from the socket
    m_Mutex->lock();
    r = m_ExtSocket->Receive(message->GetPackBodyPointer(), message->GetPackBodySize());
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

  // Get the receive timestamp from the socket
  msg->SetTimeReceived(m_ExtSocket->GetReceiveTimestamp());
  msg->SetMessagePointer(message);
  msg->SetPort(m_Port);

  //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved: " <<msg->GetMessageType();
  m_MessageCounter++;

  emit MessageReceivedSignal(msg);
  QCoreApplication::processEvents();

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

  QCoreApplication::processEvents();

  m_TimeOuter->stop();
  m_ClientConnected = false;
}
