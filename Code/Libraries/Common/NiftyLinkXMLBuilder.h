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

#ifndef NIFTYLINKXMLBUILDER_H
#define NIFTYLINKXMLBUILDER_H

#include <QString>
#include <QUrl>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkSession>
#include <QDomDocument>

#include "NiftyLinkConfig.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

#include "QsLog.h"
#include "QsLogDest.h"

class NIFTYLINKCOMMON_WINEXPORT XMLBuilderBase : public QObject
{
	Q_OBJECT

public:
  XMLBuilderBase() { m_messageValid = false; }
  // \brief Copy Constructor
  XMLBuilderBase(const XMLBuilderBase &other) { m_descriptorString = other.m_descriptorString; m_messageValid = other.m_messageValid; }
  /// \brief Basic destructor
  virtual ~XMLBuilderBase() {}
  /// \brief Assignment operator
  XMLBuilderBase & operator=(const XMLBuilderBase &other) { m_descriptorString = other.m_descriptorString; m_messageValid = other.m_messageValid; return *this; }
  
  /// \brief Returns true if the message is valid
	bool isMessageValid() { return m_messageValid; }

  virtual QString getXMLAsString(void) { return QString(); }
  virtual void setXMLString(QString desc) {}

  virtual QString getDescriptorType(void) { return m_descriptorString; }
  
  static QString parseDescriptorType(QString xmlDoco);

protected:
  QString m_descriptorString;
  bool m_messageValid;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class NIFTYLINKCOMMON_WINEXPORT ClientDescriptorXMLBuilder : public XMLBuilderBase
{
	Q_OBJECT

public:
  /// \brief Basic constructor
  ClientDescriptorXMLBuilder() : XMLBuilderBase() { m_messageValid = false; }
  // \brief Copy Constructor
  ClientDescriptorXMLBuilder(const ClientDescriptorXMLBuilder &other);
  /// \brief Basic destructor
	virtual ~ClientDescriptorXMLBuilder() {}
  /// \brief Assignment operator
  ClientDescriptorXMLBuilder & operator=(const ClientDescriptorXMLBuilder &other);

  QString getXMLAsString(void);
  void setXMLString(QString desc);

  inline void setDeviceName(QString name) { m_deviceName.clear(); m_deviceName.append(name); }
  inline QString getDeviceName(void) { return m_deviceName; }

  inline void setDeviceType(QString type) { m_deviceType.clear(); m_deviceType.append(type); }
  inline QString getDeviceType(void) { return m_deviceType; }

  inline void setCommunicationType(QString commtype) { m_commType.clear(); m_commType.append(commtype); }
  inline QString getCommunicationType(void) { return m_commType; }

  inline void setPortName(QString portName) { m_portName.clear(); m_portName.append(portName); }
  inline QString getPortName(void) { return m_portName; }

  inline void setClientIP(QString ip) { m_clientIP.clear(); m_clientIP.append(ip); }
  inline QString getClientIP(void) { return m_clientIP; }

  inline void setClientPort(QString port) { m_clientPort.clear(); m_clientPort.append(port); }
  inline QString getClientPort(void) { return m_clientPort;}

protected:
  QString      m_deviceName;
  QString      m_deviceType;
  QString      m_commType;
  QString      m_portName;
  QString      m_clientIP;
  QString      m_clientPort;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class NIFTYLINKCOMMON_WINEXPORT CommandDescriptorXMLBuilder : public XMLBuilderBase
{
	Q_OBJECT

public:
  /// \brief Basic constructor
  CommandDescriptorXMLBuilder() : XMLBuilderBase() { m_messageValid = false; m_numOfParameters = 0; }
  // \brief Copy Constructor
  CommandDescriptorXMLBuilder(const CommandDescriptorXMLBuilder &other);
  /// \brief Basic destructor
	virtual ~CommandDescriptorXMLBuilder() {}
  /// \brief Assignment operator
  CommandDescriptorXMLBuilder & operator=(const CommandDescriptorXMLBuilder &other);

  QString getXMLAsString(void);
  void setXMLString(QString desc);

  inline void setCommandName(QString name) { m_commandName.clear(); m_commandName.append(name); }
  inline QString getCommandName(void) { return m_commandName; }

  void addParameter(QString pName, QString pType, QString pVal);
  inline int  getNumOfParameters(void) { return m_numOfParameters; }
  
  QString getParameterName(int i);
  QString getParameterType(int i);
  QString getParameterValue(int i);
  
  QStringList getParameterNames(void);
  QStringList getParameterTypes(void);
  QStringList getParameterValues(void);

private:
  QString     m_commandName;
  int         m_numOfParameters;
  
  QStringList m_parameterNames;
  QStringList m_parameterTypes;
  QStringList m_parameterValues;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class NIFTYLINKCOMMON_WINEXPORT TrackerClientDescriptor : public ClientDescriptorXMLBuilder
{
	Q_OBJECT

public:
  /// \brief Basic constructor
  TrackerClientDescriptor() : ClientDescriptorXMLBuilder() { m_messageValid = false; }
  // \brief Copy Constructor
  TrackerClientDescriptor(const TrackerClientDescriptor &other);
  /// \brief Basic destructor
	virtual ~TrackerClientDescriptor() {}
  /// \brief Assignment operator
  TrackerClientDescriptor & operator=(const TrackerClientDescriptor &other);

  QString getXMLAsString(void);
  void setXMLString(QString desc);

  inline void addTrackerTool(QString toolName) { m_trackerTools.append(toolName); }
  inline QStringList getTrackerTools(void) { return m_trackerTools; }

private:
  QStringList  m_trackerTools;
};

#endif
