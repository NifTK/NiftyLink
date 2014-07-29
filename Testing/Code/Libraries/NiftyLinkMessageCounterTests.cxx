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
void NiftyLinkMessageCounterTests::BasicCountingTestCase()
{
  // Note, we need to change the class/API to enable testing.
  QVERIFY(1 == 2);
}

} // end namespace niftk

QTEST_MAIN( niftk::NiftyLinkMessageCounterTests )
