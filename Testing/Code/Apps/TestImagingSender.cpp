/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include <QObject>
#include <QApplication>
#include <QtGui/QImage>
#include <QUrl>
#include <QTest>
#include <iostream>
#include <igtlTimeStamp.h>
#include "NiftyLinkSocketObject.h"
#include "NiftyLinkImageMessage.h"
#include "TestImagingSender.h"

//-----------------------------------------------------------------------------
TestImagingSender::TestImagingSender(const QImage* image,
    const QString& hostName,
    const int& portNumber,
    const int& numberOfIterations
    )
  : m_Image(image)
  , m_HostName(hostName)
  , m_PortNumber(portNumber)
  , m_NumberOfIterations(numberOfIterations)
{
  m_TimePackingMessage = 0;
  m_NumberSent = 0;

  connect(&m_Socket, SIGNAL(MessageSent(unsigned long long)), this, SLOT(OnMessageSent(unsigned long long)));
  connect(&m_Socket, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(OnConnectToRemote()));
  connect(&m_Socket, SIGNAL(LostConnectionToRemoteSignal()), this, SLOT(OnLostConnectionToRemote()));
  connect(&m_Socket, SIGNAL(ShutdownSender()), this, SLOT(OnShutdownSender()));
}


//-----------------------------------------------------------------------------
TestImagingSender::~TestImagingSender()
{
}


//-----------------------------------------------------------------------------
void TestImagingSender::OnConnectToRemote()
{
  std::cout << "Connected" << std::endl;
}


//-----------------------------------------------------------------------------
void TestImagingSender::OnLostConnectionToRemote()
{
  std::cerr << "Remote end disconnected" << std::endl;
  emit Done();
}


//-----------------------------------------------------------------------------
void TestImagingSender::FinishUp()
{
  if (m_Socket.IsConnected())
  {
    m_Socket.CloseSocket();
  }
}


//-----------------------------------------------------------------------------
void TestImagingSender::OnShutdownSender()
{
  std::cerr << "My sender thread shutdown" << std::endl;
  emit Done();
}


//-----------------------------------------------------------------------------
void TestImagingSender::OnMessageSent(unsigned long long timestamp)
{
  m_NumberSent++;
}


//-----------------------------------------------------------------------------
bool TestImagingSender::Setup()
{
  QUrl url;
  url.setHost(m_HostName);
  url.setPort(m_PortNumber);

  bool connectionResult = m_Socket.ConnectToRemote(url);

  if (!connectionResult)
  {
    std::cerr << "Failed to connect to host=" << m_HostName.toStdString() << ", port=" << m_PortNumber << std::endl;
  }

  return connectionResult;
}


//-----------------------------------------------------------------------------
void TestImagingSender::Run()
{
  if(this->Setup())
  {
    while(!m_Socket.IsConnected())
    {
      QTest::qWait(250);
    }

    igtl::TimeStamp::Pointer startTime = igtl::TimeStamp::New();
    startTime->GetTime();


    // Throw data at the socket.
    this->SendData(m_NumberOfIterations);

    while(m_NumberSent < m_NumberOfIterations)
    {
      QTest::qWait(10);
    }

    // Finish
    igtl::TimeStamp::Pointer endTime = igtl::TimeStamp::New();
    endTime->GetTime();

    // Calculate results
    igtlUint64 result = GetDifferenceInNanoSeconds(endTime, startTime);
    double bps = ((double)m_NumberOfIterations*((double)m_Image->byteCount())*(double)8) / ((double)result/(double)1000000000.0);

    std::cout << "Timing: for " << m_NumberOfIterations << " iterations:" << std::endl;
    std::cout << "  total time=" << result / (double)1000000000.0 << "(s)" << std::endl;
    std::cout << "         fps=" << 1.0/((double)result/(double)m_NumberOfIterations/1000000000.0) << std::endl;
    std::cout << "         bps=" << bps << std::endl;
    std::cout << "        kbps=" << bps/(double)1024 << std::endl;
    std::cout << "        mbps=" << bps/(double)1024/(double)1024 << std::endl;
    std::cout << "     packing=" << m_TimePackingMessage / (double)1000000000.0 << "(s)" << " (" << 100.0*(double)m_TimePackingMessage/(double)result << "%)" << std::endl;

    // Tidy up.
    this->FinishUp();
  }
}


//-----------------------------------------------------------------------------
void TestImagingSender::SendData(const int& numberOfIterations)
{

  for (int i = 0; i < numberOfIterations; i++)
  {
    igtl::TimeStamp::Pointer startIteration = igtl::TimeStamp::New();

    igtl::Matrix4x4 matrix;
    NiftyLinkImageMessage::Pointer msg(new NiftyLinkImageMessage());
    msg->SetQImage(*m_Image);
    msg->SetMatrix(matrix);

    igtl::TimeStamp::Pointer endPacking = igtl::TimeStamp::New();
    endPacking->GetTime();

    m_Socket.SendMessage(msg);

    igtl::TimeStamp::Pointer endSending = igtl::TimeStamp::New();
    endSending->GetTime();

    igtlUint64 nanos = GetDifferenceInNanoSeconds(endPacking, startIteration);
    m_TimePackingMessage += nanos;
  }
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 5) // check number of arguments
    {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <hostname> <port> <iters> <filename>" << std::endl;
    std::cerr << "    <hostname> : IP or host name"                              << std::endl;
    std::cerr << "    <port>     : Port #"                                       << std::endl;
    std::cerr << "    <iters>    : Number of iterations"                         << std::endl;
    std::cerr << "    <filename> : Image file to send (.raw or .png)"            << std::endl;
    exit(0);
    }

  char*  hostname   = argv[1];
  int    port       = atoi(argv[2]);
  int    iters      = atoi(argv[3]);
  char*  imagename  = argv[4];

  std::cout << "TestImagingSender:" << std::endl;
  std::cout << " to host:" << hostname << std::endl;
  std::cout << " on port:" << port << std::endl;
  std::cout << "   iters:" << iters << std::endl;
  std::cout << "     img:" << imagename << std::endl;

  // Load image
  QImage image;
  if (!image.load(imagename))
  {
    std::cerr << "Failed to load image:" << imagename << std::endl;
    return EXIT_FAILURE;
  }
  image.convertToFormat(QImage::Format_Indexed8);
  TestImagingSender sender(&image, QString(hostname), port, iters);

  QApplication app(argc,argv);
  QObject::connect(&sender, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &sender, SLOT(Run()));
  int ret = app.exec();

  return ret;
}
