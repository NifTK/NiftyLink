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
#include <QMutexLocker>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpClient::NiftyLinkTcpClient(QObject *parent)
: QObject(parent)
, m_State(UNCONNECTED)
, m_Socket(NULL)
, m_Worker(NULL)
, m_Thread(NULL)
{
  this->setObjectName("NiftyLinkTcpClient");
  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - creating.").arg(objectName());

  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<NiftyLinkMessageStatsContainer>("NiftyLinkMessageStatsContainer");

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  // We create all this here, so we know that m_Socket and m_Worker are always populated.
  m_Socket = new QTcpSocket();
  connect(m_Socket, SIGNAL(connected()), this, SLOT(OnConnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));
  m_Worker = new NiftyLinkTcpNetworkWorker("NiftyLinkTcpClientWorker", &m_InboundMessages, &m_OutboundMessages, m_Socket);

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - created.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::~NiftyLinkTcpClient()
{
  QString name = objectName();
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroying.").arg(name);

  if (m_State != UNCONNECTED)
  {
    // Not sure if we need this, if we are not connected yet, we could just destroy this object?
    if (m_State == CONNECTING)
    {
      while(m_State != CONNECTED)
      {
        QCoreApplication::processEvents();
        NiftyLinkQThread::SleepCallingThread(1);
      }
    }

    // We have to request, and then wait for eveything to shut-down.
    if (this->IsConnected())
    {
      // This should block/wait until the worker really is shutdown,
      // and everything disconnected, so that we really are ready to destroy this.
      this->DisconnectFromHost();
    }

    while(m_State != SHUTDOWN)
    {
      QCoreApplication::processEvents();
      NiftyLinkQThread::SleepCallingThread(1);
    }

    NiftyLinkQThread::SleepCallingThread(1000);
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(name);
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::ClientState NiftyLinkTcpClient::GetState() const
{
  QMutexLocker locker(&m_Mutex);
  return m_State;
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::IsConnected() const
{
  QMutexLocker locker(&m_Mutex);
  return m_State == CONNECTED;
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
void NiftyLinkTcpClient::RequestStats()
{
  m_Worker->RequestStats();
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpClient::Send(NiftyLinkMessageContainer::Pointer message)
{
  return m_Worker->Send(message);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::DisconnectFromHost()
{
  if (!this->IsConnected())
  {
    QLOG_WARN() << QObject::tr("%1::DisconnectFromHost() - received request to disconnect, when not connected, so I'm ignoring it.").arg(objectName());
    return;
  }

  // This triggers the shutdown process.
  m_Worker->RequestDisconnectSocket();

  // We MUST wait until the thread is shutdown, and then objects deleted.
  while(m_State != SHUTDOWN)
  {
    QCoreApplication::processEvents();
    NiftyLinkQThread::SleepCallingThread(1);
  }
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
  QMutexLocker locker(&m_Mutex);

  m_State = CONNECTING;
  m_RequestedName = hostName;
  m_RequestedPort = portNumber;

  // There are no errors reported from this. Listen to the error signal, see OnError().
  m_Socket->connectToHost(m_RequestedName, m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnConnected()
{
  QMutexLocker locker(&m_Mutex);

  this->setObjectName(QObject::tr("NiftyLinkTcpClient(%1:%2)").arg(m_Socket->peerName()).arg(m_Socket->peerPort()));
  m_Worker->UpdateObjectName();

  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  m_Thread = new NiftyLinkQThread();
  connect(m_Thread, SIGNAL(finished()), m_Thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.
  connect(m_Thread, SIGNAL(finished()), this, SLOT(OnThreadFinished()), Qt::BlockingQueuedConnection);

  m_Worker->moveToThread(m_Thread);
  m_Socket->moveToThread(m_Thread);

  connect(m_Worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SLOT(OnWorkerSocketError(int,QAbstractSocket::SocketError,QString)));
  connect(m_Worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
  connect(m_Worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
  connect(m_Worker, SIGNAL(SocketDisconnected()), this, SLOT(OnDisconnected()), Qt::BlockingQueuedConnection);

  m_Thread->start();
  m_State = CONNECTED;

  QLOG_INFO() << QObject::tr("%1::OnConnected() - socket connected.").arg(objectName());
  emit Connected(this->m_RequestedName, this->m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnDisconnected()
{
  QMutexLocker locker(&m_Mutex);
  m_State = SHUTTINGDOWN;

  QLOG_INFO() << QObject::tr("%1::OnDisconnected() - worker disconnected.").arg(objectName());
  emit Disconnected(this->m_RequestedName, this->m_RequestedPort);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnThreadFinished()
{
  QMutexLocker locker(&m_Mutex);
  m_State = SHUTDOWN;

  QLOG_INFO() << QObject::tr("%1::OnThreadFinished().").arg(objectName());
}

} // end niftk namespace
