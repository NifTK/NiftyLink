/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLink.h"
#include <NiftyLinkUtils.h>
#include <NiftyLinkTcpClient.h>
#include <NiftyLinkImageMessageHelpers.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QMessageBox>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLink::NiftyLink(QObject *parent)
: m_InboundClient(new NiftyLinkTcpClient())
, m_OutboundClient(new NiftyLinkTcpClient())
, m_MessagesReceived(0)
, m_MessagesSent(0)
{
  this->setupUi(this);

  m_Timer = new QTimer(this);
  m_Timer->setInterval(1000);

  connect(this->m_IncomingConnect, SIGNAL(pressed()), this, SLOT(OnConnectInboundButtonPressed()));
  connect(this->m_IncomingDisconnect, SIGNAL(pressed()), this, SLOT(OnDisconnectInboundButtonPressed()));
  connect(this->m_InboundClient, SIGNAL(SocketError(QString,int,QAbstractSocket::SocketError,QString)), this, SLOT(OnSocketError(QString,int,QAbstractSocket::SocketError,QString)));
  connect(this->m_InboundClient, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedInbound(QString,int)));
  connect(this->m_InboundClient, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnectedInbound(QString,int)));
  connect(this->m_InboundClient, SIGNAL(MessageReceived(NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessageContainer::Pointer)));
  connect(this->m_OutgoingConnect, SIGNAL(pressed()), this, SLOT(OnConnectOutboundButtonPressed()));
  connect(this->m_OutgoingDisconnect, SIGNAL(pressed()), this, SLOT(OnDisconnectOutboundButtonPressed()));
  connect(this->m_OutboundClient, SIGNAL(SocketError(QString,int,QAbstractSocket::SocketError,QString)), this, SLOT(OnSocketError(QString,int,QAbstractSocket::SocketError,QString)));
  connect(this->m_OutboundClient, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedOutbound(QString,int)));
  connect(this->m_OutboundClient, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnectedOutbound(QString,int)));
  connect(this->m_Timer, SIGNAL(timeout()), this, SLOT(OnUpdateStatus()));

  this->m_IncomingDisconnect->setEnabled(false);
  this->m_OutgoingDisconnect->setEnabled(false);

  m_Timer->start();

  QLOG_INFO() << QObject::tr("%1::NiftyLink() - created.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLink::~NiftyLink()
{
  QLOG_INFO() << QObject::tr("%1::NiftyLink() - destroying.").arg(objectName());

  m_InboundClient->disconnect();
  delete m_InboundClient;

  m_OutboundClient->disconnect();
  delete m_OutboundClient;

  QLOG_INFO() << QObject::tr("%1::NiftyLink() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLink::OnConnectInboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectInboundButtonPressed().").arg(objectName());
  m_InboundClient->ConnectToHost(m_IncomingAddress->text(), m_IncomingPort->value());
}


//-----------------------------------------------------------------------------
void NiftyLink::OnDisconnectInboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnectInboundButtonPressed().").arg(objectName());
  m_InboundClient->DisconnectFromHost();
}


//-----------------------------------------------------------------------------
void NiftyLink::OnConnectOutboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectOutboundButtonPressed().").arg(objectName());
  m_OutboundClient->ConnectToHost(m_OutgoingAddress->text(), m_OutgoingPort->value());
}


//-----------------------------------------------------------------------------
void NiftyLink::OnDisconnectOutboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnectOutboundButtonPressed().").arg(objectName());
  m_OutboundClient->DisconnectFromHost();
}


//-----------------------------------------------------------------------------
void NiftyLink::OnSocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  QMessageBox::critical(this->m_CentralWidget,
                        "NiftyLink - Error!",
                        QObject::tr("(%1:%2):%3.").arg(hostName).arg(portNumber).arg(errorString),
                        QMessageBox::Ok
                        );
}


//-----------------------------------------------------------------------------
void NiftyLink::OnConnectedInbound(QString hostName, int portNumber)
{
  QMessageBox::information(this->m_CentralWidget,
                        "NiftyLink",
                        QObject::tr("(%1:%2):Connected successfully.").arg(hostName).arg(portNumber),
                        QMessageBox::Ok
                        );

  this->m_IncomingConnect->setEnabled(false);
  this->m_IncomingDisconnect->setEnabled(true);
}


//-----------------------------------------------------------------------------
void NiftyLink::OnDisconnectedInbound(QString hostName, int portNumber)
{
  QMessageBox::critical(this->m_CentralWidget,
                        "NiftyLink - Error!",
                        QObject::tr("(%1:%2):Disconnected!").arg(hostName).arg(portNumber),
                        QMessageBox::Ok
                        );

  this->m_IncomingConnect->setEnabled(true);
  this->m_IncomingDisconnect->setEnabled(false);
}


//-----------------------------------------------------------------------------
void NiftyLink::OnConnectedOutbound(QString hostName, int portNumber)
{
  QMessageBox::information(this->m_CentralWidget,
                        "NiftyLink",
                        QObject::tr("(%1:%2):Connected successfully.").arg(hostName).arg(portNumber),
                        QMessageBox::Ok
                        );

  this->m_OutgoingConnect->setEnabled(false);
  this->m_OutgoingDisconnect->setEnabled(true);
}


//-----------------------------------------------------------------------------
void NiftyLink::OnDisconnectedOutbound(QString hostName, int portNumber)
{
  QMessageBox::critical(this->m_CentralWidget,
                        "NiftyLink - Error!",
                        QObject::tr("(%1:%2):Disconnected!").arg(hostName).arg(portNumber),
                        QMessageBox::Ok
                        );

  this->m_OutgoingConnect->setEnabled(true);
  this->m_OutgoingDisconnect->setEnabled(false);
}


//-----------------------------------------------------------------------------
void NiftyLink::OnMessageReceived(NiftyLinkMessageContainer::Pointer message)
{
  QString filterText = this->m_OutgoingFilter->text();
  m_MessagesReceived.OnMessageReceived(message);

  if (filterText.size() == 0
      || filterText == QString(message->GetMessage()->GetDeviceType())
      )
  {
    if (m_OutboundClient->IsConnected())
    {
      message->GetMessage()->Pack();
      m_OutboundClient->Send(message);
      m_MessagesSent.OnMessageReceived(message);
    }
  }

  DisplayTextBasedMessage(message, this->m_TextEdit);
  DisplayImageMessage(message, this->m_ImageLabel);

  QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived().").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLink::OnUpdateStatus()
{
  QString status = QObject::tr("Received %1 (%2/sec): Sent %3 (%4/sec)")
      .arg(m_MessagesReceived.GetNumberOfMessages())
      .arg(m_MessagesReceived.GetMessagesPerSecond())
      .arg(m_MessagesSent.GetNumberOfMessages())
      .arg(m_MessagesSent.GetMessagesPerSecond())
      ;

  this->statusBar()->showMessage(status);
  QLOG_DEBUG() << QObject::tr("%1::OnUpdateStatus() - %2").arg(objectName()).arg(status);
}

} // end namespace


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc,argv);

  // Init the logging mechanism
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);

  // Set the logging path
  QString logFile = niftk::GetWritableDirectoryPath(QString("NiftyLink.log"));
  const QString logPath(logFile);

  QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(logPath) );
  logger.addDestination(fileDestination.get());

  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  niftk::NiftyLink niftyLink(&app);
  niftyLink.showMaximized();
  niftyLink.raise();

  int ret = app.exec();
  return ret;
}
