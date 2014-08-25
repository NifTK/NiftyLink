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

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpClient::NiftyLinkTcpClient(QObject *parent)
: QObject(parent)
, m_Socket(NULL)
, m_Worker(NULL)
, m_Thread(NULL)
, m_Connected(false)
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

  // This destructor can be called
  // a) When the client was happily running and processing.
  // b) When the client was already disconnected. This means all the "im disconnected" signals
  //    will have been emitted, and a result of which might be that something external decides to delete us.
  if (m_Connected)
  {
    this->DisconnectFromHost();
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::IsConnected() const
{
  // Here, prefer to check oneself first, as one always has most up-to-date knowledge of oneself.
  return m_Connected && m_Worker != NULL && m_Worker->IsSocketConnected();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  assert(this->IsConnected());
  m_Worker->SetNumberMessageReceivedThreshold(threshold);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetKeepAliveOn(bool isOn)
{
  assert(this->IsConnected());
  m_Worker->SetKeepAliveOn(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::SetCheckForNoIncomingData(bool isOn)
{
  assert(this->IsConnected());
  m_Worker->SetCheckForNoIncomingData(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OutputStats()
{
  assert(this->IsConnected());
  m_Worker->OutputStatsToConsole();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::RequestStats()
{
  assert(this->IsConnected());
  m_Worker->RequestStats();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::DisconnectFromHost()
{
  assert(this->IsConnected());
  m_Worker->RequestDisconnectSocket();

  while(this->IsConnected())
  {
    QCoreApplication::processEvents();
  }

}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::Send(NiftyLinkMessageContainer::Pointer message)
{
  assert(this->IsConnected());
  return m_Worker->Send(message);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnMessageReceived(int portNumber)
{
  NiftyLinkMessageContainer::Pointer msg = m_InboundMessages.GetContainer(portNumber);
  emit MessageReceived(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnError()
{
  // Remember, this method is called when the socket is registered with this class, and being processing in this event loop.
  QLOG_ERROR() << QObject::tr("%1::OnError() - code=%2, string=%3").arg(objectName()).arg(m_Socket->error()).arg(m_Socket->errorString());
  emit SocketError(this->m_RequestedName, this->m_RequestedPort, m_Socket->error(), m_Socket->errorString());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnWorkerSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  // Remember, this method is called when the socket has moved into the worker object, and being processed in the worker thread.
  QLOG_ERROR() << QObject::tr("%1::OnWorkerSocketError() - code=%2, string=%3").arg(objectName()).arg(errorCode).arg(errorString);
  emit SocketError(this->m_RequestedName, this->m_RequestedPort, errorCode, errorString);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::RaiseInternalError(const QString& errorMessage)
{
  QLOG_ERROR() << QObject::tr("%1::RaiseInternalError() - message=%2").arg(objectName()).arg(errorMessage);
  emit ClientError(this->m_RequestedName, this->m_RequestedPort, errorMessage);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::ConnectToHost(const QString& hostName, quint16 portNumber)
{
  m_RequestedName = hostName;
  m_RequestedPort = portNumber;

  m_Socket = new QTcpSocket();

  connect(m_Socket, SIGNAL(connected()), this, SLOT(OnConnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));

  // There are no errors reported from this. Listen to the error signal, see OnError().
  m_Socket->connectToHost(m_RequestedName, m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnDisconnected()
{
  m_Connected = false;

  QLOG_INFO() << QObject::tr("%1::OnDisconnected() - worker disconnected.").arg(objectName());
  emit Disconnected(this->m_RequestedName, this->m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnConnected()
{
  m_Connected = true;

  this->setObjectName(QObject::tr("NiftyLinkTcpClient(%1:%2)").arg(m_Socket->peerName()).arg(m_Socket->peerPort()));
  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  // We disconnect from Qt signal/slots (not the remote client).
  // Then the m_Worker takes charge of the socket.
  m_Socket->disconnect();

  m_Worker = new NiftyLinkTcpNetworkWorker("NiftyLinkTcpClientWorker", &m_InboundMessages, &m_OutboundMessages, m_Socket);
  connect(m_Worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SLOT(OnWorkerSocketError(int,QAbstractSocket::SocketError,QString)));
  connect(m_Worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
  connect(m_Worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
  connect(m_Worker, SIGNAL(SocketDisconnected()), this, SLOT(OnDisconnected()), Qt::BlockingQueuedConnection);

  m_Thread = new NiftyLinkQThread();
  connect(m_Thread, SIGNAL(finished()), m_Thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

  m_Worker->moveToThread(m_Thread);
  m_Socket->moveToThread(m_Thread);
  m_Thread->start();

  emit Connected(m_Socket->peerName(), m_Socket->peerPort());
}

} // end niftk namespace
