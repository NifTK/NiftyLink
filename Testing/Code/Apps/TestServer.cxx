/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestServer.h"
#include <NiftyLinkServer.h>
#include <QsLog.h>
#include <QsLogDest.h>
#include <QApplication>

namespace niftk
{

//-----------------------------------------------------------------------------
TestServer::TestServer(const int& portNumber)
{
  this->setObjectName("TestServer");
  m_PortNumber = portNumber;
}


//-----------------------------------------------------------------------------
TestServer::~TestServer()
{
}


//-----------------------------------------------------------------------------
void TestServer::Start()
{
  connect(&m_Server, SIGNAL(ClientConnected()), this, SLOT(OnClientConnected()));
  connect(&m_Server, SIGNAL(ClientDisconnected()), this, SLOT(OnClientDisconnected()));
  connect(&m_Server, SIGNAL(MessageReceived(NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessageContainer::Pointer)));
  connect(&m_Server, SIGNAL(CantSendToClient()), this, SLOT(OnCantSendToClient()));
  connect(&m_Server, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
  m_Server.Start(m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestServer::OnClientConnected()
{
  QLOG_INFO() << QObject::tr("%1::OnClientConnected().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnClientDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnClientDisconnected().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnMessageReceived(NiftyLinkMessageContainer::Pointer message)
{
  QLOG_INFO() << QObject::tr("%1::OnMessageReceived(%2).").arg(objectName()).arg(message->GetNiftyLinkMessageId());
}


//-----------------------------------------------------------------------------
void TestServer::OnCantSendToClient()
{
  QLOG_INFO() << QObject::tr("%1::OnCantSendToClient().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  QLOG_INFO() << QObject::tr("%1::OnMessageSent().").arg(objectName()).arg(startTimeInNanoseconds).arg(endTimeInNanoseconds);
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 2) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port>"    << std::endl;
    std::cerr << "    <port>     : Port #"            << std::endl;
    exit(0);
  }

  int    port       = atoi(argv[1]);

  std::cout << "TestServer: port = " << port << "." << std::endl;
  std::cout << "TestServer: Instantiating server." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start server.
  niftk::TestServer server(port);

  std::cout << "TestServer: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestServer: Launching app." << std::endl;

  QTimer::singleShot(220, &server, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
