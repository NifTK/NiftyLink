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
 * \brief Simple container to contain the statistics from messages, so we can pass over signals/slots etc.
 *
 * Note: We rely on Value Type semantics, i.e. copy constructor,
 * assignment operator, preferably all low cost, like a primative type.
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

  /// \brief Increments the main data.
  ///
  /// Note: The point of this method, is that you have to specify all relevant data in one call.
  /// This means the user can't mis-user the class. Don't provide Setters for other member variables.
  void Increment(const QString& deviceType, const quint64& numberOfBytes, const quint64& latency);

  /// \brief Resets everything to zero.
  void ResetAll();

  /// \brief Resets all variable that pertain to a specific timing period.
  void ResetPeriod();

  /// \brief Returns the mean latency.
  double GetMeanLatency() const;

  /// \brief Returns the sample corrected standard deviation of latency.
  double GetStdDevLatency() const;

  /// \brief Returns the maximum of the latency.
  quint64 GetMaxLatency() const;

  /// \brief Returns the number of messages by type.
  QMap< QString, quint64 > GetNumberOfMessagesByType() const;

  // Getters. You must NOT provide a Setter, cos other member variables may get out of synch.
  quint64 GetTotalBytesReceived() const;
  quint64 GetTotalNumberMessagesReceived() const;
  quint64 GetBytesReceivedBetweemTimingPoints() const;
  quint64 GetNumberMessagesReceivedBetweenTimingPoints();

private:

  void DeepCopy(const NiftyLinkMessageStatsContainer& another);

  quint64                  m_TotalBytesReceived;
  quint64                  m_TotalNumberMessagesReceived;
  quint64                  m_BytesReceivedBetweemTimingPoints;
  quint64                  m_NumberMessagesReceivedBetweenTimingPoints;
  QList<quint64>           m_ListOfLatency;
  QMap< QString, quint64 > m_MapOfMessageCounts;

}; // end class

} // end namespace

Q_DECLARE_METATYPE(niftk::NiftyLinkMessageStatsContainer)

#endif // NiftyLinkMessageStatsContainer_h
