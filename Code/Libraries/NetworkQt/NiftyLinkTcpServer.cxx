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

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpServer::NiftyLinkTcpServer(QObject *parent)
: QTcpServer(parent)
, m_SendKeepAlive(false)
, m_CheckNoIncoming(false)
{
  this->setObjectName("NiftyLinkTcpServer");
  m_ReceivedCounter.setObjectName("NiftyLinkTcpServer");

  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpServer() - started.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpServer::~NiftyLinkTcpServer()
{
  this->Shutdown();
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

    NiftyLinkQThread *thread = new NiftyLinkQThread(this);
    NiftyLinkTcpNetworkWorker *worker = new NiftyLinkTcpNetworkWorker(&m_InboundMessages, &m_OutboundMessages, socket);

    worker->moveToThread(thread);
    socket->moveToThread(thread);

    worker->SetNumberMessageReceivedThreshold(m_ReceivedCounter.GetNumberMessageReceivedThreshold());
    worker->SetKeepAliveOn(m_SendKeepAlive);
    worker->SetCheckForNoIncomingData(m_CheckNoIncoming);

    connect(worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
    connect(worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
    connect(worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
    connect(worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)));
    connect(worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

    QMutexLocker locker(&m_Mutex);
    m_Workers.insert(worker);

    thread->start();
  }
  else
  {
    QLOG_ERROR() << QObject::tr("%1::incomingConnection(%2) - failed to connect.").arg(objectName()).arg(socketDescriptor);
  }

  // Base class does this (regardless of errors), so as I'm re-implementing the function, so do I.
  this->addPendingConnection(socket);

  this->setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));
  m_ReceivedCounter.setObjectName(QObject::tr("NiftyLinkTcpServer(%1)").arg(this->serverPort()));

  QLOG_INFO() << QObject::tr("%1::incomingConnection(%2) - created.").arg(objectName()).arg(socketDescriptor);
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
  QMutexLocker locker(&m_Mutex);

  QTcpSocket *sender = qobject_cast<QTcpSocket*>(QObject::sender());
  if (sender != NULL)
  {
    NiftyLinkTcpNetworkWorker *chosenWorker = NULL;
    foreach (NiftyLinkTcpNetworkWorker *worker, m_Workers)
    {
      if (worker->GetSocket() == sender)
      {
        chosenWorker = worker;
      }
    }
    if (chosenWorker != NULL)
    {
      m_Workers.remove(chosenWorker);
      int portNumber = sender->peerPort();

      chosenWorker->disconnect();
      sender->disconnect();

      delete chosenWorker;
      delete sender;

      QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - client %2 removed.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));

      emit ClientDisconnected(portNumber);
    }
    else
    {
      QLOG_ERROR() << QObject::tr("%1::OnClientDisconnected() - failed to remove client %2.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
    }
  }
  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - number of clients = %2.").arg(objectName()).arg(m_Workers.size());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::Send(NiftyLinkMessageContainer::Pointer message)
{
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->Send(message);
  }
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
  m_SendKeepAlive = isOn;
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->SetKeepAliveOn(m_SendKeepAlive);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::SetCheckForNoIncomingData(bool isOn)
{
  m_CheckNoIncoming = isOn;
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->SetCheckForNoIncomingData(m_CheckNoIncoming);
  }
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
  QLOG_WARN() << QObject::tr("%1::Shutdown() - started.").arg(objectName());

  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    QTcpSocket *socket = worker->GetSocket();

    worker->disconnect();
    socket->disconnect();

    delete worker;
    delete socket;
  }

  QLOG_WARN() << QObject::tr("%1::Shutdown() - finished.").arg(objectName());
  emit EndShutdown();
}

} // end namespace niftk
