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
  m_TimeZero = 0;
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

    double totalTimeMs = double(endTime->GetTimeInNanoSeconds() - m_TimeZero->GetTimeInNanoSeconds())/1000.0/1000.0;
    
    // Configure cout precision
    std::cout.precision(10);
    std::cout <<std::fixed;
    std::cout << "Sending finished at: " <<endTime->GetTimeInNanoSeconds() <<std::endl; 
    std::cout << "Time spent with sending:" <<endTime->GetTimeInNanoSeconds() - m_TimeZero->GetTimeInNanoSeconds() <<"(ns) / "
              << totalTimeMs <<"(ms)\n";
    std::cout << "Waiting a bit (5s) before terminating... " << std::endl;
    QTest::qWait(5000);

    // Calculate results
    double dataThroughput = (double)m_NumberOfIterations * (double)m_Image->byteCount() / (totalTimeMs/1000.0);

    std::stringstream outfileContents;
    outfileContents <<std::endl;
    outfileContents.precision(10);
    outfileContents << std::fixed;

    outfileContents << "Number of messages received: " <<m_NumberOfIterations << std::endl;
    outfileContents << "Total time=" <<totalTimeMs << "(ms) / " << totalTimeMs/1000.0 << "(s)" << std::endl;
    outfileContents << "Fps=" <<(double)m_NumberOfIterations / (totalTimeMs/1000.0)  << std::endl;
    outfileContents << "Message Size=" <<(double)m_Image->byteCount() <<"(bytes) / " 
                                       <<(double)m_Image->byteCount()/1024.0 <<"(kbytes) / "
                                       <<(double)m_Image->byteCount()/1024.0/1024.0 <<"(Mbytes)" << std::endl;
    outfileContents << "Data Throughput=" <<dataThroughput << "(bytes/s) / " 
                                            <<dataThroughput/1024.0 << "(kbytes/s) / "
                                            <<dataThroughput/1024.0/1024.0 << "(Mbytes/s) / "<< std::endl;

    outfileContents << std::endl;
    outfileContents <<"ID,Created,EndPack,SendStart,SendFinish\n";
    for (int  i = 0; i < m_MsgAccessTimes.size(); i++)
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
  // Update the local host address
  QString lha = GetLocalHostAddress();

  // Generate a random matrix
  igtl::Matrix4x4 matrix;
  CreateRandomTransformMatrix(matrix);
  
  // Get the current time
  m_TimeZero = igtl::TimeStamp::New();
  m_TimeZero->Update();

  // Configure cout precision
  std::cout.precision(10);
  std::cout <<std::fixed;
  std::cout <<"Starting to send data at: " <<m_TimeZero->GetTimeInNanoSeconds() <<std::endl; 

  for (int i = 0; i < numberOfIterations; i++)
  {
    // Create a new message
    NiftyLinkImageMessage::Pointer msg(new NiftyLinkImageMessage());

    // Stuff the data into the message
    msg->SetQImage(*m_Image);
    msg->SetMatrix(matrix);

    // Let's update the host address and the timestamp
    msg->Update(lha);

    // Packing is finished, get timestamp
    igtl::TimeStamp::Pointer endPacking = igtl::TimeStamp::New();
    endPacking->Update();
    
    // Store timestamp into the message
    msg->TouchMessage("1. endPacking", endPacking);

    // Pass the message to the socket for transmission
    m_Socket.SendMessage(msg);
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
  std::cout << "    size:" << image.byteCount() << std::endl;

  if (!filename.empty())
    sender.SetOutfilename(filename);

  QApplication app(argc, argv);
  QObject::connect(&sender, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &sender, SLOT(Run()));
  int ret = app.exec();

  return ret;
}
