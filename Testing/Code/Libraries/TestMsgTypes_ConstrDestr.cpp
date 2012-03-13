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
#include "TestMsgTypes_ConstrDestr.h"

TestMsgTypes_ConstrDestr::TestMsgTypes_ConstrDestr(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestMsgTypes_ConstrDestr::~TestMsgTypes_ConstrDestr(void)
{
  //std::cout <<"Test class destructor \n";
}


void TestMsgTypes_ConstrDestr::setupTest()
{
  //Nothing to do right now
}


void TestMsgTypes_ConstrDestr::performTest()
{
  //Create a basic message
  std::cout <<++m_testCounter <<". Creating basic message..";
  OIGTLMessage::Pointer basicMsg;
  basicMsg.reset(); //Ensure that the pointer is NULL
  basicMsg = (OIGTLMessage::Pointer(new OIGTLMessage()));
  if (basicMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating image message..";
  OIGTLImageMessage::Pointer imageMsg;
  imageMsg.reset();
  imageMsg = (OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  if (imageMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting test image data..";
  imageMsg->initializeWithTestData();
  igtl::MessageBase::Pointer p;
  imageMsg->getMessagePointer(p);
  if (p.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  imageMsg->update(getLocalHostAddress());
  if (imageMsg->getHostName().isEmpty() || imageMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  //Create GET_IMAGE
  std::cout <<++m_testCounter <<". Creating GET_IMAGE message..";
  OIGTLMessage::Pointer getImg;
  getImg.reset();
  OIGTLImageMessage::Create_GET(getImg);
  if (getImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_IMAGE
  std::cout <<++m_testCounter <<". Creating STT_IMAGE message..";
  OIGTLMessage::Pointer sttImg;
  sttImg.reset();
  OIGTLImageMessage::Create_STT(sttImg);
  if (sttImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_IMAGE
  std::cout <<++m_testCounter <<". Creating STP_IMAGE message..";
  OIGTLMessage::Pointer stpImg;
  stpImg.reset();
  OIGTLImageMessage::Create_STP(stpImg);
  if (stpImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_IMAGE
  std::cout <<++m_testCounter <<". Creating RTS_IMAGE message..";
  OIGTLMessage::Pointer rtsImg;
  rtsImg.reset();
  OIGTLImageMessage::Create_RTS(rtsImg);
  if (rtsImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************    
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating transform message..";
  OIGTLTransformMessage::Pointer transMsg;
  transMsg.reset();
  transMsg = (OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
  if (transMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test transform data..";
  transMsg->initializeWithTestData();
  igtl::MessageBase::Pointer q;
  transMsg->getMessagePointer(q);
  if (q.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  transMsg->update(getLocalHostAddress());
  if (transMsg->getHostName().isEmpty() || transMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_testCounter <<". Creating GET_TRANS message..";
  OIGTLMessage::Pointer getTrs;
  getTrs.reset();
  OIGTLImageMessage::Create_GET(getTrs);
  if (getTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TRANS message..";
  OIGTLMessage::Pointer sttTrs;
  sttTrs.reset();
  OIGTLTransformMessage::Create_STT(sttTrs);
  if (sttTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TRANS message..";
  OIGTLMessage::Pointer stpTrs;
  stpTrs.reset();
  OIGTLTransformMessage::Create_STP(stpTrs);
  if (stpTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_testCounter <<". Creating RTS_TRANS message..";
  OIGTLMessage::Pointer rtsTrs;
  rtsTrs.reset();
  OIGTLTransformMessage::Create_RTS(rtsTrs);
  if (rtsTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestMsgTypes_ConstrDestr::quitTest()
{
  emit done();
  if (m_testCounter > m_successCounter)
    exit(-1);
  else exit(0);
}