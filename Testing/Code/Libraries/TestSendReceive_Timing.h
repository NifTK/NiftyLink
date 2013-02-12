/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

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

#include "NiftyLinkSocketObject.h"
#include "NiftyLinkTransformMessage.h"


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstdlib>

class TestSendReceive_Timing : public QObject
{
  Q_OBJECT

signals:
  void Done();

public:
  TestSendReceive_Timing(void);
  ~TestSendReceive_Timing(void);

public slots:
  void SetupTest();
  void PerformTest();
  void QuitTest2();

  void CatchMessage(NiftyLinkMessage::Pointer);
  void SendResponse();
  void StreamResponse();

private:
  igtl::Matrix4x4              m_LocalMatrix;

  NiftyLinkMessage::Pointer        m_MsgToSend;

  NiftyLinkSocketObject          * m_Socket1;
  NiftyLinkSocketObject          * m_Socket2;

  QList<ULONGLONG>             m_Socket1Timestamps;
  QList<ULONGLONG>             m_Socket2Timestamps;

  QList<NiftyLinkMessage::Pointer> m_Socket1Messages;
  QList<NiftyLinkMessage::Pointer> m_Socket2Messages;
  unsigned long long           m_TotalTimeDiff;

  int    m_NumOfMsg;
  int    m_Received;
  int    m_SuccessCounter;
  int    m_TestCounter;

  bool   m_DoStream;
  QTimer m_TimeOut;
};

