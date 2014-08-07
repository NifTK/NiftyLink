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

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageCounter::NiftyLinkMessageCounter(QObject *parent)
: m_StatsStartPoint(NULL)
, m_StatsEndPoint(NULL)
, m_NumberMessageReceivedThreshold(-1)
{
  this->setObjectName("NiftyLinkMessageCounter");
  m_StatsStartPoint = igtl::TimeStamp::New();
  m_StatsStartPoint->GetTime();
  m_StatsEndPoint = igtl::TimeStamp::New();
  m_StatsEndPoint->SetTimeInNanoseconds(m_StatsStartPoint->GetTimeStampInNanoseconds());
}


//-----------------------------------------------------------------------------
NiftyLinkMessageCounter::~NiftyLinkMessageCounter()
{
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  this->m_NumberMessageReceivedThreshold = threshold;
}


//-----------------------------------------------------------------------------
qint64 NiftyLinkMessageCounter::GetNumberMessageReceivedThreshold() const
{
  return m_NumberMessageReceivedThreshold;
}


//-----------------------------------------------------------------------------
qint64 NiftyLinkMessageCounter::GetTotalNumberOfMessages()
{
  return m_StatsContainer.GetTotalNumberMessagesReceived();
}


//-----------------------------------------------------------------------------
qint64 NiftyLinkMessageCounter::GetNumberOfMessagesSinceClear()
{
  return m_StatsContainer.GetNumberMessagesReceivedBetweenTimingPoints();
}


//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer NiftyLinkMessageCounter::GetStatsContainer()
{
  return m_StatsContainer;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageCounter::GetMessagesPerSecond()
{
  qint64 diff = GetDifferenceInNanoSeconds(m_StatsEndPoint, m_StatsStartPoint);
  if (diff == 0)
  {
    return 0;
  }
  else
  {
    return static_cast<double>(this->GetNumberOfMessagesSinceClear()) /
          (static_cast<double>(diff)/static_cast<double>(1000000000));
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnMessageReceived(NiftyLinkMessageContainer::Pointer& message)
{
  // This is so that if there is a large period of inactivity before we read
  // the first message, then it doesn't affect the stats so much.
  if (m_StatsContainer.GetTotalBytesReceived() == 0)
  {
     m_StatsStartPoint->GetTime();
  }

  // Increment container.
  // Latency however may be negative, if clocks differ.
  igtlInt64 latency = message->GetLatency();
  if (latency > 0)
  {
    m_StatsContainer.Increment(
          message->GetMessage()->GetDeviceType(),
          message->GetMessage()->GetPackSize(),
          latency
          );
  }
  else
  {
    QLOG_DEBUG() << QObject::tr("%1:OnMessageReceived() - negative latency detected, not counting message");
  }

  // Update this for each call, so we are measuring the end-point of a given period.
  m_StatsEndPoint->GetTime();

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
  igtlUint64 duration = niftk::GetDifferenceInNanoSeconds(m_StatsEndPoint, m_StatsStartPoint);
  double durationInSeconds = duration/static_cast<double>(1000000000);
  double rate = m_StatsContainer.GetBytesReceivedBetweemTimingPoints()/durationInSeconds;
  double mean = m_StatsContainer.GetMeanLatency() / static_cast<double>(1000000);     // Convert nano to milli-secs
  double stdDev = m_StatsContainer.GetStdDevLatency() / static_cast<double>(1000000); // Convert nano to milli-secs
  double max = m_StatsContainer.GetMaxLatency() / static_cast<double>(1000000);       // Convert nano to milli-secs

  QString outputString = QObject::tr("%1::OnOutputStats() - Received %2 msgs, %3 bytes, in %4 secs, %5 b/sec, mean %6, std %7, max %8: ")
      .arg(objectName())
      .arg(m_StatsContainer.GetNumberMessagesReceivedBetweenTimingPoints())
      .arg(m_StatsContainer.GetBytesReceivedBetweemTimingPoints())
      .arg(durationInSeconds)
      .arg(rate)
      .arg(mean)
      .arg(stdDev)
      .arg(max);

  QMap< QString, quint64> messageCounts = m_StatsContainer.GetNumberOfMessagesByType();
  QMap< QString, quint64>::const_iterator i = messageCounts.constBegin();
  while (i != messageCounts.constEnd())
  {
    QString deviceType = i.key();
    outputString.append(QObject::tr("%1(%2), ").arg(deviceType).arg(i.value()));
    ++i;
  }

  QLOG_INFO() << outputString;

  emit StatsProduced(m_StatsContainer); // and this is why we need copy semantics.
  emit StatsMessageProduced(outputString);

  // Reset, every time we print
  this->OnClear();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnClear()
{
  m_StatsStartPoint->GetTime();
  m_StatsEndPoint->SetTimeInNanoseconds(m_StatsStartPoint->GetTimeStampInNanoseconds());
  m_StatsContainer.ResetPeriod();
}

} // end namespace niftk
