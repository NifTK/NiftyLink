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
#include "TestSendReceive_Basic.h"

TestSendReceive_Basic::TestSendReceive_Basic(void)
{
  m_socket1 = NULL;
  m_socket2 = NULL;

  m_successCounter = 0;
  m_numOfMsg = 10;
  m_received = 0;

  m_doStream    = false;
  m_connectedTo = false;
  m_connecting  = false;

  m_testCounter = 0;
  m_successCounter = 0;

  connect(&m_timeOut, SIGNAL(timeout()), this, SLOT(quitTest()) );
  m_timeOut.start(4000);

}

TestSendReceive_Basic::~TestSendReceive_Basic(void)
{
}


void TestSendReceive_Basic::setupTest()
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
  //Starting up listener thread 

  std::cout <<++m_testCounter <<". Starting up the sender socket..";

  QUrl url;
  url.setHost(QString("localhost"));
  url.setPort(3200);

  ok = m_socket2->connectToRemote(url);
}

void TestSendReceive_Basic::continueTest()
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


  //m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
  //static_cast<OIGTLTransformMessage::Pointer>(m_msgToSend)->setMatrix(dummyTransformMatrix);
  //m_msgToSend->update(getLocalHostAddress());

  m_msgToSend.operator =(OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  m_msgToSend->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());
}

void TestSendReceive_Basic::clientConnected()
{
  m_connecting = true;

  if (m_connecting && m_connectedTo)
    continueTest();
}
void TestSendReceive_Basic::connectedToRemote()
{
  m_connectedTo = true;
  
  if (m_connecting && m_connectedTo)
    continueTest();
}

void TestSendReceive_Basic::performTest()
{
  std::cout <<++m_testCounter <<". Sending 10 messages from one socket to the other ..";
  for (int i = 0; i< m_numOfMsg; i++)
  {
    m_socket2->sendMessage(m_msgToSend);
  }
}

void TestSendReceive_Basic::quitTest()
{
  if (m_socket1 != NULL)
  {
    delete m_socket1;
    m_socket1 = NULL;
  }

  if (m_socket2 != NULL)
  {
    delete m_socket2;
    m_socket2 = NULL;
  }

  m_msgToSend.reset();
  
  emit done();
  
  if (m_testCounter > m_successCounter)
    exit(-1);
  else exit(0);
}

void TestSendReceive_Basic::catchMessage(OIGTLMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator!=(NULL))
  {
    //QLOG_INFO() <<m_received+1 <<"of" <<m_numOfMsg <<"message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

    igtl::MessageBase::Pointer message;
    msg->getMessagePointer(message);
    //QLOG_INFO() <<message->GetNameOfClass();

    if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
    {
      igtl::TransformMessage::Pointer poi = static_cast<igtl::TransformMessage *>(message.GetPointer());
      poi->Unpack();

      igtl::Matrix4x4 receivedMatrix;

      poi->GetMatrix(receivedMatrix);

      int r = memcmp((const void*)&receivedMatrix, (const void*)dummyTransformMatrix, sizeof(igtl::Matrix4x4));

      if (r == 0)
        std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received+1 <<" of " <<m_numOfMsg*2 <<": OK" <<std::endl;
      else
        std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received+1 <<" of " <<m_numOfMsg*2 <<": NOT-OK" <<std::endl;

      igtl::PrintMatrix(receivedMatrix);

      std::cout <<std::endl;

      if (r != 0)
        QLOG_ERROR() <<"Shit happens";

      m_received++;
    }
    else if (msg->getMessageType() == QString("IMAGE"))
    {
      m_received++;
      OIGTLImageMessage::Pointer imgMsg;
      imgMsg = static_cast<OIGTLImageMessage::Pointer>(msg);
      QString fname("image_received_");
      fname.append(QString::number(m_received));
      fname.append(".png");
      imgMsg->save(fname);
     }
  }
  
  if (QObject::sender()->objectName() == "Socket1")
    m_socket1Messages.append(msg);
  else
    m_socket2Messages.append(msg);

  std::cerr <<"\nNum of messages received: " <<m_received <<std::endl;

  //if (m_received >= m_numOfMsg)
  if (m_received >= m_numOfMsg)
    { std::cout <<" OK\n"; m_successCounter++; quitTest(); }
}
