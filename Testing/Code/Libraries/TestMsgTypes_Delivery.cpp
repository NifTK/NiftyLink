/*=============================================================================

NiftyLink:  A software library to facilitate communication over OpenIGTLink.

http://cmic.cs.ucl.ac.uk/
http://www.ucl.ac.uk/

Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
Revision          : $Revision: 3300 $
Last modified by  : $Author: mjc $

Original author   : m.clarkson@ucl.ac.uk

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

============================================================================*/

#include "stdlib.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include "TestMsgTypes_Delivery.h"

TestMsgTypes_Delivery::TestMsgTypes_Delivery(void)
{
  m_socket1 = NULL;
  m_socket2 = NULL;

  m_successCounter = 0;
  m_numOfMsg = 10;
  m_received = 0;
  m_sent     = 0;

  m_doStream    = false;
  m_connectedTo = false;
  m_connecting  = false;

  m_inShutdownTests = false;

  m_testCounter = 0;
  m_successCounter = 0;

  connect(&m_timeOut, SIGNAL(timeout()), this, SLOT(quitTest()) );
  m_timeOut.start(60000);

}

TestMsgTypes_Delivery::~TestMsgTypes_Delivery(void)
{
}


void TestMsgTypes_Delivery::startTest()
{

  //***********************************************
  //Instanciate socket objects
  std::cout <<++m_testCounter <<". Creating two socket object..";
  m_socket1 = new OIGTLSocketObject();
  m_socket1->setObjectName("Socket1");
  m_socket2 = new OIGTLSocketObject();
  m_socket2->setObjectName("Socket2");

  if (m_socket1 != NULL && m_socket2 != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate socket objects
  std::cout <<++m_testCounter <<". Connecting Signals and Slots..";
  bool ok = true;
  ok &= connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
  //ok &= connect(m_socket1, SIGNAL(messageSent(unsigned long long )), this, SLOT(recordSendTimestamps(unsigned long long )) );
  ok &= connect(m_socket1, SIGNAL(clientConnectedSignal()), this, SLOT(clientConnected()) ); 

  ok &= connect(m_socket2, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
  //ok &= connect(m_socket2, SIGNAL(messageSent(unsigned long long )), this, SLOT(recordSendTimestamps(unsigned long long )) );
  ok &= connect(m_socket2, SIGNAL(connectedToRemoteSignal()), this, SLOT(connectedToRemote()) ); 
  if (ok)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //This is required on Win to achieve accurate timings
  #if defined(_WIN32)
    m_socket1->initializeWinTimers(); 
  #endif

  //***********************************************
  //Starting up listener thread 

  std::cout <<++m_testCounter <<". Starting up the listener socket..";

  //Start sender / listener
  ok = m_socket1->listenOnPort(3200);

  if (ok && m_socket1->isListening())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  igtl::Sleep(500);

  //***********************************************
  //Starting up sender thread 

  std::cout <<++m_testCounter <<". Starting up the sender socket..";

  QUrl url;
  url.setHost(QString("localhost"));
  url.setPort(3200);

  ok = m_socket2->connectToRemote(url);
}

void TestMsgTypes_Delivery::continueTest()
{
  if (m_socket2->isConnected())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if the two-way connection is alive

  std::cout <<++m_testCounter <<". Testing the two-way channel..";

  bool a = m_socket1->isClientConnecting();
  bool b = m_socket2->isConnected();
  bool c = m_socket2->isAbleToSend();

  if(a && b && c)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test message delivery

  std::cout <<++m_testCounter <<". Testing message delivery..";

  m_msgToSend.operator =(OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  static_cast<OIGTLImageMessage::Pointer>(m_msgToSend)->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());

  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLImageMessage::Create_GET(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLImageMessage::Create_STT(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLImageMessage::Create_STP(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLImageMessage::Create_RTS(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_msgToSend.operator =(OIGTLStatusMessage::Pointer(new OIGTLStatusMessage()));
  static_cast<OIGTLStatusMessage::Pointer>(m_msgToSend)->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());

  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStatusMessage::Create_GET(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStatusMessage::Create_STT(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStatusMessage::Create_STP(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStatusMessage::Create_RTS(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_msgToSend.operator =(OIGTLStringMessage::Pointer(new OIGTLStringMessage()));
  static_cast<OIGTLStringMessage::Pointer>(m_msgToSend)->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());

  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStringMessage::Create_GET(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStringMessage::Create_STT(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStringMessage::Create_STP(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLStringMessage::Create_RTS(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_msgToSend.operator =(OIGTLTrackingDataMessage::Pointer(new OIGTLTrackingDataMessage()));
  static_cast<OIGTLTrackingDataMessage::Pointer>(m_msgToSend)->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());

  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTrackingDataMessage::Create_GET(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTrackingDataMessage::Create_STT(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTrackingDataMessage::Create_STP(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTrackingDataMessage::Create_RTS(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
  static_cast<OIGTLTransformMessage::Pointer>(m_msgToSend)->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());

  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTransformMessage::Create_GET(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTransformMessage::Create_STT(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTransformMessage::Create_STP(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  OIGTLTransformMessage::Create_RTS(m_msgToSend);
  m_msgToSend->update(getLocalHostAddress());
  m_socket2->sendMessage(m_msgToSend);
  m_sent++;
  m_msgToSend.reset();

  std::cout <<"\nTotal number of messages sent: " <<m_sent;
}

void TestMsgTypes_Delivery::clientConnected()
{
  m_connecting = true;

  if (m_inShutdownTests)
    qDebug() <<"Successfully continued the tests after shutdown";

  if (m_connecting && m_connectedTo)
    continueTest();
}
void TestMsgTypes_Delivery::connectedToRemote()
{
  m_connectedTo = true;

  if (m_inShutdownTests)
    qDebug() <<"Successfully continued the tests after shutdown";
  
  if (m_connecting && m_connectedTo)
    continueTest();
}

void TestMsgTypes_Delivery::quitTest()
{
  if(m_sent == m_received)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";


  if (m_socket1 != NULL)
  {
    m_socket1->closeSocket();

    while (m_socket1->isActive())
      igtl::Sleep(100);

    delete m_socket1;
    m_socket1 = NULL;
  }

  if (m_socket2 != NULL)
  {
    m_socket2->closeSocket();

    while (m_socket2->isActive())
      igtl::Sleep(100);

    delete m_socket2;
    m_socket2 = NULL;
  }

  m_msgToSend.reset();

  emit done();
    
  if (m_testCounter > m_successCounter)
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"***************** " <<(m_testCounter - m_successCounter) << " TEST(S) FAILED *****************\n";
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

void TestMsgTypes_Delivery::catchMessage(OIGTLMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator!=(NULL))
  {
    //QLOG_INFO() <<m_received+1 <<"of" <<m_numOfMsg <<"message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

    igtl::MessageBase::Pointer message;
    msg->getMessagePointer(message);

    //QLOG_INFO() <<message->GetNameOfClass();

    m_received++;

    if (m_received >= 25)
      quitTest();
    //if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
    //  m_received++;
    //else if (msg->getMessageType() == QString("IMAGE"))
    //  m_received++;

  }
}