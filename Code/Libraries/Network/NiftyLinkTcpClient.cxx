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

#include <igtlMessageBase.h>
#include <QsLog.h>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpClient::NiftyLinkTcpClient(QObject *parent)
: QObject(parent)
, m_Socket(new QTcpSocket())
, m_Worker(NULL)
{
  this->setObjectName("NiftyLinkTcpClient");

  qRegisterMetaType<igtl::MessageBase::Pointer>("igtl::MessageBase::Pointer");
  qRegisterMetaType<niftk::NiftyLinkMessageContainer::Pointer>("niftk::NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

  connect(m_Socket, SIGNAL(connected()), this, SLOT(OnConnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - started.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::~NiftyLinkTcpClient()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnConnected()
{
  if (m_Worker != NULL)
  {
    delete m_Worker;
  }

  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  NiftyLinkQThread *thread = new NiftyLinkQThread(this);
  m_Worker = new NiftyLinkTcpNetworkWorker(m_Socket);
  m_Worker->moveToThread(thread);
  m_Socket->moveToThread(thread);

  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(MessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), Qt::BlockingQueuedConnection);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

  this->setObjectName(QObject::tr("NiftyLinkTcpClient(%1:%2)").arg(m_Socket->peerName()).arg(m_Socket->peerPort()));

  thread->start();
  emit Connected();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnected() - disconnected.").arg(objectName());
  emit Disconnected();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnError()
{
  QLOG_INFO() << QObject::tr("%1::OnError() - code=%2, string=%3").arg(objectName()).arg(m_Socket->error()).arg(m_Socket->errorString());
  emit SocketError(m_Socket->peerName(), m_Socket->peerPort(), m_Socket->error(), m_Socket->errorString());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::ConnectToHost(const QString& hostName, quint16 portNumber)
{
  // There are no errors reported from this. Listen to the error signal.
  m_Socket->connectToHost(hostName, portNumber);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::Send(igtl::MessageBase::Pointer msg)
{
  // Its up to the worker to cope with thread boundaries.
  m_Worker->Send(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnMessageReceived(int /*portNumber*/, niftk::NiftyLinkMessageContainer::Pointer msg)
{
  emit MessageReceived(msg);
}

} // end niftk namespace
