/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkClientSocket.h"

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

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkClientSocket::NiftyLinkClientSocket()
{
  this->m_ConnectionTimeout.tv_sec = 5;
  this->m_ConnectionTimeout.tv_usec = 0;
}


//-----------------------------------------------------------------------------
NiftyLinkClientSocket::~NiftyLinkClientSocket()
{
}


//-----------------------------------------------------------------------------
int NiftyLinkClientSocket::ConnectToServer(const char* hostName, int port)
{
  if (this->GetConnected())
    {
    igtlWarningMacro("Client connection already exists on " << hostName << ":" << port << ". Closing it.");
    this->CloseSocket(this->m_SocketDescriptor);
    this->m_SocketDescriptor = INVALID_SOCKET;
    }
  
  this->m_SocketDescriptor = this->CreateSocket();
  if (!this->GetConnected())
    {
    igtlSocketErrorMacro(<< "Failed to create socket connecting to " << hostName << ":" << port << ".");
    return -1;
    }

  if (this->ConnectNonBlocking(this->m_SocketDescriptor, hostName, port) < 0)
    {
    igtlSocketErrorMacro(<< "Failed to connect to server on " << hostName << ":" << port << ".");
    this->CloseSocket(this->m_SocketDescriptor);
    this->m_SocketDescriptor = INVALID_SOCKET;
    return -1;
    }

  return 0;
}


//-----------------------------------------------------------------------------
int NiftyLinkClientSocket::ConnectNonBlocking(int soc, const char* hostName, int port)
{
  if (!this->GetConnected())
  {
    return -1;
  }

  int res;
  struct sockaddr_in addr;
  fd_set myset;
  int valopt;

  #if defined(_WIN32) && !defined(__CYGWIN__)
    int lon;
    int iResult;
    u_long iMode = 0;
  #else
    socklen_t lon;
    long arg;
  #endif

  struct hostent* hp;
  hp = gethostbyname(hostName);
  if (!hp)
  {
    unsigned long addr = inet_addr(hostName);
    hp = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
  }

  if (!hp)
    return -2;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

  // Set non-blocking
  #if defined(_WIN32) && !defined(__CYGWIN__)
    iMode = 1;
    iResult = ioctlsocket(soc, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
    {
      fprintf(stderr, "ioctlsocket failed with error: %ld\n", iResult);
      return -3;
    }
  #else
    if( (arg = fcntl(soc, F_GETFL, NULL)) < 0)
    {
      fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
      return -3;
    }
    arg |= O_NONBLOCK;
    if( fcntl(soc, F_SETFL, arg) < 0)
    {
      fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
      return -3;
    }
  #endif


  // Trying to connect with timeout
  res = connect(soc, (struct sockaddr *)&addr, sizeof(addr));
  if (res < 0)
  {
    bool busy = false;

    #if defined(_WIN32) && !defined(__CYGWIN__)
      int serr = WSAGetLastError();
      if (serr == WSAEWOULDBLOCK || serr == WSAEINPROGRESS)
        busy = true;
    #else
      if (errno == EINPROGRESS)
        busy = true;
    #endif

    if (busy == true)
    {
      do
      {
        FD_ZERO(&myset);
        FD_SET(soc, &myset);

        res = select(soc+1, NULL, &myset, NULL, &m_ConnectionTimeout);

        bool selectErr = false;

        #if defined(_WIN32) && !defined(__CYGWIN__)
          int serr = WSAGetLastError();
          if (serr == WSAEINTR)
            selectErr = true;
        #else
          if (errno == EINTR)
            selectErr = true;
        #endif

        if (res < 0 && selectErr == true)
        {
          igtlSocketErrorMacro(<< "connect");
          return -4;
        }
        else if (res > 0)
        {
          // Socket selected for write
          lon = sizeof(int);

          bool sockoptErr = 0;

          #if defined(_WIN32) && !defined(__CYGWIN__)
            if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &lon) < 0)
              sockoptErr = true;
          #else
            if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
              sockoptErr = true;
          #endif

          if (sockoptErr == true)
          {
            igtlSocketErrorMacro(<< "getsockopt");
            return -5;
          }

          // Check the value returned...
          if (valopt)
          {
            fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
            return -6;
          }

          //Successfully connected to remote
          break;
        }
        else
        {
          fprintf(stderr, "Connection timeout - Cancelling!\n");
          return -7;
        }
      } while (1);
    }
    else
    {
      igtlSocketErrorMacro(<< "connect");
      return -8;
    }
  }

  // Set to blocking mode again...
  #if defined(_WIN32) && !defined(__CYGWIN__)
    iMode = 0;
    iResult = ioctlsocket(soc, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
    {
      fprintf(stderr, "ioctlsocket failed with error: %ld\n", iResult);
      return -9;
    }
  #else
    if( (arg = fcntl(soc, F_GETFL, NULL)) < 0)
    {
      fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
      return -9;
    }

    arg &= (~O_NONBLOCK);
    if( fcntl(soc, F_SETFL, arg) < 0)
    {
      fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
      return -9;
    }
  #endif

  return 0;
}


//-----------------------------------------------------------------------------
int NiftyLinkClientSocket::SetConnectionTimeout(int timeout)
{
  if (!this->GetConnected())
  {
    return -1;
  }

  if (timeout < 0)
  {
    igtlWarningMacro("Timeout of " << timeout << " requested, so ignoring it.");
    return 0;
  }

  this->m_ConnectionTimeout.tv_sec  = timeout/1000;          /* second */
  this->m_ConnectionTimeout.tv_usec = (timeout%1000) * 1000; /* microsecond */

  return timeout;
}


//-----------------------------------------------------------------------------
void NiftyLinkClientSocket::PrintSelf(std::ostream& os) const
{
  this->Superclass::PrintSelf(os);
}

} // end namespace niftk

