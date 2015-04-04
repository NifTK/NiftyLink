/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTestingMacros_h
#define NiftyLinkTestingMacros_h

#include <QObject>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QsLog.h>
#include <QsLogDest.h>

#define NIFTYLINK_QTEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
  QCoreApplication app(argc, argv); \
  QsLogging::Logger& logger = QsLogging::Logger::instance(); \
  logger.setLoggingLevel(QsLogging::DebugLevel); \
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() ); \
  logger.addDestination(debugDestination.get()); \
  QTEST_DISABLE_KEYPAD_NAVIGATION \
  TestObject tc; \
  return QTest::qExec(&tc, argc, argv); \
}

#endif // NiftyLinkTestingMacros_h
