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

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpClient() - started.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkTcpClient::~NiftyLinkTcpClient()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpClient() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
QTcpSocket* NiftyLinkTcpClient::GetTcpSocket() const
{
  return m_Socket;
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::ConnectToHost(const QString& hostName, quint16 portNumber)
{  
  m_Socket->connectToHost(hostName, portNumber);
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

  connect(m_Worker, SIGNAL(BytesSent(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Worker, SIGNAL(SocketError(int,QAbstractSocket::SocketError)), this, SIGNAL(SocketError(int,QAbstractSocket::SocketError)), Qt::BlockingQueuedConnection);
  connect(m_Worker, SIGNAL(MessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), this, SIGNAL(MessageReceived(int,niftk::NiftyLinkMessageContainer::Pointer)), Qt::BlockingQueuedConnection);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // i.e. the event loop of thread deletes it when control returns to this event loop.

  thread->start();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpClient::Send(igtl::MessageBase::Pointer msg)
{
  m_Worker->Send(msg);
}

} // end niftk namespace
