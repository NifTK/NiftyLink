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

OIGTLSocketObject::OIGTLSocketObject(QObject *parent)
: QObject(parent)
{
  m_port = -1;
  m_listening = false;
  m_connectedToRemote = false;

  m_clientConnected = false;
  m_ableToSend = false;

  m_initialized = false;

  m_sender = NULL;
  m_listener = NULL;
  m_mutex = NULL;

  initThreads();
}

OIGTLSocketObject::~OIGTLSocketObject(void)
{
  this->closeSocket();

  if (m_sender != NULL)
  {
    // Disconnect signals off sender
    disconnect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
    disconnect(m_sender, SIGNAL(cannotConnectToRemote()), this, SLOT(cannotConnectToRemote()) );
    disconnect(m_sender, SIGNAL(disconnectedFromRemote(bool )), this, SLOT(disconnectedFromRemote(bool )) );
    disconnect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );
    disconnect(this, SIGNAL(messageToSend(OIGTLMessage::Pointer)), m_sender, SLOT(sendMsg(OIGTLMessage::Pointer)));

    // Delete sender
    delete m_sender;
    m_sender = NULL;
  }

  if (m_listener != NULL)
  {
    // Disconnect signals off listener
    disconnect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()) );
    disconnect(m_listener, SIGNAL(clientDisconnected(bool )), this, SLOT(clientDisconnected(bool )) );
    disconnect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SIGNAL(messageReceived(OIGTLMessage::Pointer)) );

    // Delete listener
    delete m_listener;
    m_listener = NULL;
  }

  // Delete the mutex too
  if (m_mutex != NULL)
  {
    delete m_mutex;
    m_mutex = NULL;
  }
}

void OIGTLSocketObject::initThreads()
{
  qRegisterMetaType<OIGTLMessage::Pointer>();

  // Instanciate the mutex and the threads
  m_mutex = new QMutex();
  m_sender = new OIGTLSenderThread(this);
  m_listener = new OIGTLListenerThread(this);
  //m_listener->setSocketTimeOut(500);

  m_sender->setMutex(m_mutex);
  m_listener->setMutex(m_mutex);

  bool ok;

  // Connect signals to slots
  ok =  connect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()), Qt::QueuedConnection);
  ok &= connect(m_sender, SIGNAL(cannotConnectToRemote()), this, SLOT(cannotConnectToRemote()), Qt::QueuedConnection);
  ok &= connect(m_sender, SIGNAL(disconnectedFromRemote(bool )), this, SLOT(disconnectedFromRemote(bool )), Qt::QueuedConnection);
  ok &= connect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()), Qt::QueuedConnection);
  ok &= connect(this, SIGNAL(messageToSend(OIGTLMessage::Pointer)), m_sender, SLOT(sendMsg(OIGTLMessage::Pointer)));

  ok &= connect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()), Qt::QueuedConnection);
  ok &= connect(m_listener, SIGNAL(clientDisconnected(bool )), this, SLOT(clientDisconnected(bool )), Qt::QueuedConnection);
  ok &= connect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SIGNAL(messageReceived(OIGTLMessage::Pointer)), Qt::QueuedConnection);

  // Set the flag
  if (m_mutex != NULL && m_sender != NULL && m_listener != NULL && ok)
    m_initialized = true;
}

bool OIGTLSocketObject::listenOnPort(int port)
{
  if (!m_initialized)
    return false;

  if (m_listener != NULL && m_initialized)
  {
    // Set parameters
    m_listener->setObjectName(this->objectName().append("_L"));
    // Initialize the port
    bool ok = m_listener->initialize(port);

    if (ok)
    {
      // Start the thread
      m_port = port;
      m_listening = true;
      m_listener->startThread();

      QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";

      return true;
    }
    else return false;
  }
  else return false;
}

bool OIGTLSocketObject::connectToRemote(QUrl url)
{
  if (!m_initialized)
    return false;

  if (m_sender != NULL  && m_initialized)
  {
    // Set parameters
    m_sender->setObjectName(this->objectName().append("_S"));

    std::string address_str;
    address_str.clear();

    if (validateIp(url.host()) == true)
    {
      address_str = url.host().toStdString();
    }
    else
    {
      QString ip = resolveHostName(url.host());

      if (validateIp(ip))
        address_str = ip.toStdString();
      else
        return false;
    }

    int port = url.port();

    // Initialize the sender
    bool ok = m_sender->initialize(address_str, port);

    if (ok)
    {
      m_port = port;
      
      // Start the thread
      m_sender->startThread();

      QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";

      QLOG_INFO() <<objectName() <<": " <<"Connecting to: " <<address_str.c_str() <<" : " <<port <<endl;

      return true;
    }
    else return false;

  }
  else return false;
}

