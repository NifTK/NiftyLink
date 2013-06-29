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

#include <string>
#include <sstream>
#include <fstream>

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

  connect(&m_Socket, SIGNAL(MessageSentSignal(unsigned long long)), this, SLOT(OnMessageSent(unsigned long long)));
  connect(&m_Socket, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(OnConnectToRemote()));
  connect(&m_Socket, SIGNAL(LostConnectionToRemoteSignal()), this, SLOT(OnLostConnectionToRemote()));
  connect(&m_Socket, SIGNAL(ShutdownSenderSignal()), this, SLOT(OnShutdownSender()));

  connect(&m_Socket, SIGNAL(SendMessageAccessTimes(QString)), this, SLOT(StoreMessageAccessTimes(QString)));
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

void TestImagingSender::StoreMessageAccessTimes(QString mat)
{
  m_MsgAccessTimes.append(mat);
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
  if (this->Setup())
  {
    while (!m_Socket.IsConnected())
    {
      QTest::qWait(250);
    }

    igtl::TimeStamp::Pointer startTime = igtl::TimeStamp::New();
    startTime->Update();


    // Throw data at the socket.
    this->SendData(m_NumberOfIterations);

    while (m_NumberSent < m_NumberOfIterations)
    {
      QTest::qWait(10);
    }

    // Finish
    igtl::TimeStamp::Pointer endTime = igtl::TimeStamp::New();
    endTime->Update();
    
    std::cout << "Sending Finished - Waiting a bit (5s) for good measure... " << std::endl;
    QTest::qWait(5000);


    std::stringstream outfileContents;
    outfileContents <<std::endl;
    outfileContents.precision(10);
    outfileContents << std::fixed;

    /*
    // Calculate results
    igtlUint64 result = GetDifferenceInNanoSeconds(endTime, startTime);
    double bps = ((double)m_NumberOfIterations * ((double)m_Image->byteCount()) * (double)8) / ((double)result / (double)1000000000.0);

    outfileContents << "Timing: for "  << m_NumberOfIterations << " iterations:" << std::endl;
    outfileContents << "  total time=" << result / (double)1000000000.0 << "(s)" << std::endl;
    outfileContents << "         fps=" << 1.0 / ((double)result / (double)m_NumberOfIterations / 1000000000.0) << std::endl;
    outfileContents << "         bps=" << bps << std::endl;
    outfileContents << "        kbps=" << bps / (double)1024 << std::endl;
    outfileContents << "        mbps=" << bps / (double)1024 / (double)1024 << std::endl;
    outfileContents << "     packing=" << m_TimePackingMessage / (double)1000000000.0 << "(s)" << " (" << 100.0 * (double)m_TimePackingMessage / (double)result << "%)" << std::endl;
    
    outfileContents << std::endl;
    */
    for (unsigned int  i = 1; i < m_MsgAccessTimes.size(); i++)
    {
      outfileContents <<m_MsgAccessTimes.at(i).toStdString() <<std::endl;
    }

    outfileContents.flush();

    if (!m_OutFileName.empty())
    {
      std::ofstream outfile(m_OutFileName.c_str(), std::ofstream::binary);
      outfile <<outfileContents.str();
      outfile.flush();
      outfile.close();
    }
    else
    {
      std::cout <<std::endl;
      std::cout << outfileContents.str();
    }


    // Tidy up.
    this->FinishUp();
  }
}


//-----------------------------------------------------------------------------
void TestImagingSender::SendData(const int& numberOfIterations)
{

  QString lha = GetLocalHostAddress();

  igtl::TimeStamp::Pointer timeZero = igtl::TimeStamp::New();

  std::cout.precision(10);
  std::cout <<std::fixed;
  std::cout <<"Starting to send data at: " <<timeZero->GetTimeInSeconds() <<std::endl; 

  for (int i = 0; i < numberOfIterations; i++)
  {
    igtl::TimeStamp::Pointer startIteration = igtl::TimeStamp::New();
    startIteration->Update();

    igtl::Matrix4x4 matrix;
    CreateRandomTransformMatrix(matrix);
    
    NiftyLinkImageMessage::Pointer msg(new NiftyLinkImageMessage());
    msg->TouchMessage("1. startIteration", startIteration);
    msg->SetQImage(*m_Image);
    msg->SetMatrix(matrix);

    // Let's update the host address and the timestamp
    msg->Update(lha);


    igtl::TimeStamp::Pointer endPacking = igtl::TimeStamp::New();
    endPacking->Update();
    msg->TouchMessage("2. endPacking", endPacking);

    m_Socket.SendMessage(msg);

    igtl::TimeStamp::Pointer endSending = igtl::TimeStamp::New();
    endSending->Update();
    igtlUint64 nanos = GetDifferenceInNanoSeconds(endPacking, startIteration);

    //std::cout <<"\nMessage " <<i <<" packaging took: " <<endPacking->GetTimeInSeconds() - startIteration->GetTimeInSeconds()
    //  <<"(sec) iteration took: " <<endSending->GetTimeInSeconds() - startIteration->GetTimeInSeconds()
    //  << "(sec) delay since start: " <<endSending->GetTimeInSeconds() - timeZero->GetTimeInSeconds() <<"(sec)\n";
    m_TimePackingMessage += nanos;
  }
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 5) // check number of arguments
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

  std::string filename;
  if (argv[5] != NULL)
    filename = std::string(argv[5]);

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

  if (!filename.empty())
    sender.SetOutfilename(filename);

  QApplication app(argc, argv);
  QObject::connect(&sender, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &sender, SLOT(Run()));
  int ret = app.exec();

  return ret;
}
