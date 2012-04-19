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

#include "OIGTLStatusMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLStatusMessage::OIGTLStatusMessage(void)
	: OIGTLMessage()
{
	m_messageType = "STATUS";
	m_message.operator =(NULL);
}

OIGTLStatusMessage::~OIGTLStatusMessage(void)
{
  QLOG_INFO() <<"StatusMessage Destructor" <<m_ownerName <<m_id;
}

OIGTLStatusMessage::OIGTLStatusMessage(const OIGTLStatusMessage &other)
	: OIGTLMessage(other)
{
}

void OIGTLStatusMessage::setErrorCode(int code)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StatusMessage::New());

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetCode(code);

	//Pack message data
	msgPointer->Pack();
}

int OIGTLStatusMessage::getErrorCode()
{
  if (m_message.IsNull())
		return -1;

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  int code = msgPointer->GetCode();

	//Pack message data
	msgPointer->Pack();

  return code;
}

void OIGTLStatusMessage::setErrorSubCode(igtlInt64 subcode)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StatusMessage::New());

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetSubCode(subcode);

	//Pack message data
	msgPointer->Pack();
}

igtlInt64 OIGTLStatusMessage::getErrorSubCode()
{
  if (m_message.IsNull())
		return -1;

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  igtlInt64 code = msgPointer->GetSubCode();

	//Pack message data
	msgPointer->Pack();

  return code;
}
  
void OIGTLStatusMessage::setErrorName(QString name)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StatusMessage::New());

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  msgPointer->SetErrorName(name.toStdString().c_str());

	//Pack message data
	msgPointer->Pack();
}

QString OIGTLStatusMessage::getErrorName()
{
  if (m_message.IsNull())
		return QString();

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  QString txt(msgPointer->GetErrorName());

	//Pack message data
	msgPointer->Pack();

  return txt;
}

void OIGTLStatusMessage::setStatusString(QString str)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StatusMessage::New());

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  msgPointer->SetStatusString(str.toStdString().c_str());

	//Pack message data
	msgPointer->Pack();
}

QString OIGTLStatusMessage::getStatusString()
{
  if (m_message.IsNull())
		return QString();

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  QString txt(msgPointer->GetStatusString());

	//Pack message data
	msgPointer->Pack();

  return txt;
}

void OIGTLStatusMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::StatusMessage::New());

	igtl::StatusMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StatusMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  msgPointer->SetCode(igtl::StatusMessage::STATUS_OK);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLStatusMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetStatusMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetStatusMessage::New());

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
    msgToCreate->changeMessageType("GET_STATUS");
}

void OIGTLStatusMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartStatusMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartStatusMessage::New());

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
    msgToCreate->changeMessageType("STT_STATUS");
}

void OIGTLStatusMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopStatusMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopStatusMessage::New());

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
    msgToCreate->changeMessageType("STP_STATUS");
}

void OIGTLStatusMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSStatusMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSStatusMessage::New());

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
    msgToCreate->changeMessageType("RTS_STATUS");
}
