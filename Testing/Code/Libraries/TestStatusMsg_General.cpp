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
#include "TestStatusMsg_General.h"

#include <cmath>

TestStatusMsg_General::TestStatusMsg_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestStatusMsg_General::~TestStatusMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestStatusMsg_General::setupTest()
{
  //Nothing to do right now
}


void TestStatusMsg_General::performTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating status message..";
  OIGTLStatusMessage::Pointer statusMsg;
  statusMsg.reset();
  statusMsg = (OIGTLStatusMessage::Pointer(new OIGTLStatusMessage()));
  
  if (statusMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Initializing with test data..";
  statusMsg->initializeWithTestData();

  if (statusMsg->getErrorCode() == igtl::StatusMessage::STATUS_OK &&
    statusMsg->getErrorSubCode() == igtl::StatusMessage::STATUS_PANICK_MODE &&
    statusMsg->getErrorName() == QString("ErrorName") &&
    statusMsg->getStatusString() == QString("StatusString") )
      { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  statusMsg->setStatusString("LongLongLongErrorName");
  
  qDebug() <<statusMsg->getStatusString();

  statusMsg->setStatusString("LongLongLongErrorNameLongLongLongErrorName");
  qDebug() <<statusMsg->getStatusString();
    
    
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  statusMsg->update(getLocalHostAddress());

  if (statusMsg->getHostName().isEmpty() || statusMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting message type tag..";
  statusMsg->changeMessageType("Something");
  if (statusMsg->getMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting port number..";
  statusMsg->setPort(100);
  if (statusMsg->getPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  statusMsg->setTimeReceived(tsr);
  tss = statusMsg->getTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = statusMsg->getTimeCreated();
  id = statusMsg->getId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking update function and hostname..";
  statusMsg->update("Something");
  QString hname = statusMsg->getHostName(); 
  
  statusMsg->changeHostName("Anything");
  QString hname2 = statusMsg->getHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  statusMsg->setMessagePointer(mp);
  statusMsg->getMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Set resolution settings..";
  OIGTLMessage::Pointer sttStatus;
  sttStatus.reset();
  OIGTLStatusMessage::Create_STT(sttStatus);
  sttStatus->setResolution(100);
 
  igtlUint64 res = 0;
  sttStatus->getResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  statusMsg.reset();
  statusMsg.operator =(NULL);
  
  sttStatus.reset();
  sttStatus.operator =(NULL);
   
  if (sttStatus.operator !=(NULL) || statusMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestStatusMsg_General::quitTest()
{
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
