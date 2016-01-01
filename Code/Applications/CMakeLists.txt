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
  NiftyLinkApp
)

# For basic apps that are literally just one .ui, .h and .cxx file.
foreach(APP ${APPS})
  set(QT_UI_FILES)
  set(QT_MOC_FILES)
  if(DESIRED_QT_VERSION MATCHES 5)
    qt5_wrap_ui(QT_UI_FILES ${APP}.ui )
    qt5_wrap_cpp(QT_MOC_FILES ${APP}.h)
  else()
    qt4_wrap_ui(QT_UI_FILES ${APP}.ui )
    qt4_wrap_cpp(QT_MOC_FILES ${APP}.h)
  endif()
  add_executable(${APP} ${APP}.cxx ${QT_MOC_FILES} ${QT_UI_FILES})
  target_link_libraries(${APP} ${LIBS} )
endforeach()
