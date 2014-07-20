/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkSocket_h
#define NiftyLinkSocket_h

#include <NiftyLinkCommonWin32ExportHeader.h>

#include <igtlAbstractSocket.h>
#include <igtlTimeStamp.h>

/** \class NiftyLinkSocket
 *  \brief BSD socket encapsulation.
 *
 * This socket class was originally derived from igtlSocket, which itself
 * was largely based on the socket class in the Visualization Toolkit VTK.
 *
 * In order to integrate into the OpenIGTLink project, the
 * igtlAbstractSocket base class was created within the OpenIGTLink
 * project to contain common functionality, then the two derived classes
 * igtlSocket (in OpenIGTLink project) and NiftyLinkSocket (here)
 * factored out to contain original OpenIGTLink and NiftyLink functionality
 * respectively. So this class contains functions that are NiftyLink specific,
 * and almost a complete re-write of the corresponding functions in the OpenIGTLink base class.
 *
 * Error handling: Note that in keeping with OpenIGTLink, which has a 'c' layer,
 * and a 'c++' layer, the OpenIGTLink 'c++' layer is just a thin wrapper
 * over the c-style functionality. So, in OpenIGTLink, the error handling is
 * to return error codes to the client. This is kept here.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkSocket : public igtl::AbstractSocket
{
 public:
  typedef NiftyLinkSocket                Self;
  typedef igtl::AbstractSocket           Superclass;
  typedef igtl::SmartPointer<Self>       Pointer;
  typedef igtl::SmartPointer<const Self> ConstPointer;

  igtlTypeMacro(NiftyLinkSocket, igtl::AbstractSocket)
  igtlNewMacro(NiftyLinkSocket);

public:

  /// Description:
  /// These methods send data over the socket.
  /// Returns 1 on success, negative numbers on failure.
  virtual int Send(const void* data, int length);

  /// Description:
  /// Receive data from the socket.
  /// This call blocks until some data is read from the socket.
  /// When the readFully flag is set, this call will block until all the requested data is
  /// read from the socket. The readFully flag will be ignored if any timeout is active.
  /// 0 on error, -1 on timeout, else number of bytes read is returned.
  virtual int Receive(void* data, int length, int readFully=1);

  /// Description:
  /// Set sending/receiving timeout for the existing socket in millisecond.
  /// This function should be called after opening the socket.
  virtual int SetTimeout(int timeout);

  /// Description:
  /// Should not be called. Implemented to fullfill base class API.
  virtual int SetReceiveTimeout(int timeout);

  /// Description:
  /// Should not be called. Implemented to fullfill base class API.
  virtual int SetSendTimeout(int timeout);

  /// Description:
  /// Should not be called. Implemented to fullfill base class API.
  virtual int SetReceiveBlocking(int sw);

  /// Description:
  /// Should not be called. Implemented to fullfill base class API.
  virtual int SetSendBlocking(int sw);

  /// Description:
  /// Check socket for pending data.
  /// This call returns the amount of data that can be read in a single call,
  /// which may not be the same as the total amount of data queued on the socket.
  int CheckPendingData(void);

  /// Description:
  /// This method is for checking if the socket is OK for writing or not.
  /// It sends a 2 byte message through, returns with false if not possible.
  /// Query should be performed to detect disconnection, also it can be used for "keepalive".
  bool Poke();

  /// Description:
  /// Methods for getting the send and receive timestamps from the socket.
  /// Do not expose access to the internal objects, return only data types that are copies.
  inline igtlUint64 GetReceiveTimestampInNanoseconds() const { return m_ReceiveTimeStamp->GetTimeStampInNanoseconds(); }
  inline igtlUint64 GetSendTimestampInNanoseconds() const { return m_SendTimeStamp->GetTimeStampInNanoseconds(); }

  /// Description:
  /// Returns that the port that the server/client is listening/connected to.
  /// -1 for error, port number otherwise.
  int GetPort();

protected:

  NiftyLinkSocket();
  ~NiftyLinkSocket();

  void PrintSelf(std::ostream& os) const;

private:

  NiftyLinkSocket(const NiftyLinkSocket&); // Not implemented.
  void operator=(const NiftyLinkSocket&); // Not implemented.

#if defined(_WIN32) && !defined(__CYGWIN__)
  DWORD m_Timeout;
  DWORD m_OrigTimeout;
#else
  struct timeval m_Timeout;
  struct timeval m_OrigTimeout;
#endif
  int                      m_TimeoutFlag;
  igtl::TimeStamp::Pointer m_ReceiveTimeStamp;
  igtl::TimeStamp::Pointer m_SendTimeStamp;

}; // NiftyLinkSocket_h

#endif
