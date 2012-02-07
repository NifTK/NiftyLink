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

OIGTLListenerThread::OIGTLListenerThread(QObject *parent)
: OIGTLThreadBase(parent)
{
  m_listeningOnPort = false;
  m_clientConnected = false;
  m_listenInterval = 1000;
  m_messageCounter = 0;

  connect(&m_timeouter, SIGNAL(timeout()), this, SLOT(socketTimeout()), Qt::QueuedConnection);
  connect(this, SIGNAL(restartTimer(int )), &m_timeouter, SLOT(start(int )), Qt::QueuedConnection);
  connect(this, SIGNAL(stopTimer( )), &m_timeouter, SLOT(stop( )), Qt::QueuedConnection);
}

OIGTLListenerThread::~OIGTLListenerThread(void)
{
  m_serverSocket.operator =(NULL);
  m_extSocket.operator =(NULL);

  disconnect(&m_timeouter, SIGNAL(timeout()), this, SLOT(socketTimeout()) ); 
  disconnect(this, SIGNAL(restartTimer(int )), &m_timeouter, SLOT(start(int)));
  disconnect(this, SIGNAL(stopTimer( )), &m_timeouter, SLOT(stop( )));
}

bool OIGTLListenerThread::initialize(igtl::Socket::Pointer socket, int port)
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

bool OIGTLListenerThread::initialize(int port)
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

void OIGTLListenerThread::startThread()
{
  if (m_initialized == false || m_running == true)
    return;

  m_running = true;
  this->start();

  if (m_listeningOnPort)
    QLOG_INFO() <<objectName() <<": " <<"Started listening at: " <<m_port <<"\n";
  else
    QLOG_INFO() <<objectName() <<": " <<"Started listening on socket" <<"\n";
}

void OIGTLListenerThread::stopThread()
{
  // Quitting thread	
  QLOG_INFO() <<objectName() <<": " << "Quitting listener thread \n";

  m_running = false;
  int err_p = 0;
  int err_s = 0;

  QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

  emit stopTimer();

  m_mutex->lock();
  if (m_listeningOnPort && m_extSocket.IsNotNull())
  {
    err_p |= m_extSocket->CloseSocket();
    
    this->msleep(100);
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

    this->msleep(100);
    m_serverSocket.operator =(NULL);
  }
  m_mutex->unlock();

  if (err_s != 0)
    QLOG_ERROR() <<objectName() <<"CloseServerSocket returned with error: " <<err_s;
  else
    QLOG_INFO() <<objectName() <<"CloseServerSocket succeeded: " <<err_s;

  m_listeningOnPort = false;
  m_port = -1;
  m_initialized = false;

  QLOG_INFO() <<objectName() <<": " << "Total number of messages recieved: " <<m_messageCounter;

  exit(0);
}

bool OIGTLListenerThread::activate(void)
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

  if ( (m_listeningOnPort && m_serverSocket.IsNull()) )// || (m_listeningOnPort && m_serverSocket.IsNotNull() && !m_serverSocket->IsValid()) )
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

void OIGTLListenerThread::run()
{
  QLOG_INFO() <<objectName() <<": " <<"Listener Thread started" <<endl;

  if (!m_listeningOnPort && m_initialized)
  {
    m_clientConnected = true;
    emit restartTimer(1000);

    listenOnSocket();
  }
  else if (m_listeningOnPort && m_initialized)
    listenOnPort();
}

void OIGTLListenerThread::listenOnSocket(void)
{
  while (m_running == true && m_clientConnected == true)
  {
    if (receiveMessage() == false)
      this->msleep(100);
  }
}

void OIGTLListenerThread::listenOnPort(void)
{
  igtl::Socket::Pointer socket;

  while (m_running == true)
  {
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
        emit restartTimer(1000);
        emit clientConnected();
        m_clientConnected = true;
      }

      while (m_running == true && m_clientConnected == true) // if client connected
      {
        if (receiveMessage() == false)
          this->msleep(100);
      }
    }
  }
}

