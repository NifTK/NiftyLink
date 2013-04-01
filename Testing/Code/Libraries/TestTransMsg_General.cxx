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
#include "TestTransMsg_General.h"
#include "NLTest_Data.h"

#include <cmath>

TestTransMsg_General::TestTransMsg_General(void)
{
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestTransMsg_General::~TestTransMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestTransMsg_General::SetupTest()
{
  //Nothing to do right now
}


void TestTransMsg_General::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating transform message..";
  NiftyLinkTransformMessage::Pointer transMsg;
  transMsg.reset();
  transMsg = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
  
  if (transMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test matrix..";
  transMsg->SetMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
	transMsg->GetMatrix(matrix);

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
  transMsg->InitializeWithTestData(dummyTransformMatrix);
  transMsg->GetMatrix(matrix);

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
  transMsg->InitializeWithRandomData();
  transMsg->GetMatrix(matrix);

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
  transMsg->InitializeWithRandomData();
  QString str = transMsg->GetMatrixAsString();

  if (str.isNull() || str.isEmpty() )
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting normals..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float n2[3] = { 2.1f, 2.2f, 2.3f};
  float n3[3] = { 3.1f, 3.2f, 3.3f};
  float m1[3], m2[3], m3[3];
	transMsg->SetNormals(n1, n2, n3);
	transMsg->GetNormals(m1, m2, m3);
  
  // We are getting normalized coordinates back, so have to chack the ratio instead of the exact value
  if ((m1[0] / m1[1] - n1[0] / n1[1]) > 0.01 || (m1[0] / m1[2] - n1[0] / n1[2]) > 0.01 || (m1[1] / m1[2] - n1[1] / n1[2]) > 0.01 ||
      (m2[0] / m2[1] - n2[0] / n2[1]) > 0.01 || (m2[0] / m2[2] - n2[0] / n2[2]) > 0.01 || (m2[1] / m2[2] - n2[1] / n2[2]) > 0.01 ||
      (m3[0] / m3[1] - n3[0] / n3[1]) > 0.01 || (m3[0] / m3[2] - n3[0] / n3[2]) > 0.01 || (m3[1] / m3[2] - n3[1] / n3[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

 // 
 // //std::cout <<m1[0] / m1[1] <<" vs. " <<n1[0] / n1[1] <<"\n";
 // //std::cout <<m1[0] / m1[2] <<" vs. " <<n1[0] / n1[2] <<"\n";
 // //std::cout <<m1[1] / m1[2] <<" vs. " <<n1[1] / n1[2] <<"\n";
 // //std::cout <<"\n";
 // //std::cout <<m2[0] / m2[1] <<" vs. " <<n2[0] / n2[1] <<"\n";
 // //std::cout <<m2[0] / m2[2] <<" vs. " <<n2[0] / n2[2] <<"\n";
 // //std::cout <<m2[1] / m2[2] <<" vs. " <<n2[1] / n2[2] <<"\n";
 // //std::cout <<"\n";
 // //std::cout <<m3[0] / m3[1] <<" vs. " <<n3[0] / n3[1] <<"\n";
 // //std::cout <<m3[0] / m3[2] <<" vs. " <<n3[0] / n3[2] <<"\n";
 // //std::cout <<m3[1] / m3[2] <<" vs. " <<n3[1] / n3[2] <<"\n";

 //***********************************************
  std::cout <<++m_TestCounter <<". Setting position v1..";
  transMsg->SetPosition(n1);
  memset(&m1, 0, sizeof(float)*3);
  transMsg->GetPosition(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
 
 //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting position v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  transMsg->SetPosition(s1, s2, s3);
  transMsg->GetPosition(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  transMsg->Update(GetLocalHostAddress());

  if (transMsg->GetHostName().isEmpty() || transMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting message type tag..";
  transMsg->ChangeMessageType("Something");
  if (transMsg->GetMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting port number..";
  transMsg->SetPort(100);
  if (transMsg->GetPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->Update();
  transMsg->SetTimeReceived(tsr);
  tss = transMsg->GetTimeReceived();
  if (tsr->GetTimeUint64() != tss->GetTimeUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = transMsg->GetTimeCreated();
  id = transMsg->GetId();
  if (tsr->GetTimeUint64() < tsc->GetTimeUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking Update function and hostname..";
  transMsg->Update("Something");
  QString hname = transMsg->GetHostName(); 
  
  transMsg->ChangeHostName("Anything");
  QString hname2 = transMsg->GetHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //*********************************************** 
  std::cout <<++m_TestCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  transMsg->SetMessagePointer(mp);
  transMsg->GetMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Set resolution settings..";
  NiftyLinkMessage::Pointer sttTrans;
  sttTrans.reset();
  NiftyLinkTransformMessage::Create_STT(sttTrans);
  sttTrans->SetResolution(100);
 
  igtlUint64 res = 0;
  sttTrans->GetResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  transMsg.reset();
  transMsg.operator =(NULL);
  
  sttTrans.reset();
  sttTrans.operator =(NULL);
   
  if (sttTrans.operator !=(NULL) || transMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestTransMsg_General::QuitTest()
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
