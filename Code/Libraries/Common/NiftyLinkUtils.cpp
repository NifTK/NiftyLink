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

#include "NiftyLinkUtils.h"

bool validateIp(const QString &inputIP)
{
	if (inputIP == QString("localhost"))
		return true;

	QStringList nums = inputIP.split(".");

	if (nums.count() !=4)
		return false;

	for (int i = 0; i < 4; i++)
	{
		bool ok = false;
		int frag = nums.at(i).toInt(&ok, 10);
		
		if (!ok) return false;
		else if (frag < 0 || frag > 255) return false;
	}
	
	return true;
}

QString resolveHostName(const QString &input)
{
	QHostInfo info = QHostInfo::fromName(input);
	return info.addresses().first().toString();
}

QString getLocalHostAddress(void)
{
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
	//qDebug() <<"LocalHost: " <<info.localHostName();
	QList<QHostAddress> addresses = info.addresses();

	//qDebug() <<"Has addresses: " <<addresses.count();

	for (int i=0; i< addresses.count(); i++)
	{
		if( validateIp(addresses.at(i).toString()) == true )
		{
			qDebug() <<addresses.at(i).toString();
			return addresses.at(i).toString();
		}
	}
	
	return QString("UNKNOWN");
}

int NiftyLinkDummyFunction1()
{
  return 0;
}

void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix)
{
  float position[3];
  float orientation[4];

  // random position
  static float phi = 0.0;
  position[0] = 50.0 * cos(phi);
  position[1] = 50.0 * sin(phi);
  position[2] = 50.0 * cos(phi);
  phi = phi + 0.2;

  // random orientation
  static float theta = 0.0;
  orientation[0]=0.0;
  orientation[1]=0.6666666666*cos(theta);
  orientation[2]=0.577350269189626;
  orientation[3]=0.6666666666*sin(theta);
  theta = theta + 0.1;

  //igtl::Matrix4x4 matrix;
  igtl::QuaternionToMatrix(orientation, matrix);

  matrix[0][3] = position[0];
  matrix[1][3] = position[1];
  matrix[2][3] = position[2];
  
  //igtl::PrintMatrix(matrix);
}

void CreateRandomTransformMsg(OIGTLMessage::Pointer &msgToSend)
{
	msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::TransformMessage::Pointer transMsg;
    transMsg = igtl::TransformMessage::New();
		
	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();
	ts->GetTime();

	igtl::Matrix4x4 localMatrix;
    CreateRandomTransformMatrix(localMatrix);

	QString lhn = getLocalHostAddress();

	transMsg->SetTimeStamp(ts);
	transMsg->SetDeviceName(lhn.toStdString().c_str());
    transMsg->SetMatrix(localMatrix);

	//Pack message data
	transMsg->Pack();

	//Embed igtl message data into OIGTLMessage
    msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);
	//msgToSend->setHostName(getLocalHostAddress());
}

void CreateTestTransformMsg(OIGTLMessage::Pointer &msgToSend)
{
	msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::TransformMessage::Pointer transMsg;
    transMsg = igtl::TransformMessage::New();
		
	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();
	ts->GetTime();

	QString lhn = getLocalHostAddress();

	transMsg->SetTimeStamp(ts);
	transMsg->SetDeviceName(lhn.toStdString().c_str());
    transMsg->SetMatrix(dummyTransformMatrix);

	//Pack message data
	transMsg->Pack();

	//Embed igtl message data into OIGTLMessage
    msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);
	//msgToSend->setHostName(getLocalHostAddress());
}

void CreateTransformMsg(OIGTLMessage::Pointer &msgToSend, igtl::Matrix4x4 &matrix)
{
    msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::TransformMessage::Pointer transMsg;
    transMsg = igtl::TransformMessage::New();
		
	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();
	ts->GetTime();

	QString lhn = getLocalHostAddress();

	transMsg->SetTimeStamp(ts);
	transMsg->SetDeviceName(lhn.toStdString().c_str());
    transMsg->SetMatrix(matrix);

	//Pack message data
	transMsg->Pack();

	//Embed igtl message data into OIGTLMessage
    msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);
	//msgToSend->setHostName(getLocalHostAddress());
}

void CreateGetTransformMsg(OIGTLMessage::Pointer &msgToSend)
{
	msgToSend.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetTransformMessage::Pointer getTrMsg;
	getTrMsg.operator =(igtl::GetTransformMessage::New());

	igtl::GetTransformMessage::New();
    
	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();
	ts->GetTime();

	QString lhn = getLocalHostAddress();

	//Set parameters
	getTrMsg->SetTimeStamp(ts);
	getTrMsg->SetDeviceName(lhn.toStdString().c_str());
   
	//Pack message data
	getTrMsg->Pack();

    msgToSend->setMessagePointer((igtl::MessageBase::Pointer) getTrMsg);
}

