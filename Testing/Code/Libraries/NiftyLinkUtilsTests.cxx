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
#include <QDir>
#include <QFile>

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

  igtl::TimeStamp::Pointer timeStamp2 = igtl::TimeStamp::New();

  totalTimeInNanoseconds = timeStamp->GetTimeStampInNanoseconds();
  totalTimeInNanoseconds2 = (igtlUint64)((igtlUint64)seconds * (igtlUint64)1000000000 + (igtlUint64)nanoseconds);

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  timeStamp->SetTimeInNanoseconds(totalTimeInNanoseconds);
  totalTimeInNanoseconds2 = timeStamp->GetTimeStampInNanoseconds();

  QVERIFY(totalTimeInNanoseconds == totalTimeInNanoseconds2);

  timeStamp->GetTimeStamp(&seconds2, &nanoseconds2);

  QVERIFY(seconds == seconds2);
  QVERIFY(nanoseconds == nanoseconds2);

  timeStamp2->SetTime(seconds, nanoseconds + 1);
  totalTimeInNanoseconds2 = timeStamp2->GetTimeStampInNanoseconds();

  QVERIFY(totalTimeInNanoseconds2 - totalTimeInNanoseconds == 1);
  QVERIFY(GetDifferenceInNanoSeconds(timeStamp, timeStamp2) == 1);
  QVERIFY(GetDifferenceInNanoSeconds(timeStamp2, timeStamp) == 1);
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
  QVERIFY(!ValidateIp(QString("onepotatoe.twopotatoe.threepotatoe.four")));
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

  hostName = ResolveHostName("banana");
  QVERIFY(hostName == "UNKNOWN");
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


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CalculateCloseEnoughToMatrixTest()
{
  igtl::Matrix4x4 mat1;
  igtl::Matrix4x4 mat2;

  niftk::CreateRandomTransformMatrix(mat1);
  QVERIFY(IsCloseEnoughTo(mat1,mat1));

  for (unsigned int r = 0; r < 4; r++)
  {
    for (unsigned int c = 0; c < 4; c++)
    {
      mat2[r][c] = mat1[r][c];
    }
  }

  QVERIFY(IsCloseEnoughTo(mat1, mat2));
  mat2[3][3] += 0.000001; // below default tolerance
  QVERIFY(IsCloseEnoughTo(mat1, mat2));
  mat2[3][3] += 0.00001;  // just above default tolerance
  QVERIFY(!IsCloseEnoughTo(mat1, mat2));
  QVERIFY(IsCloseEnoughTo(mat1, mat2, 0.001)); // change tolerance.
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::CopyMatrixTest()
{
  igtl::Matrix4x4 mat1;
  igtl::IdentityMatrix(mat1);

  double testData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  niftk::CopyMatrix(testData, mat1);

  for (unsigned int r = 0; r < 4; r++)
  {
    for (unsigned int c = 0; c < 4; c++)
    {
      QVERIFY(mat1[r][c] == testData[r*4 + c]);
    }
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkUtilsTests::GetWritableDirectoryTest()
{
  QString writablePath1 = niftk::GetWritableDirectoryPath();
  QVERIFY(writablePath1.length() > 0);
  QDir writableDir(writablePath1);
  QVERIFY(writableDir.exists());
  QVERIFY(writableDir.isReadable());
  QVERIFY(writableDir.isAbsolute());
  QString testFileName = "extraFileName";
  QString writablePath2 = niftk::GetWritableDirectoryPath(testFileName);
  QFile fileDesc(writablePath2);
  QVERIFY(!fileDesc.exists());
  QVERIFY(!fileDesc.isWritable()); // as doesn't exist.
  QVERIFY(writablePath2 == writablePath1 + QDir::separator() + testFileName);
}

} // end namespace

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkUtilsTests )
