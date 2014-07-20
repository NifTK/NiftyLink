/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkSocket.h"

#include <igtlOSUtil.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
  #define WSA_VERSION MAKEWORD(1,1)
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
  #include <Ws2tcpip.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netinet/tcp.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <errno.h>
  #include <sys/ioctl.h>
#endif

#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkSocket::NiftyLinkSocket()
: m_TimeoutFlag(0)
, m_SendTimeStamp(0)
, m_ReceiveTimeStamp(0)
{
}


//-----------------------------------------------------------------------------
NiftyLinkSocket::~NiftyLinkSocket()
{
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::SetReceiveTimeout(int /*timeout*/)
{
  throw std::runtime_error("SetReceiveTimeout should not be called.");
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::SetSendTimeout(int /*timeout*/)
{
  throw std::runtime_error("SetSendTimeout should not be called.");
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::SetReceiveBlocking(int /*sw*/)
{
  throw std::runtime_error("SetReceiveBlocking should not be called.");
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::SetSendBlocking(int /*sw*/)
{
  throw std::runtime_error("SetSendBlocking should not be called.");
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::GetPort()
{
  std::string address;
  int port;

  if (!this->GetConnected())
  {
    return -1;
  }

  this->GetSocketAddressAndPort(address, port);
  return port;
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::Send(const void* data, int length)
{
  if (!this->GetConnected())
  {
    return -1;
  }

  if (length == 0)
  {
    // nothing to send.
    return 1;
  }

  int rVal     = 0;
  u_long iMode = 1;

  // Will set socket to blocking mode if working on large messages
  //if (length > 1000)
  //  iMode = 0;

  //std::cerr <<"Preparing to send...";
  const char* buffer = reinterpret_cast<const char*>(data);
  int total = 0;
  int n     = 0;
  int trys  = 0;

  int flags;
  #if defined(_WIN32) && !defined(__CYGWIN__)
    flags = 0; //disable signal on Win boxes.
    rVal  = ioctlsocket(this->m_SocketDescriptor, FIONBIO, &iMode);

  #elif defined(__linux__)
    flags = MSG_NOSIGNAL; //disable signal on Unix boxes.
    rVal |= ioctl(this->m_SocketDescriptor, FIONBIO, &iMode);

  #elif defined(__APPLE__)
    int opt=1;
    rVal  = setsockopt(this->m_SocketDescriptor, SOL_SOCKET, SO_NOSIGPIPE, (char*) &opt, sizeof(int));
    rVal |= ioctl(this->m_SocketDescriptor, FIONBIO, &iMode);
    flags = SO_NOSIGPIPE; //disable signal on Mac boxes.
  #endif

  if (rVal < 0)    // ioctl error
  {
     igtlSocketErrorMacro(<< "ioctl");
     return -2;
  }

  do
  {
    n = 0;

    try
    {
      n = send(this->m_SocketDescriptor, buffer+total, length-total, flags);
    }

    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return -3;
    }

    if (n <= 0)
    {
      #if defined(_WIN32) && !defined(__CYGWIN__)
        int error = WSAGetLastError();
        trys++;

        if (((error == WSAENOBUFS) && (trys < 1000)) || ((error == WSAEWOULDBLOCK) && (trys < 1000)))
        {
          igtl::Sleep(1);
          continue;
        }
      #else
        int error = errno;

        if ((error == EWOULDBLOCK) && (++trys < 1000))
        {
          igtl::Sleep(1);
          continue;
        }
      #endif

      igtlSocketErrorMacro(<< "sendfail");
      return -1;
    }

    total += n;

  } while(total < length);

  // Record the software timestamp as the message left the device
  m_SendTimeStamp = igtl::TimeStamp::New();
  return 1;
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::Receive(void* data, int length, int readFully/*=1*/)
{
  if (!this->GetConnected())
  {
    return -1;
  }

  char* buffer = reinterpret_cast<char*>(data);
  int total       = 0;
  int bytesRead   = 0;
  //int rVal        = 0;
  int flags       = 0;
  int trys        = 0;

  // Receive a generic message
  do
  {
    bytesRead   = 0;

    // Try reading from the socket
    try
    {
      bytesRead = recv(this->m_SocketDescriptor, buffer+total, length-total, flags);
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return -2;
    }

    if (bytesRead < 1)
    {
      #if defined(_WIN32) && !defined(__CYGWIN__)
        int error = WSAGetLastError();
        trys++;

        if (((error == WSAENOBUFS) && (trys < 1000)) || ((error == WSAEWOULDBLOCK) && (trys < 1000)))
        {
          igtl::Sleep(1);
          continue;
        }
      #else
        int error = errno;

        if ((error == EWOULDBLOCK) && (++trys < 1000))
        {
          igtl::Sleep(1);
          continue;
        }
      #endif

      // Error in recv()
      if (bytesRead < 0)
        {
        igtlSocketErrorMacro(<< "recv");
        }
      return bytesRead;
    }

    total += bytesRead;

  } while(readFully && total < length);

  return total;
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::SetTimeout(int timeout)
{
  if (!this->GetConnected())
  {
    return 0;
  }

  #if defined(_WIN32) && !defined(__CYGWIN__)
    this->m_Timeout = timeout;
    int len;
  #else
    this->m_Timeout.tv_sec  = timeout/1000;          /* second */
    this->m_Timeout.tv_usec = (timeout%1000) * 1000; /* microsecond */
    socklen_t len;
  #endif
  if ( timeout > 0 )
  {
    getsockopt(this->m_SocketDescriptor, SOL_SOCKET, SO_RCVTIMEO,(char*)&(this->m_OrigTimeout), &len);
    setsockopt(this->m_SocketDescriptor, SOL_SOCKET, SO_RCVTIMEO,(char*)&(this->m_Timeout), sizeof(this->m_Timeout));
    this->m_TimeoutFlag = 1;
  }
  else if (this->m_TimeoutFlag)
  {
    setsockopt(this->m_SocketDescriptor, SOL_SOCKET, SO_RCVTIMEO,(char*)&(this->m_OrigTimeout), sizeof(this->m_OrigTimeout));
    this->m_TimeoutFlag = 0;
  }

  return timeout;
}


//-----------------------------------------------------------------------------
int NiftyLinkSocket::CheckPendingData()
{
  if (!this->GetConnected())
  {
    return -1;
  }

  int rVal     = 0;
  u_long iMode = 1; // non-blocking mode
  igtl::TimeStamp::Pointer ts;

  #if defined(_WIN32) && !defined(__CYGWIN__)
    u_long bytesToRead = 0;
  #else
    int bytesToRead = 0;
  #endif

  // Take a look at the buffer to find out the number of bytes that arrived (if any)
  try
  {
    #if defined(_WIN32) && !defined(__CYGWIN__)
      rVal  = ioctlsocket(this->m_SocketDescriptor, FIONBIO, &iMode);
      rVal |= ioctlsocket(this->m_SocketDescriptor, FIONREAD, &bytesToRead);

      // Record the software timestamp asap
      ts = igtl::TimeStamp::New();
    #else
      rVal  = ioctl(this->m_SocketDescriptor, FIONBIO, &iMode);
      rVal |= ioctl(this->m_SocketDescriptor, FIONREAD, &bytesToRead);

      // Record the software timestamp asap
      ts = igtl::TimeStamp::New();
    #endif
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return -2;
  }

   if (rVal < 0)    // ioctl error
   {
      igtlSocketErrorMacro(<< "ioctl");
      return -3;
   }

  switch (bytesToRead)
  {
    case 0:     // nothing to read
      return 0;
    case 2:     // Taking care of keepalive
      char buf[2];
      try
      {
        recv(this->m_SocketDescriptor,(char*)&buf, 2, 0);
      }
      catch (std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        return -4;
      }
      return 2;
    default:    // Large number of data waiting, probably message
      m_ReceiveTimeStamp.operator =(ts);

      return bytesToRead;
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkSocket::Poke()
{
  if (!this->GetConnected())
  {
    return false;
  }
  u_long iMode = 1;
  int total  = 0;
  int n      = 0;
  int length = 2;
  char buff[2];

  //Going to send "??"
  buff[0] = 63;
  buff[1] = 63;

  int flags;
  #if defined(_WIN32) && !defined(__CYGWIN__)
    flags = 0; //disable signal on Win boxes.
    ioctlsocket(this->m_SocketDescriptor, FIONBIO, &iMode);   // Set Non-Blocking mode

  #elif defined(__linux__)
    flags = MSG_NOSIGNAL; //disable signal on Unix boxes.
    ioctl(this->m_SocketDescriptor, FIONBIO, &iMode);   // Set Non-Blocking mode

  #elif defined(__APPLE__)
    int opt=1;
    setsockopt(this->m_SocketDescriptor, SOL_SOCKET, SO_NOSIGPIPE, (char*) &opt, sizeof(int));
    flags = SO_NOSIGPIPE; //disable signal on Mac boxes.
    ioctl(this->m_SocketDescriptor, FIONBIO, &iMode);   // Set Non-Blocking mode
  #endif

  // TEST WRITE
  do
  {
    n = 0;

    try
    {
      //std::cerr <<"Sending keepalive..." <<std::endl;
      n = send(this->m_SocketDescriptor, buff+total, length-total, flags);
    }

    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return false;
    }

    if (n < 0)
      return false;

    total += n;

  } while(total < length);

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkSocket::PrintSelf(std::ostream& os) const
{
  this->Superclass::PrintSelf(os);
}

} // end namespace niftk
