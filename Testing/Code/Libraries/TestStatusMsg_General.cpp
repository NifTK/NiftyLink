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
#include "TestStatusMsg_General.h"

#include <cmath>

TestStatusMsg_General::TestStatusMsg_General(void)
{
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestStatusMsg_General::~TestStatusMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestStatusMsg_General::SetupTest()
{
  //Nothing to do right now
}


void TestStatusMsg_General::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating status message..";
  NiftyLinkStatusMessage::Pointer statusMsg;
  statusMsg.reset();
  statusMsg = (NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage()));
  
  if (statusMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Initializing with test data..";
  statusMsg->InitializeWithTestData();

  if (statusMsg->GetErrorCode() == igtl::StatusMessage::STATUS_OK &&
    statusMsg->GetErrorSubCode() == igtl::StatusMessage::STATUS_PANICK_MODE &&
    statusMsg->GetErrorName() == QString("ErrorName") &&
    statusMsg->GetStatusString() == QString("StatusString") )
      { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  statusMsg->SetStatusString("LongLongLongErrorName");
  
  qDebug() <<statusMsg->GetStatusString();

  statusMsg->SetStatusString("LongLongLongErrorNameLongLongLongErrorName");
  qDebug() <<statusMsg->GetStatusString();
    
    
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  statusMsg->Update(GetLocalHostAddress());

  if (statusMsg->GetHostName().isEmpty() || statusMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting message type tag..";
  statusMsg->ChangeMessageType("Something");
  if (statusMsg->GetMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting port number..";
  statusMsg->SetPort(100);
  if (statusMsg->GetPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->Update();
  statusMsg->SetTimeReceived(tsr);
  tss = statusMsg->GetTimeReceived();
  if (tsr->GetTimeUint64() != tss->GetTimeUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = statusMsg->GetTimeCreated();
  id = statusMsg->GetId();
  if (tsr->GetTimeUint64() < tsc->GetTimeUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking Update function and hostname..";
  statusMsg->Update("Something");
  QString hname = statusMsg->GetHostName(); 
  
  statusMsg->ChangeHostName("Anything");
  QString hname2 = statusMsg->GetHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  statusMsg->SetMessagePointer(mp);
  statusMsg->GetMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Set resolution settings..";
  NiftyLinkMessage::Pointer sttStatus;
  sttStatus.reset();
  NiftyLinkStatusMessage::Create_STT(sttStatus);
  sttStatus->SetResolution(100);
 
  igtlUint64 res = 0;
  sttStatus->GetResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  statusMsg.reset();
  statusMsg.operator =(NULL);
  
  sttStatus.reset();
  sttStatus.operator =(NULL);
   
  if (sttStatus.operator !=(NULL) || statusMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestStatusMsg_General::QuitTest()
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
