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
#include "TestMsgTypes_ConstrDestr.h"
#include "NLTest_Data.h"

TestMsgTypes_ConstrDestr::TestMsgTypes_ConstrDestr(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestMsgTypes_ConstrDestr::~TestMsgTypes_ConstrDestr(void)
{
  //std::cout <<"Test class destructor \n";
}


void TestMsgTypes_ConstrDestr::setupTest()
{
  //Nothing to do right now
}


void TestMsgTypes_ConstrDestr::performTest()
{
  //Create a basic message
  std::cout <<++m_testCounter <<". Creating basic message..";
  OIGTLMessage::Pointer basicMsg;
  basicMsg.reset(); //Ensure that the pointer is NULL
  basicMsg = (OIGTLMessage::Pointer(new OIGTLMessage()));
  if (basicMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  basicMsg.reset();
  basicMsg.operator =(NULL);

  if (basicMsg.operator !=(NULL))
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************
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
  std::cout <<++m_testCounter <<". Testing copy constructor..";
  OIGTLImageMessage::Pointer imageMsg2;
  imageMsg2.reset();
  imageMsg2 = (OIGTLImageMessage::Pointer(new OIGTLImageMessage(*imageMsg)));

  if ((imageMsg->getHostName() != imageMsg2->getHostName()) ||
      (imageMsg->getQImage()   != imageMsg2->getQImage()))
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  imageMsg2.reset();
  imageMsg2.operator =(NULL);

  imageMsg.reset();
  imageMsg.operator =(NULL);

  if (imageMsg.operator !=(NULL) ||
     imageMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_IMAGE
  std::cout <<++m_testCounter <<". Creating GET_IMAGE message..";
  OIGTLMessage::Pointer getImg;
  getImg.reset();
  OIGTLImageMessage::Create_GET(getImg);
  if (getImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_IMAGE
  std::cout <<++m_testCounter <<". Creating STT_IMAGE message..";
  OIGTLMessage::Pointer sttImg;
  sttImg.reset();
  OIGTLImageMessage::Create_STT(sttImg);
  if (sttImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_IMAGE
  std::cout <<++m_testCounter <<". Creating STP_IMAGE message..";
  OIGTLMessage::Pointer stpImg;
  stpImg.reset();
  OIGTLImageMessage::Create_STP(stpImg);
  if (stpImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_IMAGE
  std::cout <<++m_testCounter <<". Creating RTS_IMAGE message..";
  OIGTLMessage::Pointer rtsImg;
  rtsImg.reset();
  OIGTLImageMessage::Create_RTS(rtsImg);
  if (rtsImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************    
  //Create transform message and initialize with test data
  std::cout <<++m_testCounter <<". Creating transform message..";
  OIGTLTransformMessage::Pointer transMsg;
  transMsg.reset();
  transMsg = (OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
  if (transMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test transform data..";
  transMsg->initializeWithTestData(dummyTransformMatrix);
  igtl::MessageBase::Pointer q;
  transMsg->getMessagePointer(q);
  if (q.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  transMsg->update(getLocalHostAddress());
  if (transMsg->getHostName().isEmpty() || transMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }
  
  //***********************************************
  std::cout <<++m_testCounter <<". Testing copy constructor..";
  OIGTLTransformMessage::Pointer transMsg2;
  transMsg2.reset();
  transMsg2 = (OIGTLTransformMessage::Pointer(new OIGTLTransformMessage(*transMsg)));

  igtl::Matrix4x4 mat1;
  transMsg->getMatrix(mat1);
  igtl::Matrix4x4 mat2;
  transMsg2->getMatrix(mat2);

  bool ba, bb;
  if (transMsg->getHostName() == transMsg2->getHostName())
    ba = true;
  else ba = false;
  
  if (memcmp(&mat1, &mat2, sizeof(float)*4*4) == 0)
    bb = true;
  else bb = false;

  if (ba && bb)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  transMsg2.reset();
  transMsg2.operator =(NULL);

  transMsg.reset();
  transMsg.operator =(NULL);

  if (transMsg.operator !=(NULL) ||
     transMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_testCounter <<". Creating GET_TRANS message..";
  OIGTLMessage::Pointer getTrs;
  getTrs.reset();
  OIGTLTransformMessage::Create_GET(getTrs);
  if (getTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TRANS message..";
  OIGTLMessage::Pointer sttTrs;
  sttTrs.reset();
  OIGTLTransformMessage::Create_STT(sttTrs);
  if (sttTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TRANS message..";
  OIGTLMessage::Pointer stpTrs;
  stpTrs.reset();
  OIGTLTransformMessage::Create_STP(stpTrs);
  if (stpTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_testCounter <<". Creating RTS_TRANS message..";
  OIGTLMessage::Pointer rtsTrs;
  rtsTrs.reset();
  OIGTLTransformMessage::Create_RTS(rtsTrs);
  if (rtsTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************    
  //Create status message and initialize with test data
  std::cout <<++m_testCounter <<". Creating status message..";
  OIGTLStatusMessage::Pointer statusMsg;
  statusMsg.reset();
  statusMsg = (OIGTLStatusMessage::Pointer(new OIGTLStatusMessage()));
  if (statusMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test status info..";
  statusMsg->initializeWithTestData();
  igtl::MessageBase::Pointer r;
  statusMsg->getMessagePointer(r);
  if (r.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  statusMsg->update(getLocalHostAddress());
  if (statusMsg->getHostName().isEmpty() || statusMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Testing copy constructor..";
  OIGTLStatusMessage::Pointer statusMsg2;
  statusMsg2.reset();
  statusMsg2 = (OIGTLStatusMessage::Pointer(new OIGTLStatusMessage(*statusMsg)));

  if (statusMsg->getErrorCode()    == statusMsg2->getErrorCode() &&
      statusMsg->getErrorSubCode() == statusMsg2->getErrorSubCode() &&
      statusMsg->getErrorName()    == statusMsg2->getErrorName() &&
      statusMsg->getStatusString() == statusMsg2->getStatusString())
        { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  statusMsg2.reset();
  statusMsg2.operator =(NULL);

  statusMsg.reset();
  statusMsg.operator =(NULL);

  if (statusMsg.operator !=(NULL) ||
     statusMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_testCounter <<". Creating GET_STATUS message..";
  OIGTLMessage::Pointer getSta;
  getSta.reset();
  OIGTLStatusMessage::Create_GET(getSta);
  if (getSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_testCounter <<". Creating STT_STATUS message..";
  OIGTLMessage::Pointer sttSta;
  sttSta.reset();
  OIGTLStatusMessage::Create_STT(sttSta);
  if (sttSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_testCounter <<". Creating STT_STATUS message..";
  OIGTLMessage::Pointer stpSta;
  stpSta.reset();
  OIGTLStatusMessage::Create_STP(stpSta);
  if (stpSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_testCounter <<". Creating RTS_STATUS message..";
  OIGTLMessage::Pointer rtsSta;
  rtsSta.reset();
  OIGTLStatusMessage::Create_RTS(rtsSta);
  if (rtsSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //***********************************************    
  //Create string message and initialize with test data
  std::cout <<++m_testCounter <<". Creating string message..";
  OIGTLStringMessage::Pointer stringMsg;
  stringMsg.reset();
  stringMsg = (OIGTLStringMessage::Pointer(new OIGTLStringMessage()));
  if (stringMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test string data..";
  stringMsg->initializeWithTestData();
  igtl::MessageBase::Pointer s;
  stringMsg->getMessagePointer(s);
  if (s.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  stringMsg->update(getLocalHostAddress());
  if (stringMsg->getHostName().isEmpty() || stringMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Testing copy constructor..";
  OIGTLStringMessage::Pointer stringMsg2;
  stringMsg2.reset();
  stringMsg2 = (OIGTLStringMessage::Pointer(new OIGTLStringMessage(*stringMsg)));

  if (stringMsg->getHostName() == stringMsg2->getHostName() &&
      stringMsg->getString() == stringMsg2->getString() )
        { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  stringMsg2.reset();
  stringMsg2.operator =(NULL);

  stringMsg.reset();
  stringMsg.operator =(NULL);

  if (stringMsg.operator !=(NULL) ||
     stringMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_testCounter <<". Creating GET_STRING message..";
  OIGTLMessage::Pointer getStr;
  getStr.reset();
  OIGTLStringMessage::Create_GET(getStr);
  if (getStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_testCounter <<". Creating STT_STRING message..";
  OIGTLMessage::Pointer sttStr;
  sttStr.reset();
  OIGTLStringMessage::Create_STT(sttStr);
  if (sttStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_testCounter <<". Creating STT_STRING message..";
  OIGTLMessage::Pointer stpStr;
  stpStr.reset();
  OIGTLStringMessage::Create_STP(stpStr);
  if (stpStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_testCounter <<". Creating RTS_STRING message..";
  OIGTLMessage::Pointer rtsStr;
  rtsStr.reset();
  OIGTLStringMessage::Create_RTS(rtsStr);
  if (rtsStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //***********************************************    
  //Create tdata message and initialize with test data
  std::cout <<++m_testCounter <<". Creating tdata message..";
  OIGTLTrackingDataMessage::Pointer tdataMsg;
  tdataMsg.reset();
  tdataMsg = (OIGTLTrackingDataMessage::Pointer(new OIGTLTrackingDataMessage()));
  if (tdataMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test tracking data..";
  tdataMsg->initializeWithTestData(dummyTransformMatrix);
  igtl::MessageBase::Pointer t;
  tdataMsg->getMessagePointer(t);
  if (t.IsNotNull())
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_testCounter <<". Setting timestamp and sender ID..";
  tdataMsg->update(getLocalHostAddress());
  if (tdataMsg->getHostName().isEmpty() || tdataMsg->getTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************
  std::cout <<++m_testCounter <<". Testing copy constructor..";
  OIGTLTrackingDataMessage::Pointer tdataMsg2;
  tdataMsg2.reset();
  tdataMsg2 = (OIGTLTrackingDataMessage::Pointer(new OIGTLTrackingDataMessage(*tdataMsg)));

  tdataMsg->getMatrix(mat1);
  tdataMsg2->getMatrix(mat2);
  
  if (memcmp(&mat1, &mat2, sizeof(float)*4*4) == 0)
    bb = true;
  else bb = false;

  if (tdataMsg->getHostName() == tdataMsg2->getHostName() && bb)
        { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Testing delete..";
  tdataMsg2.reset();
  tdataMsg2.operator =(NULL);

  tdataMsg.reset();
  tdataMsg.operator =(NULL);

  if (tdataMsg.operator !=(NULL) ||
     tdataMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_testCounter <<". Creating GET_TDATA message..";
  OIGTLMessage::Pointer getTdata;
  getTdata.reset();
  OIGTLTrackingDataMessage::Create_GET(getTdata);
  if (getTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TDATA message..";
  OIGTLMessage::Pointer sttTdata;
  sttTdata.reset();
  OIGTLTrackingDataMessage::Create_STT(sttTdata);
  if (sttTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_testCounter <<". Creating STT_TDATA message..";
  OIGTLMessage::Pointer stpTdata;
  stpTdata.reset();
  OIGTLTrackingDataMessage::Create_STP(stpTdata);
  if (stpTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_testCounter <<". Creating RTS_TDATA message..";
  OIGTLMessage::Pointer rtsTdata;
  rtsTdata.reset();
  OIGTLTrackingDataMessage::Create_RTS(rtsTdata);
  if (rtsTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestMsgTypes_ConstrDestr::quitTest()
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
