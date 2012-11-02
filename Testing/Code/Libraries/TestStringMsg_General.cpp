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
#include "TestStringMsg_General.h"

#include <cmath>

TestStringMsg_General::TestStringMsg_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestStringMsg_General::~TestStringMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestStringMsg_General::setupTest()
{
  //Nothing to do right now
}


void TestStringMsg_General::performTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating string message..";
  OIGTLStringMessage::Pointer stringMsg;
  stringMsg.reset();
  stringMsg = (OIGTLStringMessage::Pointer(new OIGTLStringMessage()));
  
  if (stringMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Initializing with test data..";
  stringMsg->initializeWithTestData();

  if (stringMsg->getString() == QString("This is a test string"))
      { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting string..";
  stringMsg->setString("This is a random string which meant to crash the OIGTL lib");

  if (stringMsg->getString() != QString("This is a random string which meant to crash the OIGTL lib"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting encoding..";
  stringMsg->setEncoding(3);

  if (stringMsg->getEncoding() != 3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
    
    
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  stringMsg->update(getLocalHostAddress());

  if (stringMsg->getHostName().isEmpty() || stringMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting message type tag..";
  stringMsg->changeMessageType("Something");
  if (stringMsg->getMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting port number..";
  stringMsg->setPort(100);
  if (stringMsg->getPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  stringMsg->setTimeReceived(tsr);
  tss = stringMsg->getTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = stringMsg->getTimeCreated();
  id = stringMsg->getId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking update function and hostname..";
  stringMsg->update("Something");
  QString hname = stringMsg->getHostName(); 
  
  stringMsg->changeHostName("Anything");
  QString hname2 = stringMsg->getHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  stringMsg->setMessagePointer(mp);
  stringMsg->getMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Set resolution settings..";
  OIGTLMessage::Pointer sttString;
  sttString.reset();
  OIGTLStringMessage::Create_STT(sttString);
  sttString->setResolution(100);
 
  igtlUint64 res = 0;
  sttString->getResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  stringMsg.reset();
  stringMsg.operator =(NULL);
  
  sttString.reset();
  sttString.operator =(NULL);
   
  if (sttString.operator !=(NULL) || stringMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestStringMsg_General::quitTest()
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
