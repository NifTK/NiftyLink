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

void UtilsTests::initTestCase()
{
  qDebug("called before everything else");
}

void UtilsTests::cleanupTestCase()
{
  qDebug("called after myFirstTest and mySecondTest");
}

void UtilsTests::myFirstTest()
{
  QVERIFY(1 == 1);
}

void UtilsTests::mySecondTest()
{
  QVERIFY(1 != 2);
}

void UtilsTests::myThirdTest()
{
  int result = NiftyLinkDummyFunction1();
  QVERIFY(result == 0);
}

QTEST_MAIN( UtilsTests )
