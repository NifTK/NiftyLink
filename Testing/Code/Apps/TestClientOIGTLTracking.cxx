/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include <NiftyLinkUtils.h>
#include <NiftyLinkMessageContainer.h>

#include <iostream>

#include <igtlTrackingDataMessage.h>
#include <igtlStringMessage.h>
#include <igtlClientSocket.h>

int main(int argc, char* argv[])
{
  if (argc != 6)
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <host> <port> <fps> <channels> <total>"    << std::endl;
    std::cerr << "    <host>       : Host." << std::endl;
    std::cerr << "    <port>       : Port #." << std::endl;
    std::cerr << "    <fps>        : Frequency (fps) to send image." << std::endl;
    std::cerr << "    <channels>   : Number of channels (tracking matrices per message)." << std::endl;
    std::cerr << "    <total>      : Total number of messages." << std::endl;
    exit(0);
  }

  QString hostName      = argv[1];
  int    port           = atoi(argv[2]);
  int    fps            = atoi(argv[3]);
  int    channels       = atoi(argv[4]);
  int    totalMessages  = atoi(argv[5]);

  std::cout << "hostName=" << hostName.toStdString() << std::endl;
  std::cout << "port=" << port << std::endl;
  std::cout << "fps=" << fps << std::endl;
  std::cout << "channels=" << channels << std::endl;
  std::cout << "totalMessages=" << totalMessages << std::endl;

  // Setup socket.
  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  int r = socket->ConnectToServer(hostName.toStdString().c_str(), port);
  if (r != 0)
  {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
  }

  igtl::TimeStamp::Pointer timeLastMessage = igtl::TimeStamp::New();
  timeLastMessage->GetTime();

  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  timeNow->GetTime();

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  int nanosecondsBetweenMessages = 1000000000 / fps;
  int numberMessagesSent = 0;
  int numberMessagesRequired = totalMessages;

  while (numberMessagesSent < numberMessagesRequired)
  {
    timeNow->GetTime();
    if (niftk::GetDifferenceInNanoSeconds(timeNow, timeLastMessage) > nanosecondsBetweenMessages)
    {
      timeCreated->GetTime();

      niftk::NiftyLinkMessageContainer::Pointer m = niftk::CreateTestTrackingDataMessage(timeCreated, channels);
      r = socket->Send(m->GetMessage()->GetPackPointer(), m->GetMessage()->GetPackSize());
      if (r == 0)
      {
        std::cerr << "Failed to send message." << std::endl;
        exit(0);
      }

      numberMessagesSent++;
      timeLastMessage->SetTimeInNanoseconds(timeNow->GetTimeStampInNanoseconds());
    }
  }

  igtl::StringMessage::Pointer msg = igtl::StringMessage::New();
  msg->SetDeviceName("TestClientOIGTLTracking");
  msg->SetString("STATS");
  msg->Pack();

  r = socket->Send(msg->GetPackPointer(), msg->GetPackSize());
  if (r == 0)
  {
    std::cerr << "Failed to send stats request." << std::endl;
    exit(0);
  }
}


