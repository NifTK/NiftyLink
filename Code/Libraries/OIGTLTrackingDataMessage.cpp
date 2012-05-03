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

#include "OIGTLTrackingDataMessage.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLTrackingDataMessage::OIGTLTrackingDataMessage(void)
	: OIGTLMessage()
{
	m_messageType = "TDATA";
	m_message.operator =(NULL);
}

OIGTLTrackingDataMessage::~OIGTLTrackingDataMessage(void)
{
  QLOG_INFO() <<"TrackingDataMessage Destructor" <<m_ownerName <<m_id;
}

OIGTLTrackingDataMessage::OIGTLTrackingDataMessage(const OIGTLTrackingDataMessage &other)
	: OIGTLMessage(other)
{

	m_p[0] = other.m_p[0];
}

void OIGTLTrackingDataMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetMatrix(matrix);
  msgPointer->AddTrackingDataElement(tElem);
  
	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return;
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetMatrix(matrix);

	//Pack message data
	msgPointer->Pack();

}

QString OIGTLTrackingDataMessage::getMatrixAsString()
{
  if (m_message.IsNull())
    return QString();

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());

  msgPointer->Unpack();
  
  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return QString();
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  igtl::Matrix4x4 matrix;
  tElem->GetMatrix(matrix);

  std::stringstream sstr;
  std::fixed(sstr);
  sstr << std::setprecision(5);

  sstr << "=============" << std::endl;
  sstr << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << std::endl;
  sstr << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << std::endl;
  sstr << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << std::endl;
  sstr << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << std::endl;
  sstr << "=============" << std::endl;

  QString strMat = QString(sstr.str().c_str());

  //Pack message data
  msgPointer->Pack();

  return strMat;
}

void OIGTLTrackingDataMessage::setPosition(float p[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetPosition(p);
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::getPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return;
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetPosition(p);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::setPosition(float px, float py, float pz)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetPosition(px, py, pz);
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::getPosition(float &px, float &py, float &pz)
{
	if (m_message.IsNull())
		return;

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return;
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetPosition(&px, &py, &pz);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::setTrackerToolName(QString name)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetName(name.toStdString().c_str());
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}
 
QString OIGTLTrackingDataMessage::getTrackerToolName()
{
  if (m_message.IsNull())
		return QString();

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return QString();
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  QString str(tElem->GetName());
 
	//Pack message data
	msgPointer->Pack();

  return str;
}

void OIGTLTrackingDataMessage::setTrackerToolType(igtlUint8 type)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetType(type);
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}

igtlUint8 OIGTLTrackingDataMessage::getTrackerToolType()
{
  if (m_message.IsNull())
		return -1;

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
    return -1;
  
  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  igtlUint8 type = tElem->GetType();
 
	//Pack message data
	msgPointer->Pack();

  return type;
}

void OIGTLTrackingDataMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

  tElem->SetMatrix(dummyTransformMatrix);
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TrackingDataMessage::New());

	igtl::TrackingDataMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TrackingDataMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
    tElem = igtl::TrackingDataElement::New();
  else
    msgPointer->GetTrackingDataElement(0, tElem);

	igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  tElem->SetMatrix(localMatrix);
  msgPointer->AddTrackingDataElement(tElem);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTrackingDataMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetTrackingDataMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetTrackingDataMessage::New());

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
    msgToCreate->changeMessageType("GET_TDATA");
}

void OIGTLTrackingDataMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartTrackingDataMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartTrackingDataMessage::New());

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
    msgToCreate->changeMessageType("STT_TDATA");
}

void OIGTLTrackingDataMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopTrackingDataMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopTrackingDataMessage::New());

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
    msgToCreate->changeMessageType("STP_TDATA");
}

void OIGTLTrackingDataMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSTrackingDataMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSTrackingDataMessage::New());

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
    msgToCreate->changeMessageType("RTS_TDATA");
}
