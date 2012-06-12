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

#include "OIGTLSocketObject.h"

OIGTLListenerProcess::OIGTLListenerProcess(QObject *parent)
: OIGTLProcessBase(parent)
{
  m_listeningOnPort = false;
  m_clientConnected = false;
  m_listenInterval = 1000;
}

OIGTLListenerProcess::~OIGTLListenerProcess(void)
{
  //QLOG_INFO() <<"Destructing"  <<objectName() <<"running: " <<this->isRunning() <<this->isActive(); 
  m_serverSocket.operator =(NULL);
  m_extSocket.operator =(NULL);
}

bool OIGTLListenerProcess::initialize(igtl::Socket::Pointer socket, int port)
{
  if (socket.IsNull() || (socket.IsNotNull() && !socket->IsValid()) )
  {
    QLOG_ERROR() <<objectName() <<": " << "Cannot create a listener socket, invalid external socket specified" << endl;
    return false;
  }

  if (m_initialized == true)
  {
    QLOG_ERROR() <<objectName() <<": " << "Listener already in use!" << endl;
    return false;
  }

  m_extSocket.operator =(socket);
  m_extSocket->SetTimeout(m_socketTimeout);
 
  m_listeningOnPort = false;
  m_port = port;

  if (!activate())
    return false;

  return true;
}

bool OIGTLListenerProcess::initialize(int port)
{
  //Set up a new instance of listener on a given port and create a new server socket

  if (port <= 0)
  {
    QLOG_ERROR() <<objectName() <<": " << "Cannot create a listener socket, invalid socket or port specified" << endl;
    return false;
  }

  if (m_initialized == true)
  {
    QLOG_ERROR() <<objectName() <<": " << "Listener already in use!" << endl;
    return false;
  }

  igtl::ServerSocket::Pointer ssock = igtl::ServerSocket::New();
  int	err = ssock->CreateServer(port);
  ssock->SetTimeout(m_socketTimeout);

  if (err < 0)
  {
    QLOG_ERROR() <<objectName() <<": " << "Error creating listener socket." << endl;
    m_port = -1;
    return false;
  }

  m_port = port;
  m_serverSocket.operator =(ssock);
  m_serverSocket->SetTimeout(m_socketTimeout);
  m_listeningOnPort = true;

  if (!activate())
    return false;

  return true;
}

void OIGTLListenerProcess::startProcess()
{
  if (m_initialized == false || m_running == true)
    return;

  m_running = true;   //sets the process into running state
  m_active  = true;   //sets the flag

  if (m_listeningOnPort)
    QLOG_INFO() <<objectName() <<": " <<"Started listening at: " <<m_port <<"\n";
  else
    QLOG_INFO() <<objectName() <<": " <<"Started listening on socket" <<"\n";

  // Trigger the execution of the main processing loop
  connect(this, SIGNAL(startWorking()), this, SLOT(doProcessing()));
  emit startWorking();
  QCoreApplication::processEvents();
}

void OIGTLListenerProcess::stopProcess()
{
  // Stopping Process
  QLOG_INFO() <<objectName() <<": " << "Stopping listener process... \n";

  m_running = false;

}

void OIGTLListenerProcess::terminateProcess()
{
  // Terminate the process
  m_running = false; //Just in case...

  // Quitting Process	
  QLOG_INFO() <<objectName() <<": " << "Terminating the listener process... \n";
  QLOG_INFO() <<objectName() <<": " << "Total number of messages recieved: " <<m_messageCounter;

  int err_p = 0;
  int err_s = 0;

  QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

  m_mutex->lock();
  if (m_listeningOnPort && m_extSocket.IsNotNull())
  {
    err_p |= m_extSocket->CloseSocket();
 
    try
    {
      dynamic_cast<QThreadEx *>(QThread::currentThread())->msleepEx(100);
    }
    catch (std::exception &e)
    {
      qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
    }
 

    m_extSocket.operator =(NULL);
  }
  m_mutex->unlock();

  if (err_p != 0)
    QLOG_ERROR() <<objectName() <<"CloseSocket returned with error: " <<err_p;
  else
    QLOG_INFO() <<objectName() <<"CloseSocket succeded: " <<err_p;

  QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

  m_mutex->lock();
  if (m_serverSocket.IsNotNull())
  {
    err_s |= m_serverSocket->CloseSocket();

    try
    {
      dynamic_cast<QThreadEx *>(QThread::currentThread())->msleepEx(100);
    }
    catch (std::exception &e)
    {
      qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
    }

    m_serverSocket.operator =(NULL);
  }
  m_mutex->unlock();

  if (err_s != 0)
    QLOG_ERROR() <<objectName() <<"CloseServerSocket returned with error: " <<err_s;
  else
    QLOG_INFO() <<objectName() <<"CloseServerSocket succeeded: " <<err_s;

  disconnect(this, SIGNAL(startWorking()), this, SLOT(doProcessing()));

  m_listeningOnPort = false;
  m_port = -1;
  m_initialized = false;
  m_active = false;
}

