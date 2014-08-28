/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkDescriptorTests.h"
#include <NiftyLinkUtils.h>
#include <NiftyLinkXMLBuilder.h>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkDescriptorTests::ClientDescriptorTest()
{
  NiftyLinkClientDescriptor clientDescriptor;
  QVERIFY(clientDescriptor.GetClientIP().size() == 0);
  QVERIFY(clientDescriptor.GetClientPort().size() == 0);
  QVERIFY(clientDescriptor.GetCommunicationType().size() == 0);
  QVERIFY(clientDescriptor.GetDeviceName().size() == 0);
  QVERIFY(clientDescriptor.GetDeviceType().size() == 0);
  QVERIFY(clientDescriptor.GetPortName().size() == 0);

  clientDescriptor.SetClientIP("123.456.789.012");
  clientDescriptor.SetClientPort("1234");
  clientDescriptor.SetCommunicationType("TCP");
  clientDescriptor.SetDeviceName("TestImager");
  clientDescriptor.SetDeviceType("Imager");
  clientDescriptor.SetPortName("TestPort");

  QString xml = clientDescriptor.GetXMLAsString();

  QVERIFY(xml.size() > 0);
  std::cerr << "xml=\n" << xml.toStdString() << std::endl;

  QVERIFY(clientDescriptor.SetXMLString(xml));
  QVERIFY(clientDescriptor.GetClientIP() == "123.456.789.012");
  QVERIFY(clientDescriptor.GetClientPort() == "1234");
  QVERIFY(clientDescriptor.GetCommunicationType() == "TCP");
  QVERIFY(clientDescriptor.GetDeviceName() == "TestImager");
  QVERIFY(clientDescriptor.GetDeviceType() == "Imager");
  QVERIFY(clientDescriptor.GetPortName() == "TestPort");
}


//-----------------------------------------------------------------------------
void NiftyLinkDescriptorTests::TrackerClientDescriptorTest()
{
  NiftyLinkTrackerClientDescriptor clientDescriptor;
  QVERIFY(clientDescriptor.GetClientIP().size() == 0);
  QVERIFY(clientDescriptor.GetClientPort().size() == 0);
  QVERIFY(clientDescriptor.GetCommunicationType().size() == 0);
  QVERIFY(clientDescriptor.GetDeviceName().size() == 0);
  QVERIFY(clientDescriptor.GetDeviceType().size() == 0);
  QVERIFY(clientDescriptor.GetPortName().size() == 0);

  clientDescriptor.SetClientIP("123.456.789.012");
  clientDescriptor.SetClientPort("1234");
  clientDescriptor.SetCommunicationType("TCP");
  clientDescriptor.SetDeviceName("TestImager");
  clientDescriptor.SetDeviceType("Imager");
  clientDescriptor.SetPortName("TestPort");

  QString xml = clientDescriptor.GetXMLAsString();

  QVERIFY(xml.size() > 0);
  std::cerr << "xml=\n" << xml.toStdString() << std::endl;

  QVERIFY(clientDescriptor.SetXMLString(xml));
  QVERIFY(clientDescriptor.GetClientIP() == "123.456.789.012");
  QVERIFY(clientDescriptor.GetClientPort() == "1234");
  QVERIFY(clientDescriptor.GetCommunicationType() == "TCP");
  QVERIFY(clientDescriptor.GetDeviceName() == "TestImager");
  QVERIFY(clientDescriptor.GetDeviceType() == "Imager");
  QVERIFY(clientDescriptor.GetPortName() == "TestPort");
  QVERIFY(clientDescriptor.GetTrackerTools().size() == 0);

  clientDescriptor.AddTrackerTool("PointerTool");
  QString xml2 = clientDescriptor.GetXMLAsString();
  std::cerr << "xml2=\n" << xml2.toStdString() << std::endl;

  QVERIFY(xml != xml2);

  QVERIFY(clientDescriptor.GetTrackerTools().size() == 1);
  QStringList tools;
  tools << "PointerTool";
  QVERIFY(clientDescriptor.GetTrackerTools() == tools);
}


//-----------------------------------------------------------------------------
void NiftyLinkDescriptorTests::CommandDescriptorTest()
{
  NiftyLinkCommandDescriptor descriptor;
  QVERIFY(descriptor.GetCommandName().size() == 0);
  QVERIFY(descriptor.GetNumOfParameters() == 0);

  descriptor.SetCommandName("JustDoIt");
  descriptor.AddParameter("P1", "String", "Nike");
  descriptor.AddParameter("P2", "int", "10");

  QString xml = descriptor.GetXMLAsString();

  QVERIFY(xml.size() > 0);
  std::cerr << "xml=\n" << xml.toStdString() << std::endl;

  descriptor.SetXMLString(xml);
  QVERIFY(descriptor.GetCommandName() == "JustDoIt");
  QVERIFY(descriptor.GetNumOfParameters() == 2);
  QVERIFY(descriptor.GetParameterName(0) == "P1");
  QVERIFY(descriptor.GetParameterName(1) == "P2");
  QVERIFY(descriptor.GetParameterType(0) == "String");
  QVERIFY(descriptor.GetParameterType(1) == "int");
  QVERIFY(descriptor.GetParameterValue(0) == "Nike");
  QVERIFY(descriptor.GetParameterValue(1) == "10");
}

} // end namespace niftk

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkDescriptorTests )
