/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef __NiftyLinkXMLBuilder_h
#define __NiftyLinkXMLBuilder_h

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
* In NiftyLink the non-standard requests and commands are done through string messages.
* These messages are stored in an XML descriptor format which is then wrapped into a
* string message and sent across the network. Both the descriptor's creation and parsing
* is done via "Builder" objects. XMLBuilderBase is the base class for the family of
* different XML descriptor builders.
*
* Usage: all the adjustable features are set via member functions as a string (QString)
* then the XML is composed by calling GetXMLAsString(), which returns the descriptor in
* text format as QString. To parse an existing XML descriptor the SetXMLString() method
* can be used, which parses the XML descriptor and sets the member variables accordingly.
* The values can then be queried by calling the get... member functions.
*/
class NIFTYLINKCOMMON_WINEXPORT XMLBuilderBase : public QObject
{
  Q_OBJECT

public:

  /// \brief No-arg Constructor
  XMLBuilderBase() { m_MessageValid = false; }

  /// \brief Copy Constructor
  XMLBuilderBase(const XMLBuilderBase &other)
  {
    m_DescriptorString = other.m_DescriptorString;
    m_MessageValid = other.m_MessageValid;
  }

  /// \brief Basic destructor
  virtual ~XMLBuilderBase() {}

  /// \brief Assignment operator
  XMLBuilderBase & operator=(const XMLBuilderBase &other)
  {
    m_DescriptorString = other.m_DescriptorString;
    m_MessageValid = other.m_MessageValid;
    return *this;
  }

  /// \brief Returns true if the message is valid
  bool IsMessageValid() { return m_MessageValid; }

  /// \brief Pure virtual method: in the derived classes it composes an XML descriptor
  /// from the values of the builders member variables in a text format and returns it in a QString object
  virtual QString GetXMLAsString(void) = 0;

  /// \brief Pure virtual method: in the derived classes it sets the XML string,
  /// which is then parsed and the builder object's member variables are being set accordingly
  virtual void SetXMLString(QString desc) = 0;

  /// \brief Tells the descriptor type if it's a command / client / tracker, etc.
  virtual QString GetDescriptorType(void) { return m_DescriptorString; }

  /// \brief Static function to parse the descriptor type of an XML text
  static QString ParseDescriptorType(QString xmlDoco);

protected:
  QString m_DescriptorString;
  bool    m_MessageValid;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/**
* \class ClientDescriptorXMLBuilder
* \brief This class is able to compose or parse a client descriptor XML
*
* In NiftyLink when a client is successfully connected to the remote host a client
* descriptor is being sent to the remote, which describes the client's details and
* capabilities. This is to help the identify the individual clients. The
* ClientDescriptorXMLBuilder class serves as a base class for the more specified
* descriptor builders (like TrackerDescriptorBuilder), but it can also be used as a
* generic stendalone descriptor. The client's features are categorized into to main
* groups: connection related features (like IP, Port)
* and device related features (device type, device name, etc)
*/
class NIFTYLINKCOMMON_WINEXPORT ClientDescriptorXMLBuilder : public XMLBuilderBase
{
  Q_OBJECT

public:

  /// \brief Basic constructor
  ClientDescriptorXMLBuilder() : XMLBuilderBase() { m_MessageValid = false; }

  /// \brief Copy Constructor
  ClientDescriptorXMLBuilder(const ClientDescriptorXMLBuilder &other);

  /// \brief Basic destructor
  virtual ~ClientDescriptorXMLBuilder() {}

  /// \brief Assignment operator
  ClientDescriptorXMLBuilder & operator=(const ClientDescriptorXMLBuilder &other);

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString GetXMLAsString(void);

  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void SetXMLString(QString desc);

  /// \brief This method is used to set the device name (for example "Polaris Vicra")
  inline void SetDeviceName(QString name)
  {
    m_DeviceName.clear();
    m_DeviceName.append(name);
  }

  /// \brief Returns the device name if it was set previously
  inline QString GetDeviceName(void) { return m_DeviceName; }

  /// \brief This method is used to set the device type (for example "Tracker", "Ultrasound Imager" etc.)
  inline void SetDeviceType(QString type)
  {
    m_DeviceType.clear();
    m_DeviceType.append(type);
  }

  /// \brief Returns the device type if it was set previously
  inline QString GetDeviceType(void) { return m_DeviceType; }

  /// \brief This method is used to set the device's communication type (for example "Serial", "Bluetooth" etc.)
  inline void SetCommunicationType(QString commtype)
  {
    m_CommType.clear();
    m_CommType.append(commtype);
  }

  /// \brief Returns the device's communication type if it was set previously
  inline QString GetCommunicationType(void) { return m_CommType; }

  /// \brief This method sets the port name which was used to connect the device to the PC
  inline void SetPortName(QString portName)
  {
    m_PortName.clear();
    m_PortName.append(portName);
  }

