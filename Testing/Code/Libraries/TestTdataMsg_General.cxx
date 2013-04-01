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
#include "TestTdataMsg_General.h"
#include "NLTest_Data.h"

#include <cmath>

TestTdataMsg_General::TestTdataMsg_General(void)
{
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestTdataMsg_General::~TestTdataMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestTdataMsg_General::SetupTest()
{
  //Nothing to do right now
}


void TestTdataMsg_General::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating trackerdata message..";
  NiftyLinkTrackingDataMessage::Pointer tdataMsg;
  tdataMsg.reset();
  tdataMsg = (NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage()));
  
  if (tdataMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test matrix..";
  tdataMsg->SetMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
  tdataMsg->GetMatrix(matrix);

  bool ok = true;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
      //Single precision floats aren't really accurate....
      ok &= (fabs(dummyTransformMatrix[i][j] - matrix[i][j]) < 0.01f);
    }
  if (!ok)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Initializing with test data..";
  tdataMsg->InitializeWithTestData(dummyTransformMatrix);
  tdataMsg->GetMatrix(matrix);

  ok = true;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
      //Single precision floats aren't really accurate....
      ok &= (fabs(dummyTransformMatrix[i][j] - matrix[i][j]) < 0.01f);
    }
  if (!ok)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Initializing with random data..";
  tdataMsg->InitializeWithRandomData();
  tdataMsg->GetMatrix(matrix);

  ok = true;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
      //Single precision floats aren't really accurate....
      ok &= (fabs(dummyTransformMatrix[i][j] - matrix[i][j]) < 0.01f);
    }
  if (ok)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing GetMatrixAsString..";
  tdataMsg->InitializeWithRandomData();
  QString str = tdataMsg->GetMatrixAsString();

  if (str.isNull() || str.isEmpty() )
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

 //***********************************************
  std::cout <<++m_TestCounter <<". Setting position v1..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float m1[3];

  tdataMsg->SetPosition(n1);
  memset(&m1, 0, sizeof(float)*3);
  tdataMsg->GetPosition(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
 
 //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting position v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  tdataMsg->SetPosition(s1, s2, s3);
  tdataMsg->GetPosition(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

   //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting trackertool name..";
  tdataMsg->SetTrackerToolName("Some random tool");
  if (tdataMsg->GetTrackerToolName() != QString("Some random tool"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting trackertool type..";
  tdataMsg->SetTrackerToolType(igtl::TrackingDataElement::TYPE_3D);
  if (tdataMsg->GetTrackerToolType() != igtl::TrackingDataElement::TYPE_3D)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  tdataMsg->Update(GetLocalHostAddress());

  if (tdataMsg->GetHostName().isEmpty() || tdataMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting message type tag..";
  tdataMsg->ChangeMessageType("Something");
  if (tdataMsg->GetMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting port number..";
  tdataMsg->SetPort(100);
  if (tdataMsg->GetPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->Update();
  tdataMsg->SetTimeReceived(tsr);
  tss = tdataMsg->GetTimeReceived();
  if (tsr->GetTimeUint64() != tss->GetTimeUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = tdataMsg->GetTimeCreated();
  id = tdataMsg->GetId();
  if (tsr->GetTimeUint64() < tsc->GetTimeUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking Update function and hostname..";
  tdataMsg->Update("Something");
  QString hname = tdataMsg->GetHostName();
  
  tdataMsg->ChangeHostName("Anything");
  QString hname2 = tdataMsg->GetHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  tdataMsg->SetMessagePointer(mp);
  tdataMsg->GetMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Set resolution settings..";
  NiftyLinkMessage::Pointer sttTdata;
  sttTdata.reset();
  NiftyLinkTrackingDataMessage::Create_STT(sttTdata);
  sttTdata->SetResolution(100);
 
  igtlUint64 res = 0;
  sttTdata->GetResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  tdataMsg.reset();
  tdataMsg.operator =(NULL);
  
  sttTdata.reset();
  sttTdata.operator =(NULL);
   
  if (sttTdata.operator !=(NULL) || tdataMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestTdataMsg_General::QuitTest()
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
