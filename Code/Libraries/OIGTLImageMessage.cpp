/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLImageMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>

//-----------------------------------------------------------------------------
OIGTLImageMessage::OIGTLImageMessage(void)
	: OIGTLMessage()
{
	m_messageType = "IMAGE";
	m_message.operator =(NULL);
}


//-----------------------------------------------------------------------------
OIGTLImageMessage::~OIGTLImageMessage(void)
{
  //QLOG_INFO() <<"ImageMessage Destructor" <<m_messageType <<m_id;
}


//-----------------------------------------------------------------------------
OIGTLImageMessage::OIGTLImageMessage(const OIGTLImageMessage &other)
	: OIGTLMessage(other)
{
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetMatrix(matrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
  msgPointer->GetMatrix(matrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::PreserveMatrix()
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->PreserveMatrix();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetNormals(t, s, n);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetNormals(t, s, n);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setSpacing(float s[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetSpacing(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setSpacing(float si, float sj, float sk)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetSpacing(si, sj, sk);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getSpacing(float s[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetSpacing(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getSpacing(float &si, float &sj, float &sk)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetSpacing(si, sj, sk);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setOrigin(float s[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetOrigin(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setOrigin(float si, float sj, float sk)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetOrigin(si, sj, sk);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getOrigin(float s[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetOrigin(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::getOrigin(float &si, float &sj, float &sk)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetOrigin(si, sj, sk);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::SetDimensions(int s[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetDimensions(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::SetDimensions(int i, int j , int k)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetDimensions(i,j,k);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::GetDimensions(int s[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetDimensions(s);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::GetDimensions(int &i, int &j , int &k)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetDimensions(i,j,k);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
int OIGTLImageMessage::SetSubVolume(int dim[3], int off[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	int retv = msgPointer->SetSubVolume ( dim, off);
	msgPointer->Pack();
	
	return retv;
}


//-----------------------------------------------------------------------------
int OIGTLImageMessage::SetSubVolume(int dimi, int dimj , int dimk, int offi ,int offj , int offk)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	int retv=msgPointer->SetSubVolume (dimi,dimj,dimk,offi,offj,offk);
	msgPointer->Pack();

	return retv;
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::GetSubVolume(int dim[3], int off[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetSubVolume(dim,off);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::GetSubVolume(int &dimi, int &dimj, int &dimk, 
		int &offi, int &offj, int &offk)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetSubVolume(dimi, dimj, dimk, offi, offj,offk);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::SetNumComponents(int num)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::ImageMessage::New());

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetNumComponents(num);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
int OIGTLImageMessage::GetNumComponents()
{
	if (m_message.IsNull())
		return -1;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	int retv = msgPointer->GetNumComponents();
	msgPointer->Pack();
	
	return retv;
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
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
  int x = (int)((float(image.width()*0.3f)  * sin(phi)) +image.width()*0.5f  +0.5f);
  int y = (int)((float(image.height()*0.3f) * cos(phi)) +image.height()*0.5f +0.5f);
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
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::save(QString filename)
{
  if (m_message.IsNull() || filename.isEmpty())
    return;

  QImage image = getQImage();
	//getQImage currently does not set the color table
	//lets do it here. Insert a default index table for indexed image type
	if ( image.format() == QImage::Format_Indexed8)
	{
		QVector<QRgb> colors=QVector<QRgb> (256);
		for ( int i = 0 ; i < 256 ; i ++)
			colors[i] = qRgb(i,i,i);
		image.setColorTable(colors);
	}
  
  if (!image.isNull())
    image.save(filename);
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
void OIGTLImageMessage::setQImage(const QImage& inputImage)
{
  if (inputImage.isNull())
  {
    return;
  }

  if (m_message.IsNull())
  {
    m_message.operator =(igtl::ImageMessage::New());
  }

  // Cast pointer and unpack message
  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  // The aim of this stuff is to make QImage of type Format_Indexed8 as efficient as possible.
  unsigned long int byteSizeOfImage = 0;
  if ( inputImage.format() != QImage::Format_Indexed8 )
  {
    QImage image(inputImage);
    image.convertToFormat(QImage::Format_ARGB32);
    msgPointer->SetDimensions(image.width(), image.height(), 1);
    msgPointer->SetScalarType(igtl::ImageMessage::TYPE_UINT32);
    msgPointer->AllocateScalars();
    byteSizeOfImage = image.byteCount();
    memcpy(msgPointer->GetScalarPointer(), image.bits(), byteSizeOfImage);
  }
  else
  {
    msgPointer->SetDimensions(inputImage.width(), inputImage.height(), 1);
    msgPointer->SetScalarType(igtl::ImageMessage::TYPE_UINT8);
    msgPointer->AllocateScalars();
    byteSizeOfImage = inputImage.byteCount();

    // Copy image data to igtl::ImageMessage
    // for QImage::Format_Indexed8 this is going to lose the index table
    // This may or may not be a problem depending on what happens to the
    // image at the other end
    memcpy(msgPointer->GetScalarPointer(), inputImage.bits(), byteSizeOfImage);
  }

  // Pack message data
  msgPointer->Pack();
}

//-------------------------------------------------------------
QImage OIGTLImageMessage::getQImage(void)
{
  if (m_message.IsNull())
  {
    return QImage();
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
  msgPointer->Unpack();

  int i,j,k;
  msgPointer->GetDimensions(i,j,k);

  QImage image = QImage();
  if ( msgPointer->GetScalarType() == igtl::ImageMessage::TYPE_UINT32 )
  {
    image = QImage(i, j, QImage::Format_ARGB32);
  }
  else
  {
    if ( msgPointer->GetScalarType() == igtl::ImageMessage::TYPE_UINT8 )
    {
      image = QImage(i, j, QImage::Format_Indexed8);
      QVector<QRgb> colors = QVector<QRgb>(256);
      for ( int i = 0 ; i < 256 ; i ++)
      {
        colors[i] = qRgb(i,i,i);
      }
      image.setColorTable(colors);
    }
    else
    {
      QLOG_ERROR() << "OIGTLImageMessage::getQImage(void)" << ": Attempt to get QImage from image message of type " << msgPointer->GetScalarType() << " not implemented. \n";
      return QImage();
    }
  }

  int byteSizeOfImg = image.byteCount();
  memcpy(image.bits(), msgPointer->GetScalarPointer(), byteSizeOfImg);

  // Pack message data
  msgPointer->Pack();

  return image;
}

//*************************************************************
//       ASSOCIATED CONTROL MESSAGE TYPES
//*************************************************************

//-----------------------------------------------------------------------------
void OIGTLImageMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}


//-----------------------------------------------------------------------------
void OIGTLImageMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}
