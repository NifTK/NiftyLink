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

  m_active = false;
  m_initialized = false;

  m_sender = NULL;
  m_listener = NULL;
  m_mutex = NULL;

  m_senderHostThread = NULL;
  m_listenerHostThread = NULL;

  initThreads();
}

OIGTLSocketObject::~OIGTLSocketObject(void)
{
  QLOG_INFO() <<"Destructing"  <<objectName();

  if (m_initialized)
    this->closeSocket();

  if (m_sender != NULL)
  {
    // Disconnect signals off sender
    disconnect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
    disconnect(m_sender, SIGNAL(cannotConnectToRemote()), this, SLOT(cannotConnectToRemote()) );
    disconnect(m_sender, SIGNAL(disconnectedFromRemote(bool )), this, SLOT(disconnectedFromRemote(bool )) );
    disconnect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );
    disconnect(this, SIGNAL(messageToSend(OIGTLMessage::Pointer)), m_sender, SLOT(sendMsg(OIGTLMessage::Pointer)));

    disconnect(m_senderHostThread, SIGNAL(eventloopStarted()), m_sender, SLOT(startProcess()));
    disconnect(this, SIGNAL(shutdownSender()), m_sender, SLOT(stopProcess()));


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

    disconnect(m_listenerHostThread, SIGNAL(eventloopStarted()), m_listener, SLOT(startProcess()));
    disconnect(this, SIGNAL(shutdownListener()), m_listener, SLOT(stopProcess()));

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

  if (m_senderHostThread != NULL)
  {
    delete m_senderHostThread;
    m_senderHostThread = NULL;
  }

  if (m_listenerHostThread != NULL)
  {
    delete m_listenerHostThread;
    m_listenerHostThread = NULL;
  }
}

void OIGTLSocketObject::initThreads()
{
  qRegisterMetaType<OIGTLMessage::Pointer>();

  // Instanciate the mutex and the threads
  m_mutex = new QMutex();
  m_sender = new OIGTLSenderProcess();
  m_listener = new OIGTLListenerProcess();

  m_sender->setMutex(m_mutex);
  m_listener->setMutex(m_mutex);

  m_senderHostThread   = new QThreadEx();
  m_listenerHostThread = new QThreadEx();

  m_sender->moveToThread(m_senderHostThread);
  m_listener->moveToThread(m_listenerHostThread);

  bool ok;

  // Connect signals to slots
  ok =  connect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()));
  ok &= connect(m_sender, SIGNAL(cannotConnectToRemote()), this, SLOT(cannotConnectToRemote()));
  ok &= connect(m_sender, SIGNAL(disconnectedFromRemote(bool )), this, SLOT(disconnectedFromRemote(bool )));
  ok &= connect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()));
  ok &= connect(m_sender, SIGNAL(messageSent(unsigned long long )), this, SIGNAL(messageSent(unsigned long long )));
  ok &= connect(this, SIGNAL(messageToSend(OIGTLMessage::Pointer)), m_sender, SLOT(sendMsg(OIGTLMessage::Pointer)));
   
  ok &= connect(m_senderHostThread, SIGNAL(eventloopStarted()), m_sender, SLOT(startProcess()));
  ok &= connect(this, SIGNAL(shutdownSender()), m_sender, SLOT(stopProcess()));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  ok &= connect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()));
  ok &= connect(m_listener, SIGNAL(clientDisconnected(bool )), this, SLOT(clientDisconnected(bool )));
  ok &= connect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SIGNAL(messageReceived(OIGTLMessage::Pointer)));
  
  ok &= connect(m_listenerHostThread, SIGNAL(eventloopStarted()), m_listener, SLOT(startProcess()));
  ok &= connect(this, SIGNAL(shutdownListener()), m_listener, SLOT(stopProcess()));

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

      // Attempt to start the host thread
      m_listenerHostThread->start();
      
      // Wait until the thread and it's event loop really started
      while (!m_listenerHostThread->isEventloopRunning())
      {
        m_listenerHostThread->msleepEx(100);

        // Make sure that our signals are being processed
        QCoreApplication::processEvents();
      }

      QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";

      // Set flag to indicate that the socket is busy
      m_active = true;

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
      m_senderHostThread->start();

      while (!m_senderHostThread->isEventloopRunning())
      {
        m_senderHostThread->msleepEx(100);

        // Make sure that our signals are being processed
        QCoreApplication::processEvents();
      }

      QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";
      QLOG_INFO() <<objectName() <<": " <<"Connecting to: " <<address_str.c_str() <<" : " <<port <<endl;

      // Set flag to indicate that the socket is busy
      m_active = true;

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
    emit shutdownSender();
    QCoreApplication::processEvents();

    while (m_sender->isActive())
      m_senderHostThread->msleepEx(250);
  }

  // Shut down the listener thread
  if (m_listener != NULL)
  {
    emit shutdownListener();
    QCoreApplication::processEvents();
    
    while (m_listener->isActive())
      m_listenerHostThread->msleepEx(250);
  }

  m_senderHostThread->exit(0);
  //m_senderHostThread->terminate();

  m_listenerHostThread->exit(0);
  //m_listenerHostThread->terminate();


  m_port = -1;
  m_listening = false;
  m_connectedToRemote = false;
 
  m_clientConnected = false;
  m_ableToSend = false;

  // Socket finally terminted, set flag accordingly
  m_active = false;

  QLOG_INFO() <<objectName() <<": " <<"Closing socket, threads terminated.";
}

