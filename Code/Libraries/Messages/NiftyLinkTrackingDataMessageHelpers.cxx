/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkTrackingDataMessageHelpers.h"

#include <NiftyLinkMacro.h>
#include <NiftyLinkUtils.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace niftk
{

//-----------------------------------------------------------------------------
void InitialiseTrackingDataWithTestData(const igtl::Matrix4x4& testMatrix, igtl::TrackingDataMessage::Pointer& messageToWriteTo)
{
  if (messageToWriteTo.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Tracking data message is NULL.");
  }

  int elementNum = messageToWriteTo->GetNumberOfTrackingDataElements();
  if (elementNum == 0)
  {
    igtl::TrackingDataElement::Pointer tElem = igtl::TrackingDataElement::New();
    tElem->SetMatrix(*(const_cast<igtl::Matrix4x4*>(&testMatrix)));
    messageToWriteTo->AddTrackingDataElement(tElem);
  }
  else
  {
    igtl::TrackingDataElement::Pointer tElem;
    for (int i = 0; i < elementNum; i++)
    {
      messageToWriteTo->GetTrackingDataElement(i, tElem);
      tElem->SetMatrix(*(const_cast<igtl::Matrix4x4*>(&testMatrix)));
    }
  }
}


//-----------------------------------------------------------------------------
void InitialiseTrackingDataWithRandomData(igtl::TrackingDataMessage::Pointer& messageToWriteTo)
{
  igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);
  InitialiseTrackingDataWithTestData(localMatrix, messageToWriteTo);
}


//-----------------------------------------------------------------------------
QString GetMatrixAsString(const igtl::TrackingDataMessage::Pointer& message, int elementIndex)
{
  if (message.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Tracking data message is NULL.");
  }

  if (elementIndex < 0)
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Element index is < 0, should be zero or above.");
  }

  int numElem = message->GetNumberOfTrackingDataElements();
  if (elementIndex >= numElem)
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Element index is >= number of elements.");
  }

  if (message->GetNumberOfTrackingDataElements() == 0)
  {
    return QString();
  }

  std::stringstream sstr;
  std::fixed(sstr);
  sstr << std::setprecision(5);

  for (int i = 0; i < message->GetNumberOfTrackingDataElements(); i++)
  {
    igtl::TrackingDataElement::Pointer tElem;
    message->GetTrackingDataElement(i, tElem);

    igtl::Matrix4x4 matrix;
    tElem->GetMatrix(matrix);

    sstr << "=============" << std::endl;
    sstr << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << std::endl;
    sstr << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << std::endl;
    sstr << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << std::endl;
    sstr << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << std::endl;
    sstr << "=============" << std::endl;
  }

  QString strMat = QString(sstr.str().c_str());
  return strMat;
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName, const QString& hostName, int portNumber, igtl::Matrix4x4& input)
{
  igtl::TrackingDataElement::Pointer element = igtl::TrackingDataElement::New();
  element->SetMatrix(input);

  igtl::TrackingDataMessage::Pointer msg = igtl::TrackingDataMessage::New();
  msg->SetDeviceName(deviceName.toStdString().c_str());
  msg->AddTrackingDataElement(element);

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName(deviceName);
  m->SetSenderHostName(hostName);    // don't do these lookups here. They are expensive.
  m->SetSenderPortNumber(portNumber);

  return m;
}

//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName, const QString& hostName, int portNumber,  double* input)
{
  igtl::Matrix4x4 matrix;
  CopyMatrix(input, matrix);
  return CreateTrackingDataMessage(deviceName, hostName, portNumber, matrix);
}

} // end namespace niftk
