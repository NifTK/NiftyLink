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
#include <NiftyLinkTrackingDataMessageHelpers.h>
#include <NiftyLinkUtils.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QCoreApplication>

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
, m_ShuttingDown(false)
{
  m_ImageMessage = igtl::ImageMessage::New();
  niftk::LoadImage(imageFileName, m_ImageMessage);

  this->setObjectName("TestClientNifTKQtTrackingAndImaging");
  connect(m_Client, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedToServer()));
  connect(m_Client, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnected()), Qt::QueuedConnection);
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
  QCoreApplication::quit();
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
  this->RunTest();
  QTimer::singleShot(1000, m_Client, SLOT(DisconnectFromHost()));
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::OnDisconnected()
{
  if (!m_ShuttingDown)
  {
    this->Shutdown();
  }
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtTrackingAndImaging::RunTest()
{
  igtlUint64 nanosecondsBetweenTrackingMessages = 1000000000 / 100;
  igtlUint64 nanosecondsBetweenImagingMessages = 1000000000 / m_FramesPerSecond;
  QLOG_INFO() << QObject::tr("%1::RunTest() - %2 fps = %3 ns between imaging messages.").arg(objectName()).arg(m_FramesPerSecond).arg(nanosecondsBetweenImagingMessages);

  int imgSize[3];
  m_ImageMessage->GetDimensions(imgSize);

  igtl::ImageMessage::Pointer  localImage = igtl::ImageMessage::New();
  localImage->InitPack();
  localImage->SetDimensions(imgSize[0], imgSize[1], imgSize[2]);
  localImage->SetScalarType(m_ImageMessage->GetScalarType());
  localImage->SetNumComponents(m_ImageMessage->GetNumComponents());
  localImage->AllocateScalars();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetOwnerName("TestClientNifTKQtTrackingAndImaging");
  m->SetSenderHostName("123.456.789.012");
  m->SetSenderPortNumber(1234);
  m->SetMessage(localImage.GetPointer());

  igtlUint64 numberTrackingImagesSent = 0;

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  timeNow->GetTime();

  igtl::TimeStamp::Pointer timeStarted = igtl::TimeStamp::New();
  timeStarted->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());

  // This will occupy a lot of CPU, but we have multi-cpu machines, so assumed to be no problem.
  while(m_NumberMessagesSent < m_IntendedNumberMessages)
  {
    if (!m_Client->IsConnected())
    {
      QLOG_ERROR() << QObject::tr("%1::RunTest() - Early exit, client disconnected.").arg(objectName());
      return;
    }

    timeNow->GetTime();
    igtlUint64 diff = niftk::GetDifferenceInNanoSeconds(timeNow, timeStarted);

    if (diff >= nanosecondsBetweenTrackingMessages*numberTrackingImagesSent)
    {
      NiftyLinkMessageContainer::Pointer tm = niftk::CreateTrackingDataMessageWithRandomData(timeCreated, m_Channels);
      m_Client->Send(tm);
      numberTrackingImagesSent++;
    }

    if (diff >= nanosecondsBetweenImagingMessages*m_NumberMessagesSent)
    {
      timeCreated->GetTime();
      localImage->SetTimeStamp(timeCreated);

      memcpy(localImage->GetScalarPointer(), m_ImageMessage->GetScalarPointer(),
             imgSize[0] * imgSize[1] * imgSize[2]
             * localImage->GetNumComponents()
             * sizeof(localImage->GetScalarType()));

      localImage->Pack();

      m_Client->Send(m);
      m_NumberMessagesSent++;
    }
  }
  m_Client->RequestStats();
  QLOG_INFO() << QObject::tr("%1::RunTest() - stats requested.").arg(objectName());
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

  std::cout << "TestClientNifTKQtTrackingAndImaging: Creating app." << std::endl;

  QCoreApplication app(argc, argv);

  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  std::cout << "TestClientNifTKQtTrackingAndImaging: Launching app." << std::endl;

  niftk::TestClientNifTKQtTrackingAndImaging client(hostName, port, fps, channels, total, QString::fromStdString(fileName));
  QTimer::singleShot(220, &client, SLOT(Start()));

  int ret = app.exec();

  return ret;
}
