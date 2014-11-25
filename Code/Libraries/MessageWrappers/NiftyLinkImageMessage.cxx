/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkImageMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkImageMessage::NiftyLinkImageMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "IMAGE";
  m_Message.operator = (NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkImageMessage::~NiftyLinkImageMessage(void)
{
  //QLOG_INFO() <<"ImageMessage Destructor" <<m_MessageType <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkImageMessage::NiftyLinkImageMessage(const NiftyLinkImageMessage &other)
  : NiftyLinkMessage(other)
{
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetMatrix(matrix);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetMatrix(matrix);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetNormals(t, s, n);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetNormals(t, s, n);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetSpacing(float s[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetSpacing(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetSpacing(float si, float sj, float sk)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetSpacing(si, sj, sk);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetSpacing(float s[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetSpacing(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetSpacing(float &si, float &sj, float &sk)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetSpacing(si, sj, sk);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetOrigin(float s[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetOrigin(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetOrigin(float si, float sj, float sk)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetOrigin(si, sj, sk);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetOrigin(float s[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetOrigin(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetOrigin(float &si, float &sj, float &sk)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetOrigin(si, sj, sk);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetDimensions(int s[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetDimensions(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetDimensions(int i, int j , int k)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetDimensions(i, j, k);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetDimensions(int s[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetDimensions(s);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetDimensions(int &i, int &j , int &k)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetDimensions(i, j, k);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
int NiftyLinkImageMessage::SetSubVolume(int dim[3], int off[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  int retv = msgPointer->SetSubVolume ( dim, off);
  //msgPointer->Pack();

  return retv;
}


//-----------------------------------------------------------------------------
int NiftyLinkImageMessage::SetSubVolume(int dimi, int dimj , int dimk, int offi , int offj , int offk)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  int retv = msgPointer->SetSubVolume (dimi, dimj, dimk, offi, offj, offk);
  //msgPointer->Pack();

  return retv;
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetSubVolume(int dim[3], int off[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetSubVolume(dim, off);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::GetSubVolume(int &dimi, int &dimj, int &dimk, int &offi, int &offj, int &offk)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetSubVolume(dimi, dimj, dimk, offi, offj, offk);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetNumComponents(int num)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetNumComponents(num);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
int NiftyLinkImageMessage::GetNumComponents()
{
  if (m_Message.IsNull())
  {
    return -1;
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  int retv = msgPointer->GetNumComponents();
  //msgPointer->Pack();

  return retv;
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::InitializeWithTestData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  //-------------------------------------------------------------
  // Load test image from resources
  QImage image;
  image.load(":/NiftyLink/UCL_LOGO.tif");

  if (!image.isNull())
  {
    SetQImage(image);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::InitializeWithRandomData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());
  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }

  // Load test image from resources
  QImage image;
  image.load(":/NiftyLink/UCL_LOGO.tif");

  // random position
  static float phi = 0.2;
  int x = (int)((float(image.width() * 0.3f) * sin(phi)) + image.width() * 0.5f + 0.5f);
  int y = (int)((float(image.height() * 0.3f) * cos(phi)) + image.height() * 0.5f + 0.5f);
  phi = phi + 0.2;

  QRect rectangle(x, y, 30, 30);

  QPainter painter(&image);
  QPen pen(Qt::red);
  painter.setPen(pen);
  painter.setBrush(Qt::Dense6Pattern);
  painter.fillRect(rectangle, Qt::red);

  if (!image.isNull())
  {
    SetQImage(image);
  }

  igtl::Matrix4x4 localMatrix;
  niftk::CreateRandomTransformMatrix(localMatrix);

  msgPointer->SetMatrix(localMatrix);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::Save(QString filename)
{
  if (m_Message.IsNull() || filename.isEmpty())
  {
    return;
  }

  QImage image = GetQImage();
  //GetQImage currently does not set the color table
  //lets do it here. Insert a default index table for indexed image type
  if ( image.format() == QImage::Format_Indexed8)
  {
    QVector<QRgb> colors = QVector<QRgb> (256);

    for (int i = 0; i < 256; i++)
    {
      colors[i] = qRgb(i, i, i);
    }

    image.setColorTable(colors);
  }

  if (!image.isNull())
  {
    image.save(filename);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::Load(QString filename)
{
  if (filename.isEmpty())
  {
    return;
  }

  // Load image from file
  QImage image;
  image.load(filename);

  if (!image.isNull())
  {
    SetQImage(image);
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessage::SetQImage(const QImage& inputImage)
{
  if (inputImage.isNull())
  {
    return;
  }

  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::ImageMessage::New());
  }

  // Cast pointer and unpack message
  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());
  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }

  // better check that the assumption below that images are tightly packed is actually true.
  // note that the qt docs are conflicting on this matter!
  assert(inputImage.bytesPerLine() == (inputImage.width() * (inputImage.depth() / 8)));

  // The aim of this stuff is to make QImage of type Format_Indexed8 as efficient as possible.
  unsigned long int byteSizeOfImage = 0;
  if ( inputImage.format() != QImage::Format_Indexed8 )
  {
    QImage image(inputImage);
    image = image.convertToFormat(QImage::Format_ARGB32);
    msgPointer->SetDimensions(image.width(), image.height(), 1);
    msgPointer->SetScalarType(igtl::ImageMessage::TYPE_UINT32);
    msgPointer->AllocateScalars();
    byteSizeOfImage = image.byteCount();
    // in some cases we'll end up with a null-pointer if we are short of mem.
    if ((msgPointer->GetScalarPointer() != 0) && (image.bits() != 0))
    {
      memcpy(msgPointer->GetScalarPointer(), image.bits(), byteSizeOfImage);
    }
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

    // in some cases we'll end up with a null-pointer if we are short of mem.
    if ((msgPointer->GetScalarPointer() != 0) && (inputImage.bits() != 0))
    {
      memcpy(msgPointer->GetScalarPointer(), inputImage.bits(), byteSizeOfImage);
    }
  }

  // Pack message data
  //msgPointer->Pack();
}

//-------------------------------------------------------------
QImage NiftyLinkImageMessage::GetQImage(void)
{
  if (m_Message.IsNull())
  {
    return QImage();
  }

  igtl::ImageMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::ImageMessage *>(m_Message.GetPointer());
  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }

  int i;
  int j;
  int k;
  msgPointer->GetDimensions(i, j, k);

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

      for (int i = 0; i < 256; i ++)
      {
        colors[i] = qRgb(i, i, i);
      }

      image.setColorTable(colors);
    }
    else
    {
      QLOG_ERROR() << "NiftyLinkImageMessage::GetQImage(void)" << ": Attempt to get QImage from image message of type "
                   << msgPointer->GetScalarType() << " not implemented. \n";
      return QImage();
    }
  }

  int byteSizeOfImg = image.byteCount();
  memcpy(image.bits(), msgPointer->GetScalarPointer(), byteSizeOfImg);

  // Pack message data
  //msgPointer->Pack();

  return image;
}

} // end namespace niftk
