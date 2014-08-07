/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkTcpClient.h"
#include "NiftyLinkTcpNetworkWorker.h"
#include <NiftyLinkQThread.h>
#include <NiftyLinkUtils.h>

#include <igtlMessageBase.h>
#include <QsLog.h>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpClient::NiftyLinkTcpClient(QObject *parent)
: QObject(parent)
, m_Socket(NULL)
, m_Worker(NULL)
, m_Thread(NULL)
{
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<NiftyLinkMessageStatsContainer>("NiftyLinkMessageStatsContainer");

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  this->setObjectName("NiftyLinkTcpClient");

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - constructed.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::~NiftyLinkTcpClient()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroying.").arg(objectName());

  // Try to force disconnect.
  if (this->IsConnected())
  {
    this->DisconnectFromHost();
  }

  // This NiftyLinkTcpClient may be deleted by something external.
  // So, the thread that we create inside this class needs explicitly asking to stop its event loop.
  if (!m_Thread->isFinished())
  {
    m_Thread->exit(0);
    if(!m_Thread->wait(10000))
    {
      QLOG_ERROR() << QObject::tr("%1::~NiftyLinkTcpClient() - failed to shutdown worker thread.").arg(objectName());
    }
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  m_Worker->SetNumberMessageReceivedThreshold(threshold);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetKeepAliveOn(bool isOn)
{
  m_Worker->SetKeepAliveOn(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetCheckForNoIncomingData(bool isOn)
{
  m_Worker->SetCheckForNoIncomingData(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OutputStats()
{
  m_Worker->OutputStatsToConsole();
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::RequestStats()
{
  return m_Worker->RequestStats();
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::IsConnected() const
{
  return m_Worker->IsSocketOpen();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::DisconnectFromHost()
{
  m_Socket->disconnectFromHost();
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::Send(NiftyLinkMessageContainer::Pointer message)
{
  return m_Worker->Send(message);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnMessageReceived(int portNumber)
{
  NiftyLinkMessageContainer::Pointer msg = m_InboundMessages.GetContainer(portNumber);
  emit MessageReceived(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::ConnectToHost(const QString& hostName, quint16 portNumber)
{
  m_RequestedName = hostName;
  m_RequestedPort = portNumber;

  m_Socket = new QTcpSocket();
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));
  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
  connect(m_Socket, SIGNAL(disconnected()), m_Socket, SLOT(deleteLater()));
  connect(m_Socket, SIGNAL(connected()), this, SLOT(OnConnected()));

  // There are no errors reported from this. Listen to the error signal.
  m_Socket->connectToHost(m_RequestedName, m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnError()
{
  QLOG_INFO() << QObject::tr("%1::OnError() - code=%2, string=%3").arg(objectName()).arg(m_Socket->error()).arg(m_Socket->errorString());
  emit SocketError(this->m_RequestedName, this->m_RequestedPort, m_Socket->error(), m_Socket->errorString());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnDisconnected()
{
  emit Disconnected(this->m_RequestedName, this->m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnConnected()
{
  this->setObjectName(QObject::tr("NiftyLinkTcpClient(%1:%2)").arg(m_Socket->peerName()).arg(m_Socket->peerPort()));
  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  m_Worker = new NiftyLinkTcpNetworkWorker(&m_InboundMessages, &m_OutboundMessages, m_Socket);
  connect(m_Worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
  connect(m_Worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));

  m_Thread = new NiftyLinkQThread();
  connect(m_Thread, SIGNAL(finished()), m_Thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.
  connect(m_Thread, SIGNAL(finished()), m_Worker, SLOT(deleteLater()));

  m_Worker->moveToThread(m_Thread);
  m_Socket->moveToThread(m_Thread);
  m_Thread->start();

  emit Connected(m_Socket->peerName(), m_Socket->peerPort());
}


} // end niftk namespace
