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

SET (UtilsTests_MOC_HDRS UtilsTests.h )
QT4_WRAP_CPP( UtilsTests_MOC_SRCS ${UtilsTests_MOC_HDRS} )

SET( UtilsTests_SRCS UtilsTests.cpp )
SET( TEST_LIBRARIES niftylinkcommon ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE( niftkCommonUtilsTests ${UtilsTests_MOC_SRCS} ${UtilsTests_SRCS})
TARGET_LINK_LIBRARIES( niftkCommonUtilsTests ${TEST_LIBRARIES} )
ADD_TEST(UtilsTests-1 ${EXECUTABLE_OUTPUT_PATH}/niftkCommonUtilsTests )
