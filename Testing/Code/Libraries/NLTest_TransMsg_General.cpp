/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

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

#include "NiftyLinkSocketObject.h"
#include "TestTransMsg_General.h"

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

  TestTransMsg_General * test = new TestTransMsg_General;
  QObject::connect(test, SIGNAL(Done()), &app, SLOT(quit()),Qt::QueuedConnection);
  test->SetupTest();

  QTimer::singleShot(220, test, SLOT(PerformTest()));
  //QTimer::singleShot(220, &test, SLOT(listen()));
  int ret = app.exec();
  delete test;

  std::cerr <<"TestClass Deleted";
  return ret;
}
