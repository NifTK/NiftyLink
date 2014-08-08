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
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<NiftyLinkMessageStatsContainer>("NiftyLinkMessageStatsContainer");

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  this->setObjectName("NiftyLinkTcpServer");

  m_ReceivedCounter.setObjectName("NiftyLinkTcpServer");
  connect(&m_ReceivedCounter, SIGNAL(StatsProduced(NiftyLinkMessageStatsContainer)), this, SIGNAL(StatsProduced(NiftyLinkMessageStatsContainer)));
  connect(&m_ReceivedCounter, SIGNAL(StatsMessageProduced(QString)), this, SIGNAL(StatsMessageProduced(QString)));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpServer() - created.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpServer::~NiftyLinkTcpServer()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpServer() - destroying.").arg(objectName());
  this->Shutdown();
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpServer() - destroyed.").arg(objectName());
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
    if (worker->IsSocketOpen())
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
void NiftyLinkTcpServer::incomingConnection(int socketDescriptor)
{
  QLOG_INFO() << QObject::tr("%1::incomingConnection(%2) - creating socket.").arg(objectName()).arg(socketDescriptor);

  QTcpSocket *socket = new QTcpSocket();
  connect(socket, SIGNAL(connected()), this, SLOT(OnClientConnected()));

  if (socket->setSocketDescriptor(socketDescriptor))
  {
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    NiftyLinkTcpNetworkWorker *worker = new NiftyLinkTcpNetworkWorker(&m_InboundMessages, &m_OutboundMessages, socket);
    worker->SetNumberMessageReceivedThreshold(m_ReceivedCounter.GetNumberMessageReceivedThreshold());
    worker->SetKeepAliveOn(m_SendKeepAlive);
    worker->SetCheckForNoIncomingData(m_CheckNoIncoming);
    connect(worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
    connect(worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
    connect(worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
    connect(worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)));
    connect(worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));

    QMutexLocker locker(&m_Mutex);
    m_Workers.insert(worker);

    NiftyLinkQThread *thread = new NiftyLinkQThread();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    worker->moveToThread(thread);
    socket->moveToThread(thread);

    thread->start();
  }
  else
  {
    QLOG_ERROR() << QObject::tr("%1::incomingConnection(%2) - failed to set socket descriptor.").arg(objectName()).arg(socketDescriptor);
  }

  this->setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));
  m_ReceivedCounter.setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));

  // Base class does this (regardless of errors), so as I'm re-implementing the function, so do I.
  this->addPendingConnection(socket);

  QLOG_INFO() << QObject::tr("%1::incomingConnection(%2) - created socket.").arg(objectName()).arg(socketDescriptor);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OnClientConnected()
{
  QTcpSocket *sender = qobject_cast<QTcpSocket*>(QObject::sender());
  QLOG_INFO() << QObject::tr("%1::OnClientConnected() - client %2 connected.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
  emit ClientConnected(sender->peerPort());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OnClientDisconnected()
{
  QTcpSocket *sender = qobject_cast<QTcpSocket*>(QObject::sender());
  if (sender != NULL)
  {
    NiftyLinkTcpNetworkWorker *chosenWorker = NULL;
    foreach (NiftyLinkTcpNetworkWorker *worker, m_Workers)
    {
      if (worker->GetSocket() == sender)
      {
        chosenWorker = worker;
        break;
      }
    }
    if (chosenWorker != NULL)
    {
      QMutexLocker locker(&m_Mutex);
      if (!m_Workers.remove(chosenWorker))
      {
        assert(false);
      }

      int portNumber = sender->peerPort();
      emit ClientDisconnected(portNumber);

      sender->deleteLater();

      QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - client %2 removed.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));

    }
    else
    {
      QLOG_ERROR() << QObject::tr("%1::OnClientDisconnected() - failed to remove client %2.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
    }
  }
  else
  {
    QLOG_ERROR() << QObject::tr("%1::OnClientDisconnected() - failed to find client %2 in order to remove it.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
  }
  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - number of clients = %2.").arg(objectName()).arg(m_Workers.size());
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
    QTcpSocket *socket = worker->GetSocket();
    socket->disconnectFromHost();
  }

  QLOG_INFO() << QObject::tr("%1::Shutdown() - finished.").arg(objectName());
  emit EndShutdown();
}

} // end namespace niftk
