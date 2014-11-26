/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkStringMessageHelpers.h"

#include <NiftyLinkMacro.h>
#include <NiftyLinkUtils.h>

#include <sstream>
#include <iostream>
#include <iomanip>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateStringMessage(const QString& deviceName,
                                                       const QString& hostName,
                                                       const int& portNumber,
                                                       const QString& stringMessage,
                                                       igtl::TimeStamp::Pointer& timeCreated
                                                       )
{
  igtl::StringMessage::Pointer msg = igtl::StringMessage::New();
  msg->SetDeviceName(deviceName.toStdString().c_str());
  msg->SetString(stringMessage.toStdString().c_str());

  timeCreated->GetTime();

  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName(deviceName);
  m->SetSenderHostName(hostName);    // don't do these lookups here. They are expensive.
  m->SetSenderPortNumber(portNumber);
  m->SetTimeArrived(timeCreated);
  m->SetTimeReceived(timeCreated);

  return m;
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateStringMessage(const QString& deviceName,
                                                       const QString& hostName,
                                                       const int& portNumber,
                                                       const QString& stringMessage
                                                       )
{
  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  return CreateStringMessage(deviceName, hostName, portNumber, stringMessage, timeCreated);
}

} // end namespace niftk
