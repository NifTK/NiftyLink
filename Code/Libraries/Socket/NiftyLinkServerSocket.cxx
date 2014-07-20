/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkServerSocket.h"
#include "NiftyLinkClientSocket.h"

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkServerSocket::NiftyLinkServerSocket()
{
}


//-----------------------------------------------------------------------------
NiftyLinkServerSocket::~NiftyLinkServerSocket()
{
}


//-----------------------------------------------------------------------------
int NiftyLinkServerSocket::CreateServer(int port)
{
  if (this->GetConnected())
    {
    igtlWarningMacro("Server Socket already exists. Closing old socket.");
    this->CloseSocket(this->m_SocketDescriptor);
    this->m_SocketDescriptor = INVALID_SOCKET;
    }
  this->m_SocketDescriptor = this->CreateSocket();
  if (!this->GetConnected())
    {
    igtlSocketErrorMacro(<< "Failed to create server socket on port " << port << ".");
    return -1;
    }

  int err1 = this->BindSocket(this->m_SocketDescriptor, port);
  if (err1 != 0)
    {
    igtlSocketErrorMacro(<< "Failed to bind on port " << port << ".");
    this->CloseSocket(this->m_SocketDescriptor);
    this->m_SocketDescriptor = INVALID_SOCKET;
    return -1;
    }

  int err2 = this->Listen(this->m_SocketDescriptor);
  if (err2 != 0)
    {
    igtlSocketErrorMacro(<< "Failed to listen on port " << port << ".");
    this->CloseSocket(this->m_SocketDescriptor);
    this->m_SocketDescriptor = INVALID_SOCKET;
    return -1;
    }

  // Success.
  return 0;
}


//-----------------------------------------------------------------------------
NiftyLinkClientSocket::Pointer NiftyLinkServerSocket::WaitForConnection(unsigned long msec /*=0*/)
{
  if (!this->GetConnected())
    {
    igtlErrorMacro("Server Socket not created yet!");
    return NULL;
    }
   
  int ret = this->SelectSocket(this->m_SocketDescriptor, msec);
  if (ret == 0)
    {
    igtlWarningMacro("Timed out while waiting for clients to connect, threshold was " << msec << " milliseconds.");
    return NULL;
    }
  if (ret == -1)
    {
    igtlErrorMacro("Error selecting socket.");
    return NULL;
    }

  int clientsock = this->Accept(this->m_SocketDescriptor);
  if (clientsock == -1)
    {
    igtlErrorMacro("Failed to accept the socket.");
    return NULL;
    }

  // Create a new NiftyLinkClientSocket and return it.
  NiftyLinkClientSocket::Pointer cs = NiftyLinkClientSocket::New();
  cs->m_SocketDescriptor = clientsock;

  return cs;
}


//-----------------------------------------------------------------------------
void NiftyLinkServerSocket::PrintSelf(std::ostream& os) const
{
  this->Superclass::PrintSelf(os);
}

} // end namespace niftk
