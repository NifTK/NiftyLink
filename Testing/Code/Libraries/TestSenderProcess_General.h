/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#define TESTING

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

#include "OIGTLSenderProcess.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstdlib>

class TestSenderProcess_General : public QObject
{
  Q_OBJECT

signals:
  void done();

public:
  TestSenderProcess_General(void);
  ~TestSenderProcess_General(void);

public slots:
  void setupTest();
  void performTest();
  void quitTest();

private:
  QString m_localhostname;
  int m_testCounter;
  int m_successCounter;
};

