#ifndef OIGTLTRANSFORMMESSAGE_H
#define OIGTLTRANSFORMMESSAGE_H

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

class NIFTYLINKCOMMON_WINEXPORT OIGTLTransformMessage : public OIGTLMessage
{
//	Q_OBJECT

public:
	OIGTLTransformMessage(void);
	~OIGTLTransformMessage(void);

	typedef OIGTLTransformMessage		              Self;
	typedef QExplicitlySharedDataPointer<Self>        Pointer;
	typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;

	OIGTLTransformMessage(const OIGTLTransformMessage &other); 

	void setMatrix(igtl::Matrix4x4 &matrix);
	void getMatrix(igtl::Matrix4x4 &matrix);

	void setPosition(float p[3]);
	void getPosition(float p[3]);

	void setPosition(float px, float py, float pz);
	void getPosition(float &px, float &py, float &pz);

	void setNormals(float o[3][3]);
	void getNormals(float o[3][3]);

	void setNormals(float t[3], float s[3], float n[3]);
	void getNormals(float t[3], float s[3], float n[3]);

	virtual void initializeWithTestData(void);
	virtual void initializeWithRandomData(void);

protected:

private:
	float m_p[3];
};

#endif