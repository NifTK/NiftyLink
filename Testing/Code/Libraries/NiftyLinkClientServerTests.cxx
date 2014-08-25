/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkClientServerTests.h"
#include <NiftyLinkTcpClient.h>
#include <NiftyLinkTcpServer.h>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::initTestCase()
{
  int port = 18945;
  m_Server = new NiftyLinkTcpServer();
  QVERIFY(m_Server->listen(QHostAddress::Any, port));

  QTest::qWait(1000);

  m_Client = new NiftyLinkTcpClient();
  m_Client->ConnectToHost("localhost", port);

  QTest::qWait(1000);

  QVERIFY(m_Client->IsConnected());
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::cleanupTestCase()
{
  delete m_Client;

  QTest::qWait(1000);

  delete m_Server;
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::TestSendReceiveTDATA()
{

}

} // end namespace

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkClientServerTests )
