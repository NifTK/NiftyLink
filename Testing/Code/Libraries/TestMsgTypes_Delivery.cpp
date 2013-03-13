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
#include "TestMsgTypes_Delivery.h"
#include "NLTest_Data.h"

TestMsgTypes_Delivery::TestMsgTypes_Delivery(void)
{
  m_Socket1 = NULL;
  m_Socket2 = NULL;

  m_SuccessCounter = 0;
  m_NumOfMsg = 10;
  m_Received = 0;
  m_Sent     = 0;

  m_DoStream    = false;
  m_ConnectedTo = false;
  m_Connecting  = false;

  m_InShutdownTests = false;

  m_TestCounter = 0;
  m_SuccessCounter = 0;

  connect(&m_TimeOut, SIGNAL(timeout()), this, SLOT(QuitTest()) );
  m_TimeOut.start(60000);

}

TestMsgTypes_Delivery::~TestMsgTypes_Delivery(void)
{
}


void TestMsgTypes_Delivery::StartTest()
{

  //***********************************************
  //Instanciate socket objects
  std::cout <<++m_TestCounter <<". Creating two socket object..";
  m_Socket1 = new NiftyLinkSocketObject();
  m_Socket1->setObjectName("Socket1");
  m_Socket2 = new NiftyLinkSocketObject();
  m_Socket2->setObjectName("Socket2");

  if (m_Socket1 != NULL && m_Socket2 != NULL)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate socket objects
  std::cout <<++m_TestCounter <<". Connecting Signals and Slots..";
  bool ok = true;
  ok &= connect(m_Socket1, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  //ok &= connect(m_Socket1, SIGNAL(messageSent(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );
  ok &= connect(m_Socket1, SIGNAL(ClientConnectedSignal()), this, SLOT(ClientConnected()) ); 

  ok &= connect(m_Socket2, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  //ok &= connect(m_Socket2, SIGNAL(messageSent(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );
  ok &= connect(m_Socket2, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(ConnectedToRemote()) ); 
  if (ok)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //This is required on Win to achieve accurate timings
  #if defined(_WIN32)
    m_Socket1->InitializeWinTimers(); 
  #endif

  //***********************************************
  //Starting up listener thread 

  std::cout <<++m_TestCounter <<". Starting up the listener socket..";

  //Start sender / listener
  ok = m_Socket1->ListenOnPort(3200);

  if (ok && m_Socket1->IsListening())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  igtl::Sleep(500);

  //***********************************************
  //Starting up sender thread 

  std::cout <<++m_TestCounter <<". Starting up the sender socket..";

  QUrl url;
  url.setHost(QString("localhost"));
  url.setPort(3200);

  ok = m_Socket2->ConnectToRemote(url);
}

void TestMsgTypes_Delivery::ContinueTest()
{
  if (m_Socket2->IsConnected())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if the two-way connection is alive

  std::cout <<++m_TestCounter <<". Testing the two-way channel..";

  bool a = m_Socket1->IsClientConnecting();
  bool b = m_Socket2->IsConnected();
  bool c = m_Socket2->IsAbleToSend();

  if(a && b && c)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test message delivery

  std::cout <<++m_TestCounter <<". Testing message delivery..";

  m_MsgToSend.operator =(NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
  static_cast<NiftyLinkImageMessage::Pointer>(m_MsgToSend)->InitializeWithTestData();
  m_MsgToSend->Update(GetLocalHostAddress());

  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkImageMessage::Create_GET(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkImageMessage::Create_STT(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkImageMessage::Create_STP(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkImageMessage::Create_RTS(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_MsgToSend.operator =(NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage()));
  static_cast<NiftyLinkStatusMessage::Pointer>(m_MsgToSend)->InitializeWithTestData();
  m_MsgToSend->Update(GetLocalHostAddress());

  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStatusMessage::Create_GET(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStatusMessage::Create_STT(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStatusMessage::Create_STP(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStatusMessage::Create_RTS(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_MsgToSend.operator =(NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage()));
  static_cast<NiftyLinkStringMessage::Pointer>(m_MsgToSend)->InitializeWithTestData();
  m_MsgToSend->Update(GetLocalHostAddress());

  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStringMessage::Create_GET(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStringMessage::Create_STT(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStringMessage::Create_STP(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkStringMessage::Create_RTS(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_MsgToSend.operator =(NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage()));
  static_cast<NiftyLinkTrackingDataMessage::Pointer>(m_MsgToSend)->InitializeWithTestData(dummyTransformMatrix);
  m_MsgToSend->Update(GetLocalHostAddress());

  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTrackingDataMessage::Create_GET(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTrackingDataMessage::Create_STT(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTrackingDataMessage::Create_STP(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTrackingDataMessage::Create_RTS(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_MsgToSend.operator =(NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
  static_cast<NiftyLinkTransformMessage::Pointer>(m_MsgToSend)->InitializeWithTestData(dummyTransformMatrix);
  m_MsgToSend->Update(GetLocalHostAddress());

  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTransformMessage::Create_GET(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTransformMessage::Create_STT(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTransformMessage::Create_STP(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  NiftyLinkTransformMessage::Create_RTS(m_MsgToSend);
  m_MsgToSend->Update(GetLocalHostAddress());
  m_Socket2->SendMessage(m_MsgToSend);
  m_Sent++;
  m_MsgToSend.reset();

  std::cout <<"\nTotal number of messages sent: " <<m_Sent;
}

void TestMsgTypes_Delivery::ClientConnected()
{
  m_Connecting = true;

  if (m_Connecting && m_ConnectedTo)
    ContinueTest();
}
void TestMsgTypes_Delivery::ConnectedToRemote()
{
  m_ConnectedTo = true;

  if (m_Connecting && m_ConnectedTo)
    ContinueTest();
}

void TestMsgTypes_Delivery::QuitTest()
{
  if(m_Sent == m_Received)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";


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
    
  if (m_TestCounter > m_SuccessCounter)
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"***************** " <<(m_TestCounter - m_SuccessCounter) << " TEST(S) FAILED *****************\n";
    std::cout <<"****************************************************\n";
    exit(-1);
  }
  else
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"********* ALL TESTS COMPLETED SUCCESSFULLY *********\n";
    std::cout <<"****************************************************\n";
    exit(0);
  }
}

void TestMsgTypes_Delivery::CatchMessage(NiftyLinkMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator!=(NULL))
  {
    //QLOG_INFO() <<m_Received+1 <<"of" <<m_NumOfMsg <<"message received: " <<msg->GetHostName() <<":" <<msg->GetPort() <<" " <<msg->GetMessageType();

    igtl::MessageBase::Pointer message;
    msg->GetMessagePointer(message);

    if (strcmp(message->GetNameOfClass(), "igtl::ImageMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::GetImageMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StartImageMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StopImageMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::RTSImageMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StatusMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::GetStatusMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StartStatusMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StopStatusMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::RTSStatusMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StringMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::GetStringMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StartStringMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StopStringMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::RTSStringMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::TrackingDataMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::GetTrackingDataMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StartTrackingDataMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTrackingDataMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::RTSTrackingDataMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::GetTransformMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
      m_Received++;
    else if (strcmp(message->GetNameOfClass(), "igtl::RTSTransformMessage") == 0)
      m_Received++;

    if (m_Received >= 25)
      QuitTest();

  }
}
