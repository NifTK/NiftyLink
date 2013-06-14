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
  igtl::TimeStamp::Pointer beforeTime = igtl::TimeStamp::New();

  if (m_CumulativeMessageCount == 0)
  {
    std::cerr << "Started to receive messages" << std::endl;
    m_StartTime = beforeTime;
  }

  unsigned long long nanotime;
  double seconds = 0;
  igtlUint32 sec, msec, usec, nsec, fraction;

  // Get time when the current message was created
  igtl::TimeStamp::Pointer time = message->GetTimeCreated();
  seconds = time->GetTimeInSeconds();
  m_SentTimeStamps.push_back(seconds);

  // Get time when the current message was received
  time = message->GetTimeReceived();
  seconds = time->GetTimeInSeconds();
  m_ReceivedTimeStamps.push_back(seconds);

  // Get OpenIGTLink message pointer and update the cumulative message size
  igtl::MessageBase::Pointer msgP;
  message->GetMessagePointer(msgP);
  m_CumulativeMessageSize += (msgP->GetBodySizeToRead() + 58);

  // Unpack message
  igtl::TimeStamp::Pointer afterTime = igtl::TimeStamp::New();
  igtlUint64 result = GetDifferenceInNanoSeconds(afterTime, beforeTime);

  m_CumulativeTime += result;
  m_CumulativeMessageCount++;

  m_EndTime = afterTime;

  if (m_CumulativeMessageCount >= m_NumberOfMessagesExpected)
  {
    std::ofstream outfile(m_OutFileName.c_str(), std::ofstream::binary);
    outfile <<std::endl;
    outfile.precision(10);
    outfile << std::fixed;
    
    outfile << "Number of messages received: " << m_CumulativeMessageCount << std::endl;
    outfile << "Size / message: " << (double)m_CumulativeMessageSize/(double)m_CumulativeMessageCount << std::endl;
    outfile << "Total number of bytes received: " << m_CumulativeMessageSize << std::endl;

    outfile <<"First message was created at: " <<m_SentTimeStamps.at(0) << std::endl;
    outfile <<"Last message was created at: " <<m_SentTimeStamps.at(m_SentTimeStamps.size()-1) << std::endl;
    outfile <<"Total time to send messsages: " <<m_SentTimeStamps.at(m_SentTimeStamps.size()-1) - m_SentTimeStamps.at(0) << std::endl;
    outfile <<std::endl;
    outfile <<"First message was received at: " <<m_ReceivedTimeStamps.at(0) << std::endl;
    outfile <<"Last message was received at: " <<m_ReceivedTimeStamps.at(m_ReceivedTimeStamps.size()-1) << std::endl;
    outfile <<"Total time to receive messsages: " <<m_ReceivedTimeStamps.at(m_ReceivedTimeStamps.size()-1) - m_ReceivedTimeStamps.at(0) << std::endl;

    double first = m_SentTimeStamps.at(0);
    double last  = m_ReceivedTimeStamps.at(m_ReceivedTimeStamps.size()-1);

    outfile <<std::endl;
    igtlUint32 sec, msec, usec, nsec, fraction;
    
    outfile << "Time elapsed from the creation of the first message\n";
    outfile << "till the receipt of the last: " <<last - first <<" sec" <<std::endl;
    outfile <<std::endl;

    outfile << "Throughput: " <<(double)m_CumulativeMessageSize / (last - first) <<" bytes/sec "<<std::endl;
    outfile << "            " <<(double)m_CumulativeMessageSize /1024 /1024 / (last - first) <<" megabytes/sec "<<std::endl;


    outfile <<std::endl <<std::endl;


    


/*
    outfile << "Sent stamps "<<std::endl;
    for (unsigned int  i = 0; i < m_SentTimeStamps.size(); i++)
    {
      outfile <<i <<"," << m_SentTimeStamps.at(i) <<std::endl;
    }
    
    outfile <<std::endl <<std::endl;
    outfile << "Sent stamps "<<std::endl;
    for (unsigned int  i = 0; i < m_ReceivedTimeStamps.size(); i++)
    {
      outfile <<i <<"," << m_ReceivedTimeStamps.at(i) <<std::endl;
    }
    outfile <<std::endl <<std::endl;
*/
    outfile << "NULL delay: " <<m_ReceivedTimeStamps.at(0) - m_SentTimeStamps.at(0) <<std::endl;
    outfile << "Delays "<<std::endl;
    for (unsigned int  i = 1; i < m_ReceivedTimeStamps.size(); i++)
    {
      double curDiff = m_ReceivedTimeStamps.at(i) - m_SentTimeStamps.at(i);
      double prevDiff = m_ReceivedTimeStamps.at(i-1) - m_SentTimeStamps.at(i-1);

      double sendDelay = m_SentTimeStamps.at(i) - m_SentTimeStamps.at(0);
      double receDelay = m_ReceivedTimeStamps.at(i) - m_SentTimeStamps.at(0);
      outfile <<"Msg num: " <<i <<" send delay: " <<sendDelay <<" rec delay: " <<receDelay <<" relative diff: " << curDiff - prevDiff <<std::endl;
    }

    outfile.flush();
    outfile.close();
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
  std::string filename(argv[3]);

  TestImagingReceiver receiver(port, iters);
  receiver.SetOutfilename(filename);

  QApplication app(argc, argv);
  QObject::connect(&receiver, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &receiver, SLOT(Setup()));
  int ret = app.exec();

  return ret;
}
