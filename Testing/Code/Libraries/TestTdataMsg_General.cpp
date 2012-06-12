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
#include "TestTdataMsg_General.h"

#include <cmath>

TestTdataMsg_General::TestTdataMsg_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestTdataMsg_General::~TestTdataMsg_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestTdataMsg_General::setupTest()
{
  //Nothing to do right now
}


void TestTdataMsg_General::performTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating trackerdata message..";
  OIGTLTrackingDataMessage::Pointer tdataMsg;
  tdataMsg.reset();
  tdataMsg = (OIGTLTrackingDataMessage::Pointer(new OIGTLTrackingDataMessage()));
  
  if (tdataMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test matrix..";
  tdataMsg->setMatrix(dummyTransformMatrix);
  igtl::Matrix4x4 matrix;
  tdataMsg->getMatrix(matrix);

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
  tdataMsg->initializeWithTestData();
  tdataMsg->getMatrix(matrix);

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
  tdataMsg->initializeWithRandomData();
  tdataMsg->getMatrix(matrix);

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
  tdataMsg->initializeWithRandomData();
  QString str = tdataMsg->getMatrixAsString();

  if (str.isNull() || str.isEmpty() )
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

 //***********************************************
  std::cout <<++m_testCounter <<". Setting position v1..";
  float n1[3] = { 1.1f, 1.2f, 1.3f};
  float m1[3];

  tdataMsg->setPosition(n1);
  memset(&m1, 0, sizeof(float)*3);
  tdataMsg->getPosition(m1);
  if (fabs(n1[0] - m1[0]) > 0.01 || fabs(n1[1] - m1[1]) > 0.01 || fabs(n1[2] - m1[2]) > 0.01)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
 
 //*********************************************** 
  std::cout <<++m_testCounter <<". Setting position v2..";
  float s1 = 1.1; float s2 = 1.2; float s3 = 1.3;
  float t1 = 0; float t2 = 0; float t3 = 0;
  tdataMsg->setPosition(s1, s2, s3);
  tdataMsg->getPosition(t1, t2, t3);
  if (s1 != t1 || s2 != t2 || s3 != t3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

   //*********************************************** 
  std::cout <<++m_testCounter <<". Setting trackertool name..";
  tdataMsg->setTrackerToolName("Some random tool");
  if (tdataMsg->getTrackerToolName() != QString("Some random tool"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting trackertool type..";
  tdataMsg->setTrackerToolType(igtl::TrackingDataElement::TYPE_3D);
  if (tdataMsg->getTrackerToolType() != igtl::TrackingDataElement::TYPE_3D)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  tdataMsg->update(getLocalHostAddress());

  if (tdataMsg->getHostName().isEmpty() || tdataMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting message type tag..";
  tdataMsg->changeMessageType("Something");
  if (tdataMsg->getMessageType() != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting port number..";
  tdataMsg->setPort(100);
  if (tdataMsg->getPort() != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Setting receive timestamp..";
  igtl::TimeStamp::Pointer tsr = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer tss = igtl::TimeStamp::New();
  tsr->GetTime_TAI();
  tdataMsg->setTimeReceived(tsr);
  tss = tdataMsg->getTimeReceived();
  if (tsr->GetTimeStampUint64() != tss->GetTimeStampUint64())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking create timestamp and id..";
  igtl::TimeStamp::Pointer tsc = igtl::TimeStamp::New();
  igtlUint64 id = 0;

  tsc = tdataMsg->getTimeCreated();
  id = tdataMsg->getId();
  if (tsr->GetTimeStampUint64() < tsc->GetTimeStampUint64() || id <= 0)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking update function and hostname..";
  tdataMsg->update("Something");
  QString hname = tdataMsg->getHostName();
  
  tdataMsg->changeHostName("Anything");
  QString hname2 = tdataMsg->getHostName();
  if (hname2 != QString("Anything") || hname != QString("Something"))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //*********************************************** 
  std::cout <<++m_testCounter <<". Checking message pointer get / set..";
  igtl::MessageBase::Pointer mp = igtl::MessageBase::New();
  igtl::MessageBase::Pointer mp3;
   
  tdataMsg->setMessagePointer(mp);
  tdataMsg->getMessagePointer(mp3);
  
  if (mp != mp3)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Set resolution settings..";
  OIGTLMessage::Pointer sttTdata;
  sttTdata.reset();
  OIGTLTrackingDataMessage::Create_STT(sttTdata);
  sttTdata->setResolution(100);
 
  igtlUint64 res = 0;
  sttTdata->getResolution(res);

  if (res != 100)
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  tdataMsg.reset();
  tdataMsg.operator =(NULL);
  
  sttTdata.reset();
  sttTdata.operator =(NULL);
   
  if (sttTdata.operator !=(NULL) || tdataMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestTdataMsg_General::quitTest()
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
