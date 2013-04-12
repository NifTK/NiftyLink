/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkXMLBuilder.h"

QString XMLBuilderBase::ParseDescriptorType(QString xmlString)
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ClientDescriptorXMLBuilder::ClientDescriptorXMLBuilder(const ClientDescriptorXMLBuilder &other)
  : XMLBuilderBase(other)
{
  m_DeviceName = other.m_DeviceName;
  m_DeviceType = other.m_DeviceType;
  m_CommType = other.m_CommType;
  m_PortName = other.m_PortName;
  m_ClientIP = other.m_ClientIP;
  m_ClientPort = other.m_ClientPort;
}

ClientDescriptorXMLBuilder& ClientDescriptorXMLBuilder::operator=(const ClientDescriptorXMLBuilder &other)
{
  XMLBuilderBase::operator =(other);

  m_DeviceName = other.m_DeviceName;
  m_DeviceType = other.m_DeviceType;
  m_CommType = other.m_CommType;
  m_PortName = other.m_PortName;
  m_ClientIP = other.m_ClientIP;
  m_ClientPort = other.m_ClientPort;

  return *this;
}


QString ClientDescriptorXMLBuilder::GetXMLAsString(void)
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

  m_DescriptorString.clear();
  m_DescriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_MessageValid = xmlDoco.setContent(m_DescriptorString);

  if (m_MessageValid)
  {
    return m_DescriptorString;
  }
  else
  {
    return QString();
  }
}

void ClientDescriptorXMLBuilder::SetXMLString(QString desc)
{
  m_DescriptorString.clear();
  m_DescriptorString.append(desc);

  QDomDocument xmlDoco;

  m_MessageValid = xmlDoco.setContent(desc);

  if (m_MessageValid)
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
            m_DeviceName.clear();
            m_DeviceName.append(e.attribute("DeviceName", ""));

            m_DeviceType.clear();
            m_DeviceType.append(e.attribute("DeviceType", ""));

            m_CommType.clear();
            m_CommType.append(e.attribute("CommunicationType", ""));

            m_PortName.clear();
            m_PortName.append("PortName: ");
          }
          else if (e.tagName() == "Client")
          {
            m_ClientIP.clear();
            m_ClientIP.append(e.attribute("ClientIP", ""));

            m_ClientPort.clear();
            m_ClientPort.append(e.attribute("ClientPort", ""));
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
  m_CommandName = other.m_CommandName;
  m_NumOfParameters = other.m_NumOfParameters;

  m_ParameterNames = other.m_ParameterNames;
  m_ParameterTypes = other.m_ParameterTypes;
  m_ParameterValues = other.m_ParameterValues;
}

CommandDescriptorXMLBuilder & CommandDescriptorXMLBuilder::operator=(const CommandDescriptorXMLBuilder &other)
{
  XMLBuilderBase::operator =(other);

  m_CommandName = other.m_CommandName;
  m_NumOfParameters = other.m_NumOfParameters;

  m_ParameterNames = other.m_ParameterNames;
  m_ParameterTypes = other.m_ParameterTypes;
  m_ParameterValues = other.m_ParameterValues;

  return *this;
}


void CommandDescriptorXMLBuilder::AddParameter(QString pName, QString pType, QString pVal)
{
  m_ParameterNames.append(pName);
  m_ParameterTypes.append(pType);
  m_ParameterValues.append(pVal);
  m_NumOfParameters++;
}

QString CommandDescriptorXMLBuilder::GetXMLAsString(void)
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

  m_DescriptorString.clear();
  m_DescriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_MessageValid = xmlDoco.setContent(m_DescriptorString);

  if (m_MessageValid)
  {
    return m_DescriptorString;
  }
  else
  {
    return QString();
  }
}

