/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkUtilsTests.h"
#include <NiftyLinkUtils.h>
#include <igtlTimeStamp.h>
#include <QDebug>
#include <stdlib.h>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::GetDifferenceInNanoSecondsTest()
{
  igtlUint32 seconds, nanoseconds;
  igtlUint32 seconds2, nanoseconds2;
  igtlUint64 totalTimeInNanoseconds;
  igtlUint64 totalTimeInNanoseconds2;

  igtl::TimeStamp::Pointer timeStamp = igtl::TimeStamp::New();
  timeStamp->GetTimeStamp(&seconds, &nanoseconds);

  totalTimeInNanoseconds = timeStamp->GetTimeStampInNanoseconds();
  totalTimeInNanoseconds2 = (igtlUint64)((igtlUint64)seconds * (igtlUint64)1000000000 + (igtlUint64)nanoseconds);

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  timeStamp->SetTimeInNanoseconds(totalTimeInNanoseconds);
  totalTimeInNanoseconds2 = timeStamp->GetTimeStampInNanoseconds();

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  timeStamp->GetTimeStamp(&seconds2, &nanoseconds2);

  QVERIFY(seconds == seconds2);
  QVERIFY(nanoseconds == nanoseconds2);

  timeStamp->SetTime(seconds, nanoseconds + 1);
  totalTimeInNanoseconds2 = timeStamp->GetTimeStampInNanoseconds();

  QVERIFY(totalTimeInNanoseconds2 - totalTimeInNanoseconds == 1);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::ValidateIpTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::GetLocalHostAddressTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::ResolveHostNameTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateMeanTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateStdDevTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateMaxTest()
{
  // replace with a valid test.
  QVERIFY(1==2);
}

} // end namespace

QTEST_MAIN( niftk::NiftyLinkUtilsTests )
