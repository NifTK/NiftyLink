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

SET(TestNL_Utils_SRCS
Common/NiftyLinkUtilsTests.cpp
)

SET(TestNL_Utils_HDRS
Common/NiftyLinkUtilsTests.h
)

QT4_WRAP_CPP(UTILSTEST_QT_MOC_FILES ${TestNL_Utils_HDRS})
SOURCE_GROUP("Generated Files" FILES ${UTILSTEST_QT_MOC_FILES})


SET(TestNL_Basic_SRCS
NiftyLinkTests.cpp
)

SET(TestNL_Basic_HDRS
#Common/NiftyLinkUtilsTests.h
)

QT4_WRAP_CPP(BASICTEST_QT_MOC_FILES ${TestNL_Basic_HDRS})
SOURCE_GROUP("Generated Files" FILES ${BASICTEST_QT_MOC_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(TEST_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )


ADD_EXECUTABLE(TestNL_Utils ${TestNL_Utils_SRCS} ${TestNL_Utils_HDRS} ${UTILSTEST_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_Utils ${TEST_LIBRARIES} )
ADD_TEST(TestNL_Utils ${EXECUTABLE_OUTPUT_PATH}/TestNL_Utils )

ADD_EXECUTABLE(TestNL_Basic ${TestNL_Basic_SRCS} ${TestNL_Basic_HDRS} ${BASICTEST_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_Basic ${TEST_LIBRARIES} )
ADD_TEST(TestNL_Basic ${EXECUTABLE_OUTPUT_PATH}/TestNL_Basic )


