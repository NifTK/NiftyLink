/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkMessageStatsContainer.h"
#include "NiftyLinkUtils.h"
#include <QStringList>

namespace niftk
{

const double NiftyLinkMessageStatsContainer::NANO_TO_MILLI_DIVISOR(1000000);
const double NiftyLinkMessageStatsContainer::NANO_TO_SECONDS_DIVISOR(1000000000);

//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer::NiftyLinkMessageStatsContainer()
{
  this->ResetAll();
}


//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer::~NiftyLinkMessageStatsContainer()
{
}


//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer::NiftyLinkMessageStatsContainer(const NiftyLinkMessageStatsContainer& another)
{
  this->DeepCopy(another);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageStatsContainer& NiftyLinkMessageStatsContainer::operator=(const NiftyLinkMessageStatsContainer& another)
{
  this->DeepCopy(another);
  return *this;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::DeepCopy(const NiftyLinkMessageStatsContainer& another)
{
  m_StartTimeStampInNanoseconds = another.m_StartTimeStampInNanoseconds;
  m_EndTimeStampInNanoseconds = another.m_EndTimeStampInNanoseconds;
  m_TotalBytesReceived = another.m_TotalBytesReceived;
  m_TotalNumberMessagesReceived = another.m_TotalNumberMessagesReceived;
  m_BytesReceivedBetweenCheckPoints = another.m_BytesReceivedBetweenCheckPoints;
  m_NumberMessagesReceivedBetweenCheckPoints = another.m_NumberMessagesReceivedBetweenCheckPoints;
  m_ListOfLatency = another.m_ListOfLatency;
  m_MapOfMessageCounts = another.m_MapOfMessageCounts;
}


//-----------------------------------------------------------------------------
bool NiftyLinkMessageStatsContainer::operator==(const NiftyLinkMessageStatsContainer& another) const
{
  if (this == &another) return true;

  bool doesEqual = false;

  // These should be kept in the order of "most likely to fail".
  if (   m_TotalBytesReceived == another.m_TotalBytesReceived
      && m_TotalNumberMessagesReceived == another.m_TotalNumberMessagesReceived
      && m_StartTimeStampInNanoseconds == another.m_StartTimeStampInNanoseconds
      && m_EndTimeStampInNanoseconds == another.m_EndTimeStampInNanoseconds
      && m_BytesReceivedBetweenCheckPoints == another.m_BytesReceivedBetweenCheckPoints
      && m_NumberMessagesReceivedBetweenCheckPoints == another.m_NumberMessagesReceivedBetweenCheckPoints
      && m_ListOfLatency == another.m_ListOfLatency
      && m_MapOfMessageCounts == another.m_MapOfMessageCounts
      )
  {
    doesEqual = true;
  }

  return doesEqual;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::ResetAll()
{
  this->Checkpoint();
  m_TotalBytesReceived = 0;
  m_TotalNumberMessagesReceived = 0;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::Checkpoint()
{
  m_BytesReceivedBetweenCheckPoints = 0;
  m_NumberMessagesReceivedBetweenCheckPoints = 0;
  m_ListOfLatency.clear();
  m_MapOfMessageCounts.clear();
  m_StartTimeStampInNanoseconds = 0;
  m_EndTimeStampInNanoseconds = 0;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::Increment(const QString& deviceType,
    const quint64& messageTimeInNanoseconds,
    const quint64& numberOfBytes,
    const quint64& latency)
{
  // This is so that if there is a large period of inactivity before we read
  // the first message, then it doesn't affect the stats so much.
  if (m_NumberMessagesReceivedBetweenCheckPoints == 0)
  {
    m_StartTimeStampInNanoseconds = messageTimeInNanoseconds;
  }

  m_TotalBytesReceived += numberOfBytes;
  m_BytesReceivedBetweenCheckPoints += numberOfBytes;

  m_TotalNumberMessagesReceived += 1;
  m_NumberMessagesReceivedBetweenCheckPoints += 1;

  m_ListOfLatency.push_back(latency);

  if (m_MapOfMessageCounts.contains(deviceType))
  {
    m_MapOfMessageCounts[deviceType]++;
  }
  else
  {
    m_MapOfMessageCounts.insert(deviceType, 1);
  }

  m_EndTimeStampInNanoseconds = messageTimeInNanoseconds;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMeanLatencySinceCheckpoint() const
{
  return CalculateMean(m_ListOfLatency);
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMeanLatencySinceCheckpointInMilliseconds() const
{
  return this->GetMeanLatencySinceCheckpoint()/NANO_TO_MILLI_DIVISOR;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetStdDevLatencySinceCheckpoint() const
{
  return CalculateStdDev(m_ListOfLatency);
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetStdDevLatencySinceCheckpointInMilliseconds() const
{
  return this->GetStdDevLatencySinceCheckpoint()/NANO_TO_MILLI_DIVISOR;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMaxLatencySinceCheckpoint() const
{
  return static_cast<double>(CalculateMax(m_ListOfLatency));
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMaxLatencySinceCheckpointInMilliseconds() const
{
  return this->GetMaxLatencySinceCheckpoint()/NANO_TO_MILLI_DIVISOR;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetDurationSinceLastCheckpoint() const
{
  double diff = static_cast<double>(m_EndTimeStampInNanoseconds) - static_cast<double>(m_StartTimeStampInNanoseconds);
  return diff;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetDurationSinceLastCheckpointInSeconds() const
{
  return this->GetDurationSinceLastCheckpoint()/NANO_TO_SECONDS_DIVISOR;
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMessagesPerSecondSinceLastCheckpoint() const
{
  double timeDifference = this->GetDurationSinceLastCheckpointInSeconds();
  if (timeDifference == 0)
  {
    return 0;
  }
  else
  {
    return this->GetNumberMessagesReceivedSinceCheckpoint()/timeDifference;
  }
}


//-----------------------------------------------------------------------------
QMap< QString, quint64 > NiftyLinkMessageStatsContainer::GetNumberOfMessagesByTypeSinceCheckpoint() const
{
  return m_MapOfMessageCounts;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetTotalBytesReceived() const
{
  return m_TotalBytesReceived;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetTotalNumberMessagesReceived() const
{
  return m_TotalNumberMessagesReceived;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetBytesReceivedSinceCheckpoint() const
{
  return m_BytesReceivedBetweenCheckPoints;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetNumberMessagesReceivedSinceCheckpoint() const
{
  return m_NumberMessagesReceivedBetweenCheckPoints;
}


//-----------------------------------------------------------------------------
QString NiftyLinkMessageStatsContainer::GetStatsMessage() const
{
  double durationInSeconds = this->GetDurationSinceLastCheckpointInSeconds();
  double rate = this->GetMessagesPerSecondSinceLastCheckpoint();
  double mean = this->GetMeanLatencySinceCheckpointInMilliseconds();
  double stdDev = this->GetStdDevLatencySinceCheckpointInMilliseconds();
  double max = this->GetMaxLatencySinceCheckpointInMilliseconds();

  QString outputString = QObject::tr("GetStatsMessage() - Received %1 msgs, %2 bytes, in %3 secs, %4 b/sec, mean %5, std %6, max %7: ")
      .arg(this->GetNumberMessagesReceivedSinceCheckpoint())
      .arg(this->GetBytesReceivedSinceCheckpoint())
      .arg(durationInSeconds)
      .arg(rate)
      .arg(mean)
      .arg(stdDev)
      .arg(max);

  QMap< QString, quint64>::const_iterator i = m_MapOfMessageCounts.constBegin();
  while (i != m_MapOfMessageCounts.constEnd())
  {
    QString deviceType = i.key();
    outputString.append(QObject::tr("%1(%2), ").arg(deviceType).arg(i.value()));
    ++i;
  }

  return outputString;
}

} // end namespace niftk
