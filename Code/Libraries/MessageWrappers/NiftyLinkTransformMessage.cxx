/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkTransformMessage.h"
#include <NiftyLinkTransformMessageHelpers.h>

#include "QsLog.h"
#include "QsLogDest.h"

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::NiftyLinkTransformMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "TRANSFORM";
  m_Message.operator = (NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::~NiftyLinkTransformMessage(void)
{
  //QLOG_INFO() <<"TransformMessage Destructor" <<m_OwnerName <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::NiftyLinkTransformMessage(const NiftyLinkTransformMessage &other)
  : NiftyLinkMessage(other)
{
  m_p[0] = other.m_p[0];
  m_p[1] = other.m_p[1];
  m_p[2] = other.m_p[2];
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetMatrix(matrix);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetMatrix(matrix);
  //msgPointer->Pack();
}

//-----------------------------------------------------------------------------
QString NiftyLinkTransformMessage::GetMatrixAsString()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }

  QString strMat = niftk::GetMatrixAsString(msgPointer);
  //msgPointer->Pack();

  return strMat;
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetPosition(float p[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetPosition(p);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetPosition(float p[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetPosition(p);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetPosition(float px, float py, float pz)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetPosition(px, py, pz);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetPosition(float &px, float &py, float &pz)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetPosition(&px, &py, &pz);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetNormals(float o[3][3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetNormals(o);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetNormals(float o[3][3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetNormals(o);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetNormals(t, s, n);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->GetNormals(t, s, n);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::InitializeWithTestData(igtl::Matrix4x4& testMatrix)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetMatrix(testMatrix);
  //msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::InitializeWithRandomData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TransformMessage::New());
  }

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }

  igtl::Matrix4x4 localMatrix;
  niftk::CreateRandomTransformMatrix(localMatrix);

  msgPointer->SetMatrix(localMatrix);
  //msgPointer->Pack();
}

} // end namespace niftk
