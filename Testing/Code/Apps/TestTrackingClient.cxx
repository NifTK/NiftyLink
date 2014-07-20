/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestTrackingClient.h"
#include <QsLog.h>
#include <QsLogDest.h>
#include <QApplication>
#include <QUrl>

//-----------------------------------------------------------------------------
TestTrackingClient::TestTrackingClient(const std::string& hostName, const int& portNumber, const int& fps)
{
  this->setObjectName("TestTrackingClient");
  m_HostName = QString::fromStdString(hostName);
  m_PortNumber = portNumber;
  m_Timer = new QTimer();
  m_Timer->setInterval(1000/fps);
  connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnTimeOut()));
  connect(&m_Client, SIGNAL(ConnectedToServer()), this, SLOT(OnConnectedToServer()));
  connect(&m_Client, SIGNAL(DisconnectedFromServer()), this, SLOT(OnDisconnectedFromServer()));
  connect(&m_Client, SIGNAL(ServerDisconnected()), this, SLOT(OnServerDisconnected()));
  connect(&m_Client, SIGNAL(MessageReceived(NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessageContainer::Pointer)));
  connect(&m_Client, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
}


//-----------------------------------------------------------------------------
TestTrackingClient::~TestTrackingClient()
{
}


//-----------------------------------------------------------------------------
void TestTrackingClient::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());

  QUrl url;
  url.setHost(m_HostName);
  url.setPort(m_PortNumber);

  m_Client.Start(url);

  QLOG_INFO() << QObject::tr("%1::Start() - finished.").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnTimeOut()
{
  // Send message.
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnDisconnectedFromServer()
{
  QLOG_INFO() << QObject::tr("%1::OnDisconnectedFromServer().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnServerDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnServerDisconnected().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnMessageReceived(NiftyLinkMessageContainer::Pointer msg)
{
  QLOG_INFO() << QObject::tr("%1::OnMessageReceived(%2).").arg(objectName()).arg(msg->GetNiftyLinkMessageId());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  QLOG_INFO() << QObject::tr("%1::OnMessageSent(%2, %3).").arg(objectName()).arg(startTimeInNanoseconds).arg(endTimeInNanoseconds);
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 4) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps>"    << std::endl;
    std::cerr << "    <host>     : Hostname"                       << std::endl;
    std::cerr << "    <port>     : Port #"                         << std::endl;
    std::cerr << "    <fps>      : Frames per second [30]"         << std::endl;
    exit(0);
  }

  std::string hostName = argv[1];
  int         port     = atoi(argv[2]);
  int         fps      = atoi(argv[3]);

  std::cout << "TestTrackingClient: host = " << hostName << "." << std::endl;
  std::cout << "TestTrackingClient: port = " << port << "." << std::endl;
  std::cout << "TestTrackingClient: fps = " << fps << "." << std::endl;
  std::cout << "TestTrackingClient: Instantiating client." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start client.
  TestTrackingClient client(hostName, port, fps);

  std::cout << "TestTrackingClient: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestTrackingClient: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
