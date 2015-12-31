#/*================================================================================
#
#  NiftyLink:  A software library to facilitate communication over Socket.
#  
#  Copyright (c) University College London (UCL). All rights reserved.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.  
#
#  See LICENSE.txt in the top level directory for details. 
#
#=================================================================================*/

# --------------------------------------------------------------------------
# Project Setup

PROJECT(NiftyLink)

#########################
# Sources
#########################
SET(niftylink_SRCS
Common/NiftyLinkUtils.cxx
Common/NiftyLinkMessageStatsContainer.cxx
Common/NiftyLinkMessageCounter.cxx
Common/QsDebugOutput.cxx
Common/QsLog.cxx
Common/QsLogDest.cxx
Descriptors/NiftyLinkXMLBuilder.cxx
MessageHandling/NiftyLinkMessageContainer.cxx
MessageHandling/NiftyLinkMessageManager.cxx
MessageHandling/NiftyLinkImageMessageHelpers.cxx
MessageHandling/NiftyLinkTrackingDataMessageHelpers.cxx
MessageHandling/NiftyLinkTransformMessageHelpers.cxx
MessageHandling/NiftyLinkStringMessageHelpers.cxx
NetworkOpenIGTLink/NiftyLinkSocket.cxx
NetworkOpenIGTLink/NiftyLinkClientSocket.cxx
NetworkOpenIGTLink/NiftyLinkServerSocket.cxx
NetworkOpenIGTLink/NiftyLinkNetworkProcess.cxx
NetworkOpenIGTLink/NiftyLinkServerProcess.cxx
NetworkOpenIGTLink/NiftyLinkServer.cxx
NetworkOpenIGTLink/NiftyLinkClientProcess.cxx
NetworkOpenIGTLink/NiftyLinkClient.cxx
NetworkQt/NiftyLinkTcpNetworkWorker.cxx
NetworkQt/NiftyLinkTcpServer.cxx
NetworkQt/NiftyLinkTcpClient.cxx
)

#########################
# Does need moc.
#########################
SET(niftylink_MOC_HDRS
Common/NiftyLinkMessageCounter.h
Common/NiftyLinkQThread.h
MessageHandling/NiftyLinkMessageManager.h
NetworkOpenIGTLink/NiftyLinkNetworkProcess.h
NetworkOpenIGTLink/NiftyLinkServerProcess.h
NetworkOpenIGTLink/NiftyLinkServer.h
NetworkOpenIGTLink/NiftyLinkClientProcess.h
NetworkOpenIGTLink/NiftyLinkClient.h
NetworkQt/NiftyLinkTcpNetworkWorker.h
NetworkQt/NiftyLinkTcpServer.h
NetworkQt/NiftyLinkTcpClient.h
)

#########################
# Does not need moc.
#########################
SET(niftylink_HDRS
Common/NiftyLinkCommonWin32ExportHeader.h
Common/NiftyLinkUtils.h
Common/NiftyLinkMessageStatsContainer.h
Common/QsDebugOutput.h
Common/QsLog.h
Common/QsLogDest.h
Descriptors/NiftyLinkXMLBuilder.h
MessageHandling/NiftyLinkMessageContainer.h
MessageHandling/NiftyLinkImageMessageHelpers.h
MessageHandling/NiftyLinkTrackingDataMessageHelpers.h
MessageHandling/NiftyLinkTransformMessageHelpers.h
MessageHandling/NiftyLinkStringMessageHelpers.h
NetworkOpenIGTLink/NiftyLinkSocket.h
NetworkOpenIGTLink/NiftyLinkClientSocket.h
NetworkOpenIGTLink/NiftyLinkServerSocket.h
)

#####################################
# Stuff from 1st version of NiftyLink
#####################################
if(BUILD_LEGACY)
  list(APPEND niftylink_SRCS
      MessageWrappers/NiftyLinkMessage.cxx
      MessageWrappers/NiftyLinkTransformMessage.cxx
      MessageWrappers/NiftyLinkImageMessage.cxx
      MessageWrappers/NiftyLinkStatusMessage.cxx
      MessageWrappers/NiftyLinkStringMessage.cxx
      MessageWrappers/NiftyLinkTrackingDataMessage.cxx
      NetworkOriginal/NiftyLinkProcessBase.cxx
      NetworkOriginal/NiftyLinkSenderProcess.cxx
      NetworkOriginal/NiftyLinkListenerProcess.cxx
      NetworkOriginal/NiftyLinkSocketObject.cxx
      )
  list(APPEND niftylink_MOC_HDRS
      NetworkOriginal/NiftyLinkProcessBase.h
      NetworkOriginal/NiftyLinkSenderProcess.h
      NetworkOriginal/NiftyLinkListenerProcess.h
      NetworkOriginal/NiftyLinkSocketObject.h
      )
  list(APPEND niftylink_HDRS
      MessageWrappers/NiftyLinkMessage.h
      MessageWrappers/NiftyLinkTransformMessage.h
      MessageWrappers/NiftyLinkImageMessage.h
      MessageWrappers/NiftyLinkTrackingDataMessage.h
      MessageWrappers/NiftyLinkStringMessage.h
      MessageWrappers/NiftyLinkStatusMessage.h
      )
endif()

#########################
# Resource file.
#########################

SET(niftylink_RES ../../Resources/NiftyLink.qrc)

# --------------------------------------------------------------------------
# Qt Specific Commands

if(DESIRED_QT_VERSION MATCHES 5)
  qt5_add_resources(niftylink_RCC ${niftylink_RES})
  qt5_wrap_cpp(NIFTYLINK_QT_MOC_FILES ${niftylink_MOC_HDRS} )
else()
  qt4_add_resources(niftylink_RCC ${niftylink_RES})
  qt4_wrap_cpp(NIFTYLINK_QT_MOC_FILES ${niftylink_MOC_HDRS} ) 
endif()
			  
SOURCE_GROUP("Generated Files" FILES ${NIFTYLINK_QT_MOC_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

IF(WIN32) 
  SET(LINK_LIBS
    ws2_32
    wsock32
  )
ELSE() 
  SET(LINK_LIBS
    m
    pthread
  )
ENDIF()

# --------------------------------------------------------------------------
# Build the library
SET (NIFTYLINK_LIBS
  ${QT_LIBRARIES}
  igtlutil
  OpenIGTLink
  ${LINK_LIBS}
  )
if(DESIRED_QT_VERSION MATCHES 5)
  list(APPEND NIFTYLINK_LIBS ${QT5_LINK_LIBRARIES})
else()
  list(APPEND NIFTYLINK_LIBS ${QT_LIBRARIES})
endif()

ADD_LIBRARY(niftylink 
  ${niftylink_SRCS}
  ${niftylink_HDRS}
  ${niftylink_MOC_HDRS}
  ${niftylink_RCC}
  ${NIFTYLINK_QT_MOC_FILES}
  )

IF(BUILD_SHARED_LIBS)
  IF(WIN32)
    ADD_DEFINITIONS(-DNIFTYLINKCOMMON_WINDOWS_EXPORT)
  ENDIF(WIN32)
ENDIF(BUILD_SHARED_LIBS)

TARGET_LINK_LIBRARIES(niftylink ${NIFTYLINK_LIBS} )
