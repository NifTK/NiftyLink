/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkApp.h"
#include <NiftyLinkUtils.h>
#include <NiftyLinkTcpClient.h>
#include <NiftyLinkImageMessageHelpers.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QMessageBox>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkApp::NiftyLinkApp(QObject *parent)
: m_InboundClient(new NiftyLinkTcpClient())
, m_OutboundClient(new NiftyLinkTcpClient())
, m_MessagesReceived(0)
, m_MessagesSent(0)
{
  this->setupUi(this);

  m_StatusTimer = new QTimer(this);
  m_StatusTimer->setInterval(1000);

  m_ScreenTimer = new QTimer(this);
  m_ScreenTimer->setInterval(30);

  connect(this->m_IncomingConnect, SIGNAL(pressed()), this, SLOT(OnConnectInboundButtonPressed()));
  connect(this->m_IncomingDisconnect, SIGNAL(pressed()), this, SLOT(OnDisconnectInboundButtonPressed()));
  connect(this->m_InboundClient, SIGNAL(SocketError(QString,int,QAbstractSocket::SocketError,QString)), this, SLOT(OnSocketError(QString,int,QAbstractSocket::SocketError,QString)));
  connect(this->m_InboundClient, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedInbound(QString,int)));
  connect(this->m_InboundClient, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnectedInbound(QString,int)));
  connect(this->m_InboundClient, SIGNAL(MessageReceived(NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessageContainer::Pointer)), Qt::DirectConnection);
  connect(this->m_OutgoingConnect, SIGNAL(pressed()), this, SLOT(OnConnectOutboundButtonPressed()));
  connect(this->m_OutgoingDisconnect, SIGNAL(pressed()), this, SLOT(OnDisconnectOutboundButtonPressed()));
  connect(this->m_OutboundClient, SIGNAL(SocketError(QString,int,QAbstractSocket::SocketError,QString)), this, SLOT(OnSocketError(QString,int,QAbstractSocket::SocketError,QString)));
  connect(this->m_OutboundClient, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedOutbound(QString,int)));
  connect(this->m_OutboundClient, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnectedOutbound(QString,int)));
  connect(this->m_StatusTimer, SIGNAL(timeout()), this, SLOT(OnUpdateStatus()));
  connect(this->m_ScreenTimer, SIGNAL(timeout()), this, SLOT(OnUpdateScreen()));

  this->m_IncomingDisconnect->setEnabled(false);
  this->m_OutgoingDisconnect->setEnabled(false);

  m_StatusTimer->start();
  m_ScreenTimer->start();

  QLOG_INFO() << QObject::tr("%1::NiftyLink() - created.").arg(objectName());
}


//-----------------------------------------------------------------------------
NiftyLinkApp::~NiftyLinkApp()
{
  QLOG_INFO() << QObject::tr("%1::NiftyLink() - destroying.").arg(objectName());

  m_StatusTimer->stop();
  m_ScreenTimer->stop();

  m_InboundClient->disconnect();
  delete m_InboundClient;

  m_OutboundClient->disconnect();
  delete m_OutboundClient;

  QLOG_INFO() << QObject::tr("%1::NiftyLink() - destroyed.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnConnectInboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectInboundButtonPressed().").arg(objectName());
  m_InboundClient->ConnectToHost(m_IncomingAddress->text(), m_IncomingPort->value());
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnDisconnectInboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnectInboundButtonPressed().").arg(objectName());
  m_InboundClient->DisconnectFromHost();
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnConnectOutboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectOutboundButtonPressed().").arg(objectName());
  m_OutboundClient->ConnectToHost(m_OutgoingAddress->text(), m_OutgoingPort->value());
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnDisconnectOutboundButtonPressed()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnectOutboundButtonPressed().").arg(objectName());
  m_OutboundClient->DisconnectFromHost();
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnSocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  QMessageBox::critical(this->m_CentralWidget,
                        "NiftyLink - Error!",
                        QObject::tr("(%1:%2):%3.").arg(hostName).arg(portNumber).arg(errorString),
                        QMessageBox::Ok
                        );
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnConnectedInbound(QString hostName, int portNumber)
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
void NiftyLinkApp::OnDisconnectedInbound(QString hostName, int portNumber)
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
void NiftyLinkApp::OnConnectedOutbound(QString hostName, int portNumber)
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
void NiftyLinkApp::OnDisconnectedOutbound(QString hostName, int portNumber)
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
void NiftyLinkApp::OnMessageReceived(NiftyLinkMessageContainer::Pointer message)
{
  // For stats.
  m_MessagesReceived.OnMessageReceived(message);

  // Extract Data, at each loop iteration.
  ExtractImageMessage(message, m_MostRecentImage);
  ExtractTextBasedMessage(message, m_MostRecentString);

  QString filterText = this->m_OutgoingFilter->text();
  if (filterText.size() == 0
      || filterText == QString(message->GetMessage()->GetDeviceType())
      )
  {
    if (m_OutboundClient->IsConnected())
    {
      message->GetMessage()->Pack();
      m_OutboundClient->Send(message);

      // For stats.
      m_MessagesSent.OnMessageReceived(message);
    }
  }

  QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived().").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnUpdateStatus()
{
  QString status = QObject::tr("Received %1 (%2/sec): Sent %3 (%4/sec)")
      .arg(m_MessagesReceived.GetNumberOfMessages())
      .arg(m_MessagesReceived.GetMessagesPerSecond())
      .arg(m_MessagesSent.GetNumberOfMessages())
      .arg(m_MessagesSent.GetMessagesPerSecond())
      ;

  // Clear them down, so the stats are valid for the period between timer ticks, not over all time.
  m_MessagesReceived.OnClear();
  m_MessagesSent.OnClear();

  this->statusBar()->showMessage(status);
  QLOG_DEBUG() << QObject::tr("%1::OnUpdateStatus() - %2").arg(objectName()).arg(status);
}


//-----------------------------------------------------------------------------
void NiftyLinkApp::OnUpdateScreen()
{
  this->m_TextEdit->setPlainText(m_MostRecentString);
  this->m_ImageLabel->setPixmap(QPixmap::fromImage(m_MostRecentImage));
}

} // end namespace


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  bool isDebug = false;

  for (int i = 0; i < argc; i++)
  {
    if (QString(argv[i]) == QString("--debug"))
    {
      isDebug = true;
    }
  }

  // Init the logging mechanism
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  if (isDebug)
  {
    logger.setLoggingLevel(QsLogging::DebugLevel);
  }
  else
  {
    logger.setLoggingLevel(QsLogging::InfoLevel);
  }

  // Set the logging path
  QString logFile = niftk::GetWritableDirectoryPath(QString("NiftyLink.log"));
  const QString logPath(logFile);

  QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(logPath) );
  logger.addDestination(fileDestination.get());

  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
  logger.addDestination(debugDestination.get());

  // Startup app.
  QApplication app(argc,argv);
  niftk::NiftyLinkApp niftyLink(&app);
  niftyLink.showMaximized();
  niftyLink.raise();

  int ret = app.exec();
  return ret;
}
