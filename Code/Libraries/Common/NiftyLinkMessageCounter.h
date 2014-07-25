/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMessageCounter_h
#define NiftyLinkMessageCounter_h

#include "NiftyLinkCommonWin32ExportHeader.h"
#include <NiftyLinkMessageContainer.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QList>
#include <QtGlobal>

namespace niftk
{

/**
 * \class NiftyLinkMessageCounter
 * \brief Simple container object to work out some statistics of the messages received.
 *
 * Specifically, the OpenIGTLink paper (Tokuda 2009) measures latency as the
 * difference in time between the first byte sent and the last byte received.
 * So, to use this class, you simply pass in each new message to OnMessageReceived,
 * which counts stuff. Then OnClear will clear the current numbers. OnOutputStats()
 * will dump the results to console.
 *
 * The output includes
 * <ol>
 *   <li>Number of bytes received.</li>
 *   <li>Timeframe over which this number of bytes was received.</li>
 *   <li>Rate of data transfer in bytes/sec.</li>
 *   <li>Mean latency in milliseconds.</li>
 *   <li>Std Dev of latency in milliseconds.</li>
 *   <li>Maximum of latency in milliseconds.</li>
 * </ol>
 *
 * Each time the stats are output, all counters are reset. It is
 * envisaged that this class would most typically be triggered
 * using a QTimer. However, if you call:
 * <pre>
 *   NiftyLinkMessageCounter myCounter;
 *   myCounter->SetNumberMessageReceivedThreshold(threshold);
 * </pre>
 * where <em>threshold</em> is a positive number of messages,
 * the statistics will automatically be dumped to the console every
 * time the number of messages received passes that <em>threshold</em>.
 *
 * In addition, OpenIGTLink messages have a DeviceType field, containing
 * for example IMAGE, TDATA, POSITION etc. Most of our use-cases involve
 * a socket connection receiving single message types. However, it would
 * also be possible to send data from different devices to the same
 * server socket. This means different images would be received on the
 * same port. So, this class records statistics, grouped by DeviceType.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageCounter : public QObject {

  Q_OBJECT

public:

  /// Constuctor.
  NiftyLinkMessageCounter(QObject *parent = 0);

  /// Destructor.
  ~NiftyLinkMessageCounter();

  void SetNumberMessageReceivedThreshold(qint64 threshold);
  qint64 GetNumberMessageReceivedThreshold() const;

public slots:

  /// \brief Writes stats to console.
  void OnOutputStats();

  /// \brief Clear the stats containers down.
  void OnClear();

  /// \brief Increment internal counters, i.e. accumulate statistics.
  void OnMessageReceived(NiftyLinkMessageContainer::Pointer& message);

private:

  igtl::TimeStamp::Pointer        m_StatsTimePoint;
  quint64                         m_TotalBytesReceived;
  quint64                         m_NumberMessagesReceived;
  QMap< QString, QList<quint64> > m_ListsOfLatenciesByDeviceType;
  qint64                          m_NumberMessageReceivedThreshold;

}; // end class

} // end namespace

#endif // NiftyLinkMessageCounter_h
