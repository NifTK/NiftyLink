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

class TestMsgTypes_Delivery : public QObject
{
  Q_OBJECT

signals:
  void Done();

public:
  TestMsgTypes_Delivery(void);
  ~TestMsgTypes_Delivery(void);

public slots:
  void StartTest();
  void QuitTest();

  void CatchMessage(NiftyLinkMessage::Pointer);

private slots:
  void ClientConnected();
  void ConnectedToRemote();
  void ContinueTest();

private:
  igtl::Matrix4x4       m_LocalMatrix;

  NiftyLinkMessage::Pointer m_MsgToSend;

  NiftyLinkSocketObject   * m_Socket1;
  NiftyLinkSocketObject   * m_Socket2;

  int    m_NumOfMsg;
  int    m_Received;
  int    m_Sent;
  int    m_TestCounter;
  int    m_SuccessCounter;

  bool   m_DoStream;
  bool   m_InShutdownTests;

  bool   m_ConnectedTo;
  bool   m_Connecting;
  QTimer m_TimeOut;
};

