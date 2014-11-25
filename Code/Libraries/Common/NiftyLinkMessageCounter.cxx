/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkMessageCounter.h"
#include "NiftyLinkUtils.h"

#include <QsLog.h>

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageCounter::NiftyLinkMessageCounter(QObject *parent)
: m_TimeStamp(NULL)
, m_NumberMessageReceivedThreshold(-1)
{
  this->setObjectName("NiftyLinkMessageCounter");

  // These objects are expensive to create, create them up-front and re-use them.
  m_TimeStamp = igtl::TimeStamp::New();
  m_TimeStamp->GetTime();
}


//-----------------------------------------------------------------------------
NiftyLinkMessageCounter::~NiftyLinkMessageCounter()
{
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  m_NumberMessageReceivedThreshold = threshold;
}


//-----------------------------------------------------------------------------
qint64 NiftyLinkMessageCounter::GetNumberMessageReceivedThreshold() const
{
  return m_NumberMessageReceivedThreshold;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageCounter::GetTotalNumberOfMessages()
{
  return m_StatsContainer.GetTotalNumberMessagesReceived();
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageCounter::GetNumberOfMessagesSinceClear()
{
  return m_StatsContainer.GetNumberMessagesReceivedSinceCheckpoint();
}


//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer NiftyLinkMessageCounter::GetStatsContainer()
{
  return m_StatsContainer;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnClear()
{
  m_StatsContainer.Checkpoint();
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageCounter::GetMessagesPerSecond()
{
  return m_StatsContainer.GetMessagesPerSecondSinceLastCheckpoint();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnMessageReceived(NiftyLinkMessageContainer::Pointer& message)
{
  assert(message.data() != NULL);
  assert(message->GetMessage().IsNotNull());

  message->GetTimeCreated(m_TimeStamp);

  igtlInt64 latency = message->GetLatency();
  igtlUint64 timeStamp = m_TimeStamp->GetTimeStampInNanoseconds();

  if (latency >= 0)
  {
    m_StatsContainer.Increment(
          message->GetMessage()->GetDeviceType(),
          timeStamp,
          message->GetMessage()->GetPackSize(),
          latency
          );
  }
  else
  {
    QLOG_DEBUG() << QObject::tr("%1:OnMessageReceived() - negative latency detected, not counting message");
  }

  // We do this to output stats periodically.
  // So, if m_NumberMessageReceivedThreshold == 100, you get stats to console every 100 messages.
  if (m_NumberMessageReceivedThreshold > 1
      && (m_StatsContainer.GetTotalNumberMessagesReceived() % m_NumberMessageReceivedThreshold == 0))
  {
    this->OnOutputStats();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnOutputStats()
{
  QString outputString = m_StatsContainer.GetStatsMessage();

  // Note: In unit tests, this string will not be visible, as logger is uninitialised.
  QLOG_INFO() << outputString;

  emit StatsProduced(m_StatsContainer); // and this is why we need copy semantics.
  emit StatsMessageProduced(outputString);

  // Reset, every time we print
  this->OnClear();
}

} // end namespace niftk
