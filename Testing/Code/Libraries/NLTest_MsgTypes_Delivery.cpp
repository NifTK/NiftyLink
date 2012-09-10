/*=============================================================================

NiftyGuide: A software package for light-weight client applications for
Image Guided Interventions, developed at University College London.

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
#include <QtGui/QApplication>
//#include <QtCore/QCoreApplication>
//#include <QtCore/QApplication>

#include <QDebug>
#include <QSignalSpy>
#include <QThread>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "igtlTransformMessage.h"
#include "igtlMath.h"

#include "OIGTLSocketObject.h"
#include "TestMsgTypes_Delivery.h"

#include "QsLog.h"
#include "QsLogDest.h"


int main(int argc, char **argv)
{
  QCoreApplication app(argc,argv);

  // init the logging mechanism
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::TraceLevel);
  const QString sLogPath(QDir(app.applicationDirPath()).filePath("niftylink_log.txt"));
  QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());
  logger.addDestination(fileDestination.get());

  int ret = 0;

  TestMsgTypes_Delivery * test = new TestMsgTypes_Delivery;
  QObject::connect(test, SIGNAL(done()), &app, SLOT(quit()),Qt::QueuedConnection);
  QTimer::singleShot(0, test, SLOT(startTest()));
  //QTimer::singleShot(220, &test, SLOT(listen()));
  ret = app.exec();
  
  
  delete test;

  std::cerr <<"TestClass Deleted";
  return ret;
}

