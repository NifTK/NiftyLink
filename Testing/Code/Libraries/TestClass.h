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
#include "igtlTransformMessage.h"
#include "igtlMath.h"


#include "QsLog.h"
#include "QsLogDest.h"

#include "OIGTLSocketObject.h"

// Taken from: http://doc.qt.nokia.com/latest/qtestlib-manual.html

class TestClass : public QObject
{
	Q_OBJECT

signals:
	void done();

public slots:
	void setupTest();
	void performTest();
	void quitTest();

	void catchMessage(OIGTLMessage::Pointer);

private:
	igtl::Matrix4x4 m_localMatrix;

	OIGTLSocketObject * m_socket1;
	OIGTLSocketObject * m_socket2;

	int m_numOfMsg;
	int m_received;
	int m_successCounter;
};

