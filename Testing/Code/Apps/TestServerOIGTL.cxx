/*=========================================================================

  Program:   Open IGT Link -- Example for Data Receiving Server Program
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <NiftyLinkMessageCounter.h>
#include <NiftyLinkMessageContainer.h>

#include <QsLog.h>
#include <QsLogDest.h>

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <cstring>

#include <igtlSocket.h>
#include <igtlServerSocket.h>
#include <igtlOSUtil.h>
#include <igtlMessageHeader.h>
#include <igtlTrackingDataMessage.h>
#include <igtlStringMessage.h>
#include <igtlImageMessage.h>

niftk::NiftyLinkMessageCounter counter;
int ReceiveImage(igtl::Socket * socket, igtl::MessageHeader * header);
int ReceiveString(igtl::Socket * socket, igtl::MessageHeader * header);
int ReceiveTrackingData(igtl::Socket * socket, igtl::MessageHeader * header);

int main(int argc, char* argv[])
{
  // Init the logging mechanism.
  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 2) // check number of arguments
    {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port>"    << std::endl;
    std::cerr << "    <port>     : Port # (18944 in Slicer default)"   << std::endl;
    exit(0);
    }

  int    port     = atoi(argv[1]);

  igtl::ServerSocket::Pointer serverSocket;
  serverSocket = igtl::ServerSocket::New();
  int r = serverSocket->CreateServer(port);

  if (r < 0)
    {
    std::cerr << "Cannot create a server socket." << std::endl;
    exit(0);
    }

  igtl::Socket::Pointer socket;
  
  while (1)
    {
    //------------------------------------------------------------
    // Waiting for Connection
    socket = serverSocket->WaitForConnection(10000);
    
    if (socket.IsNotNull()) // if client connected
    {
      // Create a message buffer to receive header
      igtl::MessageHeader::Pointer headerMsg;
      headerMsg = igtl::MessageHeader::New();

      while(1)
      {

        // Initialize receive buffer
        headerMsg->InitPack();

        // Receive generic header from the socket
        int r = socket->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
        if (r == 0)
        {
          socket->CloseSocket();
        }
        if (r != headerMsg->GetPackSize())
        {
          continue;
        }

        // Deserialize the header
        headerMsg->Unpack();

        // Check data type and receive data body
        if (strcmp(headerMsg->GetDeviceType(), "IMAGE") == 0)
        {
          r = ReceiveImage(socket, headerMsg);
        }
        else if (strcmp(headerMsg->GetDeviceType(), "STRING") == 0)
        {
          r = ReceiveString(socket, headerMsg);
        }
        else if (strcmp(headerMsg->GetDeviceType(), "TDATA") == 0)
        {
          r = ReceiveTrackingData(socket, headerMsg);
        }
        else
        {
          throw std::runtime_error("Unrecognised message type");
        }

        if (r != 1)
        {
          std::cerr << "Failed to read a message" << std::endl;
          exit(0);
        }
      }
    }
  }
    
  //------------------------------------------------------------
  // Close connection (The example code never reaches to this section ...)
  socket->CloseSocket();
}

int ReceiveImage(igtl::Socket * socket, igtl::MessageHeader * header)
{
  // Create a message buffer to receive transform data
  igtl::ImageMessage::Pointer imgMsg;
  imgMsg = igtl::ImageMessage::New();
  imgMsg->SetMessageHeader(header);
  imgMsg->AllocatePack();
  
  // Receive transform data from the socket
  socket->Receive(imgMsg->GetPackBodyPointer(), imgMsg->GetPackBodySize());
  imgMsg->Unpack();
  
  igtl::TimeStamp::Pointer timeReceived = igtl::TimeStamp::New();
  timeReceived->GetTime();

  niftk::NiftyLinkMessageContainer::Pointer m = (niftk::NiftyLinkMessageContainer::Pointer(new niftk::NiftyLinkMessageContainer()));
  m->SetOwnerName("ReceiveImage");
  m->SetMessage(imgMsg.GetPointer());
  m->SetTimeReceived(timeReceived);

  // For statistics.
  counter.OnMessageReceived(m);

  return 1;
}

int ReceiveString(igtl::Socket * socket, igtl::MessageHeader * header)
{
  // Create a message buffer to receive transform data
  igtl::StringMessage::Pointer stringMsg;
  stringMsg = igtl::StringMessage::New();
  stringMsg->SetMessageHeader(header);
  stringMsg->AllocatePack();

  // Receive transform data from the socket
  socket->Receive(stringMsg->GetPackBodyPointer(), stringMsg->GetPackBodySize());
  stringMsg->Unpack();

  if (std::string(stringMsg->GetDeviceType()) == std::string("STRING")
      && std::string(stringMsg->GetString()) == std::string("STATS"))
  {
    counter.OnOutputStats();
    exit(0);
  }

  igtl::TimeStamp::Pointer timeReceived = igtl::TimeStamp::New();
  timeReceived->GetTime();

  niftk::NiftyLinkMessageContainer::Pointer m = (niftk::NiftyLinkMessageContainer::Pointer(new niftk::NiftyLinkMessageContainer()));
  m->SetOwnerName("ReceiveString");
  m->SetMessage(stringMsg.GetPointer());
  m->SetTimeReceived(timeReceived);

  // For statistics.
  counter.OnMessageReceived(m);

  return 1;
}

int ReceiveTrackingData(igtl::Socket * socket, igtl::MessageHeader * header)
{
  igtl::TrackingDataMessage::Pointer trackingDataMsg = igtl::TrackingDataMessage::New();
  trackingDataMsg->SetMessageHeader(header);
  trackingDataMsg->AllocatePack();

  // Receive data from the socket
  socket->Receive(trackingDataMsg->GetPackBodyPointer(), trackingDataMsg->GetPackBodySize());
  trackingDataMsg->Unpack();

  igtl::TimeStamp::Pointer timeReceived = igtl::TimeStamp::New();
  timeReceived->GetTime();

  niftk::NiftyLinkMessageContainer::Pointer m = (niftk::NiftyLinkMessageContainer::Pointer(new niftk::NiftyLinkMessageContainer()));
  m->SetOwnerName("ReceiveTrackingData");
  m->SetMessage(trackingDataMsg.GetPointer());
  m->SetTimeReceived(timeReceived);

  // For statistics.
  counter.OnMessageReceived(m);

  return 1;
}
