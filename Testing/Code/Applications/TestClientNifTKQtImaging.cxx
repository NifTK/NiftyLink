/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestClientNifTKQtImaging.h"
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkImageMessageHelpers.h>
#include <NiftyLinkUtils.h>

#include <QsLog.h>
#include <QsLogDest.h>
#include <QCoreApplication>

namespace niftk
{

//-----------------------------------------------------------------------------
TestClientNifTKQtImaging::TestClientNifTKQtImaging(const std::string& hostName,
    const int& portNumber,
    const int& fps,
    const int& totalNumberMessages,
    const QString imageFileName,
    QObject *parent)
: m_HostName(QString::fromStdString(hostName))
, m_PortNumber(portNumber)
, m_FramesPerSecond(fps)
, m_IntendedNumberMessages(totalNumberMessages)
, m_NumberMessagesSent(0)
, m_Client(new NiftyLinkTcpClient(parent))
, m_ShuttingDown(false)
{
  m_ImageMessage = igtl::ImageMessage::New();
  niftk::LoadImage(imageFileName, m_ImageMessage);
  m_ImageMessage->Pack();

  this->setObjectName("TestClientNifTKQtImaging");
  connect(m_Client, SIGNAL(Connected(QString,int)), this, SLOT(OnConnectedToServer()));
  connect(m_Client, SIGNAL(Disconnected(QString,int)), this, SLOT(OnDisconnected()), Qt::QueuedConnection);
}


//-----------------------------------------------------------------------------
TestClientNifTKQtImaging::~TestClientNifTKQtImaging()
{
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtImaging::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());
  m_Client->ConnectToHost(m_HostName, m_PortNumber);
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtImaging::Shutdown()
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
void TestClientNifTKQtImaging::OnConnectedToServer()
{
  QLOG_INFO() << QObject::tr("%1::OnConnectedToServer().").arg(objectName());
  this->RunTest();
  QTimer::singleShot(1000, m_Client, SLOT(DisconnectFromHost()));
}



//-----------------------------------------------------------------------------
void TestClientNifTKQtImaging::OnDisconnected()
{
  if (!m_ShuttingDown)
  {
    this->Shutdown();
  }
}


//-----------------------------------------------------------------------------
void TestClientNifTKQtImaging::RunTest()
{
  igtlUint64 nanosecondsBetweenMessages = 1000000000 / m_FramesPerSecond;
  QLOG_INFO() << QObject::tr("%1::RunTest() - %2 fps = %3 ns between messages.").arg(objectName()).arg(m_FramesPerSecond).arg(nanosecondsBetweenMessages);

  int imgSize[3];
  m_ImageMessage->GetDimensions(imgSize);

  igtl::ImageMessage::Pointer  localImage = igtl::ImageMessage::New();
  localImage->InitPack();
  localImage->SetDimensions(imgSize[0], imgSize[1], imgSize[2]);
  localImage->SetScalarType(igtl::ImageMessage::TYPE_UINT8);
  localImage->AllocateScalars();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetOwnerName("TestClientNifTKQtImaging");
  m->SetSenderHostName("123.456.789.012");
  m->SetSenderPortNumber(1234);
  m->SetMessage(localImage.GetPointer());

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

    if (diff >= nanosecondsBetweenMessages*m_NumberMessagesSent)
    {
      timeCreated->GetTime();
      localImage->SetTimeStamp(timeCreated);

      memcpy(localImage->GetScalarPointer(), m_ImageMessage->GetScalarPointer(), imgSize[0]*imgSize[1]);
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

  if (argc != 6) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps> <total> <fileName>"     << std::endl;
    std::cerr << "    <host>      : Hostname"                               << std::endl;
    std::cerr << "    <port>      : Port #"                                 << std::endl;
    std::cerr << "    <fps>       : Frames per second."                     << std::endl;
    std::cerr << "    <total>     : # of msgs in total."                    << std::endl;
    std::cerr << "    <fileName>  : image file (.pgm)."                     << std::endl;
    exit(0);
  }

  std::string hostName = argv[1];
  int         port     = atoi(argv[2]);
  int         fps      = atoi(argv[3]);
  int         total    = atoi(argv[4]);
  std::string fileName = argv[5];

  std::cout << "TestClientNifTKQtImaging: host = " << hostName << "." << std::endl;
  std::cout << "TestClientNifTKQtImaging: port = " << port << "." << std::endl;
  std::cout << "TestClientNifTKQtImaging: fps = " << fps << "." << std::endl;
  std::cout << "TestClientNifTKQtImaging: total = " << total << "." << std::endl;
  std::cout << "TestClientNifTKQtImaging: fileName = " << fileName << "." << std::endl;
  std::cout << "TestClientNifTKQtImaging: Instantiating client." << std::endl;

  std::cout << "TestClientNifTKQtImaging: Creating app." << std::endl;

  QCoreApplication app(argc, argv);

  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  // This is to make sure we have the best possible system timer.
#if defined(_WIN32) && !defined(__CYGWIN__)
  niftk::InitializeWinTimers();
#endif

  std::cout << "TestClientNifTKQtImaging: Launching app." << std::endl;

  niftk::TestClientNifTKQtImaging client(hostName, port, fps, total, QString::fromStdString(fileName));
  QTimer::singleShot(220, &client, SLOT(Start()));

  int ret = app.exec();

  return ret;
}
