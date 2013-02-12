/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "stdlib.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include "TestSendReceive_Timing.h"
#include "NLTest_Data.h"

TestSendReceive_Timing::TestSendReceive_Timing(void)
{
  m_Socket1 = NULL;
  m_Socket2 = NULL;

  m_SuccessCounter = 0;
  m_NumOfMsg = 500;
  m_Received = 0;

  m_DoStream    = false;
  //m_ConnectedTo = false;
  //m_Connecting  = false;

  m_TestCounter = 0;
  m_SuccessCounter = 0;

  m_TotalTimeDiff = 0;

  connect(&m_TimeOut, SIGNAL(timeout()), this, SLOT(QuitTest2()) );
  //m_TimeOut.start(20000);
}

TestSendReceive_Timing::~TestSendReceive_Timing(void)
{
}


void TestSendReceive_Timing::SetupTest()
{
  m_Socket1 = new NiftyLinkSocketObject();
  m_Socket1->setObjectName("Socket1");
  m_Socket2 = new NiftyLinkSocketObject();
  m_Socket2->setObjectName("Socket2");

  connect(m_Socket1, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  connect(m_Socket1, SIGNAL(MessageSentSignal(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );
  connect(m_Socket2, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  connect(m_Socket2, SIGNAL(MessageSentSignal(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );

  #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    m_Socket1->InitializeWinTimers();
  #endif

  QUrl url;
  //url.setHost(QString("128.16.4.235"));
  url.setHost(QString("localhost"));
  url.setPort(3200);

  //Start sender / listener
  m_Socket1->ListenOnPort(3200);
  m_Socket2->ConnectToRemote(url);
}

void TestSendReceive_Timing::PerformTest()
{
  std::cout << "Sending " <<m_NumOfMsg <<" messages from Socket2 --> Socket 1" <<std::endl;
  for (int i = 0; i< m_NumOfMsg; i++)
  {

//  m_MsgToSend.operator =(NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
//  static_cast<NiftyLinkTransformMessage::Pointer>(m_MsgToSend)->SetMatrix(dummyTransformMatrix);
//  m_MsgToSend->Update(GetLocalHostAddress());

    m_MsgToSend.operator =(NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
    m_MsgToSend->InitializeWithRandomData();
    m_MsgToSend->Update(GetLocalHostAddress());

    m_Socket2->SendMessage(m_MsgToSend);
    //igtl::Sleep(5);
  }

  NiftyLinkMessage::Pointer reqMsg;

  NiftyLinkTransformMessage::Create_GET(reqMsg);
  m_Socket2->SendMessage(reqMsg);
}

void TestSendReceive_Timing::QuitTest2()
{
  std::cout << "********************************************************\n";
  std::cout << "Printing timestamps... \n";

  igtlUint32 sec, msec, usec, nsec, fraction;
  igtlUint32 sec_c, msec_c, usec_c, nsec_c, fraction_c;
  unsigned long long ts, nanotime, nanotime_c, nanotimeFirst, nanotimeLast;
  unsigned long long totalBytesTransmitted1to2 = 0;
  unsigned long long totalBytesTransmitted2to1 = 0;
  unsigned long long totalTimeElapsed1to2 = 0;
  unsigned long long totalTimeElapsed2to1 = 0;

  // SOCKET1 RECEIVED STAMPS
  for (int i = 0; i < m_Socket1Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time_c = m_Socket1Messages.at(i)->GetTimeCreated();
    time_c->GetTime(&sec_c, &nsec_c);
    nanotime_c = (unsigned long long)(sec_c*1e9 + nsec_c);
    
    igtl::NanoTimeToHumanReadable(nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    //std::cout <<"Message " <<i <<" was created at: " <<sec_c <<"s " <<msec_c <<"ms " <<usec_c <<"usec " <<nsec_c <<"ns " <<std::endl; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    igtl::TimeStamp::Pointer time = m_Socket1Messages.at(i)->GetTimeReceived();
    time->GetTime(&sec, &nsec);
    nanotime = (unsigned long long)(sec*1e9 + nsec);
    
    igtl::NanoTimeToHumanReadable(nanotime, sec, msec, usec, nsec);
    //std::cout <<"Socket1 received message " <<i <<" at: " <<sec <<"s " <<msec <<"ms " <<usec <<"usec " <<nsec <<"ns " <<std::endl; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    unsigned long diff = nanotime - nanotime_c;
    
    m_TotalTimeDiff += diff;
    igtl::MessageBase::Pointer msgP;
    m_Socket1Messages.at(i)->GetMessagePointer(msgP);
    totalBytesTransmitted2to1 += (msgP->GetBodySizeToRead() + 58);
    
    igtl::NanoTimeToHumanReadable(nanotime - nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    std::cout <<"Socket 1 - Delivery of message " <<i <<" took: " <<sec_c <<"s " <<msec_c <<"ms " <<usec_c <<"usec " <<nsec_c <<"ns\n"; 
  }

  std::cout << "\n********************************************************\n";
  std::cout << "Reply stream... \n";

  // SOCKET2 RECEIVED STAMPS
  for (int i = 0; i < m_Socket2Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time_c = m_Socket2Messages.at(i)->GetTimeCreated();
    time_c->GetTime(&sec_c, &nsec_c);
    nanotime_c = (unsigned long long)(sec_c*1e9 + nsec_c);

    igtl::NanoTimeToHumanReadable(nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    //std::cout <<"Message " <<i <<" was created at: " <<sec_c <<"s " <<msec_c <<"ms " <<usec_c <<"usec " <<nsec_c <<"ns " <<std::endl;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    igtl::TimeStamp::Pointer time = m_Socket2Messages.at(i)->GetTimeReceived();
    time->GetTime(&sec, &nsec);
    nanotime = (unsigned long long)(sec*1e9 + nsec);

    igtl::NanoTimeToHumanReadable(nanotime, sec, msec, usec, nsec);
    //std::cout <<"Socket2 received message " <<i <<" at: " <<sec <<"s " <<msec <<"ms " <<usec <<"usec " <<nsec <<"ns " <<std::endl; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    unsigned long diff = nanotime - nanotime_c;
    
    m_TotalTimeDiff += diff;
    igtl::MessageBase::Pointer msgP;
    m_Socket1Messages.at(i)->GetMessagePointer(msgP);
    totalBytesTransmitted1to2 += (msgP->GetBodySizeToRead() + 58);

    igtl::NanoTimeToHumanReadable(nanotime - nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    std::cout <<"Socket 2 - Delivery of message " <<i <<" took: " <<sec_c <<"s " <<msec_c <<"ms " <<usec_c <<"usec " <<nsec_c <<"ns\n";
  }

  //************** Calculate bandwith 2 to 1 ************************

  igtl::TimeStamp::Pointer timeFirstReceived = m_Socket1Messages.first()->GetTimeReceived();
  timeFirstReceived->GetTime(&sec, &nsec);
  nanotimeFirst = (unsigned long long)(sec*1e9 + nsec);
  
  igtl::TimeStamp::Pointer timeLastReceived = m_Socket1Messages.last()->GetTimeReceived();
  timeLastReceived->GetTime(&sec, &nsec);
  nanotimeLast = (unsigned long long)(sec*1e9 + nsec);

  totalTimeElapsed2to1 = nanotimeLast-nanotimeFirst; 

  double bandwidth2to1 = ((double)totalBytesTransmitted2to1 / (double)1e6) / ((double)totalTimeElapsed2to1 / (double)1e9);
  
  //************** Calculate bandwith 1 to 2 ************************

  timeFirstReceived = m_Socket2Messages.first()->GetTimeReceived();
  timeFirstReceived->GetTime(&sec, &nsec);
  nanotimeFirst = (unsigned long long)(sec*1e9 + nsec);
  
  timeLastReceived = m_Socket2Messages.last()->GetTimeReceived();
  timeLastReceived->GetTime(&sec, &nsec);
  nanotimeLast = (unsigned long long)(sec*1e9 + nsec);

  totalTimeElapsed1to2 = nanotimeLast-nanotimeFirst;

  double bandwidth1to2 = ((double)totalBytesTransmitted1to2 / (double)1e6) / ((double)totalTimeElapsed1to2 / (double)1e9);

  std::cout << "\n******************************************************** \n";

  igtl::NanoTimeToHumanReadable(m_TotalTimeDiff / (m_NumOfMsg *2), sec, msec, usec, nsec);
  std::cout << "\nDelivery time on average: "  <<sec <<"s " <<msec <<"ms " <<usec <<"usec " <<nsec <<"ns\n\n"; 
  std::cout << "\nTotal time elapsed, Socket2 --> Socket1: "  <<totalTimeElapsed2to1/1e9 <<"sec\n";
  std::cout << "\nTotal data transmitted, Socket2 --> Socket1: "  <<totalBytesTransmitted2to1 <<"bytes\n";
  std::cout << "\nBandwith, Socket2 --> Socket1: "  <<bandwidth2to1 <<"Mbyte/sec\n\n";
  std::cout << "\nTotal time elapsed, Socket1 --> Socket2: "  <<totalTimeElapsed1to2/1e9 <<"sec\n";
  std::cout << "\nTotal data transmitted, Socket1 --> Socket2: "  <<totalBytesTransmitted1to2 <<"bytes\n";
  std::cout << "\nBandwith, Socket1 --> Socket2: "  <<bandwidth1to2 <<"Mbyte/sec\n\n";

  if (m_Socket1 != NULL)
  {
    m_Socket1->CloseSocket();

    while (m_Socket1->IsActive())
      igtl::Sleep(100);

    delete m_Socket1;
    m_Socket1 = NULL;
  }

  if (m_Socket2 != NULL)
  {
    m_Socket2->CloseSocket();

    while (m_Socket2->IsActive())
      igtl::Sleep(100);

    delete m_Socket2;
    m_Socket2 = NULL;
  }

  m_MsgToSend.reset();

  emit Done();

  //qDebug() <<"Really finished...";
}


void TestSendReceive_Timing::CatchMessage(NiftyLinkMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator!=(NULL))
  {
    //QLOG_INFO() <<m_Received+1 <<"of" <<m_NumOfMsg <<"message received: " <<msg->GetHostName() <<":" <<msg->GetPort() <<" " <<msg->GetMessageType();

    igtl::MessageBase::Pointer message;
    msg->GetMessagePointer(message);
    //QLOG_INFO() <<message->GetNameOfClass();

    if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
    {
      igtl::TransformMessage::Pointer poi = static_cast<igtl::TransformMessage *>(message.GetPointer());
      poi->Unpack();

      igtl::Matrix4x4 receivedMatrix;

      poi->GetMatrix(receivedMatrix);

      int r = memcmp((const void*)&receivedMatrix, (const void*)dummyTransformMatrix, sizeof(igtl::Matrix4x4));

      if (r == 0)
        std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_Received+1 <<" of " <<m_NumOfMsg*2 <<": OK" <<std::endl;
      else
        std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_Received+1 <<" of " <<m_NumOfMsg*2 <<": NOT-OK" <<std::endl;

      igtl::PrintMatrix(receivedMatrix);

      std::cout <<std::endl;

      if (r != 0)
        QLOG_ERROR() <<"Shit happens";

      m_Received++;
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::GetTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
      SendResponse();
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
      m_DoStream = true;
      StreamResponse();
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, stopping data stream" <<std::endl;
      m_DoStream = false;
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, stopping data stream" <<std::endl;
      m_DoStream = false;
    }
    else if (msg->GetMessageType() == QString("IMAGE"))
    {
      m_Received++;
      //NiftyLinkImageMessage::Pointer imgMsg;
      //imgMsg = static_cast<NiftyLinkImageMessage::Pointer>(msg);
      //QString fname("image_received_");
      //fname.append(QString::number(m_Received));
      //fname.append(".png");
      //imgMsg->Save(fname);
     }
  }
  
  if (QObject::sender()->objectName() == "Socket1")
    m_Socket1Messages.append(msg);
  else
    m_Socket2Messages.append(msg);

  //std::cerr <<"\nNum of messages received: " <<m_Received <<std::endl;

  //if (m_Received >= m_NumOfMsg)
  if (m_Received >= 2*m_NumOfMsg)
    QuitTest2();

}

void TestSendReceive_Timing::RecordSendTimestamps(unsigned long long timestamp)
{
  if (QObject::sender()->objectName() == "Socket1")
    m_Socket1Timestamps.append(timestamp);
  else
    m_Socket2Timestamps.append(timestamp);
}


void TestSendReceive_Timing::SendResponse()
{
  //QLOG_INFO() <<"Preparing to send response messages..." <<endl;
  std::cout << "Sending " <<m_NumOfMsg <<" reply messages from Socket1 --> Socket 2" <<std::endl;
  for (int i = 0; i< m_NumOfMsg; i++)
  {
    m_MsgToSend.operator =(NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
    m_MsgToSend->InitializeWithRandomData();
    m_MsgToSend->Update(GetLocalHostAddress());

    m_Socket1->SendMessage(m_MsgToSend);
   //std::cout <<"Sending response message " <<m_NumOfMsg <<std::endl;
    //igtl::Sleep(5);
  }
}

void TestSendReceive_Timing::StreamResponse()
{
  QLOG_INFO() <<"Preparing to send response stream..." <<endl;

  for (int i = 0; i< 5; i++)
    //while (m_DoStream)
  {
    m_Socket1->SendMessage(m_MsgToSend);
    //igtl::Sleep(500);

  }
}

void TestSendReceive_Timing::CreateMessage()
{

  m_MsgToSend.operator =(NiftyLinkMessage::Pointer(new NiftyLinkMessage()));
  //m_MsgToSend->setHostName(QString("MURBELLA_O"));
  //m_MsgToSend->setMessageType(QString("TRANSFORM"));

  igtl::TransformMessage::Pointer transMsg;
  transMsg = igtl::TransformMessage::New();
  transMsg->SetDeviceName("MURBELLA_O");


  CreateRandomTransformMatrix(m_LocalMatrix);

  std::cout <<"Original matrix: " <<std::endl <<std::endl;
  igtl::PrintMatrix(m_LocalMatrix);
  std::cout <<std::endl;

  transMsg->SetMatrix(m_LocalMatrix);
  transMsg->Pack();

  m_MsgToSend->SetMessagePointer((igtl::MessageBase::Pointer) transMsg);
}
