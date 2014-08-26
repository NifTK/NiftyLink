/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkTransformMessageHelpers.h"

#include <NiftyLinkMacro.h>
#include <NiftyLinkUtils.h>
#include <NiftyLinkTransformMessageHelpers.h>

#include <sstream>
#include <iostream>
#include <iomanip>

namespace niftk
{

//-----------------------------------------------------------------------------
void InitialiseTransformWithTestData(const igtl::Matrix4x4& testMatrix, igtl::TransformMessage::Pointer& messageToWriteTo)
{
  if (messageToWriteTo.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Transform message is NULL.");
  }
  messageToWriteTo->SetMatrix(*(const_cast<igtl::Matrix4x4*>(&testMatrix)));
}


//-----------------------------------------------------------------------------
void InitialiseTransformWithRandomData(igtl::TransformMessage::Pointer& messageToWriteTo)
{
  if (messageToWriteTo.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Transform message is NULL.");
  }

  igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  messageToWriteTo->SetMatrix(localMatrix);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTransformMessageWithRandomData()
{
  igtl::Matrix4x4 matrix;
  niftk::CreateRandomTransformMatrix(matrix);

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();

  return CreateTransformMessage("TestingDevice", "TestingHost", 1234, matrix, timeCreated);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTransformMessageWithRandomData(igtl::TimeStamp::Pointer& timeStamp)
{
  igtl::Matrix4x4 matrix;
  niftk::CreateRandomTransformMatrix(matrix);

  return CreateTransformMessage("TestingDevice", "TestingHost", 1234, matrix, timeStamp);
}


//-----------------------------------------------------------------------------
QString GetMatrixAsString(const igtl::TransformMessage::Pointer& message)
{
  if (message.IsNull())
  {
    NiftyLinkStdExceptionMacro(std::invalid_argument, << "Transform message is NULL.");
  }

  std::stringstream sstr;
  std::fixed(sstr);
  sstr << std::setprecision(5);

  igtl::Matrix4x4 matrix;
  message->GetMatrix(matrix);

  sstr << "=============" << std::endl;
  sstr << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << std::endl;
  sstr << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << std::endl;
  sstr << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << std::endl;
  sstr << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << std::endl;
  sstr << "=============" << std::endl;

  QString strMat = QString(sstr.str().c_str());
  return strMat;
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTransformMessage(
    const QString& deviceName,
    const QString& hostName,
    const int& portNumber,
    const igtl::Matrix4x4& input,
    igtl::TimeStamp::Pointer& timeCreated
    )
{
  igtl::TransformMessage::Pointer msg = igtl::TransformMessage::New();
  msg->SetDeviceName(deviceName.toStdString().c_str());
  msg->SetMatrix(*(const_cast<igtl::Matrix4x4*>(&input)));

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
NiftyLinkMessageContainer::Pointer CreateTransformMessage(
    const QString& deviceName,
    const QString& hostName,
    const int& portNumber,
    const igtl::Matrix4x4& input
    )
{
  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  return CreateTransformMessage(deviceName, hostName, portNumber, input, timeCreated);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTransformMessage(const QString& deviceName, const QString& hostName, int portNumber, double* input)
{
  igtl::Matrix4x4 matrix;
  CopyMatrix(input, matrix);

  return CreateTransformMessage(deviceName, hostName, portNumber, matrix);
}

} // end namespace niftk
