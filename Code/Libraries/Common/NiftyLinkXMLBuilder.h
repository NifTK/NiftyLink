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

/**
 * \class XMLBuilderBase
 * \brief Base class for the XML based descriptor builder classes
 * 
 * In NiftyLink the non-standard requests and commands are done through string messages. These messages are stored in an XML descriptor format
 * which is then wrapped into a string message and sent across the network. Both the descriptor's creation and parsing is done via "Builder" objects.
 * XMLBuilderBase is the base class for the family of different XML descriptor builders.
 * Usage: all the adjustable features are set via member functions as a string (QString) then the XML is composed by calling getXMLAsString(), which
 * returns the descriptor in text format as QString.
 * To parse an existing XML descriptor the setXMLString() method can be used, which parses the XML descriptor and sets the member variables accordingly.
 * The values can then be queried by calling the get... member functions.
 */


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

  /// \brief Pure virtual method: in the derived classes it composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  virtual QString getXMLAsString(void) = 0;
  /// \brief Pure virtual method: in the derived classes it sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  virtual void setXMLString(QString desc) = 0;

  /// \brief Tells the descriptor type if it's a command / client / tracker, etc.
  virtual QString getDescriptorType(void) { return m_descriptorString; }
  
  /// \brief Static function to parse the descriptor type of an XML text 
  static QString parseDescriptorType(QString xmlDoco);

protected:
  QString m_descriptorString;
  bool m_messageValid;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/**
 * \class ClientDescriptorXMLBuilder
 * \brief This class is able to compose or parse a client descripor XML
 * 
 * In NiftyLink when a client is successfully connected to the remote host a client descriptor is being sent to the remote, which describes
 * the client's details and capabilities. This is to help the identify the individual clients. The ClientDescriptorXMLBuilder class serves as a 
 * base class for the more specified descriptor builders (like TrackerDescriptorBuilder), but it can also be used as a generic stendalone descriptor.
 * The client's features are categorized into to main groups: connection related features (like IP, Port) and device related features (device type, device name, etc)
 *
 */

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

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString getXMLAsString(void);
  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void setXMLString(QString desc);

  /// \brief This method is used to set the device name (for example "Polaris Vicra")
  inline void setDeviceName(QString name) { m_deviceName.clear(); m_deviceName.append(name); }
  /// \brief Returns the device name if it was set previously
  inline QString getDeviceName(void) { return m_deviceName; }

  /// \brief This method is used to set the device type (for example "Tracker", "Ultrasound Imager" etc.)
  inline void setDeviceType(QString type) { m_deviceType.clear(); m_deviceType.append(type); }
  /// \brief Returns the device type if it was set previously
  inline QString getDeviceType(void) { return m_deviceType; }

  /// \brief This method is used to set the device's communication type (for example "Serial", "Bluetooth" etc.)
  inline void setCommunicationType(QString commtype) { m_commType.clear(); m_commType.append(commtype); }
  /// \brief Returns the device's communication type if it was set previously
  inline QString getCommunicationType(void) { return m_commType; }

  /// \brief This method sets the port name which was used to connect the device to the PC
  inline void setPortName(QString portName) { m_portName.clear(); m_portName.append(portName); }
  /// \brief Returns the name of the port which was used to connect the device to the PC
  inline QString getPortName(void) { return m_portName; }

  /// \brief This method sets the IP address of the client (usually the IP of the message's sender)
  inline void setClientIP(QString ip) { m_clientIP.clear(); m_clientIP.append(ip); }
  /// \brief Returns the client's IP address (usually the IP of the message's sender)
  inline QString getClientIP(void) { return m_clientIP; }

  /// \brief This method sets the TCP port number of the client (usually the port on which the message is sent)
  inline void setClientPort(QString port) { m_clientPort.clear(); m_clientPort.append(port); }
  /// \brief Returns the TCP port number of the client (usually the port on which the message was sent)
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

/**
 * \class CommandDescriptorXMLBuilder
 * \brief This class is able to compose a command message descriptor XML
 * 
 * In NiftyLink when custom commands are sent as igtl::StringMessage, which contains an XML descriptor of the command and it's parameters.
 * The command's name is set via setCommandName(), then any number of parameters can be passed on by calling addParameter(QString pName, QString pType, QString pVal)
 * Both the command's name and it's parameters are required in QString format.
 *
 */


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

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString getXMLAsString(void);
  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void setXMLString(QString desc);

  /// \brief Sets the command's name
  inline void setCommandName(QString name) { m_commandName.clear(); m_commandName.append(name); }
  /// \brief Returns the command's name
  inline QString getCommandName(void) { return m_commandName; }

  /// \brief Appends a command parameter to the parameter list. Required fields are the name of the parameter (pname), type of the parameter (ptype) and the value of the parameter, all as QStrings
  void addParameter(QString pName, QString pType, QString pVal);
  /// \brief Returns the number of paramters which were previously assigned to the command
  inline int  getNumOfParameters(void) { return m_numOfParameters; }
  
  /// \brief Returns the name of the i-th parameter of the parameter list
  QString getParameterName(int i);
  /// \brief Returns the type of the i-th parameter of the parameter list
  QString getParameterType(int i);
  /// \brief Returns the value of the i-th parameter of the parameter list
  QString getParameterValue(int i);
  
  /// \brief Returns the list of paramter names (as QStringList)
  QStringList getParameterNames(void);
  /// \brief Returns the list of paramter types (as QStringList)
  QStringList getParameterTypes(void);
  /// \brief Returns the list of paramter values (as QStringList)
  QStringList getParameterValues(void);

private:
  QString     m_commandName;
  int         m_numOfParameters;
  
  QStringList m_parameterNames;
  QStringList m_parameterTypes;
  QStringList m_parameterValues;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/**
 * \class TrackerClientDescriptor
 * \brief This class is able to compose a tracker client descriptor XML
 * 
 * TrackerClientDescriptor is basically a generic client descriptor with the addition of Tracker Tools descriptors.
 *
 */

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

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString getXMLAsString(void);
  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void setXMLString(QString desc);

  /// \brief Appends a tracker tool to the descriptor
  inline void addTrackerTool(QString toolName) { m_trackerTools.append(toolName); }
  /// \brief Returns the list of tracker tools which were assigned to the tracker previously
  inline QStringList getTrackerTools(void) { return m_trackerTools; }

private:
  QStringList  m_trackerTools;
};

#endif
