/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkXMLBuilder.h"

#include <QDomDocument>
#include <QsLog.h>

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
QString NiftyLinkXMLBuilderBase::ParseDescriptorType(QString xmlString)
{
  QDomDocument xmlDoco;

  bool messageValid = xmlDoco.setContent(xmlString);

  if (messageValid)
  {
    // A valid XML document was received, now it's time to parse it
    QDomElement root = xmlDoco.documentElement();

    return root.tagName();
  }
  else
  {
    return QString();
  }
}


//-----------------------------------------------------------------------------
NiftyLinkClientDescriptor::NiftyLinkClientDescriptor(const NiftyLinkClientDescriptor &other)
  : NiftyLinkXMLBuilderBase(other)
,  m_DeviceName(other.m_DeviceName)
,  m_DeviceType(other.m_DeviceType)
,  m_CommType(other.m_CommType)
,  m_PortName(other.m_PortName)
,  m_ClientIP(other.m_ClientIP)
,  m_ClientPort(other.m_ClientPort)
{
}


//-----------------------------------------------------------------------------
NiftyLinkClientDescriptor& NiftyLinkClientDescriptor::operator=(const NiftyLinkClientDescriptor &other)
{
  NiftyLinkXMLBuilderBase::operator =(other);

  m_DeviceName = other.m_DeviceName;
  m_DeviceType = other.m_DeviceType;
  m_CommType = other.m_CommType;
  m_PortName = other.m_PortName;
  m_ClientIP = other.m_ClientIP;
  m_ClientPort = other.m_ClientPort;

  return *this;
}


//-----------------------------------------------------------------------------
QString NiftyLinkClientDescriptor::GetXMLAsString(void) const
{
  QDomDocument domDocument("ClientDescriptor");

  QDomElement root = domDocument.createElement("ClientDescriptor");

  QDomElement device = domDocument.createElement("Device");
  device.setAttribute("DeviceName", m_DeviceName);
  device.setAttribute("DeviceType", m_DeviceType);
  device.setAttribute("CommunicationType", m_CommType);
  device.setAttribute("PortName", m_PortName);
  root.appendChild(device);

  QDomElement client = domDocument.createElement("Client");
  client.setAttribute("ClientIP", m_ClientIP);
  client.setAttribute("ClientPort", m_ClientPort);
  root.appendChild(client);

  domDocument.appendChild(root);
  QString xmlAsString = domDocument.toString();

  QDomDocument xmlDoco;
  assert(xmlDoco.setContent(xmlAsString));

  return xmlAsString;
}


//-----------------------------------------------------------------------------
bool NiftyLinkClientDescriptor::SetXMLString(QString desc)
{
  bool isValid = false;

  QDomDocument xmlDoco;
  if (!xmlDoco.setContent(desc))
  {
    QLOG_ERROR() << "NiftyLinkClientDescriptor passed invalid XML";
    return false;
  }

  // A valid XML document was received, now it's time to parse it
  QDomElement root = xmlDoco.documentElement();
  if (root.tagName() != "ClientDescriptor")
  {
    QLOG_ERROR() << "NiftyLinkClientDescriptor::SetXMLString passed a message that wasn't a client descriptor";
    return false;
  }

  QString deviceName;
  QString deviceType;
  QString commType;
  QString portName;
  QString clientIP;
  QString clientPort;

  QDomNode n = root.firstChild();
  while (!n.isNull())
  {
    QDomElement e = n.toElement();

    if (!e.isNull())
    {
      if (e.tagName() == "Device")
      {
        deviceName.append(e.attribute("DeviceName", ""));
        deviceType.append(e.attribute("DeviceType", ""));
        commType.append(e.attribute("CommunicationType", ""));
        portName.append(e.attribute("PortName", ""));
      }
      else if (e.tagName() == "Client")
      {
        clientIP.append(e.attribute("ClientIP", ""));
        clientPort.append(e.attribute("ClientPort", ""));
      }
    }
    n = n.nextSibling();
  }

  // This might be a bit strict?
  if (deviceName.size() > 0
      && deviceType.size() > 0
      && commType.size() > 0
      && portName.size() > 0
      && clientIP.size() > 0
      && clientPort.size() > 0
  )
  {
    // ie. only update member variables if we have a valid message.
    this->m_DeviceName = deviceName;
    this->m_DeviceType = deviceType;
    this->m_CommType = commType;
    this->m_PortName = portName;
    this->m_ClientIP = clientIP;
    this->m_ClientPort = clientPort;
    isValid = true;
  }
  return isValid;
}


//-----------------------------------------------------------------------------
NiftyLinkCommandDescriptor::NiftyLinkCommandDescriptor(const NiftyLinkCommandDescriptor &other)
  : NiftyLinkXMLBuilderBase(other)
{
  m_CommandName = other.m_CommandName;
  m_NumOfParameters = other.m_NumOfParameters;

  m_ParameterNames = other.m_ParameterNames;
  m_ParameterTypes = other.m_ParameterTypes;
  m_ParameterValues = other.m_ParameterValues;
}


