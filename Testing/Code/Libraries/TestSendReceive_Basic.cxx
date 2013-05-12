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
#include "TestSendReceive_Basic.h"
#include "NLTest_Data.h"

TestSendReceive_Basic::TestSendReceive_Basic(void)
{
  m_Socket1 = NULL;
  m_Socket2 = NULL;

  m_SuccessCounter = 0;
  m_NumOfMsg = 10;
  m_Received = 0;

  m_DoStream    = false;
  m_ConnectedTo = false;
  m_Connecting  = false;

  m_InShutdownTests = false;

  m_TestCounter = 0;
  m_SuccessCounter = 0;

  connect(&m_TimeOut, SIGNAL(timeout()), this, SLOT(QuitTest()) );
  m_TimeOut.start(60000);

}

TestSendReceive_Basic::~TestSendReceive_Basic(void)
{
}


void TestSendReceive_Basic::StartTest()
{

  //***********************************************
  //Instanciate socket objects
  std::cout << ++m_TestCounter << ". Creating two socket object..";
  m_Socket1 = new NiftyLinkSocketObject();
  m_Socket2 = new NiftyLinkSocketObject();

  m_Socket2->SetObjectNames("Socket2");
  m_Socket1->SetObjectNames("Socket1");

  if (m_Socket1 != NULL && m_Socket2 != NULL)
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  //***********************************************
  //Instanciate socket objects
  std::cout << ++m_TestCounter << ". Connecting Signals and Slots..";
  bool ok = true;
  ok &= connect(m_Socket1, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  //ok &= connect(m_Socket1, SIGNAL(messageSent(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );
  ok &= connect(m_Socket1, SIGNAL(ClientConnectedSignal()), this, SLOT(ClientConnected()) );

  ok &= connect(m_Socket2, SIGNAL(MessageReceivedSignal(NiftyLinkMessage::Pointer)), this, SLOT(CatchMessage(NiftyLinkMessage::Pointer )) );
  //ok &= connect(m_Socket2, SIGNAL(messageSent(unsigned long long )), this, SLOT(RecordSendTimestamps(unsigned long long )) );
  ok &= connect(m_Socket2, SIGNAL(ConnectedToRemoteSignal()), this, SLOT(ConnectedToRemote()) );
  if (ok)
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  //This is required on Win to achieve accurate timings
#if defined(_WIN32)
  m_Socket1->InitializeWinTimers();
#endif

  //***********************************************
  //Starting up listener thread

  std::cout << ++m_TestCounter << ". Starting up the listener socket..";

  //Start sender / listener
  ok = m_Socket1->ListenOnPort(3200);

  if (ok && m_Socket1->IsListening())
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  igtl::Sleep(500);

  //***********************************************
  //Starting up sender thread - false attempt

  std::cout << ++m_TestCounter << ". Starting up the sender socket - false url attempt..";

  QUrl url;
  url.setHost(QString("non-existing-host"));
  url.setPort(3200);

  m_Socket2->ConnectToRemote(url);

  igtl::Sleep(4000);

  ok = m_Socket2->IsConnected();

  if (!ok)
  {
    std::cout << " NOT POSSIBLE: OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  //***********************************************
  //Starting up sender thread

  std::cout << ++m_TestCounter << ". Starting up the sender socket..";

  url.setHost(QString("localhost"));
  url.setPort(3200);

  ok = m_Socket2->ConnectToRemote(url);
}

void TestSendReceive_Basic::ContinueTest()
{
  if (m_Socket2->IsConnected())
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  //***********************************************
  //Test if the two-way connection is alive

  std::cout << ++m_TestCounter << ". Testing the two-way channel..";

  bool a = m_Socket1->IsClientConnecting();
  bool b = m_Socket2->IsConnected();
  bool c = m_Socket2->IsAbleToSend();

  if (a && b && c)
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }
  else
  {
    std::cout << " FAILED\n";
  }

  if (m_InShutdownTests)
  {
    TestCloseSocket2();
  }


  m_MsgToSend.operator = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
  static_cast<NiftyLinkTransformMessage::Pointer>(m_MsgToSend)->SetMatrix(dummyTransformMatrix);
  m_MsgToSend->Update(GetLocalHostAddress());

  //m_MsgToSend.operator =(NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
  //m_MsgToSend->InitializeWithTestData();
  //m_MsgToSend->Update(GetLocalHostAddress());

  SendMessages();

  //qDebug() <<"end of part 2";
}

void TestSendReceive_Basic::SendMessages()
{
  std::cout << ++m_TestCounter << ". Sending 10 messages from one socket to the other ..";
  for (int i = 0; i < m_NumOfMsg; i++)
  {
    m_Socket2->SendMessage(m_MsgToSend);
  }
}

void TestSendReceive_Basic::ClientConnected()
{
  m_Connecting = true;

  //if (m_InShutdownTests)
  //  qDebug() <<"Successfully continued the tests after shutdown";

  if (m_Connecting && m_ConnectedTo)
  {
    ContinueTest();
  }
}
void TestSendReceive_Basic::ConnectedToRemote()
{
  m_ConnectedTo = true;

  //if (m_InShutdownTests)
  //  qDebug() <<"Successfully continued the tests after shutdown";

  if (m_Connecting && m_ConnectedTo)
  {
    ContinueTest();
  }
}

void TestSendReceive_Basic::QuitTest()
{
  if (m_Socket1 != NULL)
  {
    m_Socket1->CloseSocket();

    while (m_Socket1->IsActive())
    {
      igtl::Sleep(100);
    }

    delete m_Socket1;
    m_Socket1 = NULL;
  }

  if (m_Socket2 != NULL)
  {
    m_Socket2->CloseSocket();

    while (m_Socket2->IsActive())
    {
      igtl::Sleep(100);
    }

    delete m_Socket2;
    m_Socket2 = NULL;
  }

  m_MsgToSend.reset();

  emit Done();

  if (m_TestCounter > m_SuccessCounter)
  {
    std::cout << "\n\n\n";
    std::cout << "****************************************************\n";
    std::cout << "**************** TESTING FINISHED: *****************\n";
    std::cout << "***************** " << (m_TestCounter - m_SuccessCounter) << " TEST(S) FAILED *****************\n";
    std::cout << "****************************************************\n";
    exit(-1);
  }
  else
  {
    std::cout << "\n\n\n";
    std::cout << "****************************************************\n";
    std::cout << "**************** TESTING FINISHED: *****************\n";
    std::cout << "********* ALL TESTS COMPLETED SUCCESSFULLY *********\n";
    std::cout << "****************************************************\n";
    exit(0);
  }
}

void TestSendReceive_Basic::CatchMessage(NiftyLinkMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator != (NULL))
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
      {
        std::cout << sender.toStdString().c_str() << " received matrix " << m_Received + 1 << " of " << m_NumOfMsg * 2 << ": OK" << std::endl;
      }
      else
      {
        std::cout << sender.toStdString().c_str() << " received matrix " << m_Received + 1 << " of " << m_NumOfMsg * 2 << ": NOT-OK" << std::endl;
      }

      igtl::PrintMatrix(receivedMatrix);

      std::cout << std::endl;

      if (r != 0)
      {
        QLOG_ERROR() << "Shit happens";
      }

      m_Received++;
    }
    else if (msg->GetMessageType() == QString("IMAGE"))
    {
      m_Received++;
      NiftyLinkImageMessage::Pointer imgMsg;
      imgMsg = static_cast<NiftyLinkImageMessage::Pointer>(msg);
      QString fname("image_received_");
      fname.append(QString::number(m_Received));
      fname.append(".png");
      imgMsg->Save(fname);
    }
  }

  if (QObject::sender()->objectName() == "Socket1")
  {
    m_Socket1Messages.append(msg);
  }
  else
  {
    m_Socket2Messages.append(msg);
  }

  std::cerr << "\nNum of messages received: " << m_Received << std::endl;

  //if (m_Received >= m_NumOfMsg)
  if (m_Received >= m_NumOfMsg)
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
    TestCloseSocket1();
  }
}


