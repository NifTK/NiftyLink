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
#include <NiftyLinkUtils.h>
#include <NiftyLinkImageMessageHelpers.h>
#include <NiftyLinkTrackingDataMessageHelpers.h>
#include <NiftyLinkTransformMessageHelpers.h>

#include <igtlTrackingDataMessage.h>
#include <igtlTransformMessage.h>
#include <igtlImageMessage.h>

#include <cassert>

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

  m_ExtraClient = new NiftyLinkTcpClient();
  m_ExtraClient->ConnectToHost("localhost", port);

  QTest::qWait(1000);

  connect(m_Server, SIGNAL(MessageReceived(int,NiftyLinkMessageContainer::Pointer)),
          this, SLOT(OnReceiveMessage(int,NiftyLinkMessageContainer::Pointer)));

  m_Server->SetCheckForNoIncomingData(true);
  m_Server->SetKeepAliveOn(true);
  m_Client->SetCheckForNoIncomingData(true);
  m_Client->SetKeepAliveOn(true);
  m_ExtraClient->SetCheckForNoIncomingData(true);
  m_ExtraClient->SetKeepAliveOn(true);
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::cleanupTestCase()
{
  // Not really a test ... just to force the code to be used.
  m_Client->RequestStats();
  m_Server->OutputStats();

  delete m_Client;

  // Not strictly necessary, but makes the log-file easier to read :-)
  QTest::qWait(1000);

  delete m_Server;

  // Not strictly necessary, but makes the log-file easier to read :-)
  QTest::qWait(1000);

  delete m_ExtraClient;
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::OnReceiveMessage(int /*portNumber*/, NiftyLinkMessageContainer::Pointer message)
{
  QLOG_INFO() << "OnReceiveMessage";

  assert(message.data() != NULL);

  if (dynamic_cast<igtl::ImageMessage*>(message.data()->GetMessage().GetPointer()) != NULL)
  {
    QLOG_INFO() << "Received IMAGE";
    m_ImageMessage = message;
  }
  else if (dynamic_cast<igtl::TrackingDataMessage*>(message.data()->GetMessage().GetPointer()) != NULL)
  {
    QLOG_INFO() << "Received TDATA";
    m_TdataMessage = message;
  }
  else if (dynamic_cast<igtl::TransformMessage*>(message.data()->GetMessage().GetPointer()) != NULL)
  {
    QLOG_INFO() << "Received TRANSFORM";
    m_TransformMessage = message;
  }
  else
  {
    // Invalid message.
    assert(false);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::TestSendReceiveTDATA()
{
  NiftyLinkMessageContainer::Pointer msg = CreateTrackingDataMessageWithRandomData();
  m_Client->Send(msg);
  QLOG_INFO() << "Sent TDATA";

  QTest::qWait(100);

  QVERIFY(m_TdataMessage.data() != NULL);

  igtl::TrackingDataMessage::Pointer expectedMessage = dynamic_cast<igtl::TrackingDataMessage*>(msg.data()->GetMessage().GetPointer());
  QVERIFY(expectedMessage.IsNotNull());
  QVERIFY(expectedMessage->GetNumberOfTrackingDataElements() == 1);

  igtl::TrackingDataMessage::Pointer actualMessage = dynamic_cast<igtl::TrackingDataMessage*>(m_TdataMessage->GetMessage().GetPointer());
  QVERIFY(actualMessage.IsNotNull());
  QVERIFY(actualMessage->GetNumberOfTrackingDataElements() == 1);

  igtl::TrackingDataElement::Pointer expectedElem = igtl::TrackingDataElement::New();
  igtl::TrackingDataElement::Pointer actualElem = igtl::TrackingDataElement::New();

  expectedMessage->GetTrackingDataElement(0, expectedElem);
  actualMessage->GetTrackingDataElement(0, actualElem);

  igtl::Matrix4x4 expectedMatrix;
  expectedElem->GetMatrix(expectedMatrix);

  igtl::Matrix4x4 actualMatrix;
  actualElem->GetMatrix(actualMatrix);

  QLOG_INFO() << "Checking:\n" << GetMatrixAsString(actualMessage, 0);
  QVERIFY(IsCloseEnoughTo(expectedMatrix, actualMatrix, 0.00000001));
}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::TestSendReceiveTRANSFORM()
{
  NiftyLinkMessageContainer::Pointer msg = CreateTransformMessageWithRandomData();
  m_Client->Send(msg);
  QLOG_INFO() << "Sent TDATA";

  QTest::qWait(100);

  QVERIFY(m_TransformMessage.data() != NULL);

  igtl::TransformMessage::Pointer expectedMessage = dynamic_cast<igtl::TransformMessage*>(msg.data()->GetMessage().GetPointer());
  QVERIFY(expectedMessage.IsNotNull());

  igtl::TransformMessage::Pointer actualMessage = dynamic_cast<igtl::TransformMessage*>(m_TransformMessage->GetMessage().GetPointer());
  QVERIFY(actualMessage.IsNotNull());

  igtl::Matrix4x4 expectedMatrix;
  expectedMessage->GetMatrix(expectedMatrix);

  igtl::Matrix4x4 actualMatrix;
  actualMessage->GetMatrix(actualMatrix);

  QLOG_INFO() << "Checking:\n" << GetMatrixAsString(actualMessage);
  QVERIFY(IsCloseEnoughTo(expectedMatrix, actualMatrix, 0.00000001));

}


//-----------------------------------------------------------------------------
void NiftyLinkClientServerTests::TestSendReceiveIMAGE()
{
  QImage i1(":/NiftyLink/UCL_LOGO.tif");
  QImage i2;

  NiftyLinkMessageContainer::Pointer msg = CreateImageMessage("TestingDevice", "TestingHost", 1234, i1);

  m_Client->Send(msg);
  QLOG_INFO() << "Sent IMAGE";

  QTest::qWait(100);

  QVERIFY(m_ImageMessage.data() != NULL);

  igtl::ImageMessage::Pointer expectedMessage = dynamic_cast<igtl::ImageMessage*>(msg.data()->GetMessage().GetPointer());
  QVERIFY(expectedMessage.IsNotNull());

  igtl::ImageMessage::Pointer actualMessage = dynamic_cast<igtl::ImageMessage*>(m_ImageMessage->GetMessage().GetPointer());
  QVERIFY(actualMessage.IsNotNull());

  GetQImage(actualMessage, i2);

  QVERIFY(i1 == i2);
}


} // end namespace

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkClientServerTests )
