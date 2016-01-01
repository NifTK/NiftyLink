/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkTcpServer.h"
#include <NiftyLinkUtils.h>
#include <NiftyLinkQThread.h>
#include <NiftyLinkMacro.h>

#include <QsLog.h>
#include <QMutexLocker>
#include <QTcpSocket>

#include <igtlMessageFactory.h>
#include <igtlTrackingDataMessage.h>

#include <iostream>
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpServer::NiftyLinkTcpServer(QObject *parent)
: QTcpServer(parent)
, m_SendKeepAlive(false)
, m_CheckNoIncoming(false)
{
  this->Initialise();
}


//-----------------------------------------------------------------------------
NiftyLinkTcpServer::NiftyLinkTcpServer(const QHostAddress &address, quint16 port, QObject *parent)
: QTcpServer(parent)
, m_SendKeepAlive(false)
, m_CheckNoIncoming(false)
{
  this->Initialise();

  bool success = this->listen(address, port);
  if (!success)
  {
    NiftyLinkStdExceptionMacro(std::runtime_error, << "Failed to listen on port " << port);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::Initialise()
{
  this->setObjectName("NiftyLinkTcpServer");
  QLOG_INFO() << QObject::tr("%1::Initialise() - started.").arg(objectName());

  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<niftk::NiftyLinkMessageContainer::Pointer>("niftk::NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<niftk::NiftyLinkMessageStatsContainer>("niftk::NiftyLinkMessageStatsContainer");

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  m_ReceivedCounter.setObjectName("NiftyLinkTcpServer");
  connect(&m_ReceivedCounter, SIGNAL(StatsProduced(niftk::NiftyLinkMessageStatsContainer)), this, SIGNAL(StatsProduced(niftk::NiftyLinkMessageStatsContainer)));
  connect(&m_ReceivedCounter, SIGNAL(StatsMessageProduced(QString)), this, SIGNAL(StatsMessageProduced(QString)));

  QLOG_INFO() << QObject::tr("%1::Initialise() - finished.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpServer::~NiftyLinkTcpServer()
{
  QString name = objectName();
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpServer() - destroying.").arg(name);

  if (this->GetNumberOfClientsConnected() > 0)
  {
    this->Shutdown();
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpServer() - destroyed.").arg(name);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->SetNumberMessageReceivedThreshold(threshold);
  }
  m_ReceivedCounter.SetNumberMessageReceivedThreshold(threshold);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::SetKeepAliveOn(bool isOn)
{
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->SetKeepAliveOn(m_SendKeepAlive);
  }
  m_SendKeepAlive = isOn;
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::SetCheckForNoIncomingData(bool isOn)
{
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->SetCheckForNoIncomingData(m_CheckNoIncoming);
  }
  m_CheckNoIncoming = isOn;
}


//-----------------------------------------------------------------------------
int NiftyLinkTcpServer::GetNumberOfClientsConnected()
{
  QMutexLocker locker(&m_Mutex);
  return m_Workers.size(); // items are only added/removed to this QSet on connection/disconnection.
}


//-----------------------------------------------------------------------------
int NiftyLinkTcpServer::Send(NiftyLinkMessageContainer::Pointer message)
{
  int numberSentTo = 0;
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    if (worker->IsSocketConnected())
    {
      worker->Send(message);
      numberSentTo++;
    }
    else
    {
      QLOG_ERROR() << QObject::tr("%1::Send() - discovered non-Open socket. This should not happen.").arg(objectName());
    }
  }
  return numberSentTo;
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OutputStats()
{
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->OutputStatsToConsole();
  }
  m_ReceivedCounter.OnOutputStats();
}


//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
void NiftyLinkTcpServer::incomingConnection(int socketDescriptor)
#else
void NiftyLinkTcpServer::incomingConnection(qintptr socketDescriptor)
#endif
{
  QLOG_INFO() << QObject::tr("%1::incomingConnection(%2) - creating socket.").arg(objectName()).arg(socketDescriptor);

  QTcpSocket *socket = new QTcpSocket();
  if (socket->setSocketDescriptor(socketDescriptor))
  {
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    NiftyLinkTcpNetworkWorker *worker = new NiftyLinkTcpNetworkWorker("NiftyLinkTcpServerWorker", &m_InboundMessages, &m_OutboundMessages, socket);
    worker->SetNumberMessageReceivedThreshold(m_ReceivedCounter.GetNumberMessageReceivedThreshold());
    worker->SetKeepAliveOn(m_SendKeepAlive);
    worker->SetCheckForNoIncomingData(m_CheckNoIncoming);

    connect(worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
    connect(worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
    connect(worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
    connect(worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)));
    connect(worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
    connect(worker, SIGNAL(SocketDisconnected()), this, SLOT(OnClientDisconnected()), Qt::QueuedConnection);

    QMutexLocker locker(&m_Mutex);
    m_Workers.insert(worker);

    NiftyLinkQThread *thread = new NiftyLinkQThread();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

    worker->moveToThread(thread);
    socket->moveToThread(thread);

    thread->start();

    emit ClientConnected(socketDescriptor);
  }
  else
  {
    QLOG_ERROR() << QObject::tr("%1::incomingConnection(%2) - failed to set socket descriptor.").arg(objectName()).arg(socketDescriptor);
  }

  this->setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));
  m_ReceivedCounter.setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));

  // Base class does this (regardless of errors).
  // this->addPendingConnection(socket);
  // However, I have chosen not to. We manage the construction and deletion of the socket, worker and thread.
  // If you add the socket to the list of pending connections, then the QTcpServer::~QTcpServer() will try and delete it.

  QLOG_INFO() << QObject::tr("%1::incomingConnection(%2) - created socket.").arg(objectName()).arg(socketDescriptor);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OnClientDisconnected()
{
  QMutexLocker locker(&m_Mutex);

  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - number of clients = %2.").arg(objectName()).arg(m_Workers.size());

  NiftyLinkTcpNetworkWorker *sender = qobject_cast<NiftyLinkTcpNetworkWorker*>(QObject::sender());
  if (sender == NULL)
  {
    QLOG_ERROR() << QObject::tr("%1::OnClientDisconnected() - failed to remove client %2.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
    return;
  }

  if (!m_Workers.remove(sender))
  {
    assert(false);
  }

  int portNumber = sender->GetSocket()->peerPort();

  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - client on port %2 removed, leaving %3 clients.")
                 .arg(objectName()).arg(portNumber).arg(m_Workers.size());

  emit ClientDisconnected(portNumber);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OnMessageReceived(int portNumber)
{
  NiftyLinkMessageContainer::Pointer msg = m_InboundMessages.GetContainer(portNumber);
  m_ReceivedCounter.OnMessageReceived(msg);

  emit MessageReceived(portNumber, msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::Shutdown()
{
  emit StartShutdown();
  QLOG_INFO() << QObject::tr("%1::Shutdown() - started.").arg(objectName());

  QList<NiftyLinkTcpNetworkWorker*> copyOfSet = m_Workers.toList();
  foreach (NiftyLinkTcpNetworkWorker* worker, copyOfSet)
  {
    int port = worker->GetSocket()->peerPort();

    QLOG_INFO() << QObject::tr("%1::Shutdown() - asking (%2) to disconnect.").arg(objectName()).arg(port);
    worker->RequestDisconnectSocket();
    QLOG_INFO() << QObject::tr("%1::Shutdown() - asked (%2) to disconnect.").arg(objectName()).arg(port);
  }

  while(this->GetNumberOfClientsConnected() > 0)
  {
    QCoreApplication::processEvents();
    NiftyLinkQThread::SleepCallingThread(1);
  }

  NiftyLinkQThread::SleepCallingThread(1000);

  QLOG_INFO() << QObject::tr("%1::Shutdown() - finished.").arg(objectName());
  emit EndShutdown();
}

} // end namespace niftk
