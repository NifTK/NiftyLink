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

#include <QtGui/QImage>
#include <QtGui/QPainter>

OIGTLImageMessage::OIGTLImageMessage(void)
	: OIGTLMessage()
{
	m_messageType = "IMAGE";
	m_message.operator =(NULL);
}

OIGTLImageMessage::~OIGTLImageMessage(void)
{
  //QLOG_INFO() <<"ImageMessage Destructor" <<m_messageType <<m_id;
}

OIGTLImageMessage::OIGTLImageMessage(const OIGTLImageMessage &other)
	: OIGTLMessage(other)
{
}

void OIGTLImageMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetMatrix(matrix);

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

void OIGTLImageMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(t, s, n);

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

void OIGTLImageMessage::setSpacing(float s[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetSpacing(s);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::setSpacing(float si, float sj, float sk)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetSpacing(si, sj, sk);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getSpacing(float s[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetSpacing(s);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getSpacing(float &si, float &sj, float &sk)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetSpacing(si, sj, sk);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::setOrigin(float s[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetOrigin(s);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::setOrigin(float si, float sj, float sk)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetOrigin(si, sj, sk);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getOrigin(float s[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetOrigin(s);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getOrigin(float &si, float &sj, float &sk)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetOrigin(si, sj, sk);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::initializeWithTestData(void)
{
  if (m_message.IsNull())
    m_message.operator =(igtl::ImageMessage::New());

  //-------------------------------------------------------------
  // Load test image from resources
  QImage image;
  image.load(":/NiftyLink/UCL_LOGO.tif");

  if (!image.isNull())
    setQImage(image);
}

void OIGTLImageMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  // Load test image from resources
  QImage image;
  image.load(":/NiftyLink/UCL_LOGO.tif");

  // random position
  static float phi = 0.2;
  int x = (int)((image.width()-30)  * sin(phi));
  int y = (int)((image.height()-30) * cos(phi));
  phi = phi + 0.2;
  
  QRect rectangle(x, y, 30, 30);

  QPainter painter(&image);
  QPen pen(Qt::red);
  painter.setPen(pen);
  painter.setBrush(Qt::Dense6Pattern);
  painter.fillRect(rectangle, Qt::red);

  if (!image.isNull())
    setQImage(image);

	igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  msgPointer->SetMatrix(localMatrix);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::save(QString filename)
{
  if (m_message.IsNull() || filename.isEmpty())
    return;

  QImage image = getQImage();
  
  if (!image.isNull())
    image.save(filename);
}

void OIGTLImageMessage::load(QString filename)
{
  if (filename.isEmpty())
    return;

  // Load image from file
  QImage image;
  image.load(filename);

  if (!image.isNull())
    setQImage(image);
}

void OIGTLImageMessage::setQImage(QImage image)
{
  if (image.isNull())
    return;
  else
    image.convertToFormat(QImage::Format_ARGB32);
  
  if (m_message.IsNull())
    m_message.operator =(igtl::ImageMessage::New());

  // Cast pointer and unpack message
  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  //-------------------------------------------------------------
  // Set parameters
  msgPointer->SetDimensions(image.width(), image.height(), 1);
  msgPointer->SetSpacing(1.0f, 1.0f, 1.0f);
  msgPointer->SetScalarType(igtl::ImageMessage::TYPE_UINT32);
  msgPointer->AllocateScalars();

  // Copy image data to igtl::ImageMessage
  int byteSizeOfImg = image.byteCount();
  memcpy(msgPointer->GetScalarPointer(), image.bits(), byteSizeOfImg);

  //Pack message data
  msgPointer->Pack();

}

QImage OIGTLImageMessage::getQImage(void)
{
  if (m_message.IsNull())
    return QImage();

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  int i,j,k;
  msgPointer->GetDimensions(i,j,k);
  QImage image(i, j, QImage::Format_ARGB32);
  
  int byteSizeOfImg = image.byteCount();
  memcpy(image.bits(), msgPointer->GetScalarPointer(), byteSizeOfImg);

  //Pack message data
	msgPointer->Pack();

  return image;
}

//*************************************************************
//       ASSOCIATED CONTROL MESSAGE TYPES
//*************************************************************

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
