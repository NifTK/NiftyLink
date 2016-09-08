/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkImageMessageHelpers.h"

#include <NiftyLinkUtils.h>
#include <NiftyLinkMacro.h>

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include <QsLog.h>

#include <math.h>
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
void InitialiseImageWithTestData(igtl::ImageMessage::Pointer& imageMessage)
{
  if (imageMessage.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Image message is NULL.");
  }

  QString resource(":/NiftyLink/UCL_LOGO.tif");
  QImage image;
  image.load(resource);

  if (image.isNull())
  {
    NiftyLinkStdExceptionMacro(std::runtime_error, << "Failed to load image from resource " << resource.toStdString());
  }

  SetQImage(image, imageMessage);
}


//-----------------------------------------------------------------------------
void InitialiseImageWithRandomData(igtl::ImageMessage::Pointer& imageMessage)
{
  if (imageMessage.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Image message is NULL.");
  }

  QString resource(":/NiftyLink/UCL_LOGO.tif");
  QImage image;
  image.load(resource);

  if (image.isNull())
  {
    NiftyLinkStdExceptionMacro(std::runtime_error, << "Failed to load image from resource " << resource.toStdString());
  }

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

  SetQImage(image, imageMessage);

  igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  imageMessage->SetMatrix(localMatrix);
}


//-----------------------------------------------------------------------------
void SetQImage(const QImage& imageToRead, igtl::ImageMessage::Pointer& imageToWrite)
{
  if (imageToWrite.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Image message is NULL.");
  }

  if (imageToRead.isNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "QImage is NULL.");
  }

  // better check that the assumption below that images are tightly packed is actually true.
  // note that the qt docs are conflicting on this matter!
  assert(imageToRead.bytesPerLine() == (imageToRead.width() * (imageToRead.depth() / 8)));

  unsigned long int byteSizeOfImage = 0;

  if (
#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
           imageToRead.format() == QImage::Format_Indexed8
#else
           imageToRead.format() == QImage::Format_Indexed8 || imageToRead.format() == QImage::Format_Grayscale8
#endif
           )
  {
    imageToWrite->SetDimensions(imageToRead.width(), imageToRead.height(), 1);
    imageToWrite->SetScalarType(igtl::ImageMessage::TYPE_UINT8);
    imageToWrite->AllocateScalars();
    byteSizeOfImage = imageToRead.byteCount();
  }
  else
  {
    // All other formats get converted to Format_ARGB32.
    QImage image(imageToRead);
    image = image.convertToFormat(QImage::Format_ARGB32);
    imageToWrite->SetDimensions(image.width(), image.height(), 1);
    imageToWrite->SetScalarType(igtl::ImageMessage::TYPE_UINT32);
    imageToWrite->AllocateScalars();
    byteSizeOfImage = image.byteCount();
  }

  // Copy image data to igtl::ImageMessage
  // for QImage::Format_Indexed8 this is going to lose the index table
  // This may or may not be a problem depending on what happens to the
  // image at the other end

  // in some cases we'll end up with a null-pointer if we are short of mem.
  if ((imageToWrite->GetScalarPointer() != 0) && (imageToRead.bits() != 0))
  {
    memcpy(imageToWrite->GetScalarPointer(), imageToRead.bits(), byteSizeOfImage);
  }
}


//-----------------------------------------------------------------------------
void GetQImage(const igtl::ImageMessage::Pointer& imageToRead, QImage& imageToWrite)
{
  if (imageToRead.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Image message is NULL.");
  }

  int i;
  int j;
  int k;
  imageToRead->GetDimensions(i, j, k);

  if ( imageToRead->GetScalarType() == igtl::ImageMessage::TYPE_UINT32 )
  {
    imageToWrite = QImage(i, j, QImage::Format_ARGB32);
  }
  else if ( imageToRead->GetScalarType() == igtl::ImageMessage::TYPE_UINT8
            && imageToRead->GetNumComponents() == 3
          )
  {
    imageToWrite = QImage(i, j, QImage::Format_RGB888);
  }
  else if ( imageToRead->GetScalarType() == igtl::ImageMessage::TYPE_UINT8
            && imageToRead->GetNumComponents() == 4
          )
  {
    imageToWrite = QImage(i, j, QImage::Format_ARGB32);
  }
  else
  {
    if ( imageToRead->GetScalarType() == igtl::ImageMessage::TYPE_UINT8 )
    {
#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
      imageToWrite = QImage(i, j, QImage::Format_Indexed8);
      QVector<QRgb> colors = QVector<QRgb>(256);

      for (int i = 0; i < 256; i ++)
      {
        colors[i] = qRgb(i, i, i);
      }

      imageToWrite.setColorTable(colors);
#else
      imageToWrite = QImage(i, j, QImage::Format_Grayscale8);
#endif
    }
    else
    {
      QLOG_ERROR() << "NiftyLinkImageMessage::GetQImage(void)" << ": Attempt to get QImage from image message of type "
                   << imageToRead->GetScalarType() << " not implemented. \n";
      return;
    }
  }

  int byteSizeOfImg = imageToWrite.byteCount();
  memcpy(imageToWrite.bits(), imageToRead->GetScalarPointer(), byteSizeOfImg);
}


//-----------------------------------------------------------------------------
void SaveImage(const igtl::ImageMessage::Pointer& imageToRead, const QString& outputFileName)
{
  if (imageToRead.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Image message is NULL.");
  }

  if (outputFileName.isEmpty())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Output file name is empty.");
  }

  QImage image;
  GetQImage(imageToRead, image);

  // GetQImage currently does not set the color table.
  // Lets do it here. Insert a default index table for indexed image type
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
    image.save(outputFileName);
  }
}


//-----------------------------------------------------------------------------
void LoadImage(const QString& fileName, igtl::ImageMessage::Pointer& imageToWrite)
{
  if (fileName.isEmpty())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "File name is empty.");
  }

  // Load image from file
  QImage image;
  image.load(fileName);

  if (image.isNull())
  {
    NiftyLinkStdExceptionMacro(std::runtime_error, << "Failed to load image.");
  }

  SetQImage(image, imageToWrite);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateImageMessage(
    const QString& deviceName,
    const QString& hostName,
    const int& portNumber,
    const QImage& imageToRead,
    igtl::TimeStamp::Pointer& timeCreated
    )
{
  igtl::ImageMessage::Pointer msg = igtl::ImageMessage::New();
  msg->SetDeviceName(deviceName.toStdString().c_str());
  SetQImage(imageToRead, msg);

  timeCreated->GetTime();

  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName(deviceName);
  m->SetSenderHostName(hostName);    // don't do these lookups here. They are expensive.
  m->SetSenderPortNumber(portNumber);
  m->SetTimeArrived(timeCreated);
  m->SetTimeReceived(timeCreated);

  return m;
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateImageMessage(
    const QString& deviceName,
    const QString& hostName,
    const int& portNumber,
    const QImage& imageToRead
    )
{
  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  return CreateImageMessage(deviceName, hostName, portNumber, imageToRead, timeCreated);
}

} // end namespace
