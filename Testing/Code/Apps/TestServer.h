/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestServer_h
#define TestServer_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkServer.h>

#include <igtlTimeStamp.h>

#include <QObject>

namespace niftk
{

class TestServer : public QObject
{

  Q_OBJECT

public:
  TestServer(const int& portNumber);
  virtual ~TestServer();

private slots:

  void Start();

  void OnClientConnected();
  void OnCantSendToClient();
  void OnClientDisconnected();
  void OnMessageReceived(NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private:
  int             m_PortNumber;
  NiftyLinkServer m_Server;
};

} // end namespace niftk

#endif // TestServer_h
