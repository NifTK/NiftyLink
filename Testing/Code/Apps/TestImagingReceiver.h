/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestImagingReceiver_H
#define TestImagingReceiver_H

#include <QObject>
#include <QtGui/QImage>
#include <igtlTimeStamp.h>
#include "OIGTLSocketObject.h"

class TestImagingReceiver : public QObject {

  Q_OBJECT

public:
  TestImagingReceiver(const int& portNumber,
      const int& numberOfImagesExpected
      );
  virtual ~TestImagingReceiver();

signals:
    void Done();

public slots:

  void Setup();
  void OnClientConnected();
  void OnClientDisconnected();

private slots:

  void OnMessageReceived(OIGTLMessage::Pointer);

private:

  void FinishUp();

  OIGTLSocketObject        m_Socket;
  const int                m_PortNumber;
  const int                m_NumberOfMessagesExpected;
  int                      m_CumulativeMessageCount;
  igtl::TimeStamp::Pointer m_StartTime;
  igtl::TimeStamp::Pointer m_EndTime;
  igtlUint64               m_CumulativeTime;
};

#endif TestImagingReceiver_H
