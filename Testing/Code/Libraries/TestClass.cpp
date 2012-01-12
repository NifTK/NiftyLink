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

void TestClass::setupTest()
{
	m_socket1 = new OIGTLSocketObject();
	m_socket1->setObjectName("Socket1");
	m_socket2 = new OIGTLSocketObject();
	m_socket2->setObjectName("Socket2");

	connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage * )), this, SLOT(catchMessage(OIGTLMessage * )) );
	
	QUrl url;
	url.setHost(QString("localhost"));
	url.setPort(3200);

	//Start sender / listener
	m_socket1->listenOnPort(3200);
	m_socket2->connectToRemote(url);

	m_successCounter = 0;
	m_numOfMsg = 10;
	m_received = 0;
}

void TestClass::performTest()
{
	//Create a message and send it
	OIGTLMessage * msgToSend = new OIGTLMessage();
	msgToSend->setHostName(QString("MURBELLA_O"));
	msgToSend->setMessageType(QString("TRANSFORM"));

	igtl::TransformMessage::Pointer transMsg;
	transMsg = igtl::TransformMessage::New();
	transMsg->SetDeviceName("MURBELLA_O"); 

	
	GetRandomTestMatrix(m_localMatrix);

	igtl::PrintMatrix(m_localMatrix);

	transMsg->SetMatrix(m_localMatrix);
	transMsg->Pack();

	msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);

	//for (int i = 0; i< m_numOfMsg; i++)
	for (int i = 0; i< 10; i++)
	{
		m_socket2->sendMessage(msgToSend);
	}
}

void TestClass::quitTest()
{
	m_socket1->closeSocket();
	m_socket2->closeSocket();

	delete m_socket1;
	delete m_socket2;
	emit done();
}

void TestClass::catchMessage(OIGTLMessage * msg)
{
	m_received++;
	
	if (msg != NULL)
	{
		QLOG_INFO() <<m_received <<". message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

		igtl::MessageBase::Pointer message = msg->getMessagePointer();
		QLOG_INFO() <<message->GetNameOfClass();
		
		if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
		{
			igtl::TransformMessage::Pointer poi = static_cast<igtl::TransformMessage *>(message.GetPointer());
			poi->Unpack();

			igtl::Matrix4x4 receivedMatrix;

			poi->GetMatrix(receivedMatrix);
			
			int r = memcmp((const void*)&receivedMatrix, (const void*)m_localMatrix, sizeof(igtl::Matrix4x4));
			igtl::PrintMatrix(receivedMatrix);

			if (r != 0)
				QLOG_ERROR() <<"Shit happens";
		}
	}


	
	if (m_received >= m_numOfMsg)
		quitTest();

}