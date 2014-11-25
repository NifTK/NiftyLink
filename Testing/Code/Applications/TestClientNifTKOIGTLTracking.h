/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestClientNifTKOIGTLTracking_h
#define TestClientNifTKOIGTLTracking_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpClient.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QTimer>

namespace niftk
{

/**
 * \class TestClientNifTKOIGTLTracking
 * \file TestClientNifTKOIGTLTracking.h
 * \brief Tracking client to send/receive data from an OpenIGTLink server,
 * using the NifTK OpenIGTLink based sockets.
 */
class TestClientNifTKOIGTLTracking : public QObject
{

  Q_OBJECT

public:
  TestClientNifTKOIGTLTracking(const std::string& hostName,
                       const int& portNumber,
                       const int& fps,
                       const int& totalNumberMessages,
                       const int& trackedObjectsPerMessage,
                       QObject *parent=0
                      );
  virtual ~TestClientNifTKOIGTLTracking();

private slots:

  void Start();

private:

  QString             m_HostName;
  int                 m_PortNumber;
  int                 m_FramesPerSecond;
  int                 m_IntendedNumberMessages;
  int                 m_TrackedObjectsPerMessage;
  int                 m_NumberMessagesSent;

};

} // end namespace niftk

#endif // TestClientNifTKOIGTLTracking_h
