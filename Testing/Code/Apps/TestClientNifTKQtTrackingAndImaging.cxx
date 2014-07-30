/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestClientNifTKQtTrackingAndImaging.h"
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkImageMessageHelpers.h>
#include <NiftyLinkUtils.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QApplication>

namespace niftk
{

//-----------------------------------------------------------------------------
TestClientNifTKQtTrackingAndImaging::TestClientNifTKQtTrackingAndImaging(const std::string& hostName,
    const int& portNumber,
    const int& fps,
    const int& channels,
    const int& totalNumberMessages,
    const QString imageFileName,
    QObject *parent)
: m_HostName(QString::fromStdString(hostName))
, m_PortNumber(portNumber)
, m_FramesPerSecond(fps)
, m_Channels(channels)
, m_IntendedNumberMessages(totalNumberMessages)
, m_NumberMessagesSent(0)
, m_Client(new NiftyLinkTcpClient(parent))
{
  m_ImageMessage = igtl::ImageMessage::New();
  niftk::LoadImage(imageFileName, m_ImageMessage);
  m_ImageMessage->Pack();

  this->setObjectName("TestClientNifTKQtTrackingAndImaging");
  connect(m_Client, SIGNAL(Connected()), this, SLOT(OnConnectedToServer()));
}


//-----------------------------------------------------------------------------
TestClientNifTKQtTrackingAndImaging::~TestClientNifTKQtTrackingAndImaging()
{
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());
  m_Client->ConnectToHost(m_HostName, m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::Shutdown()
{
  QLOG_INFO() << QObject::tr("%1::Shutdown() - starting.").arg(objectName());

  if (m_Client != NULL)
  {
    delete m_Client;
    m_Client = NULL;
  }

  QLOG_INFO() << QObject::tr("%1::Shutdown() - finished.").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
  this->RunTest();
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::RunTest()
{
  int nanosecondsBetweenTrackingMessages = 1000000000 / 100;
  int nanosecondsBetweenImagingMessages = 1000000000 / m_FramesPerSecond;
  QLOG_INFO() << QObject::tr("%1::RunTest() - %2 fps = %3 ns between imaging messages.").arg(objectName()).arg(m_FramesPerSecond).arg(nanosecondsBetweenImagingMessages);

  int imgSize[3];
  m_ImageMessage->GetDimensions(imgSize);

  igtl::ImageMessage::Pointer  localImage = igtl::ImageMessage::New();
  localImage->InitPack();
  localImage->SetDimensions(imgSize[0], imgSize[1], imgSize[2]);
  localImage->SetScalarType(igtl::ImageMessage::TYPE_UINT8);
  localImage->AllocateScalars();

  igtl::TimeStamp::Pointer timeLastImagingMessage = igtl::TimeStamp::New();
  timeLastImagingMessage->GetTime();

  igtl::TimeStamp::Pointer timeLastTrackingMessage = igtl::TimeStamp::New();
  timeLastTrackingMessage->GetTime();

  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  timeNow->GetTime();

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetOwnerName("TestClientNifTKQtTrackingAndImaging");
  m->SetSenderHostName("123.456.789.012");
  m->SetSenderPortNumber(1234);
  m->SetMessage(localImage.GetPointer());

  // This will occupy a lot of CPU, but we have multi-cpu machines, so no problem.
  while(m_NumberMessagesSent < m_IntendedNumberMessages)
  {
    timeNow->GetTime();

    if (niftk::GetDifferenceInNanoSeconds(timeNow, timeLastTrackingMessage) > nanosecondsBetweenTrackingMessages)
    {
      timeLastTrackingMessage->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());

      NiftyLinkMessageContainer::Pointer m = niftk::CreateTestTrackingDataMessage(timeCreated, m_Channels);
      m_Client->Send(m);
    }

    if (niftk::GetDifferenceInNanoSeconds(timeNow, timeLastImagingMessage) > nanosecondsBetweenImagingMessages)
    {
      timeLastImagingMessage->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());

      timeCreated->GetTime();
      localImage->SetTimeStamp(timeCreated);

      // Copy data from buffer.
      // Tokuda 2009 paper says latency is measure from time between the start of copying data into the
      // buffer, and the time to deserialise. So we record the time stamp just above this line.
      // So, we must be aiming to simulate a realistic example, whereby at each iteration we have to
      // copy data in, and then call Pack each time. At the moment, we don't care what the data is,
      // just that we are fairly testing the speed of the connection.
      memcpy(localImage->GetScalarPointer(), m_ImageMessage->GetScalarPointer(), imgSize[0]*imgSize[1]);
      localImage->Pack();

      m_Client->Send(m);
      m_NumberMessagesSent++;
    }
  }
  m_Client->RequestStats();
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 7) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps> <total>"                  << std::endl;
    std::cerr << "    <host>      : Hostname."                                           << std::endl;
    std::cerr << "    <port>      : Port #."                                             << std::endl;
    std::cerr << "    <fps>       : Frames per second."                                  << std::endl;
    std::cerr << "    <channels>  : Number of channels (tracking matrices per message)." << std::endl;
    std::cerr << "    <total>     : # of msgs in total."                                 << std::endl;
    std::cerr << "    <fileName>  : image file (.pgm)."                                  << std::endl;
    exit(0);
  }

  std::string hostName = argv[1];
  int         port     = atoi(argv[2]);
  int         fps      = atoi(argv[3]);
  int         channels = atoi(argv[4]);
  int         total    = atoi(argv[5]);
  std::string fileName = argv[6];

  std::cout << "TestClientNifTKQtTrackingAndImaging: host = " << hostName << "." << std::endl;
  std::cout << "TestClientNifTKQtTrackingAndImaging: port = " << port << "." << std::endl;
  std::cout << "TestClientNifTKQtTrackingAndImaging: fps = " << fps << "." << std::endl;
  std::cout << "TestClientNifTKQtTrackingAndImaging: channels = " << channels << "." << std::endl;
  std::cout << "TestClientNifTKQtTrackingAndImaging: total = " << total << "." << std::endl;
  std::cout << "TestClientNifTKQtTrackingAndImaging: fileName = " << fileName << "." << std::endl;

  std::cout << "TestClientNifTKQtTrackingAndImaging: Instantiating client." << std::endl;

  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // Start client.
  niftk::TestClientNifTKQtTrackingAndImaging client(hostName, port, fps, channels, total, QString::fromStdString(fileName));

  std::cout << "TestClientNifTKQtTrackingAndImaging: Creating app." << std::endl;

  QApplication app(argc, argv);
  QObject::connect(&app, SIGNAL(aboutToQuit()), &client, SLOT(Shutdown()));

  std::cout << "TestClientNifTKQtTrackingAndImaging: Launching app." << std::endl;

  QTimer::singleShot(220, &client, SLOT(Start()));
  int ret = app.exec();

  return ret;
}
