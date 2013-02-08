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
#include "NLTest_Data.h"
#include "TestImgMsg_General.h"

#include <cmath>

TestImgMsg_General::TestImgMsg_General(void)
{
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestImgMsg_General::~TestImgMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestImgMsg_General::SetupTest()
{
  //Nothing to do right now
}


void TestImgMsg_General::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating image message..";
  NiftyLinkImageMessage::Pointer imageMsg;
  imageMsg.reset();
  imageMsg = (NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
  
  if (imageMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test image data..";
  imageMsg->InitializeWithTestData();
  igtl::MessageBase::Pointer p;
  imageMsg->GetMessagePointer(p);
  if (p.IsNotNull())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  imageMsg->Update(GetLocalHostAddress());
  if (imageMsg->GetHostName().isEmpty() || imageMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting normals..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float n2[3] = { 2.1f, 2.2f, 2.3f};
  float n3[3] = { 3.1f, 3.2f, 3.3f};
  float m1[3], m2[3], m3[3];
  imageMsg->SetNormals(n1, n2, n3);
  imageMsg->GetNormals(m1, m2, m3);

  // We are getting normalized coordinates back, so have to chack the ratio instead of the exact value
  if ((m1[0] / m1[1] - n1[0] / n1[1]) > 0.01 || (m1[0] / m1[2] - n1[0] / n1[2]) > 0.01 || (m1[1] / m1[2] - n1[1] / n1[2]) > 0.01 ||
      (m2[0] / m2[1] - n2[0] / n2[1]) > 0.01 || (m2[0] / m2[2] - n2[0] / n2[2]) > 0.01 || (m2[1] / m2[2] - n2[1] / n2[2]) > 0.01 ||
      (m3[0] / m3[1] - n3[0] / n3[1]) > 0.01 || (m3[0] / m3[2] - n3[0] / n3[2]) > 0.01 || (m3[1] / m3[2] - n3[1] / n3[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //std::cout <<m1[0] / m1[1] <<" vs. " <<n1[0] / n1[1] <<"\n";
  //std::cout <<m1[0] / m1[2] <<" vs. " <<n1[0] / n1[2] <<"\n";
  //std::cout <<m1[1] / m1[2] <<" vs. " <<n1[1] / n1[2] <<"\n";
  //std::cout <<"\n";
  //std::cout <<m2[0] / m2[1] <<" vs. " <<n2[0] / n2[1] <<"\n";
  //std::cout <<m2[0] / m2[2] <<" vs. " <<n2[0] / n2[2] <<"\n";
  //std::cout <<m2[1] / m2[2] <<" vs. " <<n2[1] / n2[2] <<"\n";
  //std::cout <<"\n";
  //std::cout <<m3[0] / m3[1] <<" vs. " <<n3[0] / n3[1] <<"\n";
  //std::cout <<m3[0] / m3[2] <<" vs. " <<n3[0] / n3[2] <<"\n";
  //std::cout <<m3[1] / m3[2] <<" vs. " <<n3[1] / n3[2] <<"\n";


  //***********************************************
  std::cout <<++m_TestCounter <<". Setting spacing v1..";
  float spacing[3] = {1.1, 1.2, 1.3};
  float spacing2[3];
  memset(&spacing2, 0, sizeof(float)*3);
	
  imageMsg->SetSpacing(spacing);
	imageMsg->GetSpacing(spacing2);

  if (fabs(spacing[0] - spacing2[0]) > 0.01f || fabs(spacing[1] - spacing2[1]) > 0.01f || fabs(spacing[2] - spacing2[2]) > 0.01f)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting spacing v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  	
  imageMsg->SetSpacing(s1, s2, s3);
	imageMsg->GetSpacing(t1, t2, t3);

  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test matrix..";
  imageMsg->SetMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
  imageMsg->GetMatrix(matrix);

  bool ok = true;

  for (int i = 0; i < 4; i++)   
  {
    for (int j = 0; j < 3; j++)
    {
      //Single precision floats aren't really accurate....
      ok &= (fabs(dummyTransformMatrix[i][j] - matrix[i][j]) < 0.01f);
      //std::cerr << matrix[i][j] <<" ";
    }
    //std::cerr <<std::endl;
  }
  if (!ok)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting origin v1..";
  imageMsg->SetOrigin(n1);
  memset(&m1, 0, sizeof(float)*3);
  imageMsg->GetOrigin(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting origin v2..";
  imageMsg->SetOrigin(s1, s2, s3);
  t1 = 0; t2 = 0; t3 = 0;
  imageMsg->GetOrigin(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting message type tag..";
  imageMsg->ChangeMessageType("Something");
  if (imageMsg->GetMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting port number..";
  imageMsg->SetPort(100);
  if (imageMsg->GetPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  imageMsg->SetTimeReceived(tsr);
  tss = imageMsg->GetTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = imageMsg->GetTimeCreated();
  id = imageMsg->GetId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Checking Update function and hostname..";
  imageMsg->Update("Something");
  QString hname = imageMsg->GetHostName(); 
  
  imageMsg->ChangeHostName("Anything");
  QString hname2 = imageMsg->GetHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  imageMsg->SetMessagePointer(mp);
  imageMsg->GetMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

   //***********************************************
  std::cout <<++m_TestCounter <<". Set resolution settings..";
  NiftyLinkMessage::Pointer sttImg;
  sttImg.reset();
  NiftyLinkImageMessage::Create_STT(sttImg);
  sttImg->SetResolution(100);
 
  igtlUint64 res = 0;
  sttImg->GetResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  imageMsg.reset();
  imageMsg.operator =(NULL);
  
  sttImg.reset();
  sttImg.operator =(NULL);
   
  if (sttImg.operator !=(NULL) || imageMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestImgMsg_General::QuitTest()
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
