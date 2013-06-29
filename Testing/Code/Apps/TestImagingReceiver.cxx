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
#include "TestImagingReceiver.h"

#include <string>
#include <sstream>
#include <fstream>

//-----------------------------------------------------------------------------
TestImagingReceiver::TestImagingReceiver(const int& portNumber, const int& numberOfMessagesExpected)
  : m_PortNumber(portNumber)
  , m_NumberOfMessagesExpected(numberOfMessagesExpected)
{
  connect(&m_Socket, SIGNAL(ClientConnectedSignal()), this, SLOT(OnClientConnected()));
  connect(&m_Socket, SIGNAL(ClientDisconnectedSignal()), this, SLOT(OnClientDisconnected()));
  connect(&m_Socket, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(OnMessageReceived(NiftyLinkMessage::Pointer)));
}


//-----------------------------------------------------------------------------
TestImagingReceiver::~TestImagingReceiver()
{
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::OnClientConnected()
{
  std::cerr << "Client Connected" << std::endl;
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::OnClientDisconnected()
{
  std::cerr << "Remote client Disconnected" << std::endl;
  this->FinishUp();
  emit Done();
  exit(0);
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::FinishUp()
{
  if (m_Socket.IsConnected())
  {
    m_Socket.CloseSocket();
  }
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::Setup()
{
  m_Socket.SetConnectTimeOut(60); // keep it alive to avoid instantly exiting.

  m_CumulativeTime         = 0;
  m_CumulativeMessageCount = 0;
  m_CumulativeMessageSize  = 0;

  bool connectionResult = m_Socket.ListenOnPort(m_PortNumber);

  if (!connectionResult)
  {
    std::cerr << "Failed to listen on port=" << m_PortNumber << std::endl;
    emit Done();
  }
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::OnMessageReceived(NiftyLinkMessage::Pointer message)
{
  if (m_CumulativeMessageCount == 0)
  {
    std::cout << "Started to receive messages" << std::endl;
    m_StartTime = igtl::TimeStamp::New();
    m_StartTime->Update();
  }

  igtlUint64 nanoseconds = 0;

  // Get time when the current message was created
  igtl::TimeStamp::Pointer time = message->GetTimeCreated();
  nanoseconds = time->GetTimeInNanoSeconds();
  m_SentTimeStamps.push_back(nanoseconds);

  // Get time when the current message has arrived
  time = message->GetTimeArrived();
  nanoseconds = time->GetTimeInNanoSeconds();
  m_ArrivedTimeStamps.push_back(nanoseconds);

  // Get time when the current message was fully received
  time = message->GetTimeReceived();
  nanoseconds = time->GetTimeInNanoSeconds();
  m_ReceivedTimeStamps.push_back(nanoseconds);

  // Get OpenIGTLink message pointer and update the cumulative message size
  igtl::MessageBase::Pointer msgP;
  message->GetMessagePointer(msgP);
  m_CumulativeMessageSize += (msgP->GetBodySizeToRead() + 58);

  m_CumulativeMessageCount++;

  if (m_CumulativeMessageCount >= m_NumberOfMessagesExpected)
  {

    m_EndTime = igtl::TimeStamp::New();
    m_EndTime->Update();

    // Calculate results
    double totalTimeMs = double(m_EndTime->GetTimeInNanoSeconds() - m_StartTime->GetTimeInNanoSeconds())/1000.0/1000.0;
    double msgSize = (double)m_CumulativeMessageSize/(double)m_CumulativeMessageCount;
    igtlUint64 first = m_SentTimeStamps.at(0);
    igtlUint64 last  = m_ReceivedTimeStamps.at(m_ReceivedTimeStamps.size()-1);

    igtlUint64 transmissionTimeNsec = last - first;
    double transmissionTimeSec = double(last - first)/1000000000.0;
    double dataThroughput = m_CumulativeMessageSize / transmissionTimeSec;


    std::stringstream outfileContents;
    outfileContents <<std::endl;
    outfileContents.precision(10);
    outfileContents << std::fixed;

    outfileContents << "Number of messages received: " << m_CumulativeMessageCount << std::endl;
    outfileContents << "Total time elapsed=" <<totalTimeMs << "(ms) / " << totalTimeMs/1000.0 << "(s)" << std::endl;
    outfileContents << "Total transmission time (first msg created --> last received)=" <<transmissionTimeSec <<" (sec)" <<std::endl;

    outfileContents << "Fps=" <<(double)m_CumulativeMessageCount / transmissionTimeSec  << std::endl; 
    
    outfileContents << "Message Size=" <<msgSize <<"(bytes) / " 
                                       <<msgSize/1024.0 <<"(kbytes) / "
                                       <<msgSize/1024.0/1024.0 <<"(Mbytes)" << std::endl;

    outfileContents << "Data Throughput=" <<dataThroughput << "(bytes/s) / " 
                                            <<dataThroughput/1024.0 << "(kbytes/s) / "
                                            <<dataThroughput/1024.0/1024.0 << "(Mbytes/s) / "<< std::endl;

    outfileContents << std::endl;
    outfileContents <<"Created,Arrived,Received,Delay\n";
    for (unsigned int  i = 0; i < m_ReceivedTimeStamps.size(); i++)
    {
      double receDelay = m_ReceivedTimeStamps.at(i) - m_SentTimeStamps.at(i);
      outfileContents <<m_SentTimeStamps.at(i) <<"," <<m_ArrivedTimeStamps.at(i) <<"," <<m_ReceivedTimeStamps.at(i) <<"," <<receDelay <<std::endl;
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
   
    std::cout <<"Number of messages received: " << m_CumulativeMessageCount << std::endl;
    std::cout <<"Total number of bytes received: " << m_CumulativeMessageSize << std::endl;
    std::cout <<"Total transmission time (first msg created --> last received): " <<transmissionTimeSec <<" (sec)" <<std::endl;
  }
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc < 3) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port> <iters>"    << std::endl;
    std::cerr << "    <port>     : Port #"                    << std::endl;
    std::cerr << "    <iters>    : Number of images expected" << std::endl;
    exit(0);
  }

  int    port       = atoi(argv[1]);
  int    iters      = atoi(argv[2]);

  std::cout << "TestImagingSender:" << std::endl;
  std::cout << " on port:" << port << std::endl;
  std::cout << "   iters:" << iters << std::endl;

  std::string filename;
  if (argv[3] != NULL)
    filename = std::string(argv[3]);

  TestImagingReceiver receiver(port, iters);
  if (!filename.empty())
    receiver.SetOutfilename(filename);

  QApplication app(argc, argv);
  QObject::connect(&receiver, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &receiver, SLOT(Setup()));
  int ret = app.exec();

  return ret;
}
