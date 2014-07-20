/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkServer.h"
#include "NiftyLinkServerProcess.h"
#include <NiftyLinkQThread.h>

#include <QsLog.h>

//-----------------------------------------------------------------------------
NiftyLinkServer::NiftyLinkServer(QObject * /* parent */)
: m_Thread(NULL)
, m_ServerProcess(NULL)
{
  this->setObjectName("NiftyLinkServer");
  m_Thread = new NiftyLinkQThread();
  m_ServerProcess = new NiftyLinkServerProcess();
  connect(m_ServerProcess, SIGNAL(ClientConnectedSignal()), this, SLOT(OnClientConnected()));
  connect(m_ServerProcess, SIGNAL(CantReachRemote()), this, SLOT(OnCantSendToClient()));
  connect(m_ServerProcess, SIGNAL(NoResponseFromRemote()), this, SLOT(OnClientDisconnected()));
  connect(m_ServerProcess, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
  connect(m_ServerProcess, SIGNAL(MessageReceived(NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessageContainer::Pointer)));
}


//-----------------------------------------------------------------------------
NiftyLinkServer::~NiftyLinkServer()
{
  if (m_ServerProcess != NULL)
  {
    delete m_ServerProcess;
  }
  if (m_Thread != NULL)
  {
    delete m_Thread;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::SetTimeToWaitForClientConnections(const int& msec)
{
  m_ServerProcess->SetListenInterval(msec);
}


//-----------------------------------------------------------------------------
int NiftyLinkServer::GetTimeToWaitForClientConnections()
{
  return m_ServerProcess->GetListenInterval();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::Start(const int& portNumber)
{
  // Starts up the Server socket.
  m_ServerProcess->Initialise(portNumber);

  // For logging. We do this once we have initialised, as then the portNumber is assumed to be valid.
  this->setObjectName(QObject::tr("NiftyLinkServer(%1)").arg(portNumber));

  // Setup thread.
  connect(m_Thread, SIGNAL(EventLoopStarted()), this, SLOT(StartProcessing()));
  m_ServerProcess->moveToThread(m_Thread);
  m_Thread->start();

  // Wait until the thread and it's event loop really started
  while (!m_Thread->IsEventLoopRunning())
  {
    m_Thread->MsleepEx(100);

    // Make sure that our signals are being processed
    QCoreApplication::processEvents();
  }

  QLOG_INFO() << objectName() << " - NiftyLinkServerProcess thread successfully initialized.";
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::StartProcessing()
{
  m_ServerProcess->StartProcessing();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::Send(igtl::MessageBase::Pointer msg)
{
  m_ServerProcess->Send(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::Stop()
{
  m_ServerProcess->StopProcessing();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::OnClientConnected()
{
  emit ClientConnected();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::OnCantSendToClient()
{
  emit CantSendToClient();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::OnClientDisconnected()
{
  emit ClientDisconnected();
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::OnMessageReceived(NiftyLinkMessageContainer::Pointer msg)
{
  emit MessageReceived(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkServer::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  emit MessageSent(startTimeInNanoseconds, endTimeInNanoseconds);
}

