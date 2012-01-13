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
//#include <iostream>
//#include <math.h>

void TestClass::setupTest()
{
	m_socket1 = new OIGTLSocketObject();
	m_socket1->setObjectName("Socket1");
	m_socket2 = new OIGTLSocketObject();
	m_socket2->setObjectName("Socket2");

	connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
	connect(m_socket2, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
	
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
	//OIGTLMessage * msgToSend = new OIGTLMessage();
	m_msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));
	m_msgToSend->setHostName(QString("MURBELLA_O"));
	m_msgToSend->setMessageType(QString("TRANSFORM"));

	igtl::TransformMessage::Pointer transMsg;
	transMsg = igtl::TransformMessage::New();
	transMsg->SetDeviceName("MURBELLA_O"); 

	
	GetRandomTestMatrix(m_localMatrix);

	std::cout <<"Original matrix: " <<std::endl <<std::endl;
	igtl::PrintMatrix(m_localMatrix);
	std::cout <<std::endl;

	transMsg->SetMatrix(m_localMatrix);
	transMsg->Pack();

	m_msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);

	for (int i = 0; i< m_numOfMsg; i++)
	{
		m_socket2->sendMessage(m_msgToSend);
	}

	OIGTLMessage::Pointer reqMsg(new OIGTLMessage());
	igtl::GetTransformMessage::Pointer getTrMsg;
	getTrMsg = igtl::GetTransformMessage::New();
	getTrMsg->SetDeviceName("MURBELLA_O");
	getTrMsg->Pack();

	reqMsg->setMessagePointer((igtl::MessageBase::Pointer) getTrMsg);

	m_socket2->sendMessage(reqMsg);
}

void TestClass::quitTest()
{
	m_socket1->closeSocket();
	m_socket2->closeSocket();

	delete m_socket1;
	delete m_socket2;
	emit done();
}

void TestClass::catchMessage(OIGTLMessage::Pointer msg)
{
	QString sender = QObject::sender()->objectName();
	
	if (msg.operator!=(NULL))
	{
		QLOG_INFO() <<m_received <<"of" <<m_numOfMsg <<"message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

		igtl::MessageBase::Pointer message = msg->getMessagePointer();
		QLOG_INFO() <<message->GetNameOfClass();
		
		if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
		{
			igtl::TransformMessage::Pointer poi = static_cast<igtl::TransformMessage *>(message.GetPointer());
			poi->Unpack();

			igtl::Matrix4x4 receivedMatrix;

			poi->GetMatrix(receivedMatrix);

			int r = memcmp((const void*)&receivedMatrix, (const void*)m_localMatrix, sizeof(igtl::Matrix4x4));
			
			if (r == 0)
				std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received <<" of " <<m_numOfMsg*2 <<": OK" <<std::endl;
			else
				std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received <<" of " <<m_numOfMsg*2 <<": NOT-OK" <<std::endl;

			igtl::PrintMatrix(receivedMatrix);

			std::cout <<std::endl;

			if (r != 0)
				QLOG_ERROR() <<"Shit happens";

			m_received++;
		}
		else if (strcmp(message->GetNameOfClass(), "igtl::GetTransformMessage") == 0)
		{
			std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
			sendResponse();
		}
	}


	
	if (m_received >= 2*m_numOfMsg)
		quitTest();

}

void TestClass::sendResponse()
{
	for (int i = 0; i< m_numOfMsg; i++)
	{
		m_socket1->sendMessage(m_msgToSend);
	}
}