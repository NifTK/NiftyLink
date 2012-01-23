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

    connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );
    connect(m_socket2, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );

    QUrl url;
    //url.setHost(QString("128.16.4.235"));
	url.setHost(QString("localhost"));
    url.setPort(3200);

    //Start sender / listener
    m_socket1->listenOnPort(3200);
    m_socket2->connectToRemote(url);

    m_successCounter = 0;
    m_numOfMsg = 1;
    m_received = 0;

    m_doStream = false;

	m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
	//m_msgToSend->initializeWithTestData();

	static_cast<OIGTLTransformMessage::Pointer>(m_msgToSend)->setMatrix(dummyTransformMatrix);

}

void TestClass::setupTest2()
{
    m_socket1 = new OIGTLSocketObject();
    m_socket1->setObjectName("Socket1");

    connect(m_socket1, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMessage(OIGTLMessage::Pointer )) );

    //Start sender / listener
    m_socket1->listenOnPort(3200);
    //m_socket2->connectToRemote(url);

    m_successCounter = 0;
    m_numOfMsg = 10;
    m_received = 0;

    m_doStream = false;

    m_msgToSend.operator =(OIGTLTransformMessage::Pointer(new OIGTLTransformMessage()));
	
	m_msgToSend->initializeWithTestData();
}

void TestClass::performTest()
{

    for (int i = 0; i< m_numOfMsg; i++)
    {
        m_socket2->sendMessage(m_msgToSend);
    }

    OIGTLMessage::Pointer reqMsg;

    OIGTLTransformMessage::Create_GET(reqMsg);
    //Create_GET_TransformMsg(reqMsg);
    m_socket2->sendMessage(reqMsg);
}

void TestClass::quitTest()
{
    if (m_socket1 != NULL)
        m_socket1->closeSocket();

    if (m_socket2 != NULL)
        m_socket2->closeSocket();

    if (m_socket1 != NULL)
        delete m_socket1;

    if (m_socket2 != NULL)
        delete m_socket2;
    emit done();
}

void TestClass::catchMessage(OIGTLMessage::Pointer msg)
{
    QString sender = QObject::sender()->objectName();

    if (msg.operator!=(NULL))
    {
        QLOG_INFO() <<m_received+1 <<"of" <<m_numOfMsg <<"message received: " <<msg->getHostName() <<":" <<msg->getPort() <<" " <<msg->getMessageType();

        igtl::MessageBase::Pointer message;
		msg->getMessagePointer(message);
        QLOG_INFO() <<message->GetNameOfClass();

        if (strcmp(message->GetNameOfClass(), "igtl::TransformMessage") == 0)
        {
            igtl::TransformMessage::Pointer poi = static_cast<igtl::TransformMessage *>(message.GetPointer());
            poi->Unpack();

            igtl::Matrix4x4 receivedMatrix;

            poi->GetMatrix(receivedMatrix);

            int r = memcmp((const void*)&receivedMatrix, (const void*)dummyTransformMatrix, sizeof(igtl::Matrix4x4));

            if (r == 0)
                std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received+1 <<" of " <<m_numOfMsg*2 <<": OK" <<std::endl;
            else
                std::cout <<sender.toStdString().c_str() <<" received matrix " <<m_received+1 <<" of " <<m_numOfMsg*2 <<": NOT-OK" <<std::endl;

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
        else if (strcmp(message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
        {
            std::cout <<sender.toStdString().c_str() <<" received message request, sending response" <<std::endl;
            m_doStream = true;
            streamResponse();
        }
        else if (strcmp(message->GetNameOfClass(), "igtl::StopTransformMessage") == 0)
        {
            std::cout <<sender.toStdString().c_str() <<" received message request, stopping data stream" <<std::endl;
            m_doStream = false;
        }
    }


    if (m_received >= 2*m_numOfMsg)
        //if (m_received >= 2*m_numOfMsg)
        quitTest();

}

void TestClass::sendResponse()
{
    QLOG_INFO() <<"Preparing to send response messages..." <<endl;

    for (int i = 0; i< m_numOfMsg; i++)
    {
        m_socket1->sendMessage(m_msgToSend);
    }
}

void TestClass::streamResponse()
{
    QLOG_INFO() <<"Preparing to send response stream..." <<endl;

    for (int i = 0; i< 5; i++)
    //while (m_doStream)
    {
        m_socket1->sendMessage(m_msgToSend);
        //igtl::Sleep(500);

    }
}

void TestClass::createMessage()
{

    m_msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));
    //m_msgToSend->setHostName(QString("MURBELLA_O"));
    //m_msgToSend->setMessageType(QString("TRANSFORM"));

    igtl::TransformMessage::Pointer transMsg;
    transMsg = igtl::TransformMessage::New();
    transMsg->SetDeviceName("MURBELLA_O");


    CreateRandomTransformMatrix(m_localMatrix);

    std::cout <<"Original matrix: " <<std::endl <<std::endl;
    igtl::PrintMatrix(m_localMatrix);
    std::cout <<std::endl;

    transMsg->SetMatrix(m_localMatrix);
    transMsg->Pack();

    m_msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);
}
