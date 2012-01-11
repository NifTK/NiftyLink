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

ADD_SUBDIRECTORY(Common)

# --------------------------------------------------------------------------
# Project Setup

PROJECT(NiftyLinkTests)

# Find QT4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# --------------------------------------------------------------------------
# Sources

SET(UtilsTests_SRCS
Common/NiftyLinkUtilsTests.cpp
)

SET(UtilsTests_HDRS
Common/NiftyLinkUtilsTests.h
)

SET(NiftyLinkTests_SRCS
NiftyLinkTests.cpp
)

SET(NiftyLinkTests_HDRS
#Common/NiftyLinkUtilsTests.h
)

QT4_WRAP_CPP(TESTS_QT_MOC_FILES ${NiftyLinkTests_HDRS} ${UtilsTests_HDRS})
SOURCE_GROUP("Generated Files" FILES ${TESTS_QT_MOC_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


SET(TEST_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE( NiftyLinkTests ${UtilsTests_SRCS} ${UtilsTests_HDRS} ${NiftyLinkTests_SRCS} ${NiftyLinkTests_HDRS} ${TESTS_QT_MOC_FILES})
TARGET_LINK_LIBRARIES( NiftyLinkTests ${TEST_LIBRARIES} )
ADD_TEST(NiftyLinkTests-1 ${EXECUTABLE_OUTPUT_PATH}/NiftyLinkTests )




