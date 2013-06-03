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
Common/NiftyLinkXMLBuilder.cxx
Common/QsDebugOutput.cxx
Common/QsLog.cxx
Common/QsLogDest.cxx
NiftyLinkMessage.cxx
NiftyLinkProcessBase.cxx
NiftyLinkSenderProcess.cxx
NiftyLinkListenerProcess.cxx
NiftyLinkSocketObject.cxx
NiftyLinkTransformMessage.cxx
NiftyLinkImageMessage.cxx
NiftyLinkStatusMessage.cxx
NiftyLinkStringMessage.cxx
NiftyLinkTrackingDataMessage.cxx
)

#########################
# Does need moc.
#########################
SET(niftylink_MOC_HDRS
Common/NiftyLinkXMLBuilder.h
NiftyLinkProcessBase.h
NiftyLinkSenderProcess.h
NiftyLinkListenerProcess.h
NiftyLinkSocketObject.h
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
NiftyLinkMessage.h
NiftyLinkTransformMessage.h
NiftyLinkImageMessage.h
NiftyLinkTrackingDataMessage.h
NiftyLinkStringMessage.h
NiftyLinkStatusMessage.h
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
