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
#include "TestSendReceive_Timing.h"

TestSendReceive_Timing::TestSendReceive_Timing(void)
{
  m_socket1 = NULL;
  m_socket2 = NULL;

  m_successCounter = 0;
  m_numOfMsg = 100;
  m_received = 0;

  m_doStream    = false;
  //m_connectedTo = false;
  //m_connecting  = false;

  m_testCounter = 0;
  m_successCounter = 0;

  m_totalTimeDiff = 0;

  connect(&m_timeOut, SIGNAL(timeout()), this, SLOT(quitTest2()) );
  //m_timeOut.start(20000);
}

TestSendReceive_Timing::~TestSendReceive_Timing(void)
{
}


void TestSendReceive_Timing::setupTest()
{
  m_socket1 = new OIGTLSocketObject();
  m_socket1->setObjectName("Socket1");
  m_socket2 = new OIGTLSocketObject();
  m_socket2->setObjectName("Socket2");

  connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
  connect(m_socket1, SIGNAL(messageSent(unsigned long long )), this, SLOT(recordSendTimestamps(unsigned long long )) );
  connect(m_socket2, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
  connect(m_socket2, SIGNAL(messageSent(unsigned long long )), this, SLOT(recordSendTimestamps(unsigned long long )) );

  #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    m_socket1->initializeWinTimers();
  #endif

  QUrl url;
  //url.setHost(QString("128.16.4.235"));
  url.setHost(QString("localhost"));
  url.setPort(3200);

  //Start sender / listener
  m_socket1->listenOnPort(3200);
  m_socket2->connectToRemote(url);

//  m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
//  static_cast<OIGTLTransformMessage::Pointer>(m_msgToSend)->setMatrix(dummyTransformMatrix);
//  m_msgToSend->update(getLocalHostAddress());

  m_msgToSend.operator =(OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  m_msgToSend->initializeWithTestData();
  m_msgToSend->update(getLocalHostAddress());
}

void TestSendReceive_Timing::performTest()
{

  for (int i = 0; i< m_numOfMsg; i++)
  {
    m_socket2->sendMessage(m_msgToSend);
    //igtl::Sleep(250);
  }

  OIGTLMessage::Pointer reqMsg;

  OIGTLTransformMessage::Create_GET(reqMsg);
  m_socket2->sendMessage(reqMsg);
}

void TestSendReceive_Timing::quitTest()
{
  QLOG_INFO() << "********************************************************";
  QLOG_INFO() << "Printing timestamps... \n";

  igtlUint32 sec, msec, usec, nsec, fraction;
  unsigned long long ts, nanotime;

  // SOCKET2 SENT STAMPS
  for (int i = 0; i < m_socket2Timestamps.count(); i++)
  {
    ts = m_socket2Timestamps.at(i);
    igtlUint32 sec      = static_cast<igtlUint32>((ts >> 32 ) & 0xFFFFFFFF);
    igtlUint32 fraction = static_cast<igtlUint32>(ts & 0xFFFFFFFF);
    unsigned long long nanotime = sec*1e9 + igtl_frac_to_nanosec(static_cast<igtlUint32>(fraction));
    
    igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    QLOG_INFO() <<"Socket2 sent message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 
  }
  
  QLOG_INFO() << " ";

  // SOCKET1 RECEIVED STAMPS
  for (int i = 0; i < m_socket1Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time = m_socket1Messages.at(i)->getTimeReceived();
    time->GetTimeStamp(&sec, &nsec);
    nanotime = sec*1e9 + nsec;
    
    igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    QLOG_INFO() <<"Socket1 received message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 
  }

  QLOG_INFO() << "\n********************************************************";
  QLOG_INFO() << "Reply stream... \n";

  // SOCKET1 SENT STAMPS
  for (int i = 0; i < m_socket1Timestamps.count(); i++)
  {
    ts = m_socket1Timestamps.at(i);
    sec      = static_cast<igtlUint32>((ts >> 32 ) & 0xFFFFFFFF);
    fraction = static_cast<igtlUint32>(ts & 0xFFFFFFFF);
    nanotime = sec*1e9 + igtl_frac_to_nanosec(static_cast<igtlUint32>(fraction));

    igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    QLOG_INFO() <<"Socket1 sent message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 
  }
  
  QLOG_INFO() << " ";

  // SOCKET2 RECEIVED STAMPS
  for (int i = 0; i < m_socket2Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time = m_socket2Messages.at(i)->getTimeReceived();
    time->GetTimeStamp(&sec, &nsec);
    nanotime = sec*1e9 + nsec;

    igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    QLOG_INFO() <<"Socket2 received message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 
  }

  QLOG_INFO() << "\n******************************************************** \n";

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

  //qDebug() <<"Really finished...";
}

void TestSendReceive_Timing::quitTest2()
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
  for (int i = 0; i < m_socket1Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time = m_socket1Messages.at(i)->getTimeReceived();
    time->GetTimeStamp(&sec, &nsec);
    nanotime = sec*1e9 + nsec;
    
    //igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    //QLOG_INFO() <<"Socket1 received message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    igtl::TimeStamp::Pointer time_c = m_socket1Messages.at(i)->getTimeCreated();
    time_c->GetTimeStamp(&sec_c, &nsec_c);
    nanotime_c = sec_c*1e9 + nsec_c;
    
    //igtl::ConvertToSec(nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    //QLOG_INFO() <<"Message " <<i <<"was created at:" <<sec_c <<"s" <<msec_c <<"ms" <<usec_c <<"usec" <<nsec_c <<"ns"; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    unsigned long diff = nanotime - nanotime_c;
    
    m_totalTimeDiff += diff;
    igtl::MessageBase::Pointer msgP;
    m_socket1Messages.at(i)->getMessagePointer(msgP);
    totalBytesTransmitted2to1 += (msgP->GetBodySizeToRead() + 58);
    
    igtl::ConvertToSec(nanotime - nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    std::cout <<"Socket 1 - Delivery of message " <<i <<" took: " <<sec_c <<"s" <<msec_c <<"ms" <<usec_c <<"usec" <<nsec_c <<"ns\n"; 
  }

  std::cout << "\n********************************************************\n";
  std::cout << "Reply stream... \n";

  // SOCKET2 RECEIVED STAMPS
  for (int i = 0; i < m_socket2Messages.count(); i++)
  {
    igtl::TimeStamp::Pointer time = m_socket2Messages.at(i)->getTimeReceived();
    time->GetTimeStamp(&sec, &nsec);
    nanotime = sec*1e9 + nsec;

    //igtl::ConvertToSec(nanotime, sec, msec, usec, nsec);
    //QLOG_INFO() <<"Socket2 received message" <<i <<"at:" <<sec <<"s" <<msec <<"ms" <<usec <<"usec" <<nsec <<"ns"; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    igtl::TimeStamp::Pointer time_c = m_socket2Messages.at(i)->getTimeCreated();
    time_c->GetTimeStamp(&sec_c, &nsec_c);
    nanotime_c = sec_c*1e9 + nsec_c;

    //igtl::ConvertToSec(nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    //QLOG_INFO() <<"Message " <<i <<"was created at:" <<sec_c <<"s" <<msec_c <<"ms" <<usec_c <<"usec" <<nsec_c <<"ns"; 

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    unsigned long diff = nanotime - nanotime_c;
    
    m_totalTimeDiff += diff;
    igtl::MessageBase::Pointer msgP;
    m_socket1Messages.at(i)->getMessagePointer(msgP);
    totalBytesTransmitted1to2 += (msgP->GetBodySizeToRead() + 58);

    igtl::ConvertToSec(nanotime - nanotime_c, sec_c, msec_c, usec_c, nsec_c);
    std::cout <<"Socket 2 - Delivery of message " <<i <<" took: " <<sec_c <<"s" <<msec_c <<"ms" <<usec_c <<"usec" <<nsec_c <<"ns\n";
  }

  //************** Calculate bandwith 2 to 1 ************************

  igtl::TimeStamp::Pointer timeFirstReceived = m_socket1Messages.first()->getTimeReceived();
  timeFirstReceived->GetTimeStamp(&sec, &nsec);
  nanotimeFirst = sec*1e9 + nsec;
  
  igtl::TimeStamp::Pointer timeLastReceived = m_socket1Messages.last()->getTimeReceived();
  timeLastReceived->GetTimeStamp(&sec, &nsec);
  nanotimeLast = sec*1e9 + nsec;

  totalTimeElapsed2to1 = nanotimeLast-nanotimeFirst; 

  double bandwidth2to1 = ((double)totalBytesTransmitted2to1 / (double)1e6) / ((double)totalTimeElapsed2to1 / (double)1e9);
  
  //************** Calculate bandwith 1 to 2 ************************

  timeFirstReceived = m_socket2Messages.first()->getTimeReceived();
  timeFirstReceived->GetTimeStamp(&sec, &nsec);
  nanotimeFirst = sec*1e9 + nsec;
  
  timeLastReceived = m_socket2Messages.last()->getTimeReceived();
  timeLastReceived->GetTimeStamp(&sec, &nsec);
  nanotimeLast = sec*1e9 + nsec;

  totalTimeElapsed1to2 = nanotimeLast-nanotimeFirst;

  double bandwidth1to2 = ((double)totalBytesTransmitted1to2 / (double)1e6) / ((double)totalTimeElapsed1to2 / (double)1e9);

  std::cout << "\n******************************************************** \n";

  igtl::ConvertToSec(m_totalTimeDiff / (m_numOfMsg *2), sec, msec, usec, nsec);
  std::cout << "\nDelivery time on average: "  <<sec <<"s " <<msec <<"ms " <<usec <<"usec " <<nsec <<"ns\n\n"; 
  std::cout << "\nTotal time elapsed, Socket2 --> Socket1: "  <<totalTimeElapsed2to1/1e9 <<"sec\n";
  std::cout << "\nTotal data transmitted, Socket2 --> Socket1: "  <<totalBytesTransmitted2to1 <<"bytes\n";
  std::cout << "\nBandwith, Socket2 --> Socket1: "  <<bandwidth2to1 <<"Mbyte/sec\n\n";
  std::cout << "\nTotal time elapsed, Socket1 --> Socket2: "  <<totalTimeElapsed1to2/1e9 <<"sec\n";
  std::cout << "\nTotal data transmitted, Socket1 --> Socket2: "  <<totalBytesTransmitted1to2 <<"bytes\n";
  std::cout << "\nBandwith, Socket1 --> Socket2: "  <<bandwidth1to2 <<"Mbyte/sec\n\n";

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

  //qDebug() <<"Really finished...";
}


void TestSendReceive_Timing::catchMessage(OIGTLMessage::Pointer msg)
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
    else if (strcmp(message->GetNameOfClass(), "igtl::GetTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
      sendResponse();
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
      m_doStream = true;
      streamResponse();
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, stopping data stream" <<std::endl;
      m_doStream = false;
    }
    else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
    {
      std::cout <<sender.toStdString().c_str() <<" received message request, stopping data stream" <<std::endl;
      m_doStream = false;
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

  //std::cerr <<"\nNum of messages received: " <<m_received <<std::endl;

  //if (m_received >= m_numOfMsg)
  if (m_received >= 2*m_numOfMsg)
    quitTest2();

}

void TestSendReceive_Timing::recordSendTimestamps(unsigned long long timestamp)
{
  if (QObject::sender()->objectName() == "Socket1")
    m_socket1Timestamps.append(timestamp);
  else
    m_socket2Timestamps.append(timestamp);
}


void TestSendReceive_Timing::sendResponse()
{
  QLOG_INFO() <<"Preparing to send response messages..." <<endl;

  for (int i = 0; i< m_numOfMsg; i++)
  {
    m_socket1->sendMessage(m_msgToSend);
    //igtl::Sleep(250);
  }
}

void TestSendReceive_Timing::streamResponse()
{
  QLOG_INFO() <<"Preparing to send response stream..." <<endl;

  for (int i = 0; i< 5; i++)
    //while (m_doStream)
  {
    m_socket1->sendMessage(m_msgToSend);
    //igtl::Sleep(500);

  }
}

void TestSendReceive_Timing::createMessage()
{

  m_msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));
  //m_msgToSend->setHostName(QString("MURBELLA_O"));
  //m_msgToSend->setMessageType(QString("TRANSFORM"));

  igtl::TransformMessage::Pointer transMsg;
  transMsg = igtl::TransformMessage::New();
  transMsg->SetDeviceName("MURBELLA_O");


  CreateRandomTransformMatrix(m_localMatrix);

  std::cout <<"Original matrix: " <<std::endl <<std::endl;
  igtl::PrintMatrix(m_localMatrix);
  std::cout <<std::endl;

  transMsg->SetMatrix(m_localMatrix);
  transMsg->Pack();

  m_msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);
}