void OIGTLSocketObject::sendMessage(OIGTLMessage::Pointer msg)
{
  // Do not add the message to the message queue if the socket is not connected 
  // (otherwise messages will pile up and use up memory)
  if (m_sender != NULL && msg.operator !=(NULL) && (m_clientConnected || m_connectedToRemote) )
  {
    //m_sender->sendMsg(msg);
    emit messageToSend(msg);
    QCoreApplication::processEvents();
  }
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

    // Time to set up a listener on the socket
    if (m_listener->initialize(m_sender->getSocketPointer(), m_port) == true)
      m_listenerHostThread->start();
    else return;

    emit connectedToRemoteSignal();
    QCoreApplication::processEvents();
  }
}

void OIGTLSocketObject::cannotConnectToRemote(void)
{
  //Could not establish connection to remote host
  m_connectedToRemote = false;
  m_ableToSend = false;
  m_port = -1;

  // This signal notifies the outside world that it isn't possible to connect to the given host
  emit cannotConnectToRemoteSignal();
  QCoreApplication::processEvents();
}

void OIGTLSocketObject::disconnectedFromRemote(bool onPort)
{
  // We were successfully connected to a remote host (onPort = true) but the the connection lost
  // Need to stop the sender thread, and also the listener which was listening on the same socket

  if (onPort)
  {
    if (m_sender != NULL)
      emit shutdownSender();

    if (m_listener != NULL)
      emit shutdownListener();

    QCoreApplication::processEvents();
  }
  // There was a client connecting to the local listener, but we cannot send messages through the socket any more
  else
  {
    if (m_sender != NULL)
      emit shutdownSender();

    QCoreApplication::processEvents();
  }

  m_connectedToRemote = false;
  m_ableToSend = false;

}

void OIGTLSocketObject::clientConnected(void)
{
  // A new client has connected to the local listener
  if (m_listener != NULL && m_listener->isInitialized())
  {
    m_sender->setObjectName(this->objectName().append("_S"));

    // Time to set up a sender on the socket
    if (m_sender->initialize(m_listener->getSocketPointer(), m_port) == true)
    {
      m_ableToSend = true;
      m_senderHostThread->start();
    }
    else return;
    
    m_clientConnected = true;
    emit clientConnectedSignal();

    QCoreApplication::processEvents();
  }
}

void OIGTLSocketObject::clientDisconnected(bool onPort)
{
  if (onPort)
  {
    if (m_sender != NULL)
      emit shutdownSender();

    emit clientDisconnectedSignal();
  }

  QCoreApplication::processEvents();
  
  m_clientConnected = false;
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

void OIGTLSocketObject::initializeWinTimers()
{
   // Typedef functions to hold what is in the DLL
  FunctionPtr_SETRES _NtSetTimerResolution;
  FunctionPtr_GETRES _NtQueryTimerResolution;

  // Use LoadLibrary used to load ntdll
  HINSTANCE hInstLibrary = LoadLibrary("ntdll.dll");

  if (hInstLibrary)
  {
    // the DLL is loaded and ready to go.
    _NtSetTimerResolution = (FunctionPtr_SETRES)GetProcAddress(hInstLibrary, "NtSetTimerResolution");
    _NtQueryTimerResolution = (FunctionPtr_GETRES)GetProcAddress(hInstLibrary, "NtQueryTimerResolution");

    if (_NtSetTimerResolution)
    {
      uint DesiredResolution = 5000;
      bool SetResolution= true;
      ULONG MinResolution = 0;
      ULONG MaxResolution = 0;
      ULONG CurrentResolution = 0;

      NTSTATUS status;

      status = _NtQueryTimerResolution(&MinResolution, &MaxResolution, &CurrentResolution);
      //QLOG_INFO() <<"Current Clock Resolution - Before: " <<CurrentResolution <<std::endl;

      status = _NtSetTimerResolution(MaxResolution, SetResolution, &CurrentResolution);

      status = _NtQueryTimerResolution(&MinResolution, &MaxResolution, &CurrentResolution);
      //QLOG_INFO() <<"Current Clock Resolution - After: " <<CurrentResolution <<std::endl;
    }
  }

  ULONGLONG tscfreq = 0;
  ULONGLONG tscsd = 0;
  ULONGLONG ugly_hack_offset = 0;

  QSettings settings("Niftk", "NiftyLink-ptp");

  QDateTime last_calTime;
  last_calTime = settings.value("tcslastupdate", 0).toDateTime();

  QDateTime nowdt = QDateTime::currentDateTime();

  if (last_calTime.secsTo(nowdt) > 6000)
  {
    // calibrate clock frequency counter
    calibrate_clock_freq();

    last_calTime = QDateTime::currentDateTime();

    get_tsctparam(tscfreq, tscsd, ugly_hack_offset);

    settings.setValue("tscfreq", tscfreq);
    settings.setValue("tscsd", tscsd);
    settings.setValue("ugly_hack_offset", ugly_hack_offset);
    settings.setValue("tcslastupdate", last_calTime);
  }
  else
  {
    tscfreq = settings.value("tscfreq", 500000000uI64).toULongLong();
    tscsd = settings.value("tscsd", 500000000uI64).toULongLong();
    ugly_hack_offset = settings.value("ugly_hack_offset", 0).toULongLong();

    set_tsctparam(tscfreq, tscsd, ugly_hack_offset);
  }
}

#endif
