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
TestQtTrackingClient::TestQtTrackingClient(
    const std::string& hostName,
    const int& portNumber,
    const int& fps,
    const int& totalNumberMessages,
    const int& trackedObjectsPerMessage,
    QObject *parent)
: m_HostName(QString::fromStdString(hostName))
, m_PortNumber(portNumber)
, m_FramesPerSecond(fps)
, m_IntendedNumberMessages(totalNumberMessages)
, m_TrackedObjectsPerMessage(trackedObjectsPerMessage)
, m_NumberMessagesSent(0)
, m_Client(new NiftyLinkTcpClient(parent))
{
  this->setObjectName("TestQtTrackingClient");
  connect(m_Client, SIGNAL(Connected()), this, SLOT(OnConnectedToServer()));
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
  this->RunTest();
}


//-----------------------------------------------------------------------------
void TestQtTrackingClient::RunTest()
{
  igtl::TimeStamp::Pointer timeLastMessage = igtl::TimeStamp::New();
  timeLastMessage->GetTime();

  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  timeNow->GetTime();

  int nanosecondsBetweenMessages = 1000000000 / m_FramesPerSecond;
  QLOG_INFO() << QObject::tr("%1::RunTest() - %2 fps = %3 ns between messages.").arg(objectName()).arg(m_FramesPerSecond).arg(nanosecondsBetweenMessages);

  // This will occupy a lot of CPU, but we have multi-cpu machines, so no problem.
  while(m_NumberMessagesSent < m_IntendedNumberMessages)
  {
    timeNow->GetTime();
    if (niftk::GetDifferenceInNanoSeconds(timeNow, timeLastMessage) > nanosecondsBetweenMessages)
    {
      NiftyLinkMessageContainer::Pointer m = this->CreateTrackingData();
      m_Client->Send(m);
      m_NumberMessagesSent++;
      timeLastMessage->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());
    }
  }
  m_Client->OutputStats();
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer TestQtTrackingClient::CreateTrackingData()
{
  igtl::Matrix4x4 matrix;

  igtl::TrackingDataMessage::Pointer msg = igtl::TrackingDataMessage::New();
  msg->SetDeviceName("TestTrackingClient");

  for (int i = 0; i < m_TrackedObjectsPerMessage; i++)
  {
    niftk::CreateRandomTransformMatrix(matrix);

    igtl::TrackingDataElement::Pointer element = igtl::TrackingDataElement::New();
    element->SetMatrix(matrix);

    msg->AddTrackingDataElement(element);
  }

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName("TestTrackingClient");
  m->SetSenderHostName("123.456.789.012");
  m->SetSenderPortNumber(1234);

  return m;
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 6) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps> <total>"    << std::endl;
    std::cerr << "    <host>     : Hostname"                               << std::endl;
    std::cerr << "    <port>     : Port #"                                 << std::endl;
    std::cerr << "    <fps>      : Frames per second [30]"                 << std::endl;
    std::cerr << "    <total>    : # of frames in total [1000]"            << std::endl;
    std::cerr << "    <objects>  : # of matrices per message [1]"          << std::endl;
    exit(0);
  }

  std::string hostName = argv[1];
  int         port     = atoi(argv[2]);
  int         fps      = atoi(argv[3]);
  int         total    = atoi(argv[4]);
  int         objects  = atoi(argv[5]);

  std::cout << "TestQtTrackingClient: host = " << hostName << "." << std::endl;
  std::cout << "TestQtTrackingClient: port = " << port << "." << std::endl;
  std::cout << "TestQtTrackingClient: fps = " << fps << "." << std::endl;
  std::cout << "TestQtTrackingClient: total = " << total << "." << std::endl;
  std::cout << "TestQtTrackingClient: objects = " << objects << "." << std::endl;
  std::cout << "TestQtTrackingClient: Instantiating client." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start client.
  niftk::TestQtTrackingClient client(hostName, port, fps, total, objects);

  std::cout << "TestQtTrackingClient: Creating app." << std::endl;

  QApplication app(argc, argv);

  std::cout << "TestQtTrackingClient: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