bool OIGTLListenerThread::receiveMessage()
{
  igtl::MessageBase::Pointer message;

  // Create a message buffer to receive header
  igtl::MessageHeader::Pointer msgHeader;
  msgHeader = igtl::MessageHeader::New();

  //Create message wrapper
  OIGTLMessage::Pointer msg;

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
  else if (r == 2 || r != msgHeader->GetPackSize())
  {
    // Corrupted package or keepalive, but still something
    emit restartTimer(1000);
    msgHeader.operator =(NULL);
    return true;
  }

  emit restartTimer(1000);

  // Deserialize the header
  msgHeader->Unpack();

  // Interpret message and instanciate the appropriate OIGTL message wrapper type
  if (strcmp(msgHeader->GetDeviceType(), "BIND") == 0)
    message = igtl::BindMessage::New(); 
  else if (strcmp(msgHeader->GetDeviceType(), "GET_BIND") == 0)
    message = igtl::GetBindMessage::New(); 
  else if (strcmp(msgHeader->GetDeviceType(), "STT_BIND") == 0)
    message = igtl::StartBindMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "STP_BIND") == 0)
    message = igtl::StopBindMessage::New(); 
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_BIND") == 0)
    message = igtl::RTSBindMessage::New(); 
  else if (strcmp(msgHeader->GetDeviceType(), "IMAGE") == 0)
  {
    message = igtl::ImageMessage::New(); 
    msg.operator =(OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  }
  else if (strcmp(msgHeader->GetDeviceType(), "GET_IMAGE") == 0)
    message = igtl::GetImageMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "STT_IMAGE") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STP_IMAGE") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_IMAGE") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "TRANSFORM") == 0)
  {
    message = igtl::TransformMessage::New(); 
    msg.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
  }
  else if (strcmp(msgHeader->GetDeviceType(), "GET_TRANS") == 0)
  {
    message = igtl::GetTransformMessage::New();
    OIGTLTransformMessage::Create_GET(msg);
  }
  else if (strcmp(msgHeader->GetDeviceType(), "STT_TRANS") == 0)
  {
    message = igtl::StartTransformMessage::New();
    OIGTLTransformMessage::Create_STT(msg);
  }
  else if (strcmp(msgHeader->GetDeviceType(), "STP_TRANS") == 0)
  {
    message = igtl::StopTransformMessage::New();
    OIGTLTransformMessage::Create_STP(msg);
  }
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_TRANS") == 0)
  {
    message = igtl::RTSTransformMessage::New();
    OIGTLTransformMessage::Create_RTS(msg);
  }
  else if (strcmp(msgHeader->GetDeviceType(), "POSITION") == 0)
    message = igtl::PositionMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "GET_POSITION") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STT_POSITION") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STP_POSITION") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_POSITION") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STATUS") == 0)
    message = igtl::StatusMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "GET_STATUS") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_STATUS") == 0)	
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "POINT") == 0) 
    message = igtl::PointMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "GET_POINT") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STT_POINT") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STP_POINT") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_POINT") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STRING") == 0) 
    message = igtl::StringMessage::New();
  else if (strcmp(msgHeader->GetDeviceType(), "GET_STRING") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STT_STRING") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "STP_STRING") == 0)
  {}
  else if (strcmp(msgHeader->GetDeviceType(), "RTS_STRING") == 0)
  {}
  else
  {
    // if the data type is unknown, skip reading.
    QLOG_INFO() <<objectName() <<": " << "Unknown message recieved: " << msgHeader->GetDeviceType() << endl;

    m_mutex->lock();
    m_extSocket->Skip(msgHeader->GetBodySizeToRead(), 0);
    m_mutex->unlock();

    msgHeader.operator =(NULL);
    return true;
  }

  if (msg.operator ==(NULL))
  {
    QLOG_ERROR() <<objectName() <<": " <<"Cannot to create OIGTLMessage, receiveMessage() failed" <<endl;
    msgHeader.operator =(NULL);
    return false;
  }

  message->SetMessageHeader(msgHeader);
  message->AllocatePack(); 

  // Receive data from the socket
  m_mutex->lock();
  r = m_extSocket->Receive(message->GetPackBodyPointer(), message->GetPackBodySize());
  m_mutex->unlock();

  //QLOG_INFO() <<objectName()  <<"Total message bytes received: " <<r;
  //std::cerr <<"Total message bytes received: " <<r <<std::endl;

  if (r <= 0)
    return false;
  else if (r <= 2 || r != message->GetPackBodySize())
  {
    emit restartTimer(1000);
    return true;
  }

  emit restartTimer(1000);

  igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
  ts->GetTime();

  msg->setTimeReceived(ts);
  msg->setMessagePointer(message);
  msg->setPort(m_port);

  //QLOG_INFO() <<objectName() <<": " << "Message successfully recieved";
  m_messageCounter++;
  
  emit messageReceived(msg);
  
  return true;
}

void OIGTLListenerThread::setListenInterval(int msec) 
{ 
  m_listenInterval = msec;
} 

int OIGTLListenerThread::getListenInterval(void) 
{ 
  return m_listenInterval; 
}

void OIGTLListenerThread::socketTimeout(void)
{
  QLOG_INFO() <<objectName() <<": " <<"Client disconnected.. terminating socket." <<"\n";
  
  if (m_listeningOnPort)
    emit clientDisconnected(true);
  else
    emit clientDisconnected(false);

  emit stopTimer();
  m_clientConnected = false;
}
