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

# Find QT4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# --------------------------------------------------------------------------
# Sources

SET(niftylink_SRCS
Common/NiftyLinkUtils.cxx
Common/NiftyLinkMessageCounter.cxx
Common/QsDebugOutput.cxx
Common/QsLog.cxx
Common/QsLogDest.cxx
Descriptors/NiftyLinkXMLBuilder.cxx
Messages/NiftyLinkMessageContainer.cxx
Messages/NiftyLinkMessageManager.cxx
Messages/NiftyLinkImageMessageHelpers.cxx
Messages/NiftyLinkTrackingDataMessageHelpers.cxx
Messages/NiftyLinkTransformMessageHelpers.cxx
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
Descriptors/NiftyLinkXMLBuilder.h
Messages/NiftyLinkMessageManager.h
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
Common/QsDebugOutput.h
Common/QsLog.h
Common/QsLogDest.h
Messages/NiftyLinkMessageContainer.h
Messages/NiftyLinkImageMessageHelpers.h
Messages/NiftyLinkTrackingDataMessageHelpers.h
Messages/NiftyLinkTransformMessageHelpers.h
NetworkOpenIGTLink/NiftyLinkSocket.h
NetworkOpenIGTLink/NiftyLinkClientSocket.h
NetworkOpenIGTLink/NiftyLinkServerSocket.h
)

#########################
# Resource file.
#########################

SET(niftylink_RES ../../Resources/NiftyLink.qrc)

# --------------------------------------------------------------------------
# Qt Specific Commands

QT4_ADD_RESOURCES(niftylink_RCC ${niftylink_RES})

QT4_WRAP_CPP(NIFTYLINK_QT_MOC_FILES ${niftylink_MOC_HDRS} )			  
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