void OIGTLSocketObject::closeSocket(void)
{
  // Shuts down the sender thread
  if (m_sender != NULL)
  {
    m_sender->stopThread();

    while (m_sender->isRunning());
  }

  // Shut down the listener thread
  if (m_listener != NULL)
  {
    m_listener->stopThread();
    while (m_listener->isRunning());
  }

  m_port = -1;
  m_listening = false;
  m_connectedToRemote = false;

  m_clientConnected = false;
  m_ableToSend = false;

  QLOG_INFO() <<objectName() <<": " <<"Closing socket, threads terminated.";
}

void OIGTLSocketObject::sendMessage(OIGTLMessage::Pointer msg)
{
  if (m_sender != NULL && m_sender->isInitialized())
    emit messageToSend(msg);

  //QLOG_INFO() <<"Emitting.........";
}


//*********************************************************
//                    INTERNAL SLOTS
//*********************************************************

void OIGTLSocketObject::connectedToRemote(void)
{
  //Sender established connection to remote host, need to set up listener on socket
  if (m_sender != NULL && m_sender->isInitialized())
  {
    m_connectedToRemote = true;
    m_ableToSend = true;

    m_listener->setObjectName(this->objectName().append("_L"));
    if (m_listener->initialize(m_sender->getSocketPointer(), m_port) == true)
      m_listener->startThread();

    emit connectedToRemoteSignal();
  }
}

void OIGTLSocketObject::cannotConnectToRemote(void)
{
  //Could not establish connection to remote host
  m_connectedToRemote = false;
  m_ableToSend = false;
  m_port = -1;

  emit cannotConnectToRemoteSignal();
}

void OIGTLSocketObject::disconnectedFromRemote(bool onPort)
{
  if (onPort)
  {
    if (m_sender != NULL)
      m_sender->stopThread();

    if (m_listener != NULL)
      m_listener->stopThread();

    emit lostConnectionToRemoteSignal();
  }
  else
  {
    if (m_sender != NULL)
      m_sender->stopThread();
  }

  m_connectedToRemote = false;
  m_ableToSend = false;

}

void OIGTLSocketObject::clientConnected(void)
{
  if (m_listener != NULL && m_listener->isInitialized())
  {
    m_sender->setObjectName(this->objectName().append("_S"));
    if (m_sender->initialize(m_listener->getSocketPointer(), m_port) == true)
    {
      m_ableToSend = true;
      m_sender->startThread();
    }

    emit clientConnectedSignal();
  }
}

void OIGTLSocketObject::clientDisconnected(bool onPort)
{
  if (onPort)
  {
    if (m_sender != NULL)
      m_sender->stopThread();

    emit clientDisconnectedSignal();
  }
  m_ableToSend = false;
}

void OIGTLSocketObject::catchMsgSignal(OIGTLMessage::Pointer msg)
{
  //QLOG_INFO() <<objectName() <<": " <<"Message signal received";
  if (msg.operator !=(NULL))
  {
    emit messageReceived(msg);
  }
}

//*********************************************************
//               GETTING / SETTING PARAMETERS
//*********************************************************


void OIGTLSocketObject::setConnectTimeOut(int sec)
{
  if (m_sender != NULL)
    m_sender->setConnectTimeOut(sec);
}

int OIGTLSocketObject::getConnectTimeOut(void)
{
  if (m_sender != NULL)
    return m_sender->getConnectTimeOut();
  else return -1;
}

void OIGTLSocketObject::setListenInterval(int msec)
{
  if (m_listener != NULL)
    m_listener->setListenInterval(msec);
}

int OIGTLSocketObject::getListenInterval(void)
{
  if (m_listener != NULL)
    return m_listener->getListenInterval();
  else return -1;
}

void OIGTLSocketObject::setSocketTimeOut(int msec)
{
  if (m_listener != NULL)
    m_listener->setSocketTimeOut(msec);

  if (m_sender != NULL)
    m_sender->setSocketTimeOut(msec);
}

int OIGTLSocketObject::getSocketTimeOut(void)
{
  if (m_listener != NULL)
    return m_listener->getSocketTimeOut();
  else return -1;
}
