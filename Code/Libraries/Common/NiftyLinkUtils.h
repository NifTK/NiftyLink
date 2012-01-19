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

#ifndef NIFTYLINKCOMMONUTILS_H
#define NIFTYLINKCOMMONUTILS_H

#include <QString>
#include <QUrl>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>

#include <cmath>
#include "igtlMath.h"

#include "NiftyLinkConfig.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

static igtl::Matrix4x4 dummyTransformMatrix = 
{{ -1, 0, 0, 50},
{  0, 0.15, 0.98, 0},
{ 0, 0.98, -0.15, 50},
{ 0, 0, 0, 1} };
 

extern "C" NIFTYLINKCOMMON_WINEXPORT int NiftyLinkDummyFunction1();

extern NIFTYLINKCOMMON_WINEXPORT bool validateIp(const QString &inputIP);
extern NIFTYLINKCOMMON_WINEXPORT QString getLocalHostAddress(void);
extern NIFTYLINKCOMMON_WINEXPORT QString resolveHostName(const QString &input);

extern NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);
extern NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void CreateTestTransformMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void CreateTransformMsg(OIGTLMessage::Pointer &msgToSend, igtl::Matrix4x4 &matrix, std::string name);

extern NIFTYLINKCOMMON_WINEXPORT void CreateGetTransformMsg(OIGTLMessage::Pointer &msgToSend);


extern NIFTYLINKCOMMON_WINEXPORT void GetTestImageMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void GetTestPositionMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void GetTestPointMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void GetTestStatusMsg(OIGTLMessage::Pointer &msg);
extern NIFTYLINKCOMMON_WINEXPORT void GetTestStringMsg(OIGTLMessage::Pointer &msg);

extern NIFTYLINKCOMMON_WINEXPORT bool CompareMsgData(OIGTLMessage::Pointer &msg1, OIGTLMessage::Pointer &msg2);

class NIFTYLINKCOMMON_WINEXPORT MessageCatcher : public QObject
{
	Q_OBJECT

public:
	MessageCatcher() { m_messageValid = false; }
	~MessageCatcher() {}

	OIGTLMessage* getMessage() { return m_msg; }
	bool isMessageValid() { return m_messageValid; }

public slots:
	void catchMessage(OIGTLMessage * msg) 
	{ 
		m_msg = msg; 
		m_messageValid = true; 
	}

	void catchMessage() 
	{ 
		m_messageValid = true; 
	}

private:
	OIGTLMessage * m_msg;
	bool m_messageValid;

};

#endif