//-----------------------------------------------------------------------------
NiftyLinkCommandDescriptor & NiftyLinkCommandDescriptor::operator=(const NiftyLinkCommandDescriptor &other)
{
  NiftyLinkXMLBuilderBase::operator =(other);

  m_CommandName = other.m_CommandName;
  m_NumOfParameters = other.m_NumOfParameters;

  m_ParameterNames = other.m_ParameterNames;
  m_ParameterTypes = other.m_ParameterTypes;
  m_ParameterValues = other.m_ParameterValues;

  return *this;
}


//-----------------------------------------------------------------------------
void NiftyLinkCommandDescriptor::AddParameter(QString pName, QString pType, QString pVal)
{
  m_ParameterNames.append(pName);
  m_ParameterTypes.append(pType);
  m_ParameterValues.append(pVal);
  m_NumOfParameters++;
}


//-----------------------------------------------------------------------------
QString NiftyLinkCommandDescriptor::GetXMLAsString(void) const
{
  QDomDocument domDocument("CommandDescriptor");

  QDomElement root = domDocument.createElement("CommandDescriptor");

  QDomElement cmd = domDocument.createElement("Command");
  cmd.setAttribute("CommandName", m_CommandName);
  cmd.setAttribute("NumOfParameters", m_NumOfParameters);

  root.appendChild(cmd);

  for (int i = 0; i < m_NumOfParameters; i++)
  {
    QDomElement param = domDocument.createElement("Parameter");
    param.setAttribute("Name", m_ParameterNames.at(i));
    param.setAttribute("Type", m_ParameterTypes.at(i));
    param.setAttribute("Value", m_ParameterValues.at(i));
    root.appendChild(param);
  }

  domDocument.appendChild(root);
  QString xmlAsString = domDocument.toString();

  QDomDocument xmlDoco;
  assert(xmlDoco.setContent(xmlAsString));

  return xmlAsString;
}


//-----------------------------------------------------------------------------
bool NiftyLinkCommandDescriptor::SetXMLString(QString desc)
{
  bool isValid = false;

  QDomDocument xmlDoco;
  if (!xmlDoco.setContent(desc))
  {
    QLOG_ERROR() << "NiftyLinkCommandDescriptor passed invalid XML";
    return false;
  }

  // A valid XML document was received, now it's time to parse it
  QDomElement root = xmlDoco.documentElement();
  if (root.tagName() != "CommandDescriptor")
  {
    QLOG_ERROR() << "NiftyLinkCommandDescriptor::SetXMLString passed a message that wasn't a command descriptor";
    return false;
  }

  QDomNode n = root.firstChild();

  QString commandName;
  int numParams;
  QStringList paramNames;
  QStringList paramTypes;
  QStringList paramValues;
  bool parsedNumParams = false;

  while (!n.isNull())
  {
    QDomElement e = n.toElement();

    if (!e.isNull())
    {
      if (e.tagName() == "Command")
      {
        commandName.append(e.attribute("CommandName", ""));
        numParams = e.attribute("NumOfParameters", "").toInt(&parsedNumParams, 10);
      }
      else if (e.tagName() == "Parameter")
      {
        paramNames.append(e.attribute("Name", ""));
        paramTypes.append(e.attribute("Type", ""));
        paramValues.append(e.attribute("Value", ""));
      }
    }
    n = n.nextSibling();
  }

  if (parsedNumParams
      && commandName.size() > 0
      && paramNames.size() == numParams
      && paramTypes.size() == numParams
      && paramValues.size() == numParams
      )
  {
    this->m_CommandName = commandName;
    this->m_NumOfParameters = numParams;
    this->m_ParameterNames = paramNames;
    this->m_ParameterTypes = paramTypes;
    this->m_ParameterValues = paramValues;
    isValid = true;
  }

  return isValid;
}


//-----------------------------------------------------------------------------
QString NiftyLinkCommandDescriptor::GetParameterName(int i) const
{
  if (i >= m_ParameterNames.count())
  {
    return QString();
  }

  QString pname = m_ParameterNames.at(i);
  if (pname.isNull() || pname.isEmpty())
  {
    return QString();
  }
  else
  {
    return pname;
  }
}


//-----------------------------------------------------------------------------
QString NiftyLinkCommandDescriptor::GetParameterType(int i) const
{
  if (i >= m_ParameterTypes.count())
  {
    return QString();
  }

  QString ptype = m_ParameterTypes.at(i);
  if (ptype.isNull() || ptype.isEmpty())
  {
    return QString();
  }
  else
  {
    return ptype;
  }
}


