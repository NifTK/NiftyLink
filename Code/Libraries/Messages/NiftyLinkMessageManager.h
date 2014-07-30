/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMessageManager_h
#define NiftyLinkMessageManager_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QtGlobal>

namespace niftk
{
/**
 * \class NiftyLinkMessageManager
 * \file NiftyLinkMessageManager.h
 * \brief Helper class to avoid passing NiftyLinkMessageContainer over Qt::QueuedConnection.
 *
 * If you pass NiftyLinkMessageContainer::Pointer over a Qt::QueuedConnection,
 * the pointer survives, but the contained igtl::MessageBase::Pointer appeared not to.
 * So, this class was created. The aim is that a message receiver can insert a message
 * into a QMap. The map is keyed by port number, so multiple clients can use the
 * same object. There is only one 'current' message per client however. This is not
 * a queue structure. Then the receiver can signal that they have received something.
 * Someone else can register to that signal, and then extract the data from this class.
 * The Qt signal/slots mechanism is used to work across threads, so the correct
 * choice of connection type is important.
 */
class NiftyLinkMessageManager : public QObject
{
  Q_OBJECT

public:

  NiftyLinkMessageManager(QObject *parent = 0);
  virtual ~NiftyLinkMessageManager();

  /// \brief Adds a container to the buffer.
  void InsertContainer(int portNumber, NiftyLinkMessageContainer::Pointer container);

  /// \brief Retrives the container for a certain port.
  NiftyLinkMessageContainer::Pointer GetContainer(int portNumber);

  /// \brief Retrives the container for the first port in the QMap.
  NiftyLinkMessageContainer::Pointer GetContainer();

private:

  QMap<int, NiftyLinkMessageContainer::Pointer > m_Data;
  QMutex                                         m_Mutex;

}; // end class

} // end namespace niftk

#endif
