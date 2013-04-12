/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkUtilsTests.h"
#include "stdlib.h"
#include <QDebug>
#include "NiftyLinkUtils.h"

void UtilsTests::InitTestCase()
{
}

void UtilsTests::CleanupTestCase()
{
}

void UtilsTests::TimeStampSetGetDifferenceTest()
{
  igtlUint32 seconds, nanoseconds;
  igtlUint32 seconds2, nanoseconds2;
  igtlUint64 totalTimeInNanoseconds;
  igtlUint64 totalTimeInNanoseconds2;

  igtl::TimeStamp::Pointer timeStamp = igtl::TimeStamp::New();
  timeStamp->GetTime(&seconds, &nanoseconds);

  totalTimeInNanoseconds = GetTimeInNanoSeconds(timeStamp);
  totalTimeInNanoseconds2 = (igtlUint64)((igtlUint64)seconds * (igtlUint64)1000000000 + (igtlUint64)nanoseconds);

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  SetTimeInNanoSeconds(timeStamp, totalTimeInNanoseconds);
  totalTimeInNanoseconds2 = GetTimeInNanoSeconds(timeStamp);

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  timeStamp->GetTime(&seconds2, &nanoseconds2);

  QVERIFY(seconds == seconds2);
  QVERIFY(nanoseconds == nanoseconds2);

  timeStamp->SetTime(seconds, nanoseconds + 1);
  totalTimeInNanoseconds2 = GetTimeInNanoSeconds(timeStamp);

  QVERIFY(totalTimeInNanoseconds2 - totalTimeInNanoseconds == 1);
}

QTEST_MAIN( UtilsTests )
