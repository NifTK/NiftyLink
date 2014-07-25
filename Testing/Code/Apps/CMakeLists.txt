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

SET(LIBS
  niftylink
  igtlutil
  ${OpenIGTLink_LIBRARIES} 
  ${QT_LIBRARIES} 
  ${QT_QTTEST_LIBRARY} 
)

SET(APPS
#  TestImagingSender
#  TestImagingReceiver
  TestQtServer
  TestOIGTLinkTrackingClient
  TestNifTKOIGTLinkTrackingClient
  TestNifTKQtTrackingClient
  TestNifTKQtImagingClient
)

FOREACH(APP ${APPS})
  SET(QT_MOC_FILES)
  QT4_WRAP_CPP(QT_MOC_FILES ${APP}.h)
  ADD_EXECUTABLE(${APP} ${APP}.cxx ${QT_MOC_FILES})
  TARGET_LINK_LIBRARIES(${APP} ${LIBS} )
ENDFOREACH()
