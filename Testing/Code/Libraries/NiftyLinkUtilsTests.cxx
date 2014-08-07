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
  QVERIFY(!ValidateIp(QString("")));
  QVERIFY(!ValidateIp(QString(".")));
  QVERIFY(!ValidateIp(QString(".1")));
  QVERIFY(!ValidateIp(QString("1.")));
  QVERIFY(!ValidateIp(QString("1.1")));
  QVERIFY(!ValidateIp(QString(".1.1.1.1")));
  QVERIFY(!ValidateIp(QString("1.1.1.1.")));
  QVERIFY(!ValidateIp(QString("1.1.1.1.1")));
  QVERIFY(ValidateIp(QString("1.1.1.1")));
  QVERIFY(ValidateIp(QString("127.0.0.1")));
  QVERIFY(ValidateIp(QString("0.0.0.0")));
  QVERIFY(ValidateIp(QString("255.255.255.255")));
  QVERIFY(!ValidateIp(QString("-1.255.255.255")));
  QVERIFY(!ValidateIp(QString("255.255.255.256")));
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::GetLocalHostAddressTest()
{
  QString result = GetLocalHostAddress();
  QVERIFY(result == "UNKNOWN" || ValidateIp(result));
  QVERIFY(result != "127.0.0.1");
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::ResolveHostNameTest()
{
  QString ip = GetLocalHostAddress();
  QString hostName = ResolveHostName(ip);
  QVERIFY(hostName == "UNKNOWN" || hostName.length() > 0);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateMeanTest()
{
  QList<igtlUint64> list;
  QVERIFY(CalculateMean(list) == 0);
  list.push_back(1);
  QVERIFY(CalculateMean(list) == 1);
  list.push_back(2);
  QVERIFY(CalculateMean(list) == 1.5);
  list.push_back(3);
  QVERIFY(CalculateMean(list) == 2);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateStdDevTest()
{
  QList<igtlUint64> list;
  QVERIFY(CalculateStdDev(list) == 0);
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  QVERIFY(IsCloseEnoughTo(CalculateStdDev(list), 1.290994449));
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateMaxTest()
{
  QList<igtlUint64> list;
  QVERIFY(CalculateMax(list) == 0);
  list.push_back(1);
  QVERIFY(CalculateMax(list) == 1);
  list.push_back(2);
  list.push_back(4);
  list.push_back(3);
  QVERIFY(CalculateMax(list) == 4);
  list.push_back(5);
  QVERIFY(CalculateMax(list) == 5);
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateCloseEnoughToTest()
{
  QVERIFY(IsCloseEnoughTo(0, 0));
  QVERIFY(!IsCloseEnoughTo(1, 0, 1));
  QVERIFY(IsCloseEnoughTo(1, 0, 1.001));
  QVERIFY(!IsCloseEnoughTo(-1, 0, 1));
  QVERIFY(IsCloseEnoughTo(-1, 0, 1.001));
  QVERIFY(IsCloseEnoughToZero(0));
  QVERIFY(!IsCloseEnoughToZero(1,1));
  QVERIFY(!IsCloseEnoughToZero(-1,1));
  QVERIFY(IsCloseEnoughToZero(-1,1.001));
}


} // end namespace

QTEST_MAIN( niftk::NiftyLinkUtilsTests )
