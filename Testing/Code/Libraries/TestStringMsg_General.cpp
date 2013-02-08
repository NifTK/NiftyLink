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
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestStringMsg_General::~TestStringMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestStringMsg_General::SetupTest()
{
  //Nothing to do right now
}


void TestStringMsg_General::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating string message..";
  NiftyLinkStringMessage::Pointer stringMsg;
  stringMsg.reset();
  stringMsg = (NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage()));
  
  if (stringMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Initializing with test data..";
  stringMsg->InitializeWithTestData();

  if (stringMsg->GetString() == QString("This is a test string"))
      { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting string..";
  stringMsg->SetString("This is a random string which meant to crash the NiftyLink lib");

  if (stringMsg->GetString() != QString("This is a random string which meant to crash the NiftyLink lib"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting encoding..";
  stringMsg->SetEncoding(3);

  if (stringMsg->GetEncoding() != 3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
    
    
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  stringMsg->Update(GetLocalHostAddress());

  if (stringMsg->GetHostName().isEmpty() || stringMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting message type tag..";
  stringMsg->ChangeMessageType("Something");
  if (stringMsg->GetMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting port number..";
  stringMsg->SetPort(100);
  if (stringMsg->GetPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  stringMsg->SetTimeReceived(tsr);
  tss = stringMsg->GetTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = stringMsg->GetTimeCreated();
  id = stringMsg->GetId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking Update function and hostname..";
  stringMsg->Update("Something");
  QString hname = stringMsg->GetHostName(); 
  
  stringMsg->ChangeHostName("Anything");
  QString hname2 = stringMsg->GetHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  stringMsg->SetMessagePointer(mp);
  stringMsg->GetMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Set resolution settings..";
  NiftyLinkMessage::Pointer sttString;
  sttString.reset();
  NiftyLinkStringMessage::Create_STT(sttString);
  sttString->SetResolution(100);
 
  igtlUint64 res = 0;
  sttString->GetResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  stringMsg.reset();
  stringMsg.operator =(NULL);
  
  sttString.reset();
  sttString.operator =(NULL);
   
  if (sttString.operator !=(NULL) || stringMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestStringMsg_General::QuitTest()
{
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
