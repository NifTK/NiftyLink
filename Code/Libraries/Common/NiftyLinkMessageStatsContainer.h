/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMessageStatsContainer_h
#define NiftyLinkMessageStatsContainer_h

#include "NiftyLinkCommonWin32ExportHeader.h"

#include <QMap>
#include <QtGlobal>
#include <QMetaType>

namespace niftk
{

/**
 * \class NiftyLinkMessageStatsContainer
 * \brief Simple container to contain the statistics from messages, so we can pass statistics information over signals/slots etc.
 *
 * This class stores the total number of bytes and total number of messages
 * received since the existence of this object. Other variables are running totals
 * since the last checkpoint. So, for example, you may start recording data, and check-point every 30 seconds.
 *
 * This class relies on Value Type semantics, so must provide a correct and preferably cheap copy and assignment operators.
 * This is a requirement for signals and slots, and for passing these objects about quickly.
 *
 * For this reason, note that we do not use igtl::TimeStamp in this class. igtl::TimeStamp is expensive to create/destroy.
 * So, we don't want to create/destroy them each time we use signals and slots and copy constructors etc.
 * So, this class works in conjunction with the NiftyLinkMessageCounter class to measure stats.
 * The reason this class is separate is to make it easier to test.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageStatsContainer {

public:

  /// \brief Constuctor.
  NiftyLinkMessageStatsContainer();

  /// \brief Destructor.
  ~NiftyLinkMessageStatsContainer();

  /// \brief Copy semantics - copy constructor.
  NiftyLinkMessageStatsContainer(const NiftyLinkMessageStatsContainer& another);

  /// \brief Copy semantics - assignment operator.
  NiftyLinkMessageStatsContainer& operator=(const NiftyLinkMessageStatsContainer& another);

  /// \brief Check for equality, not strictly speaking necessary, but we might as well.
  bool operator==(const NiftyLinkMessageStatsContainer& another) const;

  /// \brief Equals 1000000.
  static const double NANO_TO_MILLI_DIVISOR;

  /// \brief Equals 1000000000.
  static const double NANO_TO_SECONDS_DIVISOR;

  /// \brief Increments the main data.
  /// \param latency This must be non-negative, hence the use of unsigned int.
  /// Note: The point of this method, is that you have to specify all relevant data in one call.
  /// This means the user is less likely to mis-use the class. Don't provide Setters for individual member variables.
  void Increment(const QString& deviceType,
                 const quint64& messageTimeInNanoseconds,
                 const quint64& numberOfBytes,
                 const quint64& latency);

  /// \brief Resets everything to zero.
  void ResetAll();

  /// \brief Resets all variable that pertain to a specific timing period.
  void Checkpoint();

  /// \brief Returns the mean latency in nanoseconds since the last checkpoint.
  double GetMeanLatencySinceCheckpoint() const;

  /// \brief Returns the mean latency in milliseconds since the last checkpoint.
  double GetMeanLatencySinceCheckpointInMilliseconds() const;

  /// \brief Returns the sample corrected standard deviation of latency in nanoseconds since the last checkpoint.
  double GetStdDevLatencySinceCheckpoint() const;

  /// \brief Returns the sample corrected standard deviation of latency in milliseconds since the last checkpoint.
  double GetStdDevLatencySinceCheckpointInMilliseconds() const;

  /// \brief Returns the maximum of the latency in nanoseconds since the last checkpoint.
  double GetMaxLatencySinceCheckpoint() const;

  /// \brief Returns the maximum of the latency in milliseconds since the last checkpoint.
  double GetMaxLatencySinceCheckpointInMilliseconds() const;

  /// \brief Returns the duration over which the stats are currently calculated, ie. since the last checkpoint.
  /// No attempt is made to detect or recover from underflow, so if time drifts backwards, this can be negative.
  double GetDurationSinceLastCheckpoint() const;

  /// \brief Returns the duration over which the stats are currently calculated in seconds, ie. since the last checkpoint.
  /// No attempt is made to detect or recover from underflow, so if time drifts backwards, this can be negative.
  double GetDurationSinceLastCheckpointInSeconds() const;

  /// \brief Returns the number of messages per second, in the duration since the last checkpoint.
  /// No attempt is made to detect or recover from underflow, so if time drifts backwards, this can be negative.
  double GetMessagesPerSecondSinceLastCheckpoint() const;

  /// \brief Returns the number of messages by type since the last checkpoint.
  QMap< QString, quint64 > GetNumberOfMessagesByTypeSinceCheckpoint() const;

  // Getters. You must NOT provide a Setter, cos other member variables may get out of synch.
  quint64 GetStartTimeStampInNanoseconds() const;
  quint64 GetEndTimeStampInNanoseconds() const;
  quint64 GetTotalBytesReceived() const;
  quint64 GetTotalNumberMessagesReceived() const;
  quint64 GetBytesReceivedSinceCheckpoint() const;
  quint64 GetNumberMessagesReceivedSinceCheckpoint() const;
  QString GetStatsMessage() const;

private:

  void DeepCopy(const NiftyLinkMessageStatsContainer& another);

  quint64                  m_StartTimeStampInNanoseconds;
  quint64                  m_EndTimeStampInNanoseconds;
  quint64                  m_TotalBytesReceived;
  quint64                  m_TotalNumberMessagesReceived;
  quint64                  m_BytesReceivedBetweenCheckPoints;
  quint64                  m_NumberMessagesReceivedBetweenCheckPoints;
  QList<quint64>           m_ListOfLatency;
  QMap< QString, quint64 > m_MapOfMessageCounts;

}; // end class

} // end namespace

Q_DECLARE_METATYPE(niftk::NiftyLinkMessageStatsContainer)

#endif // NiftyLinkMessageStatsContainer_h
