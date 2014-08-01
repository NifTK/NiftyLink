/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestClientNifTKOIGTLTracking.h"
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkUtils.h>
#include <NiftyLinkClientSocket.h>

#include <igtlTrackingDataMessage.h>
#include <igtlStringMessage.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QCoreApplication>

namespace niftk
{

//-----------------------------------------------------------------------------
TestClientNifTKOIGTLTracking::TestClientNifTKOIGTLTracking(
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
{
  this->setObjectName("TestClientNifTKOIGTLTracking");
}


//-----------------------------------------------------------------------------
TestClientNifTKOIGTLTracking::~TestClientNifTKOIGTLTracking()
{
}


//-----------------------------------------------------------------------------
void TestClientNifTKOIGTLTracking::Start()
{
  NiftyLinkClientSocket::Pointer socket;
  socket = NiftyLinkClientSocket::New();

  int r = socket->ConnectToServer(m_HostName.toStdString().c_str(), m_PortNumber);
  if (r != 0)
    {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
    }

  QLOG_INFO() << QObject::tr("%1::Start() - Connected to host=%2, port=%3.").arg(m_HostName).arg(m_PortNumber);

  igtl::TimeStamp::Pointer timeLastMessage = igtl::TimeStamp::New();
  timeLastMessage->GetTime();

  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  timeNow->GetTime();

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  int nanosecondsBetweenMessages = 1000000000 / m_FramesPerSecond;
  QLOG_INFO() << QObject::tr("%1::Start() - %2 fps = %3 ns between messages.").arg(objectName()).arg(m_FramesPerSecond).arg(nanosecondsBetweenMessages);

  // This will occupy a lot of CPU, but we have multi-cpu machines, so no problem.
  while(m_NumberMessagesSent < m_IntendedNumberMessages)
  {
    timeNow->GetTime();
    if (niftk::GetDifferenceInNanoSeconds(timeNow, timeLastMessage) > nanosecondsBetweenMessages)
    {
      NiftyLinkMessageContainer::Pointer m = niftk::CreateTestTrackingDataMessage(timeCreated, m_TrackedObjectsPerMessage);
      igtl::MessageBase::Pointer msg = m->GetMessage();

      QLOG_DEBUG() << QObject::tr("%1::Start() - sending msg %2.").arg(objectName()).arg(m_NumberMessagesSent);

      r = socket->Send(msg->GetPackPointer(), msg->GetPackSize());
      if (r == 0)
        {
        std::cerr << "Failed to send, r=" << r << std::endl;
        exit(0);
        }

      m_NumberMessagesSent++;
      timeLastMessage->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());
    }
  }

  // Output a stats message, just in-case in of our end-points is listening.
  igtl::StringMessage::Pointer msg = igtl::StringMessage::New();
  msg->SetDeviceName("TestClientNifTKOIGTLTracking");
  msg->SetString("STATS");
  msg->Pack();

  r = socket->Send(msg->GetPackPointer(), msg->GetPackSize());
  if (r == 0)
    {
    std::cerr << "Failed to send stats request." << std::endl;
    exit(0);
    }
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

  std::cout << "TestClientNifTKOIGTLTracking: host = " << hostName << "." << std::endl;
  std::cout << "TestClientNifTKOIGTLTracking: port = " << port << "." << std::endl;
  std::cout << "TestClientNifTKOIGTLTracking: fps = " << fps << "." << std::endl;
  std::cout << "TestClientNifTKOIGTLTracking: total = " << total << "." << std::endl;
  std::cout << "TestClientNifTKOIGTLTracking: objects = " << objects << "." << std::endl;
  std::cout << "TestClientNifTKOIGTLTracking: Instantiating client." << std::endl;

  std::cout << "TestClientNifTKOIGTLTracking: Creating app." << std::endl;

  QCoreApplication app(argc, argv);

  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  niftk::TestClientNifTKOIGTLTracking client(hostName, port, fps, total, objects);

  std::cout << "TestClientNifTKOIGTLTracking: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
