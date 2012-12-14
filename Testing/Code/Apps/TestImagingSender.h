/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestImagingSender_H
#define TestImagingSender_H

#include <QObject>
#include <QtGui/QImage>
#include <igtlTimeStamp.h>
#include "OIGTLSocketObject.h"

class TestImagingSender : public QObject {

  Q_OBJECT

public:
  TestImagingSender(const QImage* image,
                    const QString& hostName,
                    const int& portNumber,
                    const int& numberOfIterations
                    );
  virtual ~TestImagingSender();

signals:
  void Done();

public slots:

  void Run();
  void OnMessageSent(unsigned long long timestamp);
  void OnConnectToRemote();
  void OnLostConnectionToRemote();
  void OnShutdownSender();

private:

  bool Setup();
  void SendData(const int& numberOfIterations);
  void FinishUp();

  OIGTLSocketObject m_Socket;
  const QImage*     m_Image;
  const QString     m_HostName;
  const int         m_PortNumber;
  const int         m_NumberOfIterations;
  int               m_NumberSent;

  igtlUint64 m_TimePackingMessage;

};

#endif TestImagingSender_H
