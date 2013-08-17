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

INCLUDE(ExternalProject)

SET(EP_BASE "${CMAKE_BINARY_DIR}/CMakeExternals")
SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${EP_BASE})

# For external projects like OpenIGTLink we always turn off their testing targets.
SET(EP_BUILD_TESTING OFF)
SET(EP_BUILD_EXAMPLES OFF)
SET(EP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

IF(MSVC90 OR MSVC10)
  SET(EP_COMMON_C_FLAGS "${CMAKE_C_FLAGS} /bigobj /MP")
  SET(EP_COMMON_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
ELSE()
  IF(${BUILD_SHARED_LIBS})
    SET(EP_COMMON_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX_EXTRA")
    SET(EP_COMMON_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX_EXTRA")
  ELSE()
    SET(EP_COMMON_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -DLINUX_EXTRA")
    SET(EP_COMMON_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DLINUX_EXTRA")    
  ENDIF()
ENDIF()

SET(EP_COMMON_ARGS
  -DBUILD_TESTING:BOOL=${EP_BUILD_TESTING}
  -DBUILD_SHARED_LIBS:BOOL=${EP_BUILD_SHARED_LIBS}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
  -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
  -DCMAKE_C_FLAGS:STRING=${EP_COMMON_C_FLAGS}
  -DCMAKE_CXX_FLAGS:STRING=${EP_COMMON_CXX_FLAGS}
  -DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG}
  -DCMAKE_C_FLAGS_DEBUG:STRING=${CMAKE_C_FLAGS_DEBUG}
  -DCMAKE_CXX_FLAGS_RELEASE:STRING=${CMAKE_CXX_FLAGS_RELEASE}
  -DCMAKE_C_FLAGS_RELEASE:STRING=${CMAKE_C_FLAGS_RELEASE}
)

# Compute -G arg for configuring external projects with the same CMake generator:
IF(CMAKE_EXTRA_GENERATOR)
  SET(GEN "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
ELSE()
  SET(GEN "${CMAKE_GENERATOR}")
ENDIF()

######################################################################
# Loop round for each external project, compiling it
######################################################################

SET(EXTERNAL_PROJECTS
  OPENIGTLINK
)

FOREACH(p ${EXTERNAL_PROJECTS})
  INCLUDE("CMake/CMakeExternals/${p}.cmake")
ENDFOREACH()

######################################################################
# Now compile NiftyLink, using the packages we just provided.
######################################################################
IF(NOT DEFINED SUPERBUILD_EXCLUDE_NIFTYLINKBUILD_TARGET OR NOT SUPERBUILD_EXCLUDE_NIFTYLINKBUILD_TARGET)

  SET(proj NIFTYLINK)
  SET(proj_DEPENDENCIES ${OPENIGTLINK_DEPENDS} )

  ExternalProject_Add(${proj}
    DOWNLOAD_COMMAND ""
    INSTALL_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} 
    BINARY_DIR ${CMAKE_BINARY_DIR}/NiftyLink-build
    CMAKE_GENERATOR ${GEN}
    CMAKE_ARGS
      ${EP_COMMON_ARGS}
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
      -DBUILD_TESTING:BOOL=${BUILD_TESTING} # The value set in EP_COMMON_ARGS normally forces this off, but we may need NiftyGuide to be on.
      -DBUILD_SUPERBUILD:BOOL=OFF           # Must force this to be off, or else you will loop forever.
      -DNIFTYLINK_VERSION_MAJOR:STRING=${NIFTYLINK_VERSION_MAJOR}
      -DNIFTYLINK_VERSION_MINOR:STRING=${NIFTYLINK_VERSION_MINOR}
      -DNIFTYLINK_VERSION_PATCH:STRING=${NIFTYLINK_VERSION_PATCH}
      -DNIFTYLINK_NAME:STRING=${NIFTYLINK_NAME}
      -DNIFTYLINK_COPYRIGHT:STRING=${NIFTYLINK_COPYRIGHT}
      -DNIFTYLINK_ORIGIN_URL:STRING=${NIFTYLINK_ORIGIN_URL}
      -DNIFTYLINK_ORIGIN_SHORT_TEXT:STRING=${NIFTYLINK_ORIGIN_SHORT_TEXT}
      -DNIFTYLINK_ORIGIN_LONG_TEXT:STRING=${NIFTYLINK_ORIGIN_LONG_TEXT}
      -DNIFTYLINK_WIKI_URL:STRING=${NIFTYLINK_WIKI_URL}
      -DNIFTYLINK_WIKI_TEXT:STRING=${NIFTYLINK_WIKI_TEXT}
      -DNIFTYLINK_DASHBOARD_URL:STRING=${NIFTYLINK_DASHBOARD_URL}
      -DNIFTYLINK_DASHBOARD_TEXT:STRING=${NIFTYLINK_DASHBOARD_TEXT}
      -DNIFTYLINK_USER_CONTACT:STRING=${NIFTYLINK_USER_CONTACT}
      -DNIFTYLINK_VERSION_STRING:STRING=${NIFTYLINK_VERSION_STRING}
      -DNIFTYLINK_DEPLOY_NAME:STRING=${NIFTYLINK_DEPLOY_NAME}
      -DNIFTYLINK_CHECK_COVERAGE:BOOL=${NIFTYLINK_CHECK_COVERAGE}
      -DNIFTYLINK_ADDITIONAL_C_FLAGS:STRING=${NIFTYLINK_ADDITIONAL_C_FLAGS}
      -DNIFTYLINK_ADDITIONAL_CXX_FLAGS:STRING=${NIFTYLINK_ADDITIONAL_CXX_FLAGS} 
      -DNIFTYLINK_OIGTLINK_DEV:BOOL=${NIFTYLINK_OIGTLINK_DEV}     
	    -DBUILDNAME:STRING=${BUILDNAME}
	    -DCTEST_BUILD_NAME:STRING=${CTEST_BUILD_NAME}
      -DOpenIGTLink_SOURCE_DIR:PATH=${OpenIGTLink_SOURCE_DIR}
      -DOpenIGTLink_DIR:PATH=${OpenIGTLink_DIR}
      -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      -DNIFTYLINK_USE_KWSTYLE:BOOL=${NIFTYLINK_USE_KWSTYLE} 
     DEPENDS ${proj_DEPENDENCIES}
  )

ENDIF()
