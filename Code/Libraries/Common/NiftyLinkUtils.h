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


extern "C" NIFTYLINKCOMMON_WINEXPORT int NiftyLinkDummyFunction1();

extern NIFTYLINKCOMMON_WINEXPORT bool validateIp(const QString &inputIP);
extern NIFTYLINKCOMMON_WINEXPORT QString getLocalHostAddress(void);
extern NIFTYLINKCOMMON_WINEXPORT QString resolveHostName(QString &input);
extern NIFTYLINKCOMMON_WINEXPORT void GetRandomTestMatrix(igtl::Matrix4x4& matrix);


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

private:
	OIGTLMessage * m_msg;
	bool m_messageValid;

};


#endif
