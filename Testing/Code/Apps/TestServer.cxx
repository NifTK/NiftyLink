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
#include <igtlTrackingDataMessage.h>

namespace niftk
{

//-----------------------------------------------------------------------------
TestServer::TestServer(const int& portNumber, const bool& isEchoing, QObject *parent)
: m_PortNumber(portNumber)
, m_IsEchoing(isEchoing)
, m_Server(new NiftyLinkTcpServer(parent))
{
  this->setObjectName("TestServer");
}


//-----------------------------------------------------------------------------
TestServer::~TestServer()
{
}


//-----------------------------------------------------------------------------
void TestServer::Start()
{
  /*
  connect(&m_Server, SIGNAL(ClientConnected()), this, SLOT(OnClientConnected()));
  connect(&m_Server, SIGNAL(FailedToSendKeepAliveMessage()), this, SLOT(OnFailedToSendKeepAliveMessage()));
  connect(&m_Server, SIGNAL(NoIncommingData()), this, SLOT(OnNoIncommingData()));
  connect(&m_Server, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
  m_Server.Start(m_PortNumber);
  */
  connect(m_Server, SIGNAL(MessageReceived(int, niftk::NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(int, niftk::NiftyLinkMessageContainer::Pointer)), Qt::DirectConnection);
  connect(m_Server, SIGNAL(SocketError(int,QAbstractSocket::SocketError, QString)), this, SLOT(OnSocketError(int,QAbstractSocket::SocketError, QString)));
  connect(m_Server, SIGNAL(ClientConnected(int)), this, SLOT(OnClientConnected(int)));

  if (!m_Server->listen(QHostAddress::LocalHost, m_PortNumber))
  {
    QLOG_ERROR() << QObject::tr("%1::Start() - Unable to start server, error code %2, error string %3").arg(objectName()).arg(m_Server->serverError()).arg(m_Server->errorString());
  }
}


//-----------------------------------------------------------------------------
void TestServer::OnClientConnected(int portNumber)
{
  QLOG_INFO() << QObject::tr("%1::OnClientConnected(%2).").arg(objectName()).arg(portNumber);
}


//-----------------------------------------------------------------------------
void TestServer::OnNoIncommingData()
{
  QLOG_INFO() << QObject::tr("%1::OnNoIncommingData().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServer::OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  QLOG_ERROR() << QObject::tr("%1::OnSocketError(port=%2, code=%3, string=%4).").arg(objectName()).arg(portNumber).arg(errorCode).arg(errorString);
}


//-----------------------------------------------------------------------------
void TestServer::OnMessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer message)
{
  QLOG_INFO() << QObject::tr("%1::OnMessageReceived(%2, %3), type=%4").arg(objectName()).arg(portNumber).arg(message->GetNiftyLinkMessageId()).arg(message->GetMessage()->GetDeviceType());
/*
  igtl::TrackingDataMessage::Pointer msg = dynamic_cast<igtl::TrackingDataMessage*>(message->GetMessage().GetPointer());
  if (msg.IsNotNull())
  {
    igtl::Matrix4x4 matrix;
    igtl::TrackingDataElement::Pointer elem = igtl::TrackingDataElement::New();
    msg->GetTrackingDataElement(0, elem);
    elem->GetMatrix(matrix);
    std::cerr << "Matrix=" << std::endl;
    for (int i = 0; i < 4; i++)
    {
      std::cerr << matrix[i][0] << ", " << matrix[i][1] << ", " << matrix[i][2] << ", " << matrix[i][3] << std::endl;
    }
  }
*/
/*
  if (m_IsEchoing)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived(%2), sending.").arg(objectName()).arg(message->GetNiftyLinkMessageId());
    //message->GetMessage()->Pack();
    //m_Server.Send(message->GetMessage());
  }
  */
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
