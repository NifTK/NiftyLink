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
  m_testCounter = 0;
  m_successCounter = 0;
}

TestImgMsg_General::~TestImgMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestImgMsg_General::setupTest()
{
  //Nothing to do right now
}


void TestImgMsg_General::performTest()
{
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
  std::cout <<++m_testCounter <<". Setting normals..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float n2[3] = { 2.1f, 2.2f, 2.3f};
  float n3[3] = { 3.1f, 3.2f, 3.3f};
  float m1[3], m2[3], m3[3];
  imageMsg->setNormals(n1, n2, n3);
  imageMsg->getNormals(m1, m2, m3);

  // We are getting normalized coordinates back, so have to chack the ratio instead of the exact value
  if ((m1[0] / m1[1] - n1[0] / n1[1]) > 0.01 || (m1[0] / m1[2] - n1[0] / n1[2]) > 0.01 || (m1[1] / m1[2] - n1[1] / n1[2]) > 0.01 ||
      (m2[0] / m2[1] - n2[0] / n2[1]) > 0.01 || (m2[0] / m2[2] - n2[0] / n2[2]) > 0.01 || (m2[1] / m2[2] - n2[1] / n2[2]) > 0.01 ||
      (m3[0] / m3[1] - n3[0] / n3[1]) > 0.01 || (m3[0] / m3[2] - n3[0] / n3[2]) > 0.01 || (m3[1] / m3[2] - n3[1] / n3[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

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
  std::cout <<++m_testCounter <<". Setting spacing v1..";
  float spacing[3] = {1.1, 1.2, 1.3};
  float spacing2[3];
  memset(&spacing2, 0, sizeof(float)*3);
	
  imageMsg->setSpacing(spacing);
	imageMsg->getSpacing(spacing2);

  if (fabs(spacing[0] - spacing2[0]) > 0.01f || fabs(spacing[1] - spacing2[1]) > 0.01f || fabs(spacing[2] - spacing2[2]) > 0.01f)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting spacing v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  	
  imageMsg->setSpacing(s1, s2, s3);
	imageMsg->getSpacing(t1, t2, t3);

  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test matrix..";
  imageMsg->setMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
  imageMsg->getMatrix(matrix);

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
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting origin v1..";
  imageMsg->setOrigin(n1);
  memset(&m1, 0, sizeof(float)*3);
  imageMsg->getOrigin(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting origin v2..";
  imageMsg->setOrigin(s1, s2, s3);
  t1 = 0; t2 = 0; t3 = 0;
  imageMsg->getOrigin(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting message type tag..";
  imageMsg->changeMessageType("Something");
  if (imageMsg->getMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting port number..";
  imageMsg->setPort(100);
  if (imageMsg->getPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  imageMsg->setTimeReceived(tsr);
  tss = imageMsg->getTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = imageMsg->getTimeCreated();
  id = imageMsg->getId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Checking update function and hostname..";
  imageMsg->update("Something");
  QString hname = imageMsg->getHostName(); 
  
  imageMsg->changeHostName("Anything");
  QString hname2 = imageMsg->getHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  imageMsg->setMessagePointer(mp);
  imageMsg->getMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

   //***********************************************
  std::cout <<++m_testCounter <<". Set resolution settings..";
  OIGTLMessage::Pointer sttImg;
  sttImg.reset();
  OIGTLImageMessage::Create_STT(sttImg);
  sttImg->setResolution(100);
 
  igtlUint64 res = 0;
  sttImg->getResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  imageMsg.reset();
  imageMsg.operator =(NULL);
  
  sttImg.reset();
  sttImg.operator =(NULL);
   
  if (sttImg.operator !=(NULL) || imageMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestImgMsg_General::quitTest()
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
