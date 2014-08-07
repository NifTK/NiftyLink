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
void NiftyLinkMessageStatsContainer::ResetAll()
{
  m_TotalBytesReceived = 0;
  m_TotalNumberMessagesReceived = 0;
  this->ResetPeriod();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::ResetPeriod()
{
  m_BytesReceivedBetweemTimingPoints = 0;
  m_NumberMessagesReceivedBetweenTimingPoints = 0;
  m_ListOfLatency.clear();
  m_MapOfMessageCounts.clear();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::DeepCopy(const NiftyLinkMessageStatsContainer& another)
{
  m_TotalBytesReceived = another.m_TotalBytesReceived;
  m_TotalNumberMessagesReceived = another.m_TotalNumberMessagesReceived;
  m_BytesReceivedBetweemTimingPoints = another.m_BytesReceivedBetweemTimingPoints;
  m_NumberMessagesReceivedBetweenTimingPoints = another.m_NumberMessagesReceivedBetweenTimingPoints;
  m_ListOfLatency = another.m_ListOfLatency;
  m_MapOfMessageCounts = another.m_MapOfMessageCounts;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageStatsContainer::Increment(const QString& deviceType, const quint64& numberOfBytes, const quint64& latency)
{
  m_TotalBytesReceived += numberOfBytes;
  m_BytesReceivedBetweemTimingPoints += numberOfBytes;
  m_TotalNumberMessagesReceived += 1;
  m_NumberMessagesReceivedBetweenTimingPoints += 1;
  m_ListOfLatency.push_back(latency);

  if (m_MapOfMessageCounts.contains(deviceType))
  {
    m_MapOfMessageCounts[deviceType]++;
  }
  else
  {
    m_MapOfMessageCounts.insert(deviceType, 1);
  }
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetMeanLatency() const
{
  return CalculateMean(m_ListOfLatency);
}


//-----------------------------------------------------------------------------
double NiftyLinkMessageStatsContainer::GetStdDevLatency() const
{
  return CalculateStdDev(m_ListOfLatency);
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetMaxLatency() const
{
  return CalculateMax(m_ListOfLatency);
}


//-----------------------------------------------------------------------------
QMap< QString, quint64 > NiftyLinkMessageStatsContainer::GetNumberOfMessagesByType() const
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
quint64 NiftyLinkMessageStatsContainer::GetBytesReceivedBetweemTimingPoints() const
{
  return m_BytesReceivedBetweemTimingPoints;
}


//-----------------------------------------------------------------------------
quint64 NiftyLinkMessageStatsContainer::GetNumberMessagesReceivedBetweenTimingPoints()
{
  return m_NumberMessagesReceivedBetweenTimingPoints;
}

} // end namespace niftk
