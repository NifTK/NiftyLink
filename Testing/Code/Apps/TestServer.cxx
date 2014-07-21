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
TestServer::TestServer(const int& portNumber, const bool& isEchoing)
{
  this->setObjectName("TestServer");
  m_PortNumber = portNumber;
  m_IsEchoing = isEchoing;
}


//-----------------------------------------------------------------------------
TestServer::~TestServer()
{
}


//-----------------------------------------------------------------------------
void TestServer::Start()
{
  connect(&m_Server, SIGNAL(ClientConnected()), this, SLOT(OnClientConnected()));
  connect(&m_Server, SIGNAL(FailedToSendKeepAliveMessage()), this, SLOT(OnFailedToSendKeepAliveMessage()));
  connect(&m_Server, SIGNAL(NoIncommingData()), this, SLOT(OnNoIncommingData()));
  connect(&m_Server, SIGNAL(MessageReceived(niftk::NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer)));
  connect(&m_Server, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
  m_Server.Start(m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestServer::OnClientConnected()
{
  QLOG_INFO() << QObject::tr("%1::OnClientConnected().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnNoIncommingData()
{
  QLOG_INFO() << QObject::tr("%1::OnNoIncommingData().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer message)
{
  QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived(%2), type=%3").arg(objectName()).arg(message->GetNiftyLinkMessageId()).arg(message->GetMessage()->GetDeviceType());
  if (m_IsEchoing)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived(%2), sending.").arg(objectName()).arg(message->GetNiftyLinkMessageId());
    message->GetMessage()->Pack();
    m_Server.Send(message->GetMessage());
  }
}


//-----------------------------------------------------------------------------
void TestServer::OnFailedToSendKeepAliveMessage()
{
  QLOG_INFO() << QObject::tr("%1::OnFailedToSendKeepAliveMessage().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  QLOG_DEBUG() << QObject::tr("%1::OnMessageSent(%2, %3), time taken=%4 nsec").arg(objectName()).arg(startTimeInNanoseconds).arg(endTimeInNanoseconds).arg(endTimeInNanoseconds - startTimeInNanoseconds);
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 3) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port>"    << std::endl;
    std::cerr << "    <port>     : Port #"            << std::endl;
    std::cerr << "    <bool>     : is echoing [0|1]"  << std::endl;
    exit(0);
  }

  int    port       = atoi(argv[1]);
  int    isEcho     = atoi(argv[2]);

  std::cout << "TestServer: port = " << port << "." << std::endl;
  std::cout << "TestServer: echo = " << isEcho << "." << std::endl;
  std::cout << "TestServer: Instantiating server." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start server.
  bool isEchoing = (isEcho == 1 ? true : false);
  niftk::TestServer server(port, isEchoing);

  std::cout << "TestServer: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestServer: Launching app." << std::endl;

  QTimer::singleShot(220, &server, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
