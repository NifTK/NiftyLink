/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkServerSocket_h
#define NiftyLinkServerSocket_h

#include <NiftyLinkCommonWin32ExportHeader.h>

#include "NiftyLinkSocket.h"
#include "NiftyLinkClientSocket.h"

#include <igtlWin32Header.h>

namespace niftk
{

/**
 * \class NiftyLinkServerSocket
 * \file NiftyLinkServerSocket.h
 * \brief Represents the server side of a socket connection.
 *
 * Note: Both server and client can send and receive messages.
 * The main conceptual difference is that a server starts up,
 * binds and listens on a port, and the client connects to it.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkServerSocket : public NiftyLinkSocket
{
public:
  typedef NiftyLinkServerSocket          Self;
  typedef NiftyLinkSocket                Superclass;
  typedef igtl::SmartPointer<Self>       Pointer;
  typedef igtl::SmartPointer<const Self> ConstPointer;

  igtlTypeMacro(NiftyLinkServerSocket, NiftyLinkSocket);
  igtlNewMacro(NiftyLinkServerSocket);

  /// Description:
  /// Creates a server socket at a given port and binds to it.
  /// Returns -1 on error. 0 on success.
  int CreateServer(int port);

  /// Description:
  /// Waits for a connection. When a connection is received
  /// a new NiftyLinkClientSocket object is created and returned.
  /// Returns NULL on timeout.
  NiftyLinkSocket::Pointer WaitForConnection(unsigned long msec=0);

protected:
  NiftyLinkServerSocket();
  ~NiftyLinkServerSocket();

  void PrintSelf(std::ostream& os) const;

private:
  NiftyLinkServerSocket(const NiftyLinkServerSocket&); // Purposefully not implemented.
  void operator=(const NiftyLinkServerSocket&); // Purposefully not implemented.
};

} // end namespace niftk

#endif