  /// \brief Returns the name of the port which was used to connect the device to the PC
  inline QString GetPortName(void) { return m_PortName; }

  /// \brief This method sets the IP address of the client (usually the IP of the message's sender)
  inline void SetClientIP(QString ip)
  {
    m_ClientIP.clear();
    m_ClientIP.append(ip);
  }

  /// \brief Returns the client's IP address (usually the IP of the message's sender)
  inline QString GetClientIP(void) { return m_ClientIP; }

  /// \brief This method sets the TCP port number of the client (usually the port on which the message is sent)
  inline void SetClientPort(QString port)
  {
    m_ClientPort.clear();
    m_ClientPort.append(port);
  }

  /// \brief Returns the TCP port number of the client (usually the port on which the message was sent)
  inline QString GetClientPort(void) { return m_ClientPort;}

protected:
  QString m_DeviceName;
  QString m_DeviceType;
  QString m_CommType;
  QString m_PortName;
  QString m_ClientIP;
  QString m_ClientPort;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/**
* \class CommandDescriptorXMLBuilder
* \brief This class is able to compose a command message descriptor XML
*
* In NiftyLink when custom commands are sent as igtl::StringMessage, which contains
* an XML descriptor of the command and it's parameters. The command's name is set
* via SetCommandName(), then any number of parameters can be passed on by calling
* AddParameter(QString pName, QString pType, QString pVal). Both the command's name
* and it's parameters are required in QString format.
*
*/
class NIFTYLINKCOMMON_WINEXPORT CommandDescriptorXMLBuilder : public XMLBuilderBase
{
  Q_OBJECT

public:
  /// \brief Basic constructor
  CommandDescriptorXMLBuilder() : XMLBuilderBase()
  {
    m_MessageValid = false;
    m_NumOfParameters = 0;
  }

  /// \brief Copy Constructor
  CommandDescriptorXMLBuilder(const CommandDescriptorXMLBuilder &other);

  /// \brief Basic destructor
  virtual ~CommandDescriptorXMLBuilder() {}

  /// \brief Assignment operator
  CommandDescriptorXMLBuilder & operator=(const CommandDescriptorXMLBuilder &other);

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString GetXMLAsString(void);

  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void SetXMLString(QString desc);

  /// \brief Sets the command's name
  inline void SetCommandName(QString name)
  {
    m_CommandName.clear();
    m_CommandName.append(name);
  }

  /// \brief Returns the command's name
  inline QString GetCommandName(void) { return m_CommandName; }

  /// \brief Appends a command parameter to the parameter list. Required fields are the name of the parameter (pname),
  /// type of the parameter (ptype) and the value of the parameter, all as QStrings
  void AddParameter(QString pName, QString pType, QString pVal);

  /// \brief Returns the number of paramters which were previously assigned to the command
  inline int GetNumOfParameters(void) { return m_NumOfParameters; }

  /// \brief Returns the name of the i-th parameter of the parameter list
  QString GetParameterName(int i);

  /// \brief Returns the type of the i-th parameter of the parameter list
  QString GetParameterType(int i);

  /// \brief Returns the value of the i-th parameter of the parameter list
  QString GetParameterValue(int i);

  /// \brief Returns all fields of the i-th parameter of the parameter list
  void GetParameterAllFields(int i, QString &pName, QString &pType, QString &pValue);

  /// \brief Returns the list of paramter names (as QStringList)
  QStringList GetParameterNames(void);

  /// \brief Returns the list of paramter types (as QStringList)
  QStringList GetParameterTypes(void);

  /// \brief Returns the list of paramter values (as QStringList)
  QStringList GetParameterValues(void);

private:
  QString     m_CommandName;
  int         m_NumOfParameters;
  QStringList m_ParameterNames;
  QStringList m_ParameterTypes;
  QStringList m_ParameterValues;
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
  TrackerClientDescriptor() : ClientDescriptorXMLBuilder() { m_MessageValid = false; }

  /// \brief Copy Constructor
  TrackerClientDescriptor(const TrackerClientDescriptor &other);

  /// \brief Basic destructor
  virtual ~TrackerClientDescriptor() {}

  /// \brief Assignment operator
  TrackerClientDescriptor & operator=(const TrackerClientDescriptor &other);

  /// \brief This method composes an XML descriptor from the values of the builders member variables in a text format and returns it in a QString object
  QString GetXMLAsString(void);

  /// \brief This method sets the XML string, which is then parsed and the builder object's member variables are being set accordingly
  void SetXMLString(QString desc);

  /// \brief Appends a tracker tool to the descriptor
  inline void AddTrackerTool(QString toolName) { m_TrackerTools.append(toolName); }

  /// \brief Returns the list of tracker tools which were assigned to the tracker previously
  inline QStringList GetTrackerTools(void) { return m_TrackerTools; }

private:
  QStringList m_TrackerTools;
};

#endif
