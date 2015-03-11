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
#include <NiftyLinkMessageContainer.h>

namespace niftk
{

class NiftyLinkTcpServer;
class NiftyLinkTcpClient;

/**
* \class NiftyLinkClientServerTests
* \brief Tests for basic single message client/server sending/receiving.
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
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
   * \brief Generate random TDATA, test Sending and Receive produces identical TDATA.
   *
   * Spec:
   *   - Generate a random TDATA,
   *   - Send it
   *   - Wait 1sec
   *   - Check received message in m_TdataMessage is not null
   *   - Check received message in m_TdataMessage has 1 tracking data element
   *   - Extract matrix
   *   - Check received matrix is close enough to created matrix, using method niftk::NiftyLinkUtils::IsCloseEnoughTo(), tolerance=0.00000001.
   */
  void TestSendReceiveTDATA();


  /**
   * \brief Generate a random TRANSFORM message, basically same spec as for TDATA.
   *
   * Spec:
   *   - Generate a random TRANSFORM,
   *   - Send it
   *   - Wait 1sec
   *   - Check received message in m_TransformMessage is not null
   *   - Extract matrix
   *   - Check received matrix is close enough to created matrix, using method niftk::NiftyLinkUtils::IsCloseEnoughTo(), tolerance=0.00000001.
   */
  void TestSendReceiveTRANSFORM();

  /**
   * \brief Generate IMAGE, test Sending and Receive produces identical QImage.
   *
   * Spec:
   *   - Generate an IMAGE by loading ":/NiftyLink/UCL_LOGO.tif" from resource system,
   *   - Send it
   *   - Wait 1sec
   *   - Check received message in m_ImageMessage is not null
   *   - Extract image
   *   - Check received image exactly equals (using QImage::operator ==) input image.
   */
  void TestSendReceiveIMAGE();

  /// \brief To parse/receive incoming messages.
  void OnReceiveMessage(int, niftk::NiftyLinkMessageContainer::Pointer);

private:

  NiftyLinkTcpServer *m_Server;
  NiftyLinkTcpClient *m_Client;
  NiftyLinkTcpClient *m_ExtraClient; // Extra client to force server into Shutdown method when destructing.

  NiftyLinkMessageContainer::Pointer m_TransformMessage;
  NiftyLinkMessageContainer::Pointer m_TdataMessage;
  NiftyLinkMessageContainer::Pointer m_ImageMessage;

};

} // end namespace

#endif // NiftyLinkClientServerTests_h
