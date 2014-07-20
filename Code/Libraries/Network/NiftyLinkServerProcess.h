/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkServerProcess_h
#define NiftyLinkServerProcess_h

#include <NiftyLinkCommonWin32ExportHeader.h>

#include <NiftyLinkNetworkProcess.h>
#include <NiftyLinkServer.h>
#include <NiftyLinkServerSocket.h>
#include <NiftyLinkClientSocket.h>
#include <NiftyLinkMessageContainer.h>

#include <QObject>

namespace niftk
{

/**
 * \class NiftyLinkServerProcess
 * \brief Represents a Process running a NiftyLinkServer socket in a separate thread.
 *
 * Note: Error handling strategy is to throw std::exception sub-classes for everything.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkServerProcess : public NiftyLinkNetworkProcess {

  Q_OBJECT

public:

  /// \brief Constructor.
  NiftyLinkServerProcess();

  /// \brief Destructor.
  virtual ~NiftyLinkServerProcess();

  /// \brief Function for setting the length of time in milliseconds for which to listen for clients.
  void SetListenInterval(const int& msec);

  /// \brief Returns the currently listen interval.
  int GetListenInterval();

  /// \brief Starts up the server socket.
  void Initialise(const int& port);

signals:

  /// \brief Emmitted when a client has established connection with us.
  void ClientConnectedSignal();

protected:

  /// \see NiftyLinkNetworkProcess::DoProcessing()
  virtual void DoProcessing();

private:

  int                             m_ListenInterval;

}; // end class

} // end namespace niftk

#endif // NiftyLinkServerProcess_h
