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

#include <iostream>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpServer::NiftyLinkTcpServer(QObject *parent)
: QTcpServer(parent)
{
  this->setObjectName("NiftyLinkTcpServer");

  qRegisterMetaType<niftk::NiftyLinkMessageContainer::Pointer>("niftk::NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

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
  if (socket->setSocketDescriptor(socketDescriptor))
  {
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    NiftyLinkQThread *thread = new NiftyLinkQThread(this);
    NiftyLinkTcpNetworkWorker *worker = new NiftyLinkTcpNetworkWorker(socket);
    worker->moveToThread(thread);
    socket->moveToThread(thread);

    connect(socket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(socket, SIGNAL(connected()), this, SLOT(OnClientConnected()));
    connect(worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), this, SIGNAL(SocketError(int,QAbstractSocket::SocketError,QString)), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(MessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), this, SIGNAL(MessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), Qt::BlockingQueuedConnection);
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
      QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - client %2 removed.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));

      emit ClientDisconnected(sender->peerPort());
    }
    else
    {
      QLOG_ERROR() << QObject::tr("%1::OnClientDisconnected() - failed to remove client %2.").arg(objectName()).arg(reinterpret_cast<qulonglong>(sender));
    }
  }
  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected() - number of clients = %2.").arg(objectName()).arg(m_Workers.size());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::Send(igtl::MessageBase::Pointer msg)
{
  // Send same message to all clients.
  foreach (NiftyLinkTcpNetworkWorker* worker, m_Workers)
  {
    worker->Send(msg);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::OutputStats()
{

}


//-----------------------------------------------------------------------------
void NiftyLinkTcpServer::Shutdown()
{
  emit StartShutdown();
  QLOG_WARN() << QObject::tr("%1::Shutdown() - started.").arg(objectName());


  QLOG_WARN() << QObject::tr("%1::Shutdown() - finished.").arg(objectName());
  emit EndShutdown();
}

} // end namespace niftk
