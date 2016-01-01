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

SET(LIBS niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

SET(SRCS
  NiftyLinkUtilsTests
  NiftyLinkMessageCounterTests
  NiftyLinkImageMessageHelpersTests
  NiftyLinkTrackingDataMessageHelpersTests
  NiftyLinkTransformMessageHelpersTests
  NiftyLinkClientServerTests
  NiftyLinkDescriptorTests
  NiftyLinkMessageContainerTests
)

FOREACH(APP ${SRCS})
  SET(QT_MOC_FILES)
  if(DESIRED_QT_VERSION MATCHES 5)
    qt5_wrap_cpp(QT_MOC_FILES ${APP}.h)
  else(DESIRED_QT_VERSION MATCHES 5)
    qt4_wrap_cpp(QT_MOC_FILES ${APP}.h)
  endif()
  ADD_EXECUTABLE(${APP} ${APP}.cxx ${QT_MOC_FILES})
  TARGET_LINK_LIBRARIES(${APP} ${LIBS} )
  ADD_TEST(${APP} ${EXECUTABLE_OUTPUT_PATH}/${APP} )
ENDFOREACH()

