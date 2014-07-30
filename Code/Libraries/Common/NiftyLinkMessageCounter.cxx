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
, m_TotalBytesReceived(0)
, m_NumberMessagesReceived(0)
, m_NumberMessageReceivedThreshold(-1)
{
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
qint64 NiftyLinkMessageCounter::GetNumberOfMessages()
{
  return m_NumberMessagesReceived;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnClear()
{
  m_StatsStartPoint->GetTime();
  m_StatsEndPoint->SetTimeInNanoseconds(m_StatsStartPoint->GetTimeStampInNanoseconds());
  m_TotalBytesReceived = 0;
  m_NumberMessagesReceived = 0;
  m_ListsOfLatenciesByDeviceType.clear();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnOutputStats()
{
  // Output stats on a per message type basic.
  QMap< QString, QList<quint64> >::const_iterator i = m_ListsOfLatenciesByDeviceType.constBegin();
  while (i != m_ListsOfLatenciesByDeviceType.constEnd())
  {
    QString deviceType = i.key();
    double mean = niftk::CalculateMean(i.value()) / static_cast<double>(1000000);
    double stdDev = niftk::CalculateStdDev(i.value()) / static_cast<double>(1000000);
    double max = niftk::CalculateMax(i.value()) / static_cast<double>(1000000);

    igtlUint64 duration = niftk::GetDifferenceInNanoSeconds(m_StatsEndPoint, m_StatsStartPoint);
    double durationInSeconds = duration/static_cast<double>(1000000000);
    double rate = m_TotalBytesReceived/durationInSeconds;

    QLOG_INFO() << QObject::tr("%1::OnOutputStats(%2) - Received %3 msgs, %4 bytes, in %5 secs, %6 b/sec, mean %7, std %8, max %9.").arg(objectName()).arg(deviceType).arg(m_NumberMessagesReceived).arg(m_TotalBytesReceived).arg(durationInSeconds).arg(rate).arg(mean).arg(stdDev).arg(max);
    ++i;
  }

  // Reset, every time we print
  this->OnClear();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageCounter::OnMessageReceived(NiftyLinkMessageContainer::Pointer& message)
{
  if (m_NumberMessagesReceived == 0)
  {
     m_StatsStartPoint->GetTime();
  }

  m_NumberMessagesReceived++;
  m_TotalBytesReceived += message->GetMessage()->GetPackSize();
  m_StatsEndPoint->GetTime();

  // In OpenIGTLink paper (Tokuda 2009), Latency is defined as the difference
  // between last byte received and first byte sent.
  igtlUint64 latency = message->GetLatency();
  QString deviceType(message->GetMessage()->GetDeviceType());

  // Create map entry first.
  if (!m_ListsOfLatenciesByDeviceType.contains(deviceType))
  {
    m_ListsOfLatenciesByDeviceType.insert(deviceType, QList<quint64>());
  }

  // Then insert latency value.
  QMap< QString, QList<quint64> >::iterator i = m_ListsOfLatenciesByDeviceType.find(deviceType);
  if (i != m_ListsOfLatenciesByDeviceType.end())
  {
    i.value().append(latency);
  }

  // Do this to get stats based on a fixed number of message counts.
  if (m_NumberMessageReceivedThreshold > 1
      && (m_NumberMessagesReceived % m_NumberMessageReceivedThreshold == 0))
  {
    this->OnOutputStats();
  }
}

} // end namespace niftk
