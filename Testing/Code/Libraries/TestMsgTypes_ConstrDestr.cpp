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
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestMsgTypes_ConstrDestr::~TestMsgTypes_ConstrDestr(void)
{
  //std::cout <<"Test class destructor \n";
}


void TestMsgTypes_ConstrDestr::SetupTest()
{
  //Nothing to do right now
}


void TestMsgTypes_ConstrDestr::PerformTest()
{
  //Create a basic message
  std::cout <<++m_TestCounter <<". Creating basic message..";
  NiftyLinkMessage::Pointer basicMsg;
  basicMsg.reset(); //Ensure that the pointer is NULL
  basicMsg = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));
  if (basicMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  basicMsg.reset();
  basicMsg.operator =(NULL);

  if (basicMsg.operator !=(NULL))
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************
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
  std::cout <<++m_TestCounter <<". Testing copy constructor..";
  NiftyLinkImageMessage::Pointer imageMsg2;
  imageMsg2.reset();
  imageMsg2 = (NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage(*imageMsg)));

  if ((imageMsg->GetHostName() != imageMsg2->GetHostName()) ||
      (imageMsg->GetQImage()   != imageMsg2->GetQImage()))
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  imageMsg2.reset();
  imageMsg2.operator =(NULL);

  imageMsg.reset();
  imageMsg.operator =(NULL);

  if (imageMsg.operator !=(NULL) ||
     imageMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_IMAGE
  std::cout <<++m_TestCounter <<". Creating GET_IMAGE message..";
  NiftyLinkMessage::Pointer getImg;
  getImg.reset();
  NiftyLinkImageMessage::Create_GET(getImg);
  if (getImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_IMAGE
  std::cout <<++m_TestCounter <<". Creating STT_IMAGE message..";
  NiftyLinkMessage::Pointer sttImg;
  sttImg.reset();
  NiftyLinkImageMessage::Create_STT(sttImg);
  if (sttImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_IMAGE
  std::cout <<++m_TestCounter <<". Creating STP_IMAGE message..";
  NiftyLinkMessage::Pointer stpImg;
  stpImg.reset();
  NiftyLinkImageMessage::Create_STP(stpImg);
  if (stpImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_IMAGE
  std::cout <<++m_TestCounter <<". Creating RTS_IMAGE message..";
  NiftyLinkMessage::Pointer rtsImg;
  rtsImg.reset();
  NiftyLinkImageMessage::Create_RTS(rtsImg);
  if (rtsImg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************    
  //Create transform message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating transform message..";
  NiftyLinkTransformMessage::Pointer transMsg;
  transMsg.reset();
  transMsg = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage()));
  if (transMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test transform data..";
  transMsg->InitializeWithTestData(dummyTransformMatrix);
  igtl::MessageBase::Pointer q;
  transMsg->GetMessagePointer(q);
  if (q.IsNotNull())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  transMsg->Update(GetLocalHostAddress());
  if (transMsg->GetHostName().isEmpty() || transMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Testing copy constructor..";
  NiftyLinkTransformMessage::Pointer transMsg2;
  transMsg2.reset();
  transMsg2 = (NiftyLinkTransformMessage::Pointer(new NiftyLinkTransformMessage(*transMsg)));

  igtl::Matrix4x4 mat1;
  transMsg->GetMatrix(mat1);
  igtl::Matrix4x4 mat2;
  transMsg2->GetMatrix(mat2);

  bool ba, bb;
  if (transMsg->GetHostName() == transMsg2->GetHostName())
    ba = true;
  else ba = false;
  
  if (memcmp(&mat1, &mat2, sizeof(float)*4*4) == 0)
    bb = true;
  else bb = false;

  if (ba && bb)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  transMsg2.reset();
  transMsg2.operator =(NULL);

  transMsg.reset();
  transMsg.operator =(NULL);

  if (transMsg.operator !=(NULL) ||
     transMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_TestCounter <<". Creating GET_TRANS message..";
  NiftyLinkMessage::Pointer getTrs;
  getTrs.reset();
  NiftyLinkTransformMessage::Create_GET(getTrs);
  if (getTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_TRANS message..";
  NiftyLinkMessage::Pointer sttTrs;
  sttTrs.reset();
  NiftyLinkTransformMessage::Create_STT(sttTrs);
  if (sttTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_TRANS message..";
  NiftyLinkMessage::Pointer stpTrs;
  stpTrs.reset();
  NiftyLinkTransformMessage::Create_STP(stpTrs);
  if (stpTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_TestCounter <<". Creating RTS_TRANS message..";
  NiftyLinkMessage::Pointer rtsTrs;
  rtsTrs.reset();
  NiftyLinkTransformMessage::Create_RTS(rtsTrs);
  if (rtsTrs.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //***********************************************    
  //Create status message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating status message..";
  NiftyLinkStatusMessage::Pointer statusMsg;
  statusMsg.reset();
  statusMsg = (NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage()));
  if (statusMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test status info..";
  statusMsg->InitializeWithTestData();
  igtl::MessageBase::Pointer r;
  statusMsg->GetMessagePointer(r);
  if (r.IsNotNull())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  statusMsg->Update(GetLocalHostAddress());
  if (statusMsg->GetHostName().isEmpty() || statusMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing copy constructor..";
  NiftyLinkStatusMessage::Pointer statusMsg2;
  statusMsg2.reset();
  statusMsg2 = (NiftyLinkStatusMessage::Pointer(new NiftyLinkStatusMessage(*statusMsg)));

  if (statusMsg->GetErrorCode()    == statusMsg2->GetErrorCode() &&
      statusMsg->GetErrorSubCode() == statusMsg2->GetErrorSubCode() &&
      statusMsg->GetErrorName()    == statusMsg2->GetErrorName() &&
      statusMsg->GetStatusString() == statusMsg2->GetStatusString())
        { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  statusMsg2.reset();
  statusMsg2.operator =(NULL);

  statusMsg.reset();
  statusMsg.operator =(NULL);

  if (statusMsg.operator !=(NULL) ||
     statusMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_TestCounter <<". Creating GET_STATUS message..";
  NiftyLinkMessage::Pointer getSta;
  getSta.reset();
  NiftyLinkStatusMessage::Create_GET(getSta);
  if (getSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_STATUS message..";
  NiftyLinkMessage::Pointer sttSta;
  sttSta.reset();
  NiftyLinkStatusMessage::Create_STT(sttSta);
  if (sttSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_STATUS message..";
  NiftyLinkMessage::Pointer stpSta;
  stpSta.reset();
  NiftyLinkStatusMessage::Create_STP(stpSta);
  if (stpSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_TestCounter <<". Creating RTS_STATUS message..";
  NiftyLinkMessage::Pointer rtsSta;
  rtsSta.reset();
  NiftyLinkStatusMessage::Create_RTS(rtsSta);
  if (rtsSta.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //***********************************************    
  //Create string message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating string message..";
  NiftyLinkStringMessage::Pointer stringMsg;
  stringMsg.reset();
  stringMsg = (NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage()));
  if (stringMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test string data..";
  stringMsg->InitializeWithTestData();
  igtl::MessageBase::Pointer s;
  stringMsg->GetMessagePointer(s);
  if (s.IsNotNull())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  stringMsg->Update(GetLocalHostAddress());
  if (stringMsg->GetHostName().isEmpty() || stringMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing copy constructor..";
  NiftyLinkStringMessage::Pointer stringMsg2;
  stringMsg2.reset();
  stringMsg2 = (NiftyLinkStringMessage::Pointer(new NiftyLinkStringMessage(*stringMsg)));

  if (stringMsg->GetHostName() == stringMsg2->GetHostName() &&
      stringMsg->GetString() == stringMsg2->GetString() )
        { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  stringMsg2.reset();
  stringMsg2.operator =(NULL);

  stringMsg.reset();
  stringMsg.operator =(NULL);

  if (stringMsg.operator !=(NULL) ||
     stringMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_TestCounter <<". Creating GET_STRING message..";
  NiftyLinkMessage::Pointer getStr;
  getStr.reset();
  NiftyLinkStringMessage::Create_GET(getStr);
  if (getStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_STRING message..";
  NiftyLinkMessage::Pointer sttStr;
  sttStr.reset();
  NiftyLinkStringMessage::Create_STT(sttStr);
  if (sttStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_STRING message..";
  NiftyLinkMessage::Pointer stpStr;
  stpStr.reset();
  NiftyLinkStringMessage::Create_STP(stpStr);
  if (stpStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_TestCounter <<". Creating RTS_STRING message..";
  NiftyLinkMessage::Pointer rtsStr;
  rtsStr.reset();
  NiftyLinkStringMessage::Create_RTS(rtsStr);
  if (rtsStr.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //***********************************************    
  //Create tdata message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating tdata message..";
  NiftyLinkTrackingDataMessage::Pointer tdataMsg;
  tdataMsg.reset();
  tdataMsg = (NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage()));
  if (tdataMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test tracking data..";
  tdataMsg->InitializeWithTestData(dummyTransformMatrix);
  igtl::MessageBase::Pointer t;
  tdataMsg->GetMessagePointer(t);
  if (t.IsNotNull())
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting timestamp and sender ID..";
  tdataMsg->Update(GetLocalHostAddress());
  if (tdataMsg->GetHostName().isEmpty() || tdataMsg->GetTimeCreated().IsNull())
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing copy constructor..";
  NiftyLinkTrackingDataMessage::Pointer tdataMsg2;
  tdataMsg2.reset();
  tdataMsg2 = (NiftyLinkTrackingDataMessage::Pointer(new NiftyLinkTrackingDataMessage(*tdataMsg)));

  tdataMsg->GetMatrix(mat1);
  tdataMsg2->GetMatrix(mat2);
  
  if (memcmp(&mat1, &mat2, sizeof(float)*4*4) == 0)
    bb = true;
  else bb = false;

  if (tdataMsg->GetHostName() == tdataMsg2->GetHostName() && bb)
        { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_TestCounter <<". Testing delete..";
  tdataMsg2.reset();
  tdataMsg2.operator =(NULL);

  tdataMsg.reset();
  tdataMsg.operator =(NULL);

  if (tdataMsg.operator !=(NULL) ||
     tdataMsg2.operator !=(NULL) )
          std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  //***********************************************
  //Create GET_TRANS
  std::cout <<++m_TestCounter <<". Creating GET_TDATA message..";
  NiftyLinkMessage::Pointer getTdata;
  getTdata.reset();
  NiftyLinkTrackingDataMessage::Create_GET(getTdata);
  if (getTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STT_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_TDATA message..";
  NiftyLinkMessage::Pointer sttTdata;
  sttTdata.reset();
  NiftyLinkTrackingDataMessage::Create_STT(sttTdata);
  if (sttTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create STP_TRANS
  std::cout <<++m_TestCounter <<". Creating STT_TDATA message..";
  NiftyLinkMessage::Pointer stpTdata;
  stpTdata.reset();
  NiftyLinkTrackingDataMessage::Create_STP(stpTdata);
  if (stpTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Create RTS_TRANS
  std::cout <<++m_TestCounter <<". Creating RTS_TDATA message..";
  NiftyLinkMessage::Pointer rtsTdata;
  rtsTdata.reset();
  NiftyLinkTrackingDataMessage::Create_RTS(rtsTdata);
  if (rtsTdata.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  QuitTest();
}

void TestMsgTypes_ConstrDestr::QuitTest()
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
