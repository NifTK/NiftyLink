#/*================================================================================
#
#  NiftyLink:  A software library to facilitate communication over OpenIGTLink. 
#  
#              http://cmic.cs.ucl.ac.uk/
#              http://www.ucl.ac.uk/
#
#  Copyright (c) UCL : See LICENSE.txt in the top level directory for details. 
#
#  Last Changed      : $LastChangedDate: 2011-12-01 15:19:31 +0000 (Thu, 01 Dec 2011) $ 
#  Revision          : $Revision: 7901 $
#  Last modified by  : $Author: gerge $
#
#  Original author   : m.clarkson@ucl.ac.uk
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.  See the above copyright notices for more information.
#
#=================================================================================*/

ADD_SUBDIRECTORY(TimeSync)

# --------------------------------------------------------------------------
# Project Setup

PROJECT(NiftyLink)

# Find QT4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# --------------------------------------------------------------------------
# Sources

SET(niftylink_SRCS
Common/NiftyLinkUtils.cpp
Common/QsDebugOutput.cpp
Common/QsLog.cpp
Common/QsLogDest.cpp
OIGTLMessage.cpp
OIGTLThreadBase.cpp
OIGTLSenderThread.cpp
OIGTLListenerThread.cpp
OIGTLSocketObject.cpp
OIGTLTransformMessage.cpp
OIGTLImageMessage.cpp
)

#########################
# Does need moc.
#########################
SET(niftylink_MOC_HDRS
Common/NiftyLinkUtils.h
OIGTLThreadBase.h
OIGTLSenderThread.h
OIGTLListenerThread.h
OIGTLSocketObject.h
)

#########################
# Does not need moc.
#########################
SET(niftylink_HDRS
Common/NiftyLinkCommonWin32ExportHeader.h
Common/QsDebugOutput.h
Common/QsLog.h
Common/QsLogDest.h
OIGTLMessage.h
OIGTLTransformMessage.h
OIGTLImageMessage.h
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
