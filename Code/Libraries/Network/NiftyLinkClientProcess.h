/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkClientProcess_h
#define NiftyLinkClientProcess_h

#include <NiftyLinkCommonWin32ExportHeader.h>

#include <NiftyLinkNetworkProcess.h>
#include <NiftyLinkClient.h>
#include <NiftyLinkClientSocket.h>
#include <NiftyLinkMessageContainer.h>

#include <QObject>
#include <QMutex>

namespace niftk
{

/**
 * \class NiftyLinkClientProcess
 * \brief Represents a Process running a NiftyLinkClient socket in a separate thread.
 *
 * Note: Error handling strategy is to throw std::exception sub-classes for everything.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkClientProcess : public NiftyLinkNetworkProcess {

  Q_OBJECT

public:

  /// \brief Constructor.
  NiftyLinkClientProcess();

  /// \brief Destructor.
  virtual ~NiftyLinkClientProcess();

  /// \brief Starts up the client socket.
  void Initialise(const QString& hostname, const int &port);

  /// \brief This function is for setting the time to wait (in milliseconds) when connecting to a remote host.
  void SetConnectionTimeOut(const int& msec);

  /// \brief Returns the current value of the connection timeout.
  int GetConnectionTimeOut();

  /// \brief Used for testing, and performance analysis, writing some stats to console.
  void OutputStats();

signals:

  /// \brief Emmitted when this client connects to remote server.
  void ConnectedToRemote();

protected:

  /// \see NiftyLinkNetworkProcess::DoProcessing()
  virtual void DoProcessing();

private:

  int                               m_ConnectionTimeout;
  int                               m_Port;
  QString                           m_HostName;

}; // end class

} // end namespace niftk

#endif // NiftyLinkClientProcess_h
