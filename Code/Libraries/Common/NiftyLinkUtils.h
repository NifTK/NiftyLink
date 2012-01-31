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
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkSession>

#include "igtlPolyDataMessage.h"
#include "igtlTrajectoryMessage.h"
#include "igtlLightObject.h"
#include "igtlObjectFactory.h"

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
 
/// \brief dummy function to test if the building and linking works correctly
extern "C" NIFTYLINKCOMMON_WINEXPORT int NiftyLinkDummyFunction1();

/// \brief Simple function which validates if an IP address is valid or not
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool validateIp(const QString &inputIP);
/// \brief This function which returns the local IP address
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString getLocalHostAddress(void);
/// \brief This function resolves a given hostname to IP
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString resolveHostName(const QString &input);

/// \brief This function creates a random 4x4 test matrix 
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);
/// \brief This function compares two image messages if they are of the same kind
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool CompareMsgData(OIGTLMessage::Pointer &msg1, OIGTLMessage::Pointer &msg2);

/**
 * \class MessageCatcher
 * \brief A simple class to catch OIGTLMessage and check validity.
 */
class NIFTYLINKCOMMON_WINEXPORT MessageCatcher : public QObject
{
	Q_OBJECT

public:
  /// \brief Basic constructor
	MessageCatcher() { m_messageValid = false; }
  /// \brief Basic destructor
	~MessageCatcher() {}

  /// \brief Return the last message caught
	OIGTLMessage* getMessage() { return m_msg; }
  /// \brief Returns true if the message is valid
	bool isMessageValid() { return m_messageValid; }

public slots:
  /// \brief Slot to catch OIGTLMessage
	void catchMessage(OIGTLMessage * msg) 
	{ 
		m_msg = msg; 
		m_messageValid = true; 
	}

  /// \brief Basic slot to trigger a catch event
	void catchMessage() 
	{ 
		m_messageValid = true; 
	}

private:
	OIGTLMessage * m_msg;
	bool m_messageValid;

};

#endif