void CommandDescriptorXMLBuilder::SetXMLString(QString desc)
{
  m_DescriptorString.clear();
  m_DescriptorString.append(desc);

  QDomDocument xmlDoco;

  m_MessageValid = xmlDoco.setContent(desc);

  if (m_MessageValid)
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
            m_CommandName.clear();
            m_CommandName.append(e.attribute("CommandName", ""));

            bool ok = false;
            m_NumOfParameters = e.attribute("NumOfParameters", "").toInt(&ok, 10);
          }
          else if (e.tagName() == "Parameter")
          {
            m_ParameterNames.append(e.attribute("Name", ""));
            m_ParameterTypes.append(e.attribute("Type", ""));
            m_ParameterValues.append(e.attribute("Value", ""));
          }
        }

        n = n.nextSibling();
      }
    }

  }
}

QString CommandDescriptorXMLBuilder::GetParameterName(int i)
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

QString CommandDescriptorXMLBuilder::GetParameterType(int i)
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

QString CommandDescriptorXMLBuilder::GetParameterValue(int i)
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

void CommandDescriptorXMLBuilder::GetParameterAllFields(int i, QString &pName, QString &pType, QString &pValue)
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


QStringList CommandDescriptorXMLBuilder::GetParameterNames(void)
{
  return m_ParameterNames;
}

QStringList CommandDescriptorXMLBuilder::GetParameterTypes(void)
{
  return m_ParameterTypes;
}

QStringList CommandDescriptorXMLBuilder::GetParameterValues(void)
{
  return m_ParameterValues;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

TrackerClientDescriptor::TrackerClientDescriptor(const TrackerClientDescriptor &other)
  : ClientDescriptorXMLBuilder(other)
{
  m_TrackerTools = other.m_TrackerTools;
}

TrackerClientDescriptor & TrackerClientDescriptor::operator=(const TrackerClientDescriptor &other)
{
  ClientDescriptorXMLBuilder::operator=(other);
  m_TrackerTools = other.m_TrackerTools;

  return *this;
}

QString TrackerClientDescriptor::GetXMLAsString(void)
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

  if (m_DeviceType == "Tracker")
  {
    for (int i = 0; i < m_TrackerTools.count(); i++)
    {
      QDomElement param = domDocument.createElement("TrackerTool");
      param.setAttribute("Name", m_TrackerTools.at(i));
      root.appendChild(param);
    }
  }

  domDocument.appendChild(root);

  m_DescriptorString.clear();
  m_DescriptorString.append(domDocument.toString());

  QDomDocument xmlDoco;
  m_MessageValid = xmlDoco.setContent(m_DescriptorString);

  if (m_MessageValid)
  {
    return m_DescriptorString;
  }
  else
  {
    return QString();
  }
}

void TrackerClientDescriptor::SetXMLString(QString desc)
{
  m_DescriptorString.clear();
  m_DescriptorString.append(desc);

  QDomDocument xmlDoco;

  m_MessageValid = xmlDoco.setContent(desc);

  if (m_MessageValid)
  {
    // A valid XML document was received, now it's time to parse it
    QDomElement root = xmlDoco.documentElement();

    if (root.tagName() == "TrackerClientDescriptor")
    {
      QDomNode n = root.firstChild();

      while (!n.isNull())
      {
        QDomElement e = n.toElement();

        if (!e.isNull())
        {
          if (e.tagName() == "Device")
          {
            m_DeviceName.clear();
            m_DeviceName.append(e.attribute("DeviceName", ""));

            m_DeviceType.clear();
            m_DeviceType.append(e.attribute("DeviceType", ""));

            m_CommType.clear();
            m_CommType.append(e.attribute("CommunicationType", ""));

            m_PortName.clear();
            m_PortName.append("PortName: ");
          }
          else if (e.tagName() == "Client")
          {
            m_ClientIP.clear();
            m_ClientIP.append(e.attribute("ClientIP", ""));

            m_ClientPort.clear();
            m_ClientPort.append(e.attribute("ClientPort", ""));
          }
          else if (e.tagName() == "TrackerTool")
          {
            m_TrackerTools.append(e.attribute("Name", ""));
          }
        }

        n = n.nextSibling();
      }
    }
  }
}
