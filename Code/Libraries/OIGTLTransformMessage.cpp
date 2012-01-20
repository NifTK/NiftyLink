#include "OIGTLTransformMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLTransformMessage::OIGTLTransformMessage(void)
	: OIGTLMessage()
{
	m_messageType = "TRANSFORM";
	m_message.operator =(igtl::TransformMessage::New());

	update();
}

OIGTLTransformMessage::~OIGTLTransformMessage(void)
{
}

OIGTLTransformMessage::OIGTLTransformMessage(const OIGTLTransformMessage &other)
	: OIGTLMessage(other)
{

	m_p[0] = other.m_p[0];
}

void OIGTLTransformMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetMatrix(matrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
    msgPointer->GetMatrix(matrix);

	//Pack message data
	msgPointer->Pack();

}

void OIGTLTransformMessage::setPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetPosition(p);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetPosition(p);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setPosition(float px, float py, float pz)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetPosition(px, py, pz);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getPosition(float &px, float &py, float &pz)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetPosition(&px, &py, &pz);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(o);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(o);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(t, s, n);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(t, s, n);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

    msgPointer->SetMatrix(dummyTransformMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	igtl::Matrix4x4 localMatrix;
    CreateRandomTransformMatrix(localMatrix);

    msgPointer->SetMatrix(localMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}
