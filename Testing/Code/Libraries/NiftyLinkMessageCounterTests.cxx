/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkMessageCounterTests.h"
#include <NiftyLinkMessageCounter.h>
#include <igtlTimeStamp.h>
#include <QDebug>
#include <stdlib.h>

namespace niftk
{


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounterTests::QMapCopyingTest()
{
  QMap<QString, quint64> map1;
  map1.insert("TDATA", 1);
  map1.insert("IMAGE", 2);

  QMap<QString, quint64> map2(map1);
  QVERIFY(map2 == map1);

  QMap<QString, quint64> map3;
  QVERIFY(map3 != map1);

  map3 = map1;
  QVERIFY(map3 == map1);
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounterTests::BasicStatsCountingTestCase()
{
  QVERIFY(1 == 2);
}

} // end namespace niftk

QTEST_MAIN( niftk::NiftyLinkMessageCounterTests )
