/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkSocketObject.h"

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkSocketObject::NiftyLinkSocketObject(QObject *parent)
  : QObject(parent)
{
  m_Port = -1;
  m_Listening = false;
  m_ConnectedToRemote = false;

  m_ClientConnected = false;
  m_AbleToSend = false;

  m_Active = false;
  m_Initialized = false;

  m_Sender = NULL;
  m_Listener = NULL;
  m_Mutex = NULL;

  m_SenderHostThread = NULL;
  m_ListenerHostThread = NULL;

  InitThreads();
}


//-----------------------------------------------------------------------------
NiftyLinkSocketObject::~NiftyLinkSocketObject(void)
{
  QLOG_INFO() << "Destructing" << objectName();

  // Call CloseSocket, just in case
  this->CloseSocket();

  if (m_Sender != NULL)
  {
    // Disconnect signals off sender
    disconnect(m_Sender, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(OnConnectedToRemote()) );
    disconnect(m_Sender, SIGNAL(CannotConnectToRemoteSignal()), this, SLOT(OnCannotConnectToRemote()) );
    disconnect(m_Sender, SIGNAL(DisconnectedFromRemoteSignal(bool )), this, SLOT(OnDisconnectedFromRemote(bool )) );
    disconnect(m_Sender, SIGNAL(SendingFinishedSignal()), this, SIGNAL(SendingFinishedSignal()) );
    disconnect(m_Sender, SIGNAL(MessageSentSignal(unsigned long long )), this, SIGNAL(MessageSentSignal(unsigned long long )));
    disconnect(this, SIGNAL(MessageToSendSignal(NiftyLinkMessage::Pointer)), m_Sender, SLOT(AddMsgToSendQueue(NiftyLinkMessage::Pointer)));

    disconnect(m_SenderHostThread, SIGNAL(EventloopStarted()), m_Sender, SLOT(StartProcess()));
    disconnect(this, SIGNAL(ShutdownSenderSignal()), m_Sender, SLOT(StopProcess()));
    disconnect(m_Sender, SIGNAL(ShutdownHostThreadSignal()), m_SenderHostThread, SLOT(quit()));


    // Delete sender
    delete m_Sender;
    m_Sender = NULL;
  }

  if (m_Listener != NULL)
  {
    // Disconnect signals off listener
    disconnect(m_Listener, SIGNAL(ClientConnectedSignal()), this, SLOT(OnClientConnected()) );
    disconnect(m_Listener, SIGNAL(ClientDisconnectedSignal(bool )), this, SLOT(OnClientDisconnected(bool )) );
    disconnect(m_Listener, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)) );

    disconnect(m_ListenerHostThread, SIGNAL(EventloopStarted()), m_Listener, SLOT(StartProcess()));
    disconnect(this, SIGNAL(ShutdownListenerSignal()), m_Listener, SLOT(StopProcess()));
    disconnect(m_Listener, SIGNAL(ShutdownHostThreadSignal()), m_ListenerHostThread, SLOT(quit()));

    // Delete listener
    delete m_Listener;
    m_Listener = NULL;
  }

  // Delete the mutex too
  if (m_Mutex != NULL)
  {
    delete m_Mutex;
    m_Mutex = NULL;
  }

  if (m_SenderHostThread != NULL)
  {
    delete m_SenderHostThread;
    m_SenderHostThread = NULL;
  }

  if (m_ListenerHostThread != NULL)
  {
    delete m_ListenerHostThread;
    m_ListenerHostThread = NULL;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::InitThreads()
{
  qRegisterMetaType<NiftyLinkMessage::Pointer>();

  // Instanciate the mutex and the threads
  m_Mutex = new QMutex();
  m_Sender = new NiftyLinkSenderProcess();
  m_Listener = new NiftyLinkListenerProcess();

  m_Sender->SetMutex(m_Mutex);
  m_Listener->SetMutex(m_Mutex);

  m_SenderHostThread = new NiftyLinkQThread();
  m_ListenerHostThread = new NiftyLinkQThread();

  m_Sender->moveToThread(m_SenderHostThread);
  m_Listener->moveToThread(m_ListenerHostThread);

  bool ok;

  // Connect signals to slots
  ok = connect(m_Sender, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(OnConnectedToRemote()));
  ok &= connect(m_Sender, SIGNAL(CannotConnectToRemoteSignal()), this, SLOT(OnCannotConnectToRemote()));
  ok &= connect(m_Sender, SIGNAL(DisconnectedFromRemoteSignal(bool )), this, SLOT(OnDisconnectedFromRemote(bool )));
  ok &= connect(m_Sender, SIGNAL(SendingFinishedSignal()), this, SIGNAL(SendingFinishedSignal()));
  ok &= connect(m_Sender, SIGNAL(MessageSentSignal(unsigned long long )), this, SIGNAL(MessageSentSignal(unsigned long long )));
  ok &= connect(this, SIGNAL(MessageToSendSignal(NiftyLinkMessage::Pointer)),
                m_Sender, SLOT(AddMsgToSendQueue(NiftyLinkMessage::Pointer)), Qt::DirectConnection);

  ok &= connect(m_SenderHostThread, SIGNAL(EventloopStarted()), m_Sender, SLOT(StartProcess()));
  ok &= connect(this, SIGNAL(ShutdownSenderSignal()), m_Sender, SLOT(StopProcess()));
  ok &= connect(m_Sender, SIGNAL(ShutdownHostThreadSignal()), m_SenderHostThread, SLOT(quit()));

  ok &= connect(m_Sender, SIGNAL(SendMessageAccessTimes(QString)), this, SIGNAL(SendMessageAccessTimes(QString)));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  ok &= connect(m_Listener, SIGNAL(ClientConnectedSignal()), this, SLOT(OnClientConnected()));
  ok &= connect(m_Listener, SIGNAL(ClientDisconnectedSignal(bool )), this, SLOT(OnClientDisconnected(bool )));
  ok &= connect(m_Listener, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)),
                this, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), Qt::DirectConnection);

  ok &= connect(m_ListenerHostThread, SIGNAL(EventloopStarted()), m_Listener, SLOT(StartProcess()));
  ok &= connect(this, SIGNAL(ShutdownListenerSignal()), m_Listener, SLOT(StopProcess()));
  ok &= connect(m_Listener, SIGNAL(ShutdownHostThreadSignal()), m_ListenerHostThread, SLOT(quit()));

  ok &= connect(m_Listener, SIGNAL(SendMessageAccessTimes(QString)), this, SIGNAL(SendMessageAccessTimes(QString)));

  // Set the flag
  if (m_Mutex != NULL && m_Sender != NULL && m_Listener != NULL && ok)
  {
    m_Initialized = true;
  }

}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::SetObjectNames(QString name)
{
  QString tmp;
  tmp.append(name);

  this->setObjectName(name);

  if (m_Sender != NULL)
  {
    m_Sender->setObjectName(tmp.append("_S"));
  }

  if (m_SenderHostThread != NULL)
  {
    m_SenderHostThread->setObjectName(tmp.append("H"));
  }

  tmp.clear();
  tmp.append(name);

  if (m_Listener != NULL)
  {
    m_Listener->setObjectName(tmp.append("_L"));
  }

  if (m_ListenerHostThread != NULL)
  {
    m_ListenerHostThread->setObjectName(tmp.append("H"));
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkSocketObject::ListenOnPort(int port)
{
  if (!m_Initialized)
  {
    return false;
  }

  if (m_Listener != NULL && m_Initialized)
  {
    // Set parameters
    m_Listener->setObjectName(this->objectName().append("_L"));

    // Initialize the port
    bool ok = m_Listener->Initialize(port);

    if (ok)
    {
      // Start the thread
      m_Port = port;
      m_Listening = true;

      // Attempt to start the host thread
      m_ListenerHostThread->start();

      // Wait until the thread and it's event loop really started
      while (!m_ListenerHostThread->IsEventLoopRunning())
      {
        m_ListenerHostThread->SleepCallingThread(100);

        // Make sure that our signals are being processed
        QCoreApplication::processEvents();
      }

      QLOG_INFO() << objectName() << ": " << "Threads successfully initialized";

      // Set flag to indicate that the socket is busy
      m_Active = true;

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkSocketObject::ConnectToRemote(QUrl url)
{
  if (!m_Initialized)
  {
    return false;
  }

  if (m_Sender != NULL && m_Initialized)
  {
    // Set parameters
    m_Sender->setObjectName(this->objectName().append("_S"));

    std::string address_str;
    address_str.clear();

    if (ValidateIp(url.host()) == true)
    {
      address_str = url.host().toStdString();
    }
    else
    {
      QString ip = ResolveHostName(url.host());

      if (ValidateIp(ip))
      {
        address_str = ip.toStdString();
      }
      else
      {
        return false;
      }
    }

    int port = url.port();

    // Initialize the sender
    bool ok = m_Sender->Initialize(address_str, port);

    if (ok)
    {
      m_Port = port;

      // Start the thread
      m_SenderHostThread->start();

      while (!m_SenderHostThread->IsEventLoopRunning())
      {
        m_SenderHostThread->SleepCallingThread(100);

        // Make sure that our signals are being processed
        QCoreApplication::processEvents();
      }

      QLOG_INFO() << objectName() << ": " << "Threads successfully initialized";
      QLOG_INFO() << objectName() << ": " << "Connecting to: " << address_str.c_str() << " : " << port << endl;

      // Set flag to indicate that the socket is busy
      m_Active = true;

      return true;
    }
    else
    {
      return false;
    }

  }
  else
  {
    return false;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::CloseSocket(void)
{
  // Shuts down the sender thread
  if (m_Sender != NULL)
  {
    emit ShutdownSenderSignal();
    QCoreApplication::processEvents();

    while (m_Sender->IsActive())
    {
      m_SenderHostThread->SleepCallingThread(250);
    }
  }

  // Shut down the listener thread
  if (m_Listener != NULL)
  {
    emit ShutdownListenerSignal();
    QCoreApplication::processEvents();

    while (m_Listener->IsActive())
    {
      m_ListenerHostThread->SleepCallingThread(250);
    }
  }

  if (m_SenderHostThread != NULL)
  {
    //Terminate sender host thread
    m_SenderHostThread->exit(0);
    m_SenderHostThread->wait(500);

    //If it is still running (like usually on cmicdev) then forcibly terminate
    if (m_SenderHostThread->isRunning())
    {
      m_SenderHostThread->terminate();
    }
  }

  if (m_ListenerHostThread != NULL)
  {
    //Terminate listener host thread
    m_ListenerHostThread->exit(0);
    m_ListenerHostThread->wait(500);

    //If it is still running (like usually on cmicdev) then forcibly terminate
    if (m_ListenerHostThread->isRunning())
    {
      m_ListenerHostThread->terminate();
    }
  }

  m_Port = -1;
  m_Listening = false;
  m_ConnectedToRemote = false;
  m_ClientConnected = false;
  m_AbleToSend = false;

  // Socket finally terminted, set flag accordingly
  m_Active = false;

  bool senderOn = false, listenerOn = false;

  if (m_SenderHostThread != NULL)
  {
    senderOn = m_SenderHostThread->isRunning();
    QLOG_INFO() << objectName() << ": " << "Sender host thread running: " << m_SenderHostThread->isRunning();
  }

  if (m_ListenerHostThread != NULL)
  {
    listenerOn = m_ListenerHostThread->isRunning();
    QLOG_INFO() << objectName() << ": " << "Listener host thread running: " << m_ListenerHostThread->isRunning();
  }

  if (!senderOn && !listenerOn)
  {
    QLOG_INFO() << objectName() << ": " << "Closing socket, threads terminated.";
  }
  else
  {
    QLOG_ERROR() << objectName() << ": " << "Threads DID NOT terminate when closing socket!";
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::SendMessage(NiftyLinkMessage::Pointer msg)
{
  // Do not add the message to the message queue if the socket is not connected
  // (otherwise messages will pile up and use up memory)
  // FIXME: this does not seem to work reliably. i can still trigger situations where the
  //        socket becomes disconnected (various gui bits notice) but more and more data is
  //        piled up in the send queue until the process dies with out-of-memory.
  if (m_Sender != NULL && msg.operator != (NULL) && (m_ClientConnected || m_ConnectedToRemote) )
  {
    msg->Pack();
    //m_Sender->AddMsgToSendQueue(msg);
    emit MessageToSendSignal(msg);

    // do not call processEvents()! not ever, never!
    // this will end up overflowing the call stack at some point because some signals
    // will be delivered synchronously, re-entering this method, entering the event loop,
    // deliverying signals, re-entering this method, and so on. crash.
    // BUT: leave it here until https://cmicdev.cs.ucl.ac.uk/trac/ticket/3025 is being worked on!
    //QCoreApplication::processEvents();
  }
}

//*********************************************************
// INTERNAL SLOTS
//*********************************************************


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::OnConnectedToRemote(void)
{
  //Sender established connection to remote host, need to set up listener on socket
  if (m_Sender != NULL && m_Sender->IsInitialized())
  {
    m_ConnectedToRemote = true;
    m_AbleToSend = true;

    m_Listener->setObjectName(this->objectName().append("_L"));

    // Time to set up a listener on the socket
    if (m_Listener->Initialize(m_Sender->GetSocketPointer(), m_Port) == true)
    {
      m_ListenerHostThread->start();
    }
    else
    {
      return;
    }

    emit ConnectedToRemoteSignal();
    //QCoreApplication::processEvents();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::OnCannotConnectToRemote(void)
{
  //Could not establish connection to remote host
  m_ConnectedToRemote = false;
  m_AbleToSend = false;
  m_Port = -1;

  // This signal notifies the outside world that it isn't possible to connect to the given host
  emit CannotConnectToRemoteSignal();
  //QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::OnDisconnectedFromRemote(bool onPort)
{
  // We were successfully connected to a remote host (onPort = true) but the the connection lost
  // Need to stop the sender thread, and also the listener which was listening on the same socket

  if (onPort)
  {
    if (m_Sender != NULL)
    {
      emit ShutdownSenderSignal();
    }

    if (m_Listener != NULL)
    {
      emit ShutdownListenerSignal();
    }

    emit LostConnectionToRemoteSignal();

    //QCoreApplication::processEvents();
  }
  // There was a client connecting to the local listener, but we cannot send messages through the socket any more
  else
  {
    if (m_Sender != NULL)
    {
      emit ShutdownSenderSignal();
    }

    //QCoreApplication::processEvents();
  }

  m_ConnectedToRemote = false;
  m_AbleToSend = false;
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::OnClientConnected(void)
{
  // A new client has connected to the local listener
  if (m_Listener != NULL && m_Listener->IsInitialized())
  {
    m_Sender->setObjectName(this->objectName().append("_S"));

    // Time to set up a sender on the socket
    if (m_Sender->Initialize(m_Listener->GetSocketPointer(), m_Port) == true)
    {
      m_AbleToSend = true;
      m_SenderHostThread->start();
    }
    else
    {
      return;
    }

    m_ClientConnected = true;
    emit ClientConnectedSignal();

    //QCoreApplication::processEvents();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::OnClientDisconnected(bool onPort)
{
  if (onPort)
  {
    if (m_Sender != NULL)
    {
      emit ShutdownSenderSignal();
      //QCoreApplication::processEvents();
    }


    emit ClientDisconnectedSignal();
    //QCoreApplication::processEvents();
  }

  m_ClientConnected = false;
  m_AbleToSend = false;
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::CatchMsgSignal(NiftyLinkMessage::Pointer msg)
{
  //QLOG_INFO() <<objectName() <<": " <<"Message signal received";
  if (msg.operator != (NULL))
  {
    emit MessageReceivedSignal(msg);
  }
}

//*********************************************************
// GETTING / SETTING PARAMETERS
//*********************************************************

//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::SetConnectTimeOut(int sec)
{
  if (m_Sender != NULL)
  {
    m_Sender->SetConnectTimeOut(sec);
  }
}


//-----------------------------------------------------------------------------
int NiftyLinkSocketObject::GetConnectTimeOut(void)
{
  if (m_Sender != NULL)
  {
    return m_Sender->GetConnectTimeOut();
  }
  else
  {
    return -1;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::SetListenInterval(int msec)
{
  if (m_Listener != NULL)
  {
    m_Listener->SetListenInterval(msec);
  }
}


//-----------------------------------------------------------------------------
int NiftyLinkSocketObject::GetListenInterval(void)
{
  if (m_Listener != NULL)
  {
    return m_Listener->GetListenInterval();
  }
  else
  {
    return -1;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkSocketObject::SetSocketTimeOut(int msec)
{
  if (m_Listener != NULL)
  {
    m_Listener->SetSocketTimeOut(msec);
  }

  if (m_Sender != NULL)
  {
    m_Sender->SetSocketTimeOut(msec);
  }
}


//-----------------------------------------------------------------------------
int NiftyLinkSocketObject::GetSocketTimeOut(void)
{
  if (m_Listener != NULL)
  {
    return m_Listener->GetSocketTimeOut();
  }
  else
  {
    return -1;
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

void NiftyLinkSocketObject::InitializeWinTimers()
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
      bool SetResolution = true;
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

  // Don't need the TSC timer stuff any more
  //ULONGLONG tscfreq = 0;
  //ULONGLONG tscsd = 0;
  //ULONGLONG ugly_hack_offset = 0;

  //QSettings settings("Niftk", "NiftyLink-ptp");

  //QDateTime last_calTime;
  //last_calTime = settings.value("tcslastUpdate", 0).toDateTime();

  //QDateTime nowdt = QDateTime::currentDateTime();

  //if (last_calTime.secsTo(nowdt) > 6000)
  //{
  // // calibrate clock frequency counter
  // calibrate_clock_freq();

  // last_calTime = QDateTime::currentDateTime();

  // get_tsctparam(tscfreq, tscsd, ugly_hack_offset);

  // settings.setValue("tscfreq", tscfreq);
  // settings.setValue("tscsd", tscsd);
  // settings.setValue("ugly_hack_offset", ugly_hack_offset);
  // settings.setValue("tcslastUpdate", last_calTime);
  //}
  //else
  //{
  // tscfreq = settings.value("tscfreq", 500000000uI64).toULongLong();
  // tscsd = settings.value("tscsd", 500000000uI64).toULongLong();
  // ugly_hack_offset = settings.value("ugly_hack_offset", 0).toULongLong();

  // set_tsctparam(tscfreq, tscsd, ugly_hack_offset);
  //}
}

#endif

} // end namespace niftk
