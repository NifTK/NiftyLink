/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkClientSocket_h
#define NiftyLinkClientSocket_h

#include "NiftyLinkSocket.h"

#include <igtlWin32Header.h>

namespace niftk
{

/**
 * \class NiftyLinkClientSocket
 * \brief Represents the client side of a socket connection.
 *
 * Note: Both server and client can send and receive messages.
 * The main conceptual difference is that a server starts up,
 * binds and listens on a port, and the client connects to it.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkClientSocket : public NiftyLinkSocket
{
public:
  typedef NiftyLinkClientSocket          Self;
  typedef NiftyLinkSocket                Superclass;
  typedef igtl::SmartPointer<Self>       Pointer;
  typedef igtl::SmartPointer<const Self> ConstPointer;

  igtlTypeMacro(NiftyLinkClientSocket, NiftyLinkSocket)
  igtlNewMacro(NiftyLinkClientSocket);

  /// Description:
  /// Set connection timeout in millisecond.
  /// This function should be called before connecting.
  int SetConnectionTimeout(int timeout);

  /// Connects to a server. Returns 0 on success, -1 on error.
  int ConnectToServer(const char* hostname, int port); 

protected:
  NiftyLinkClientSocket();
  ~NiftyLinkClientSocket();

  void PrintSelf(std::ostream& os) const;

  friend class NiftyLinkServerSocket;

private:
  NiftyLinkClientSocket(const NiftyLinkClientSocket&); // Purposefully not implemented.
  void operator=(const NiftyLinkClientSocket&); // Purposefully not implemented.

  /// Description:
  /// Connect to a server socket using non-blocking sockets. Returns 0 on success, or the error code.
  int ConnectNonBlocking(int socketdescriptor, const char* hostname, int port);

  // To store the timeout details for when we connect to a server.
  struct timeval m_ConnectionTimeout;
};

} // end namespace niftk

#endif

