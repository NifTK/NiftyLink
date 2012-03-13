/*=============================================================================

NiftyLink:  A software library to facilitate communication over OpenIGTLink.

http://cmic.cs.ucl.ac.uk/
http://www.ucl.ac.uk/

Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
Revision          : $Revision: 3300 $
Last modified by  : $Author: mjc $

Original author   : m.clarkson@ucl.ac.uk

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

============================================================================*/

#include <QObject>
#include <QtGui/QImage>

#include "igtlTransformMessage.h"
#include "igtlMath.h"
#include "igtl_util.h"

#include "igtlBindMessage.h"
#include "igtlColorTableMessage.h"
#include "igtlImageMessage.h"
#include "igtlImageMessage2.h"
#include "igtlImageMetaMessage.h"
#include "igtlLabelMetaMessage.h"
#include "igtlPointMessage.h"
#include "igtlPolyDataMessage.h"
#include "igtlTrajectoryMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include "OIGTLSocketObject.h"
#include "OIGTLTransformMessage.h"


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstdlib>

class TestSendReceive_Timing : public QObject
{
  Q_OBJECT

signals:
  void done();

public:
  TestSendReceive_Timing(void);
  ~TestSendReceive_Timing(void);

public slots:
  void setupTest();
  void performTest();
  void quitTest();

  void createMessage();

  void catchMessage(OIGTLMessage::Pointer);
  void recordSendTimestamps(unsigned long long timestamp);
  void sendResponse();
  void streamResponse();

private:
  igtl::Matrix4x4 m_localMatrix;

  OIGTLMessage::Pointer m_msgToSend;

  OIGTLSocketObject * m_socket1;
  OIGTLSocketObject * m_socket2;

  QList<ULONGLONG> m_socket1Timestamps;
  QList<ULONGLONG> m_socket2Timestamps;
  
  QList<OIGTLMessage::Pointer> m_socket1Messages;
  QList<OIGTLMessage::Pointer> m_socket2Messages;

  int m_numOfMsg;
  int m_received;
  int m_successCounter;
  int m_testCounter;

  bool   m_doStream;
  QTimer m_timeOut;
};

