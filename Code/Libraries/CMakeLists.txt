#/*================================================================================
#
#  NiftyLink:  A software library to facilitate communication over OpenIGTLink. 
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
Common/QsDebugOutput.cxx
Common/QsLog.cxx
Common/QsLogDest.cxx
Descriptors/NiftyLinkXMLBuilder.cxx
Messages/NiftyLinkMessage.cxx
Messages/NiftyLinkTransformMessage.cxx
Messages/NiftyLinkImageMessage.cxx
Messages/NiftyLinkStatusMessage.cxx
Messages/NiftyLinkStringMessage.cxx
Messages/NiftyLinkTrackingDataMessage.cxx
Network/NiftyLinkProcessBase.cxx
Network/NiftyLinkSenderProcess.cxx
Network/NiftyLinkListenerProcess.cxx
Network/NiftyLinkSocketObject.cxx
)

#########################
# Does need moc.
#########################
SET(niftylink_MOC_HDRS
Common/NiftyLinkQThread.h
Descriptors/NiftyLinkXMLBuilder.h
Network/NiftyLinkProcessBase.h
Network/NiftyLinkSenderProcess.h
Network/NiftyLinkListenerProcess.h
Network/NiftyLinkSocketObject.h
)

#########################
# Does not need moc.
#########################
SET(niftylink_HDRS
Common/NiftyLinkUtils.h
Common/NiftyLinkCommonWin32ExportHeader.h
Common/QsDebugOutput.h
Common/QsLog.h
Common/QsLogDest.h
Messages/NiftyLinkMessage.h
Messages/NiftyLinkTransformMessage.h
Messages/NiftyLinkImageMessage.h
Messages/NiftyLinkTrackingDataMessage.h
Messages/NiftyLinkStringMessage.h
Messages/NiftyLinkStatusMessage.h
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

# --------------------------------------------------------------------------
# Build the library
SET (NIFTYLINK_LIBS
  ${QT_LIBRARIES}
  igtlutil
  OpenIGTLink
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
