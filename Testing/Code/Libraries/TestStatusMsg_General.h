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

#include "OIGTLStatusMessage.h"

#include "igtlStatusMessage.h"
#include "igtlMath.h"
#include "igtl_util.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstdlib>

class TestStatusMsg_General : public QObject
{
  Q_OBJECT

signals:
  void done();

public:
  TestStatusMsg_General(void);
  ~TestStatusMsg_General(void);

public slots:
  void setupTest();
  void performTest();
  void quitTest();

private:
  QString m_localhostname;
  int m_testCounter;
  int m_successCounter;
};

