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

#include "UtilsTests.h"
#include "stdlib.h"
#include <QDebug>
#include "Utils.h"

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
  int result = DummyFunction1();
  QVERIFY(result == 0);
}

QTEST_MAIN( UtilsTests )
