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
#include "TestPTPSendReceive_Basic.h"

#define PTP_GROUP "225.0.0.37"

TestPTPSendReceive_Basic::TestPTPSendReceive_Basic(void)
{
  m_successCounter = 0;
  m_numOfMsg = 10;
  m_received = 0;

  m_doStream    = false;
  m_connectedTo = false;
  m_connecting  = false;

  m_testCounter = 0;
  m_successCounter = 0;

  //m_ptpd = NULL;

  connect(&m_timeOut, SIGNAL(timeout()), this, SLOT(quitTest()) );
  m_timeOut.start(4000);

}

TestPTPSendReceive_Basic::~TestPTPSendReceive_Basic(void)
{
}


void TestPTPSendReceive_Basic::setupTest()
{
  //***********************************************
  //Instanciate socket objects
  std::cout <<++m_testCounter <<". Creating a ptp object..";
  m_ptpd = ptp::PTPDaemon::New();
  if (m_ptpd.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //This is required on Win to achieve accurate timings
  #if defined(_WIN32)
    m_socket1->initializeWinTimers();
  #endif

    m_ptpd->initialize();
//  m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
//  static_cast<OIGTLTransformMessage::Pointer>(m_msgToSend)->setMatrix(dummyTransformMatrix);
//  m_msgToSend->update(getLocalHostAddress());

//  // Instanciate pure igtl socket and configure for UDP
//  igtl::MulticastSocket::Pointer udpSock1 = igtl::MulticastSocket::New();
  
//  //udpSock1->CreateSocket(igtl::P_TCP_MULTI);
//  udpSock1->InitMultiCast(PTP_GROUP, 3900, 1, true);

//  uchar data1[100];
//  memset(&data1, 100, 100);

//  uchar data2[100];
//  memset(&data2, 100, 100);

//  udpSock1->SendMulticast(&data1, 100);
//  int bytes = udpSock1->CheckPendingData();
//  if (bytes > 0)
//  {
//    udpSock1->Receive2(&data2, bytes);
//  }
  
//  std::cerr<< memcmp(&data1, &data2, bytes);
}

void TestPTPSendReceive_Basic::performTest()
{
  std::cerr <<++m_testCounter <<". Sending ptp message ..";
  m_ptpd->startSync();
}

void TestPTPSendReceive_Basic::quitTest()
{
  if (m_ptpd.IsNotNull())
    m_ptpd.operator =(NULL);
  
  emit done();
  
  if (m_testCounter > m_successCounter)
    exit(-1);
  else exit(0);
}

void TestPTPSendReceive_Basic::catchMessage(OIGTLMessage::Pointer msg)
{
  QString sender = QObject::sender()->objectName();

  if (msg.operator!=(NULL))
  {
    QLOG_INFO() <<m_received+1 <<"of" <<m_numOfMsg <<"message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

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
  //if (m_received >= m_numOfMsg)
  //  { std::cout <<" OK\n"; m_successCounter++; quitTest(); }
}