bool OIGTLListenerProcess::activate(void)
{
  if (m_mutex == NULL)
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, mutex not set" <<endl;
    return false;
  }

  if (m_listeningOnPort && m_port <= 0)
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, port is invalid" <<endl;
    return false;
  }

  if ( (m_listeningOnPort && m_serverSocket.IsNull()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, server socket is invalid" <<endl;
    return false;
  }

  if ( (!m_listeningOnPort && m_extSocket.IsNull())  || (!m_listeningOnPort && m_serverSocket.IsNotNull() && !m_extSocket->IsValid()) )
  {
    QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, external socket is invalid" <<endl;
    return false;
  }

  m_initialized = true;

  QLOG_INFO() <<objectName() <<": " <<"Listener successfully activated" <<endl;

  return true;
}

void OIGTLListenerProcess::doProcessing(void)
{
  m_timeouter = new QTimer();
  connect(m_timeouter, SIGNAL(timeout()), this, SLOT(socketTimeout()));

  //QCoreApplication::processEvents();

  if (!m_listeningOnPort && m_initialized)
  {
    m_clientConnected = true;
    m_timeouter->start(2000);

    listenOnSocket();
  }
  else if (m_listeningOnPort && m_initialized)
    listenOnPort();

  m_timeouter->stop();
  delete m_timeouter;
  m_timeouter = NULL;

  terminateProcess();
}

void OIGTLListenerProcess::listenOnSocket(void)
{
  while (m_running == true && m_clientConnected == true)
  {
    QCoreApplication::processEvents();

    int bytesPending = m_extSocket->CheckPendingData();
   
    if (bytesPending <= 0)
    {
      try
      {
        dynamic_cast<QThreadEx *>(QThread::currentThread())->msleepEx(100);
      }
      catch (std::exception &e)
      {
        qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
      }
    }
    else if (bytesPending == 2)
      m_timeouter->start(2000);
    else
      receiveMessage();
  }
}

void OIGTLListenerProcess::listenOnPort(void)
{
  igtl::Socket::Pointer socket;

  while (m_running == true)
  {
    QCoreApplication::processEvents();

    //------------------------------------------------------------
    // If running a server socket wait for connection
    if (m_serverSocket.IsNotNull() && m_running == true)
    {
      m_mutex->lock();
      socket = m_serverSocket->WaitForConnection(m_listenInterval);
      m_mutex->unlock();

      if (!socket->IsValid())
      {
        QLOG_INFO() <<objectName() <<": " << "No client connecting\n";
        continue;
      }
      else
      {
        // Client connected, need to set socket parameters
        m_extSocket.operator =(socket);
        m_extSocket->SetTimeout(m_socketTimeout);
        m_timeouter->start(2000);

        //qDebug() <<m_timeouter->isActive();

        emit clientConnected();
        QCoreApplication::processEvents();
        m_clientConnected = true;
      }

      while (m_running == true && m_clientConnected == true) // if client connected
      {
        QCoreApplication::processEvents();

        // Quickly check if there's any pending data on the socket
        m_mutex->lock();
        int bytesPending = m_extSocket->CheckPendingData();
        m_mutex->unlock();
       
        if (bytesPending <= 0)
        {
          try
          {
            dynamic_cast<QThreadEx *>(QThread::currentThread())->msleepEx(100);
          }
          catch (std::exception &e)
          {
            qDebug() <<"Type cast error.Always run this process from QThreadEx. Exception: " <<e.what();
          }
        }
        else if (bytesPending == 2)
          m_timeouter->start(2000);  //Keepalive operation, let's reset the timeout clock
        else
          receiveMessage();         //Message arrived, need to copy it from the socket
      }
    }
  }
}

