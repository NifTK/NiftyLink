/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkMessageContainerTests.h"
#include <NiftyLinkMessageCounter.h>
#include <NiftyLinkUtils.h>
#include <NiftyLinkQThread.h>
#include <NiftyLinkTrackingDataMessageHelpers.h>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkMessageContainerTests::SetGetTest()
{
  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  QVERIFY(m->GetOwnerName().size() == 0);
  QVERIFY(m->GetNiftyLinkMessageId() == 0);
  QVERIFY(m->GetLatency() == 0);
  QVERIFY(m->GetMessage().IsNull());
  QVERIFY(m->GetSenderHostName().size() == 0);
  QVERIFY(m->GetSenderPortNumber() == -1);
  QVERIFY(m->GetTimeArrived() == 0);
  QVERIFY(m->GetTimeReceived() == 0);

  m->SetOwnerName("TestOwner");
  m->SetSenderHostName("TestHost");
  m->SetSenderPortNumber(1234);

  igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
  ts->GetTime();
  m->SetTimeArrived(ts);
  m->SetTimeReceived(ts);

  QVERIFY(m->GetLatency() == 0);
  QVERIFY(m->GetMessage().IsNull());
  QVERIFY(m->GetOwnerName() == "TestOwner");
  QVERIFY(m->GetSenderHostName() == "TestHost");
  QVERIFY(m->GetSenderPortNumber() == 1234);
  QVERIFY(m->GetTimeArrived() == ts->GetTimeStampInNanoseconds());
  QVERIFY(m->GetTimeReceived() == ts->GetTimeStampInNanoseconds());
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainerTests::CopyAssignTest()
{
  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetOwnerName("TestOwner");
  m->SetSenderHostName("TestHost");
  m->SetSenderPortNumber(1234);

  igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
  ts->GetTime();
  m->SetTimeArrived(ts);
  m->SetTimeReceived(ts);

  NiftyLinkMessageContainer::Pointer m2 = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  *m2 = *m;

  QVERIFY(m2->GetLatency() == 0);
  QVERIFY(m2->GetMessage().IsNull());
  QVERIFY(m2->GetOwnerName() == "TestOwner");
  QVERIFY(m2->GetSenderHostName() == "TestHost");
  QVERIFY(m2->GetSenderPortNumber() == 1234);
  QVERIFY(m2->GetTimeArrived() == ts->GetTimeStampInNanoseconds());
  QVERIFY(m2->GetTimeReceived() == ts->GetTimeStampInNanoseconds());

  NiftyLinkMessageContainer::Pointer m3= (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer(*m2)));
  QVERIFY(m3->GetLatency() == 0);
  QVERIFY(m3->GetMessage().IsNull());
  QVERIFY(m3->GetOwnerName() == "TestOwner");
  QVERIFY(m3->GetSenderHostName() == "TestHost");
  QVERIFY(m3->GetSenderPortNumber() == 1234);
  QVERIFY(m3->GetTimeArrived() == ts->GetTimeStampInNanoseconds());
  QVERIFY(m3->GetTimeReceived() == ts->GetTimeStampInNanoseconds());
}

} // end namespace niftk

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkMessageContainerTests )
