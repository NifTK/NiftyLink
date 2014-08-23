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
#include <NiftyLinkUtils.h>
#include <NiftyLinkQThread.h>

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
void NiftyLinkMessageCounterTests::StatsContainerCopyAssignmentTest()
{
  NiftyLinkMessageStatsContainer container;
  container.Increment("STRING", 1, 1, 1);
  container.Increment("STATUS", 2, 2, 2);

  NiftyLinkMessageStatsContainer container2(container);
  QVERIFY(container2.GetMeanLatencySinceCheckpoint() == 1.5);
  QVERIFY(niftk::IsCloseEnoughTo(container2.GetStdDevLatencySinceCheckpoint(), 0.707106781));
  QVERIFY(container2.GetMaxLatencySinceCheckpoint() == 2);
  QVERIFY(container2.GetBytesReceivedSinceCheckpoint() == 3);
  QVERIFY(container2.GetNumberMessagesReceivedSinceCheckpoint() == 2);
  QVERIFY(container2.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 2);
  QVERIFY(container2.GetNumberOfMessagesByTypeSinceCheckpoint()["STRING"] == 1);
  QVERIFY(container2.GetNumberOfMessagesByTypeSinceCheckpoint()["STATUS"] == 1);
  QVERIFY(container2.GetTotalBytesReceived() == 3);
  QVERIFY(container2.GetTotalNumberMessagesReceived() == 2);
  QVERIFY(container2.GetDurationSinceLastCheckpoint() == 1);

  NiftyLinkMessageStatsContainer container3;
  QVERIFY(!(container3 == container));

  container3 = container;

  QVERIFY(container3.GetMeanLatencySinceCheckpoint() == 1.5);
  QVERIFY(niftk::IsCloseEnoughTo(container3.GetStdDevLatencySinceCheckpoint(), 0.707106781));
  QVERIFY(container3.GetMaxLatencySinceCheckpoint() == 2);
  QVERIFY(container3.GetBytesReceivedSinceCheckpoint() == 3);
  QVERIFY(container3.GetNumberMessagesReceivedSinceCheckpoint() == 2);
  QVERIFY(container3.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 2);
  QVERIFY(container3.GetNumberOfMessagesByTypeSinceCheckpoint()["STRING"] == 1);
  QVERIFY(container3.GetNumberOfMessagesByTypeSinceCheckpoint()["STATUS"] == 1);
  QVERIFY(container3.GetTotalBytesReceived() == 3);
  QVERIFY(container3.GetTotalNumberMessagesReceived() == 2);
  QVERIFY(container3.GetDurationSinceLastCheckpoint() == 1);

  QVERIFY(container == container2);
  QVERIFY(container == container3);
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounterTests::BasicStatsContainerCountingTest()
{
  NiftyLinkMessageStatsContainer container;
  QVERIFY(container.GetMeanLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetStdDevLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetMaxLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetBytesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberMessagesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 0);
  QVERIFY(container.GetTotalBytesReceived() == 0);
  QVERIFY(container.GetTotalNumberMessagesReceived() == 0);
  QVERIFY(container.GetDurationSinceLastCheckpoint() == 0);

  // When we say "add two messages", you only need information from the messaage
  container.Increment("STRING", 1, 1, 1);
  container.Increment("STATUS", 2, 2, 2);

  QVERIFY(container.GetMeanLatencySinceCheckpoint() == 1.5);
  QVERIFY(container.GetMeanLatencySinceCheckpointInMilliseconds() == container.GetMeanLatencySinceCheckpoint()/static_cast<double>(NiftyLinkMessageStatsContainer::m_NANO_TO_MILLI_DIVISOR));
  QVERIFY(niftk::IsCloseEnoughTo(container.GetStdDevLatencySinceCheckpoint(), 0.707106781));
  QVERIFY(container.GetStdDevLatencySinceCheckpointInMilliseconds() == container.GetStdDevLatencySinceCheckpoint()/static_cast<double>(NiftyLinkMessageStatsContainer::m_NANO_TO_MILLI_DIVISOR));
  QVERIFY(container.GetMaxLatencySinceCheckpointInMilliseconds() == container.GetMaxLatencySinceCheckpoint()/static_cast<double>(NiftyLinkMessageStatsContainer::m_NANO_TO_MILLI_DIVISOR));

  QVERIFY(container.GetBytesReceivedSinceCheckpoint() == 3); // 1+2 from the last two calls to Increment.
  QVERIFY(container.GetNumberMessagesReceivedSinceCheckpoint() == 2);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 2);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint()["STRING"] == 1);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint()["STATUS"] == 1);
  QVERIFY(container.GetTotalBytesReceived() == 3);
  QVERIFY(container.GetTotalNumberMessagesReceived() == 2);
  QVERIFY(container.GetDurationSinceLastCheckpoint() == 1);
  QVERIFY(container.GetDurationSinceLastCheckpointInSeconds() == container.GetDurationSinceLastCheckpoint()/static_cast<double>(NiftyLinkMessageStatsContainer::m_NANO_TO_SECONDS_DIVISOR));

  container.Checkpoint();

  // i.e. reset the variables that are pertaining to a period.
  QVERIFY(container.GetMeanLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetStdDevLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetMaxLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetBytesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberMessagesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 0);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint()["STRING"] == 0);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint()["STATUS"] == 0);
  QVERIFY(container.GetDurationSinceLastCheckpoint() == 0);
  QVERIFY(container.GetTotalBytesReceived() == 3); // i.e dont reset this yet.
  QVERIFY(container.GetTotalNumberMessagesReceived() == 2); // i.e. dont reset this yet.

  container.ResetAll();

  QVERIFY(container.GetMeanLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetStdDevLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetMaxLatencySinceCheckpoint() == 0);
  QVERIFY(container.GetBytesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberMessagesReceivedSinceCheckpoint() == 0);
  QVERIFY(container.GetNumberOfMessagesByTypeSinceCheckpoint().size() == 0);
  QVERIFY(container.GetDurationSinceLastCheckpoint() == 0);
  QVERIFY(container.GetTotalBytesReceived() == 0);
  QVERIFY(container.GetTotalNumberMessagesReceived() == 0);
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounterTests::BasicStatsCounterTest()
{
  NiftyLinkMessageCounter counter;
  QVERIFY(counter.GetNumberMessageReceivedThreshold() == -1); // i.e. initialised to invalid value.
  counter.SetNumberMessageReceivedThreshold(10);
  QVERIFY(counter.GetNumberMessageReceivedThreshold() == 10);
  QVERIFY(counter.GetNumberOfMessagesSinceClear() == 0);
  QVERIFY(counter.GetTotalNumberOfMessages() == 0);

  igtl::TimeStamp::Pointer testTime = igtl::TimeStamp::New();

  NiftyLinkMessageContainer::Pointer message1 = CreateTestTrackingDataMessage(testTime, 1);
  counter.OnMessageReceived(message1);

  QVERIFY(counter.GetTotalNumberOfMessages() == 1);
  QVERIFY(counter.GetNumberOfMessagesSinceClear() == 1);

  NiftyLinkQThread::SleepCallingThread(10);

  NiftyLinkMessageContainer::Pointer message2 = CreateTestTrackingDataMessage(testTime, 1);
  counter.OnMessageReceived(message2);

  QVERIFY(counter.GetTotalNumberOfMessages() == 2);
  QVERIFY(counter.GetNumberOfMessagesSinceClear() == 2);

  NiftyLinkMessageStatsContainer copyofContainer = counter.GetStatsContainer();
  QVERIFY(copyofContainer.GetNumberMessagesReceivedSinceCheckpoint() == 2);
  QVERIFY(copyofContainer.GetTotalNumberMessagesReceived() == 2);

  counter.OnClear();

  QVERIFY(counter.GetTotalNumberOfMessages() == 2);
  QVERIFY(counter.GetNumberOfMessagesSinceClear() == 0);

  // Check if latency < 0, ignored.
  igtl::TimeStamp::Pointer createdTime = igtl::TimeStamp::New();
  igtl::TimeStamp::Pointer receivedTime = igtl::TimeStamp::New();
  message1->GetTimeCreated(createdTime);
  receivedTime->SetTimeInNanoseconds(createdTime->GetTimeStampInNanoseconds() - 1000);
  message1->SetTimeReceived(receivedTime);
  counter.OnMessageReceived(message1);
  QVERIFY(counter.GetTotalNumberOfMessages() == 2);
  QVERIFY(counter.GetNumberOfMessagesSinceClear() == 0);

}

} // end namespace niftk

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkMessageCounterTests )
