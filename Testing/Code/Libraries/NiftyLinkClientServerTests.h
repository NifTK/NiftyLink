/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkClientServerTests_h
#define NiftyLinkClientServerTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

class NiftyLinkTcpServer;
class NiftyLinkTcpClient;

/**
* \class NiftyLinkClientServerTests
* \brief Tests for basic single message client/server sending/receiving.
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a>
* framework. The test harness also provides documentary evidence of the specification.
* PLEASE keep comments in this class (a) correct, (b) concise and (c) complete.
*/
class NiftyLinkClientServerTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief To startup a client and server.
   */
  void initTestCase();

  /**
   * \brief To tear down a client and server.
   */
  void cleanupTestCase();

  /**
   * \brief Generate random TDATA, test Sending and Receive and identical TDATA.
   *
   * Spec:
   *   - Generate a random TDATA, send it, check it arrives identically.
   */
  void TestSendReceiveTDATA();

private:

  NiftyLinkTcpServer *m_Server;
  NiftyLinkTcpClient *m_Client;
};

} // end namespace

#endif // NiftyLinkClientServerTests_h
