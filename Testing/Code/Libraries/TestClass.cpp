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

#include "stdlib.h"
#include <QDebug>
#include "TestClass.h"

void TestClass::performTest()
{
	OIGTLSocketObject * socket1 = new OIGTLSocketObject();
	delete socket1;

	socket1 = new OIGTLSocketObject();

	OIGTLSocketObject * socket2 = new OIGTLSocketObject();
	MessageCatcher * catcher = new MessageCatcher();

	connect(socket1, SIGNAL(messageReceived(OIGTLMessage * )), catcher, SLOT(catchMessage(OIGTLMessage * )) );
	connect(socket1, SIGNAL(testSignal()), catcher, SLOT(catchMessage( )), Qt::DirectConnection);
	//QSignalSpy * spy = new QSignalSpy(socket1, SIGNAL(messageReceived(OIGTLMessage * )));
	QSignalSpy * spy = new QSignalSpy(socket1, SIGNAL(testSignal()));

	QUrl url;
	url.setHost(QString("localhost"));
	url.setPort(3200);



	socket1->listenOnPort(3200);
	socket2->connectToRemote(url);

	OIGTLMessage * msgToSend = new OIGTLMessage();
	msgToSend->setHostName(QString("MURBELLA_O"));
	msgToSend->setMessageType(QString("TRANSFORM"));

	igtl::TransformMessage::Pointer transMsg;
	transMsg = igtl::TransformMessage::New();
	transMsg->SetDeviceName("Tracker"); 

	igtl::Matrix4x4 matrix;
	GetRandomTestMatrix(matrix);
	transMsg->SetMatrix(matrix);
	transMsg->Pack();

	msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);

	while (!catcher->isMessageValid())
	{
	igtl::Sleep(500);
	socket2->sendMessage(msgToSend);
	qDebug() <<"MessageSignal received: " <<spy->count() <<endl;
	}

	OIGTLMessage * recievedMsg = catcher->getMessage();

	//igtl::MessageBase::Pointer recievedIGTMsg = recievedMsg->getMessagePointer();

	//igtl::TransformMessage::Pointer receivedTransMsg;

	//receivedTransMsg.operator =(recievedIGTMsg.GetPointer());
	//
	//igtl::Matrix4x4 receivedMatrix;

	//receivedTransMsg->GetMatrix(receivedMatrix);

	//if (matrix != receivedMatrix)
	//{
	// qDebug() <<"shit happenes";
	//}

	emit done();
}