//-----------------------------------------------------------------------------
QString NiftyLinkCommandDescriptor::GetParameterValue(int i) const
{
  if (i >= m_ParameterValues.count())
  {
    return QString();
  }

  QString pval = m_ParameterValues.at(i);
  if (pval.isNull() || pval.isEmpty())
  {
    return QString();
  }
  else
  {
    return pval;
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkCommandDescriptor::GetParameterAllFields(int i, QString &pName, QString &pType, QString &pValue) const
{
  if (i >= m_ParameterValues.count())
  {
    return;
  }

  QString temp = m_ParameterNames.at(i);
  if (temp.isNull() || temp.isEmpty())
  {
    pName = temp;
  }
  else
  {
    return;
  }

  temp = m_ParameterTypes.at(i);
  if (temp.isNull() || temp.isEmpty())
  {
    pType = temp;
  }
  else
  {
    return;
  }

  temp = m_ParameterValues.at(i);
  if (temp.isNull() || temp.isEmpty())
  {
    pValue = temp;
  }
  else
  {
    return;
  }

}


//-----------------------------------------------------------------------------
QStringList NiftyLinkCommandDescriptor::GetParameterNames(void) const
{
  return m_ParameterNames;
}


//-----------------------------------------------------------------------------
QStringList NiftyLinkCommandDescriptor::GetParameterTypes(void) const
{
  return m_ParameterTypes;
}


//-----------------------------------------------------------------------------
QStringList NiftyLinkCommandDescriptor::GetParameterValues(void) const
{
  return m_ParameterValues;
}


//-----------------------------------------------------------------------------
NiftyLinkTrackerClientDescriptor::NiftyLinkTrackerClientDescriptor(const NiftyLinkTrackerClientDescriptor &other)
  : NiftyLinkClientDescriptor(other)
{
  m_TrackerTools = other.m_TrackerTools;
}


//-----------------------------------------------------------------------------
NiftyLinkTrackerClientDescriptor & NiftyLinkTrackerClientDescriptor::operator=(const NiftyLinkTrackerClientDescriptor &other)
{
  NiftyLinkClientDescriptor::operator=(other);
  m_TrackerTools = other.m_TrackerTools;

  return *this;
}


//-----------------------------------------------------------------------------
QString NiftyLinkTrackerClientDescriptor::GetXMLAsString(void) const
{
  QDomDocument domDocument("TrackerClientDescriptor");

  QDomElement root = domDocument.createElement("TrackerClientDescriptor");

  QDomElement device = domDocument.createElement("Device");
  device.setAttribute("DeviceName", m_DeviceName);
  device.setAttribute("DeviceType", m_DeviceType);
  device.setAttribute("CommunicationType", m_CommType);
  device.setAttribute("PortName", m_PortName);
  root.appendChild(device);

  QDomElement client = domDocument.createElement("Client");
  client.setAttribute("ClientIP", m_ClientIP);
  client.setAttribute("ClientPort", m_ClientPort);
  root.appendChild(client);

  for (int i = 0; i < m_TrackerTools.count(); i++)
  {
    QDomElement param = domDocument.createElement("TrackerTool");
    param.setAttribute("Name", m_TrackerTools.at(i));
    root.appendChild(param);
  }

  domDocument.appendChild(root);
  QString xmlAsString = domDocument.toString();

  QDomDocument xmlDoco;
  assert(xmlDoco.setContent(xmlAsString));

  return xmlAsString;
}


//-----------------------------------------------------------------------------
bool NiftyLinkTrackerClientDescriptor::SetXMLString(QString desc)
{
  bool isValid = false;

  QDomDocument xmlDoco;
  if (!xmlDoco.setContent(desc))
  {
    QLOG_ERROR() << "NiftyLinkTrackerClientDescriptor passed invalid XML";
    return false;
  }

  // A valid XML document was received, now it's time to parse it
  QDomElement root = xmlDoco.documentElement();
  if (root.tagName() != "TrackerClientDescriptor")
  {
    QLOG_ERROR() << "NiftyLinkTrackerClientDescriptor::SetXMLString passed a message that wasn't a tracker client descriptor";
    return false;
  }

  QString deviceName;
  QString deviceType;
  QString commType;
  QString portName;
  QString clientIP;
  QString clientPort;
  QStringList trackerTools;

  QDomNode n = root.firstChild();

  while (!n.isNull())
  {
    QDomElement e = n.toElement();

    if (!e.isNull())
    {
      if (e.tagName() == "Device")
      {
        deviceName.append(e.attribute("DeviceName", ""));
        deviceType.append(e.attribute("DeviceType", ""));
        commType.append(e.attribute("CommunicationType", ""));
        portName.append(e.attribute("PortName", ""));
      }
      else if (e.tagName() == "Client")
      {
        clientIP.append(e.attribute("ClientIP", ""));
        clientPort.append(e.attribute("ClientPort", ""));
      }
      else if (e.tagName() == "TrackerTool")
      {
        trackerTools.append(e.attribute("Name", ""));
      }
    }
    n = n.nextSibling();
  }

  // This might be a bit strict?
  if (deviceName.size() > 0
      && deviceType.size() > 0
      && commType.size() > 0
      && portName.size() > 0
      && clientIP.size() > 0
      && clientPort.size() > 0
  )
  {
    // ie. only update member variables if we have a valid message.
    this->m_DeviceName = deviceName;
    this->m_DeviceType = deviceType;
    this->m_CommType = commType;
    this->m_PortName = portName;
    this->m_ClientIP = clientIP;
    this->m_ClientPort = clientPort;
    this->m_TrackerTools = trackerTools;
    isValid = true;
  }

  return isValid;
}

} // end namespace niftk
