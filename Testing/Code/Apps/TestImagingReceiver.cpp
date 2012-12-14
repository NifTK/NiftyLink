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
#include "OIGTLSocketObject.h"
#include "OIGTLImageMessage.h"
#include "TestImagingReceiver.h"

//-----------------------------------------------------------------------------
TestImagingReceiver::TestImagingReceiver(const int& portNumber, const int& numberOfMessagesExpected)
  : m_PortNumber(portNumber)
, m_NumberOfMessagesExpected(numberOfMessagesExpected)
{
  connect(&m_Socket, SIGNAL(clientConnectedSignal()), this, SLOT(OnClientConnected()));
  connect(&m_Socket, SIGNAL(clientDisconnectedSignal()), this, SLOT(OnClientDisconnected()));
  connect(&m_Socket, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(OnMessageReceived(OIGTLMessage::Pointer)));
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
  if (m_Socket.isConnected())
  {
    m_Socket.closeSocket();
  }
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::Setup()
{
  m_Socket.setConnectTimeOut(60); // keep it alive to avoid instantly exiting.

  m_CumulativeTime = 0;
  m_CumulativeMessageCount = 0;

  bool connectionResult = m_Socket.listenOnPort(m_PortNumber);

  if (!connectionResult)
  {
    std::cerr << "Failed to listen on port=" << m_PortNumber << std::endl;
    emit Done();
  }
}


//-----------------------------------------------------------------------------
void TestImagingReceiver::OnMessageReceived(OIGTLMessage::Pointer)
{
  igtl::TimeStamp::Pointer beforeTime = igtl::TimeStamp::New();

  if (m_CumulativeMessageCount == 0)
  {
    std::cerr << "Started to receive messages" << std::endl;
    m_StartTime = beforeTime;
  }

  // Unpack message

  igtl::TimeStamp::Pointer afterTime = igtl::TimeStamp::New();
  igtlUint64 result = GetDifferenceInNanoSeconds(afterTime, beforeTime);

  m_CumulativeTime += result;
  m_CumulativeMessageCount++;

  m_EndTime = afterTime;

  if (m_CumulativeMessageCount >= m_NumberOfMessagesExpected)
  {
    igtlUint64 totalTime = GetDifferenceInNanoSeconds(m_EndTime, m_StartTime);
    std::cout << "Timing: for " << m_NumberOfMessagesExpected << " iterations:" << std::endl;
    std::cout << "       total time=" << totalTime / (double)1000000000.0 << "(s)" << std::endl;
    std::cout << "              fps=" << 1.0/((double)totalTime/(double)m_NumberOfMessagesExpected/1000000000.0) << std::endl;

  }
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 3) // check number of arguments
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

  TestImagingReceiver receiver(port, iters);

  QApplication app(argc,argv);
  QObject::connect(&receiver, SIGNAL(Done()), &app, SLOT(quit()), Qt::QueuedConnection);

  QTimer::singleShot(220, &receiver, SLOT(Setup()));
  int ret = app.exec();

  return ret;
}
