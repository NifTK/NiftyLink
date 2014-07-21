/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkClient.h"

#include "NiftyLinkClientProcess.h"
#include "NiftyLinkClientSocket.h"
#include <NiftyLinkQThread.h>
#include <NiftyLinkUtils.h>

#include <QsLog.h>
#include <cassert>
#include <stdexcept>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkClient::NiftyLinkClient(QObject *parent)
{
  this->setObjectName("NiftyLinkClient");
  m_Thread = new NiftyLinkQThread();
  m_ClientProcess = new NiftyLinkClientProcess();
  connect(m_ClientProcess, SIGNAL(ConnectedToRemote()), this, SLOT(OnProcessesConnectedToRemote()));
  connect(m_ClientProcess, SIGNAL(CantReachRemote()), this, SLOT(OnCantReachRemote()));
  connect(m_ClientProcess, SIGNAL(NoResponseFromRemote()), this, SLOT(OnNoResponseFromRemote()));
  connect(m_ClientProcess, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
  connect(m_ClientProcess, SIGNAL(MessageReceived(niftk::NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer)));
}


//-----------------------------------------------------------------------------
NiftyLinkClient::~NiftyLinkClient()
{
  if (m_ClientProcess != NULL)
  {
    delete m_ClientProcess;
  }
  if (m_Thread != NULL)
  {
    delete m_Thread;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::SetTimeToWaitToConnectToServer(const int& msec)
{
  this->m_ClientProcess->SetConnectionTimeOut(msec);
}


//-----------------------------------------------------------------------------
int NiftyLinkClient::GetTimeToWaitToConnectToServer()
{
  return this->m_ClientProcess->GetConnectionTimeOut();
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::Start(const QUrl& url)
{
  QString address;

  if (ValidateIp(url.host()))
  {
    address = url.host();
  }
  else
  {
    QString ip = ResolveHostName(url.host());

    if (ValidateIp(ip))
    {
      address = ip;
    }
  }

  if (address.size() == 0)
  {
    QString errorMessage = QObject::tr("%1::Start(%2) - Cannot validate Url host name").arg(objectName()).arg(url.toString());

    QLOG_ERROR() << errorMessage;
    throw std::invalid_argument(errorMessage.toStdString());
  }

  int port = url.port();
  if (port <= 0)
  {
    QString errorMessage = QObject::tr("%1::Start(%2) - Invalid port.").arg(objectName()).arg(port);

    QLOG_ERROR() << errorMessage;
    throw std::invalid_argument(errorMessage.toStdString());
  }
  assert(m_ClientProcess != NULL);

  // Initialises the socket... Note that in contrast to
  // the server side, this does not create it. Creation is
  // deferred until the DoProcessing method, because you
  // need to immediately start sending keep-alive requests.
  m_ClientProcess->Initialise(address, port);

  // For logging.
  this->setObjectName(QObject::tr("NiftyLinkClient(%1:%2)").arg(address).arg(port));

  // Setup thread.
  connect(m_Thread, SIGNAL(EventLoopStarted()), this, SLOT(OnStartProcessing()));
  m_ClientProcess->moveToThread(m_Thread);
  m_Thread->start();

  // Wait until the thread and it's event loop really started
  while (!m_Thread->IsEventLoopRunning())
  {
    NiftyLinkQThread::SleepCallingThread(100);
  }

  QLOG_INFO() << objectName() << " - NiftyLinkClientProcess thread successfully initialized";
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::Send(igtl::MessageBase::Pointer msg)
{
  m_ClientProcess->Send(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::Stop()
{
  m_ClientProcess->StopProcessing();
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnStartProcessing()
{
  m_ClientProcess->StartProcessing();
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnProcessesConnectedToRemote()
{
  emit ConnectedToServer();
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg)
{
  emit MessageReceived(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  emit MessageSent(startTimeInNanoseconds, endTimeInNanoseconds);
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnCantReachRemote()
{
  emit FailedToSendKeepAliveMessage();
}


//-----------------------------------------------------------------------------
void NiftyLinkClient::OnNoResponseFromRemote()
{
  emit NoIncommingData();
}

} // end namespace niftk
