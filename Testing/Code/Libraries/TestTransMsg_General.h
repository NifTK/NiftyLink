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

class TestTransMsg_General : public QObject
{
  Q_OBJECT

signals:
  void Done();

public:
  TestTransMsg_General(void);
  ~TestTransMsg_General(void);

public slots:
  void SetupTest();
  void PerformTest();
  void QuitTest();

private:
  QString m_LocalHostname;
  int     m_TestCounter;
  int     m_SuccessCounter;
};

