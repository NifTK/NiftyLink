/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestQtTrackingClient.h"
#include <QsLog.h>
#include <QsLogDest.h>
#include <QApplication>
#include <QUrl>
#include <QAbstractSocket>
#include "NiftyLinkMessageContainer.h"
#include <igtlTrackingDataMessage.h>
#include <NiftyLinkUtils.h>

namespace niftk
{

//-----------------------------------------------------------------------------
TestQtTrackingClient::TestQtTrackingClient(const std::string& hostName, const int& portNumber, const int& fps, const int& numberMessages, QObject *parent)
: m_HostName(QString::fromStdString(hostName))
, m_PortNumber(portNumber)
, m_IntendedNumberMessages(numberMessages)
, m_Client(new NiftyLinkTcpClient(parent))
{
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");

  this->setObjectName("TestQtTrackingClient");

  m_NumberMessagesSent = 0;

  m_Timer = new QTimer();
  m_Timer->setInterval(1000/fps);

  connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnTimeOut()));  
  connect(m_Client, SIGNAL(Connected()), this, SLOT(OnConnectedToServer()));
  connect(m_Client, SIGNAL(BytesSent(qint64)), this, SLOT(OnBytesSent(qint64)));
}


//-----------------------------------------------------------------------------
TestQtTrackingClient::~TestQtTrackingClient()
{
}


//-----------------------------------------------------------------------------
void TestQtTrackingClient::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());
  m_Client->ConnectToHost(m_HostName, m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestQtTrackingClient::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
  m_Timer->start();
}


//-----------------------------------------------------------------------------
void TestQtTrackingClient::OnTimeOut()
{
  igtl::Matrix4x4 matrix;
  niftk::CreateRandomTransformMatrix(matrix);

  igtl::TrackingDataElement::Pointer element = igtl::TrackingDataElement::New();
  element->SetMatrix(matrix);

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  igtl::TrackingDataMessage::Pointer msg = igtl::TrackingDataMessage::New();
  msg->SetDeviceName("TestQtTrackingClient");
  msg->AddTrackingDataElement(element);
  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName("TestQtTrackingClient");
  m->SetSenderHostName("123.456.789.012");  // NiftyLink should provide easy way to access the machine name.
  m->SetSenderPortNumber(1234);

  // Send message.
  m_Client->Send(m);

  m_NumberMessagesSent++;
  if (m_NumberMessagesSent == m_IntendedNumberMessages)
  {
    m_Client->OutputStats();
    m_Timer->stop();
    m_NumberMessagesSent = 0;
  }
}


//-----------------------------------------------------------------------------
void TestQtTrackingClient::OnBytesSent(qint64 bytes)
{
  QLOG_DEBUG() << QObject::tr("%1::OnBytesSent(%2).").arg(objectName()).arg(bytes);
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 5) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps> <total>"    << std::endl;
    std::cerr << "    <host>     : Hostname"                               << std::endl;
    std::cerr << "    <port>     : Port #"                                 << std::endl;
    std::cerr << "    <fps>      : Frames per second [30]"                 << std::endl;
    std::cerr << "    <total>    : # of frames in total [1000]"            << std::endl;
    exit(0);
  }

  std::string hostName = argv[1];
  int         port     = atoi(argv[2]);
  int         fps      = atoi(argv[3]);
  int         total    = atoi(argv[4]);

  std::cout << "TestQtTrackingClient: host = " << hostName << "." << std::endl;
  std::cout << "TestQtTrackingClient: port = " << port << "." << std::endl;
  std::cout << "TestQtTrackingClient: fps = " << fps << "." << std::endl;
  std::cout << "TestQtTrackingClient: total = " << total << "." << std::endl;
  std::cout << "TestQtTrackingClient: Instantiating client." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start client.
  niftk::TestQtTrackingClient client(hostName, port, fps, total);

  std::cout << "TestQtTrackingClient: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestQtTrackingClient: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