void GetTestImageMsg(OIGTLMessage::Pointer &msg)
{}
void GetTestPositionMsg(OIGTLMessage::Pointer &msg)
{}
void GetTestPointMsg(OIGTLMessage::Pointer &msg)
{}
void GetTestStatusMsg(OIGTLMessage::Pointer &msg)
{}
void GetTestStringMsg(OIGTLMessage::Pointer &msg)
{}


bool CompareMsgData(OIGTLMessage::Pointer &msg1, OIGTLMessage::Pointer &msg2)
{
	igtl::MessageBase::Pointer basePointer1 = msg1->getMessagePointer();
	igtl::MessageBase::Pointer basePointer2 = msg2->getMessagePointer();

	if (strcmp(basePointer1->GetNameOfClass(), basePointer2->GetNameOfClass()) != 0)
	{
		QLOG_ERROR() <<"CompareMsgData: Incompatible message types, cannot comapre them" <<endl;
		return false;
	}

    if (strcmp(basePointer1->GetNameOfClass(), "igtl::TransformMessage") == 0)
    {
        igtl::TransformMessage::Pointer msgPointer1 = static_cast<igtl::TransformMessage *>(basePointer1.GetPointer());
        msgPointer1->Unpack();

        igtl::TransformMessage::Pointer msgPointer2 = static_cast<igtl::TransformMessage *>(basePointer2.GetPointer());
        msgPointer2->Unpack();
		
		igtl::Matrix4x4 matrix1;
		igtl::Matrix4x4 matrix2;

        msgPointer1->GetMatrix(matrix1);
		msgPointer2->GetMatrix(matrix2);

        int r = memcmp((const void*)&matrix1, (const void*)matrix2, sizeof(igtl::Matrix4x4));

        if (r != 0)
		{
			std::cout <<"Data do not match!" <<std::endl;
			std::cout <<"Matrix1: " <<std::endl;
	        igtl::PrintMatrix(matrix1);
	        std::cout <<std::endl;

			std::cout <<"Matrix2: " <<std::endl;
			igtl::PrintMatrix(matrix2);
	        std::cout <<std::endl;

			QLOG_ERROR() <<"CompareMsgData: Data fields are not equal!" <<endl;
			return false;
		}
		else
		{
			QLOG_INFO() <<"CompareMsgData: Data fields are equal";
			return true;
		}
    }
	else if (strcmp(basePointer1->GetNameOfClass(), "igtl::StatusMessage") == 0)
	{
		igtl::StatusMessage::Pointer msgPointer1 = static_cast<igtl::StatusMessage *>(basePointer1.GetPointer());
        msgPointer1->Unpack();

        igtl::StatusMessage::Pointer msgPointer2 = static_cast<igtl::StatusMessage *>(basePointer2.GetPointer());
        msgPointer2->Unpack();
		
		if ( msgPointer1->GetCode() != msgPointer2->GetCode() ||
			msgPointer1->GetSubCode() != msgPointer2->GetSubCode() ||
			strcmp(msgPointer1->GetStatusString(), msgPointer2->GetStatusString()) != 0 ||
			strcmp(msgPointer1->GetErrorName(), msgPointer2->GetErrorName()) != 0 )
		{
			std::cout <<"Data do not match!" <<std::endl;
			std::cout <<"ErrorCode 1: " <<msgPointer1->GetCode() <<std::endl;
			std::cout <<"ErrorSubCode 1: " <<msgPointer1->GetSubCode() <<std::endl;
			std::cout <<"ErrorString 1: " <<msgPointer1->GetErrorName() <<std::endl;
			std::cout <<"StatusString 1: " <<msgPointer1->GetStatusString() <<std::endl;
			
			std::cout <<"ErrorCode 2: " <<msgPointer2->GetCode() <<std::endl;
			std::cout <<"ErrorSubCode 2: " <<msgPointer2->GetSubCode() <<std::endl;
			std::cout <<"ErrorString 2: " <<msgPointer2->GetErrorName() <<std::endl;
			std::cout <<"StatusString 2: " <<msgPointer2->GetStatusString() <<std::endl;
	        std::cout <<std::endl;

			QLOG_ERROR() <<"CompareMsgData: Data fields are not equal!" <<endl;
			return false;
		}
		else
		{
			QLOG_INFO() <<"CompareMsgData: Data fields are equal";
			return true;
		}
	}
	else
	{
		QLOG_ERROR() <<"CompareMsgData: Unknown message type, cannot compare" <<endl;
		return false;
	}
	
	//should never get here, really
	return false;
}