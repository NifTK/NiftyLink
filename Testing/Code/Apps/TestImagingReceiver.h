/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestImagingReceiver_h
#define TestImagingReceiver_h

#include <QObject>
#include <QtGui/QImage>
#include <igtlTimeStamp.h>
#include "NiftyLinkSocketObject.h"

class TestImagingReceiver : public QObject
{

  Q_OBJECT

public:
  TestImagingReceiver(const int& portNumber,
                      const int& numberOfImagesExpected
                     );
  virtual ~TestImagingReceiver();

  inline void SetOutfilename(std::string outf) { m_OutFileName = outf; }

signals:
  void Done();

public slots:

  void Setup();
  void OnClientConnected();
  void OnClientDisconnected();

private slots:

  void OnMessageReceived(NiftyLinkMessage::Pointer);

private:

  void FinishUp();

  NiftyLinkSocketObject     m_Socket;
  const int                 m_PortNumber;
  const int                 m_NumberOfMessagesExpected;
  int                       m_CumulativeMessageCount;
  igtl::TimeStamp::Pointer  m_StartTime;
  igtl::TimeStamp::Pointer  m_EndTime;
  igtlUint64                m_CumulativeTime;
  unsigned long long        m_CumulativeMessageSize;
  
  std::vector<double> m_SentTimeStamps;
  std::vector<double> m_ReceivedTimeStamps;
  std::string         m_OutFileName;
};

#endif // TestImagingReceiver_h
