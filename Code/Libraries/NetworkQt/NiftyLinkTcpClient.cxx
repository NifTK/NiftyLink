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

  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");

  connect(m_Socket, SIGNAL(connected()), this, SLOT(OnConnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - started.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::~NiftyLinkTcpClient()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroying.").arg(objectName());

  if (m_Worker != NULL)
  {
    m_Worker->disconnect();
    delete m_Worker;
  }

  if (m_Socket != NULL)
  {
    m_Socket->disconnect();
    delete m_Socket;
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnConnected()
{
  if (m_Worker != NULL)
  {
    m_Worker->disconnect();
    delete m_Worker;
  }

  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  NiftyLinkQThread *thread = new NiftyLinkQThread(this);
  m_Worker = new NiftyLinkTcpNetworkWorker(&m_InboundMessages, &m_OutboundMessages, m_Socket);
  m_Worker->moveToThread(thread);
  m_Socket->moveToThread(thread);

  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
  connect(m_Worker, SIGNAL(NoIncomingData()), this, SIGNAL(NoIncomingData()));
  connect(m_Worker, SIGNAL(SentKeepAlive()), this, SIGNAL(SentKeepAlive()));
  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(MessageReceived(int)), this, SLOT(OnMessageReceived(int)));
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

  this->setObjectName(QObject::tr("NiftyLinkTcpClient(%1:%2)").arg(m_Socket->peerName()).arg(m_Socket->peerPort()));

  thread->start();
  emit Connected();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OnDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnected() - disconnected.").arg(objectName());
  m_Socket->close();
  while (m_Socket->isOpen())
  {
    QThread::currentThread()->wait(10);
  }
  QLOG_INFO() << QObject::tr("%1::OnDisconnected() - socket closed.").arg(objectName());
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
void NiftyLinkTcpClient::Send(NiftyLinkMessageContainer::Pointer message)
{
  m_Worker->Send(message);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::OutputStats()
{
  m_Worker->OutputStatsToConsole();
  m_Worker->RequestStats(); // to get the remote end to dump stats.
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
void NiftyLinkTcpClient::OnMessageReceived(int /*portNumber*/)
{
  NiftyLinkMessageContainer::Pointer msg = m_InboundMessages.GetContainer();
  emit MessageReceived(msg);
}

} // end niftk namespace
