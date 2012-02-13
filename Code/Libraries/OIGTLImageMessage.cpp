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

#include "OIGTLImageMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLImageMessage::OIGTLImageMessage(void)
	: OIGTLMessage()
{
	m_messageType = "IMAGE";
	m_message.operator =(igtl::ImageMessage::New());

	update();
}

OIGTLImageMessage::~OIGTLImageMessage(void)
{
}

OIGTLImageMessage::OIGTLImageMessage(const OIGTLImageMessage &other)
	: OIGTLMessage(other)
{
}

void OIGTLImageMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetMatrix(matrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
    msgPointer->GetMatrix(matrix);

	//Pack message data
	msgPointer->Pack();

}

//void OIGTLImageMessage::setPosition(float p[3])
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->SetPosition(p);
//	update();
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::getPosition(float p[3])
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->GetPosition(p);
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::setPosition(float px, float py, float pz)
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->SetPosition(px, py, pz);
//	update();
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::getPosition(float &px, float &py, float &pz)
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->GetPosition(&px, &py, &pz);
//
//	//Pack message data
//	msgPointer->Pack();
//}

void OIGTLImageMessage::setNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(o);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(o);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(t, s, n);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(t, s, n);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::initializeWithTestData(void)
{
  if (m_message.IsNull())
    return;

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  //-------------------------------------------------------------
  // Load test image from resources
  QImage image;
  image.load(":/NiftyLink/UCL_LOGO.tif");

  //-------------------------------------------------------------
  // Set parameters
  msgPointer->SetDimensions(image.width(), image.height(), 1);
  msgPointer->SetSpacing(1.0f, 1.0f, 1.0f);
  msgPointer->SetScalarType(igtl::ImageMessage::TYPE_UINT32);
  //msgPointer->AllocateCompressedImage(image.byteCount());
  msgPointer->AllocateScalars();
  
  //int fsize = msgPointer->GetImageSize();
  int byteSizeOfImg = image.byteCount();
  memcpy(msgPointer->GetScalarPointer(), image.bits(), byteSizeOfImg);

  igtl::Matrix4x4 matrix;
  igtl::IdentityMatrix(matrix);
  msgPointer->SetMatrix(matrix);



//	if (m_message.IsNull())
//		return;

//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();

//    msgPointer->SetMatrix(dummyTransformMatrix);

  // Update timestamp, hostname and such
  update();

  //Pack message data
  msgPointer->Pack();
}

void OIGTLImageMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	igtl::Matrix4x4 localMatrix;
    CreateRandomTransformMatrix(localMatrix);

    msgPointer->SetMatrix(localMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::save(QString filename)
{
  if (m_message.IsNull())
    return;

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  if (!filename.isNull())
  {
    int i,j,k;
    msgPointer->GetDimensions(i,j,k);
    QImage image(i, j, QImage::Format_ARGB32);
    
    int byteSizeOfImg = image.byteCount();
    memcpy(image.bits(), msgPointer->GetScalarPointer(), byteSizeOfImg);

    image.save(filename);
  }

  //Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}
