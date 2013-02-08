/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include <QObject>
#include <QtTest/QtTest>

// Taken from: http://doc.qt.nokia.com/latest/qtestlib-manual.html

class UtilsTests: public QObject
{
  Q_OBJECT

private slots:
     void InitTestCase();
     void CleanupTestCase();
     void TimeStampSetGetDifferenceTest();
 };
