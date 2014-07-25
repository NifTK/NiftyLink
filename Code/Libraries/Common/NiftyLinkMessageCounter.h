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

  /// \brief Increment/Count stuff.
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
