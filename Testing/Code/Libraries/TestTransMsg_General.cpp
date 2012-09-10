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
#include "TestTransMsg_General.h"
#include "NLTest_Data.h"

#include <cmath>

TestTransMsg_General::TestTransMsg_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestTransMsg_General::~TestTransMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestTransMsg_General::setupTest()
{
  //Nothing to do right now
}


void TestTransMsg_General::performTest()
{
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
  std::cout <<++m_testCounter <<". Setting test matrix..";
  transMsg->setMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
	transMsg->getMatrix(matrix);

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
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Initializing with test data..";
  transMsg->initializeWithTestData(dummyTransformMatrix);
  transMsg->getMatrix(matrix);

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
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Initializing with random data..";
  transMsg->initializeWithRandomData();
  transMsg->getMatrix(matrix);

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
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Testing GetMatrixAsString..";
  transMsg->initializeWithRandomData();
  QString str = transMsg->getMatrixAsString();

  if (str.isNull() || str.isEmpty() )
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting normals..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float n2[3] = { 2.1f, 2.2f, 2.3f};
  float n3[3] = { 3.1f, 3.2f, 3.3f};
  float m1[3], m2[3], m3[3];
	transMsg->setNormals(n1, n2, n3);
	transMsg->getNormals(m1, m2, m3);
  
  // We are getting normalized coordinates back, so have to chack the ratio instead of the exact value
  if ((m1[0] / m1[1] - n1[0] / n1[1]) > 0.01 || (m1[0] / m1[2] - n1[0] / n1[2]) > 0.01 || (m1[1] / m1[2] - n1[1] / n1[2]) > 0.01 ||
      (m2[0] / m2[1] - n2[0] / n2[1]) > 0.01 || (m2[0] / m2[2] - n2[0] / n2[2]) > 0.01 || (m2[1] / m2[2] - n2[1] / n2[2]) > 0.01 ||
      (m3[0] / m3[1] - n3[0] / n3[1]) > 0.01 || (m3[0] / m3[2] - n3[0] / n3[2]) > 0.01 || (m3[1] / m3[2] - n3[1] / n3[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

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
  std::cout <<++m_testCounter <<". Setting position v1..";
  transMsg->setPosition(n1);
  memset(&m1, 0, sizeof(float)*3);
  transMsg->getPosition(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
 
 //*********************************************** 
  std::cout <<++m_testCounter <<". Setting position v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  transMsg->setPosition(s1, s2, s3);
  transMsg->getPosition(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  transMsg->update(getLocalHostAddress());

  if (transMsg->getHostName().isEmpty() || transMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting message type tag..";
  transMsg->changeMessageType("Something");
  if (transMsg->getMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting port number..";
  transMsg->setPort(100);
  if (transMsg->getPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  transMsg->setTimeReceived(tsr);
  tss = transMsg->getTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = transMsg->getTimeCreated();
  id = transMsg->getId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking update function and hostname..";
  transMsg->update("Something");
  QString hname = transMsg->getHostName(); 
  
  transMsg->changeHostName("Anything");
  QString hname2 = transMsg->getHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  transMsg->setMessagePointer(mp);
  transMsg->getMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Set resolution settings..";
  OIGTLMessage::Pointer sttTrans;
  sttTrans.reset();
  OIGTLTransformMessage::Create_STT(sttTrans);
  sttTrans->setResolution(100);
 
  igtlUint64 res = 0;
  sttTrans->getResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  transMsg.reset();
  transMsg.operator =(NULL);
  
  sttTrans.reset();
  sttTrans.operator =(NULL);
   
  if (sttTrans.operator !=(NULL) || transMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestTransMsg_General::quitTest()
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
