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

#include "NiftyLinkXMLBuilder.h"

QString XMLBuilderBase::parseDescriptorType(QString xmlString)
{
  QDomDocument xmlDoco;

  bool messageValid = xmlDoco.setContent(xmlString);

  if (messageValid)
  {
     // A valid XML document was received, now it's time to parse it
      QDomElement root = xmlDoco.documentElement();
      
      return root.tagName();
  }
  else return QString();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ClientDescriptorXMLBuilder::ClientDescriptorXMLBuilder(const ClientDescriptorXMLBuilder &other)
  : XMLBuilderBase(other)
{
  m_deviceName = other.m_deviceName;
  m_deviceType = other.m_deviceType;
  m_commType   = other.m_commType;
  m_portName   = other.m_portName;
  m_clientIP   = other.m_clientIP;
  m_clientPort = other.m_clientPort;
}

ClientDescriptorXMLBuilder& ClientDescriptorXMLBuilder::operator=(const ClientDescriptorXMLBuilder &other)
{
  XMLBuilderBase::operator =(other);
  
  m_deviceName = other.m_deviceName;
  m_deviceType = other.m_deviceType;
  m_commType   = other.m_commType;
  m_portName   = other.m_portName;
  m_clientIP   = other.m_clientIP;
  m_clientPort = other.m_clientPort;

  return *this;
}


QString ClientDescriptorXMLBuilder::getXMLAsString(void)
{
  QDomDocument domDocument("ClientDescriptor");

  QDomElement root = domDocument.createElement("ClientDescriptor");
  
  QDomElement device = domDocument.createElement("Device");
  device.setAttribute("DeviceName", m_deviceName);
  device.setAttribute("DeviceType", m_deviceType);
  device.setAttribute("CommunicationType", m_commType);
  device.setAttribute("PortName", m_portName);
  root.appendChild(device);

  QDomElement client = domDocument.createElement("Client");
  device.setAttribute("ClientIP", m_clientIP);
  device.setAttribute("ClientPort", m_clientPort);
  root.appendChild(client);

  domDocument.appendChild(root);

  m_descriptorString.clear();
  m_descriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_messageValid = xmlDoco.setContent(m_descriptorString);

  if (m_messageValid)
    return m_descriptorString;
  else
    return QString();
}

void ClientDescriptorXMLBuilder::setXMLString(QString desc)
{
  m_descriptorString.clear();
  m_descriptorString.append(desc);

  QDomDocument xmlDoco;

  m_messageValid = xmlDoco.setContent(desc);

  if (m_messageValid)
  {
     // A valid XML document was received, now it's time to parse it
      QDomElement root = xmlDoco.documentElement();
      
      if (root.tagName() == "ClientDescriptor")
      {
        QDomNode n = root.firstChild();
        
        while (!n.isNull())
        {
          QDomElement e = n.toElement();
          
          if (!e.isNull())
          {
            if (e.tagName() == "Device")
            {
              m_deviceName.clear();
              m_deviceName.append(e.attribute("DeviceName", ""));
              
              m_deviceType.clear();
              m_deviceType.append(e.attribute("DeviceType", ""));
              
              m_commType.clear();
              m_commType.append(e.attribute("CommunicationType", ""));
              
              m_portName.clear();
              m_portName.append("PortName: ");              
            }
            else if (e.tagName() == "Client")
            {
              m_clientIP.clear();
              m_clientIP.append(e.attribute("ClientIP", ""));
              
              m_clientPort.clear();
              m_clientPort.append(e.attribute("ClientPort", ""));
            }
          }

          n = n.nextSibling();
        }
      }

  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

CommandDescriptorXMLBuilder::CommandDescriptorXMLBuilder(const CommandDescriptorXMLBuilder &other)
  : XMLBuilderBase(other)
{
  m_commandName = other.m_commandName;
  m_numOfParameters = other.m_numOfParameters;
    
  m_parameterNames = other.m_parameterNames;
  m_parameterTypes = other.m_parameterTypes;
  m_parameterValues = other.m_parameterValues;
}

CommandDescriptorXMLBuilder & CommandDescriptorXMLBuilder::operator =(const CommandDescriptorXMLBuilder &other)
{
  XMLBuilderBase::operator =(other);

  m_commandName = other.m_commandName;
  m_numOfParameters = other.m_numOfParameters;
    
  m_parameterNames = other.m_parameterNames;
  m_parameterTypes = other.m_parameterTypes;
  m_parameterValues = other.m_parameterValues;

  return *this;
}


void CommandDescriptorXMLBuilder::addParameter(QString pName, QString pType, QString pVal)
{ 
  m_parameterNames.append(pName); 
  m_parameterTypes.append(pType); 
  m_parameterValues.append(pVal); 
  m_numOfParameters++; 
}

QString CommandDescriptorXMLBuilder::getXMLAsString(void)
{
  QDomDocument domDocument("CommandDescriptor");

  QDomElement root = domDocument.createElement("CommandDescriptor");
  
  QDomElement cmd = domDocument.createElement("Command");
  cmd.setAttribute("CommandName", m_commandName);
  cmd.setAttribute("NumOfParameters", m_numOfParameters);
  
  root.appendChild(cmd);

  for (int i = 0; i < m_numOfParameters; i++)
  {
    QDomElement param = domDocument.createElement("Parameter");
    param.setAttribute("Name", m_parameterNames.at(i));
    param.setAttribute("Type", m_parameterTypes.at(i));
    param.setAttribute("Value", m_parameterValues.at(i));
    root.appendChild(param);
  }

  domDocument.appendChild(root);

  m_descriptorString.clear();
  m_descriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_messageValid = xmlDoco.setContent(m_descriptorString);

  if (m_messageValid)
    return m_descriptorString;
  else
    return QString();
}

void CommandDescriptorXMLBuilder::setXMLString(QString desc)
{
  m_descriptorString.clear();
  m_descriptorString.append(desc);

  QDomDocument xmlDoco;

  m_messageValid = xmlDoco.setContent(desc);

  if (m_messageValid)
  {
     // A valid XML document was received, now it's time to parse it
      QDomElement root = xmlDoco.documentElement();
      
      if (root.tagName() == "CommandDescriptor")
      {
        QDomNode n = root.firstChild();
        
        while (!n.isNull())
        {
          QDomElement e = n.toElement();
          
          if (!e.isNull())
          {
            if (e.tagName() == "Command")
            {
              m_commandName.clear();
              m_commandName.append(e.attribute("CommandName", ""));
              
              bool ok = false;
              m_numOfParameters = e.attribute("NumOfParameters", "").toInt(&ok, 10);       
            }
            else if (e.tagName() == "Parameter")
            {
              m_parameterNames.append(e.attribute("Name", ""));
              m_parameterTypes.append(e.attribute("Type", ""));
              m_parameterValues.append(e.attribute("Value", ""));
            }
          }

          n = n.nextSibling();
        }
      }

  }
}

QString CommandDescriptorXMLBuilder::getParameterName(int i)
{
  if (i >= m_parameterNames.count())
    return QString();
  
  QString pname = m_parameterNames.at(i);
  if (pname.isNull() || pname.isEmpty())
    return QString();
  else return pname;
}

QString CommandDescriptorXMLBuilder::getParameterType(int i)
{
  if (i >= m_parameterTypes.count())
    return QString();
  
  QString ptype = m_parameterTypes.at(i);
  if (ptype.isNull() || ptype.isEmpty())
    return QString();
  else return ptype;
}

QString CommandDescriptorXMLBuilder::getParameterValue(int i)
{
  if (i >= m_parameterValues.count())
    return QString();
  
  QString pval = m_parameterValues.at(i);
  if (pval.isNull() || pval.isEmpty())
    return QString();
  else return pval;
}
  
QStringList CommandDescriptorXMLBuilder::getParameterNames(void)
{
  return m_parameterNames;
}

QStringList CommandDescriptorXMLBuilder::getParameterTypes(void)
{
  return m_parameterTypes;
}

QStringList CommandDescriptorXMLBuilder::getParameterValues(void)
{
  return m_parameterValues;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

TrackerClientDescriptor::TrackerClientDescriptor(const TrackerClientDescriptor &other)
  : ClientDescriptorXMLBuilder(other)
{
  m_trackerTools = other.m_trackerTools;
}

TrackerClientDescriptor & TrackerClientDescriptor::operator=(const TrackerClientDescriptor &other)
{
  ClientDescriptorXMLBuilder::operator=(other);
  m_trackerTools = other.m_trackerTools;

  return *this;
}

QString TrackerClientDescriptor::getXMLAsString(void)
{
  QDomDocument domDocument("ClientDescriptor");

  QDomElement root = domDocument.createElement("ClientDescriptor");
  
  QDomElement device = domDocument.createElement("Device");
  device.setAttribute("DeviceName", m_deviceName);
  device.setAttribute("DeviceType", m_deviceType);
  device.setAttribute("CommunicationType", m_commType);
  device.setAttribute("PortName", m_portName);
  root.appendChild(device);

  QDomElement client = domDocument.createElement("Client");
  device.setAttribute("ClientIP", m_clientIP);
  device.setAttribute("ClientPort", m_clientPort);
  root.appendChild(client);

  if (m_deviceType == "Tracker")
  {
    for (int i = 0; i < m_trackerTools.count(); i++)
    {
      QDomElement param = domDocument.createElement("TrackerTool");
      param.setAttribute("Name", m_trackerTools.at(i));
      root.appendChild(param);
    }
  }

  domDocument.appendChild(root);

  m_descriptorString.clear();
  m_descriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_messageValid = xmlDoco.setContent(m_descriptorString);

  if (m_messageValid)
    return m_descriptorString;
  else
    return QString();
}

void TrackerClientDescriptor::setXMLString(QString desc)
{
  m_descriptorString.clear();
  m_descriptorString.append(desc);

  QDomDocument xmlDoco;

  m_messageValid = xmlDoco.setContent(desc);

  if (m_messageValid)
  {
     // A valid XML document was received, now it's time to parse it
      QDomElement root = xmlDoco.documentElement();
      
      if (root.tagName() == "ClientDescriptor")
      {
        QDomNode n = root.firstChild();
        
        while (!n.isNull())
        {
          QDomElement e = n.toElement();
          
          if (!e.isNull())
          {
            if (e.tagName() == "Device")
            {
              m_deviceName.clear();
              m_deviceName.append(e.attribute("DeviceName", ""));
              
              m_deviceType.clear();
              m_deviceType.append(e.attribute("DeviceType", ""));
              
              m_commType.clear();
              m_commType.append(e.attribute("CommunicationType", ""));
              
              m_portName.clear();
              m_portName.append("PortName: ");              
            }
            else if (e.tagName() == "Client")
            {
              m_clientIP.clear();
              m_clientIP.append(e.attribute("ClientIP", ""));
              
              m_clientPort.clear();
              m_clientPort.append(e.attribute("ClientPort", ""));
            }
            else if (e.tagName() == "TrackerTool")
            {
              m_trackerTools.append(e.attribute("Name", ""));
            }
          }

          n = n.nextSibling();
        }
      }

  }
}