bool OIGTLListenerProcess::receiveMessage()
{
  //Message wrapper pointer
  OIGTLMessage::Pointer msg;
  //Message pointer
  igtl::MessageBase::Pointer message;

  // Create a message buffer to receive header
  igtl::MessageHeader::Pointer msgHeader;
  msgHeader = igtl::MessageHeader::New();

  // Initialize receive buffer
  msgHeader->InitPack();

  // Receive generic header from the socket
  m_mutex->lock();
  int r = m_extSocket->Receive(msgHeader->GetPackPointer(), msgHeader->GetPackSize());
  m_mutex->unlock();

  //QLOG_INFO() <<objectName()  <<"Bytes received: " <<r;

  if (r <= 0)
  {
    msgHeader.operator =(NULL);
    return false;
  }

  m_timeouter->start(2000);

  // Deserialize the header
  msgHeader->Unpack();

  //Double check if message types are mapped or not
  if (strMsgTypes.size() == 0)
    InitMessageTypes(strMsgTypes);

  //ULONGLONG time1 = gethectonanotime_first();

  // Interpret message and instanciate the appropriate OIGTL message wrapper type
  switch(strMsgTypes[msgHeader->GetDeviceType()])
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
        msg.operator =(OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
        //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved" <<msgHeader->GetDeviceType();
        break;
    case GET_IMAGE:
        message = igtl::GetImageMessage::New();
        OIGTLImageMessage::Create_GET(msg);
        break;
    case STT_IMAGE:
        message = igtl::StartImageMessage::New();
        OIGTLImageMessage::Create_STT(msg);
        break;
    case STP_IMAGE:
        message = igtl::StopImageMessage::New();
        OIGTLImageMessage::Create_STP(msg);
        break;
    case RTS_IMAGE:
        message = igtl::RTSImageMessage::New();
        OIGTLImageMessage::Create_RTS(msg);
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
        msg.operator =(OIGTLStatusMessage::Pointer(new OIGTLStatusMessage()));
        break;
    case GET_STATUS:
        message = igtl::GetStatusMessage::New();
        OIGTLStatusMessage::Create_GET(msg);
        break;
    case STT_STATUS:
        message = igtl::StartStatusMessage::New();
        OIGTLStatusMessage::Create_STT(msg);
        break;
    case STP_STATUS:
        message = igtl::StopStatusMessage::New();
        OIGTLStatusMessage::Create_STP(msg);
        break;
    case RTS_STATUS:
        message = igtl::RTSStatusMessage::New();
        OIGTLStatusMessage::Create_RTS(msg);
        break;
    case STRING:
        message = igtl::StringMessage::New();
        msg.operator =(OIGTLStringMessage::Pointer(new OIGTLStringMessage()));
        break;
    case GET_STRING:
        message = igtl::GetStringMessage::New();
        OIGTLStringMessage::Create_GET(msg);
        break;
    case STT_STRING:
        message = igtl::StartStringMessage::New();
        OIGTLStringMessage::Create_STT(msg);
        break;
    case STP_STRING:
        message = igtl::StopStringMessage::New();
        OIGTLStringMessage::Create_STP(msg);
        break;
    case RTS_STRING:
        message = igtl::RTSStringMessage::New();
        OIGTLStringMessage::Create_RTS(msg);
        break;
    case TDATA:
        message = igtl::TrackingDataMessage::New();
        msg.operator =(OIGTLTrackingDataMessage::Pointer(new OIGTLTrackingDataMessage()));
        break;
    case GET_TDATA:
        message = igtl::GetTrackingDataMessage::New();
        OIGTLTrackingDataMessage::Create_GET(msg);
        break;
    case STT_TDATA:
        message = igtl::StartTrackingDataMessage::New();
        OIGTLTrackingDataMessage::Create_STT(msg);
        break;
    case STP_TDATA:
        message = igtl::StopTrackingDataMessage::New();
        OIGTLTrackingDataMessage::Create_STP(msg);
        break;
    case RTS_TDATA:
        message = igtl::RTSTrackingDataMessage::New();
        OIGTLTrackingDataMessage::Create_RTS(msg);
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
        msg.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
        break;
    case GET_TRANS:
        message = igtl::GetTransformMessage::New();
        OIGTLTransformMessage::Create_GET(msg);
        break;
    case STT_TRANS:
        message = igtl::StartTransformMessage::New();
        OIGTLTransformMessage::Create_STT(msg);
        break;
    case STP_TRANS:
        message = igtl::StopTransformMessage::New();
        OIGTLTransformMessage::Create_STP(msg);
        break;
    case RTS_TRANS:
        message = igtl::RTSTransformMessage::New();
        OIGTLTransformMessage::Create_RTS(msg);
        break;
    default:
      // if the data type is unknown, skip reading.
      QLOG_INFO() <<objectName() <<": " << "Unknown message recieved: " << msgHeader->GetDeviceType() << endl;

      m_mutex->lock();
      m_extSocket->Skip(msgHeader->GetBodySizeToRead(), 0);
      m_mutex->unlock();

      msgHeader.operator =(NULL);
      return true;
  }

  //ULONGLONG time2 = gethectonanotime_first();

  message->SetMessageHeader(msgHeader);
  message->AllocatePack(); 

  if (message->GetPackBodySize() > 0)
  {
    // Receive data from the socket
    m_mutex->lock();
    r = m_extSocket->Receive(message->GetPackBodyPointer(), message->GetPackBodySize());
    m_mutex->unlock();

    //QLOG_INFO() <<objectName()  <<"Total message bytes received: " <<r;
    //std::cerr <<"Total message bytes received: " <<r <<std::endl;

    if (r < 0)
      return false;
  }
  
  // Get the receive timestamp from the socket
  msg->setTimeReceived(m_extSocket->GetReceiveTimestamp());
  msg->setMessagePointer(message);
  msg->setPort(m_port);

  //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved: " <<msg->getMessageType();
  m_messageCounter++;
  
  emit messageReceived(msg);
  QCoreApplication::processEvents();
 
  return true;
}

void OIGTLListenerProcess::setListenInterval(int msec) 
{ 
  m_listenInterval = msec;
} 

int OIGTLListenerProcess::getListenInterval(void) 
{ 
  return m_listenInterval; 
}

void OIGTLListenerProcess::socketTimeout(void)
{
  QLOG_INFO() <<objectName() <<": " <<"Client disconnected.. terminating socket." <<"\n";
  
  if (m_listeningOnPort)
    emit clientDisconnected(true);
  else
    emit clientDisconnected(false);

  QCoreApplication::processEvents();

  m_timeouter->stop();
  m_clientConnected = false;
}