void TestSendReceive_Basic::TestCloseSocket1()
{
  //***********************************************
  //Test what happens if one socket shuts down

  std::cout << ++m_TestCounter << ". Testing client shutdown..";

  m_Socket2->CloseSocket();

  igtl::Sleep(200);

  bool a = m_Socket1->IsClientConnecting();
  bool b = m_Socket2->IsConnected();
  bool c = m_Socket2->IsAbleToSend();

  unsigned int count  = 0;

  while ((a || b || c) && count < 4000)
  {
    QCoreApplication::processEvents();

    a = m_Socket1->IsClientConnecting();
    b = m_Socket2->IsConnected();
    c = m_Socket2->IsAbleToSend();

    igtl::Sleep(200);
    count += 200;
  }

  if (a || b || c)
  {
    std::cout << " FAILED\n";
  }
  else
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }

  m_InShutdownTests = true;
  m_ConnectedTo = false;
  m_Connecting = false;

  //***********************************************
  //Starting up sender thread

  std::cout << ++m_TestCounter << ". Starting up the sender socket..";

  QUrl url;
  url.setHost(QString("localhost"));
  url.setPort(3200);

  m_Socket2->ConnectToRemote(url);
  //igtl::Sleep(1000);

}

void TestSendReceive_Basic::TestCloseSocket2()
{
  std::cout << ++m_TestCounter << ". Testing server shutdown..";

  m_Socket1->CloseSocket();

  //igtl::Sleep(4000);

  bool a = m_Socket1->IsListening();
  bool b = m_Socket2->IsConnected();
  bool c = m_Socket2->IsAbleToSend();

  unsigned int count  = 0;

  while ((a || b || c) && count < 4000)
  {
    QCoreApplication::processEvents();

    a = m_Socket1->IsClientConnecting();
    b = m_Socket2->IsConnected();
    c = m_Socket2->IsAbleToSend();

    igtl::Sleep(200);
    count += 200;
  }

  if (a || b || c)
  {
    std::cout << " FAILED\n";
  }
  else
  {
    std::cout << " OK\n";
    m_SuccessCounter++;
  }

  QuitTest();
}
