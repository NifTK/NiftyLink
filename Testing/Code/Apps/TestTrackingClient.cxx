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
#include <QAbstractSocket>
#include "NiftyLinkMessageContainer.h"
#include <igtlTrackingDataMessage.h>
#include <NiftyLinkUtils.h>

namespace niftk
{

//-----------------------------------------------------------------------------
TestTrackingClient::TestTrackingClient(const std::string& hostName, const int& portNumber, const int& fps, const int& numberMessages, QObject *parent)
: m_HostName(QString::fromStdString(hostName))
, m_PortNumber(portNumber)
, m_IntendedNumberMessages(numberMessages)
, m_Client(new NiftyLinkTcpClient(parent))
{
  qRegisterMetaType<igtl::MessageBase::Pointer>("igtl::MessageBase::Pointer");
  qRegisterMetaType<niftk::NiftyLinkMessageContainer::Pointer>("niftk::NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

  this->setObjectName("TestTrackingClient");

  m_NumberMessagesSent = 0;
  m_Timer = new QTimer();
  m_Timer->setInterval(1000/fps);
  connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnTimeOut()));
  connect(m_Client, SIGNAL(Connected()), this, SLOT(OnConnectedToServer()));
  connect(m_Client, SIGNAL(BytesSent(qint64)), this, SLOT(OnBytesSent(qint64)));

  /*
  connect(&m_Client, SIGNAL(ConnectedToServer()), this, SLOT(OnConnectedToServer()));
  connect(&m_Client, SIGNAL(FailedToSendKeepAliveMessage()), this, SLOT(OnFailedToSendKeepAliveMessage()));
  connect(&m_Client, SIGNAL(NoIncommingData()), this, SLOT(OnNoIncommingData()));
  connect(&m_Client, SIGNAL(MessageReceived(niftk::NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer)));
  connect(&m_Client, SIGNAL(MessageSent(igtlUint64, igtlUint64)), this, SLOT(OnMessageSent(igtlUint64, igtlUint64)));
*/
}


//-----------------------------------------------------------------------------
TestTrackingClient::~TestTrackingClient()
{
}


//-----------------------------------------------------------------------------
void TestTrackingClient::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());
  m_Client->ConnectToHost(m_HostName, m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
  m_Timer->start();
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnTimeOut()
{
  igtl::Matrix4x4 matrix;
  niftk::CreateRandomTransformMatrix(matrix);

  igtl::TrackingDataElement::Pointer element = igtl::TrackingDataElement::New();
  element->SetMatrix(matrix);

  // Send message. We should NOT pack it, as our client class does it for us.
  igtl::TrackingDataMessage::Pointer msg = igtl::TrackingDataMessage::New();
  msg->SetDeviceName("TestTrackingClient");
  msg->AddTrackingDataElement(element);
  m_Client->Send(msg.GetPointer());

  m_NumberMessagesSent++;
  if (m_NumberMessagesSent == m_IntendedNumberMessages)
  {
    //m_Client->OutputStats();
    m_NumberMessagesSent = 0;
  }
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  QLOG_ERROR() << QObject::tr("%1::OnSocketError(port=%2, code=%3, string=%4).").arg(objectName()).arg(portNumber).arg(errorCode).arg(errorString);
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnFailedToSendKeepAliveMessage()
{
  QLOG_ERROR() << QObject::tr("%1::OnFailedToSendKeepAliveMessage().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnNoIncommingData()
{
  QLOG_ERROR() << QObject::tr("%1::OnNoIncommingData().").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg)
{
  QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived(%2).").arg(objectName()).arg(msg->GetNiftyLinkMessageId());
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds)
{
  QLOG_DEBUG() << QObject::tr("%1::OnMessageSent(%2, %3).").arg(objectName()).arg(startTimeInNanoseconds).arg(endTimeInNanoseconds);
}


//-----------------------------------------------------------------------------
void TestTrackingClient::OnBytesSent(qint64 bytes)
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

  std::cout << "TestTrackingClient: host = " << hostName << "." << std::endl;
  std::cout << "TestTrackingClient: port = " << port << "." << std::endl;
  std::cout << "TestTrackingClient: fps = " << fps << "." << std::endl;
  std::cout << "TestTrackingClient: total = " << total << "." << std::endl;
  std::cout << "TestTrackingClient: Instantiating client." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start client.
  niftk::TestTrackingClient client(hostName, port, fps, total);

  std::cout << "TestTrackingClient: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestTrackingClient: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
