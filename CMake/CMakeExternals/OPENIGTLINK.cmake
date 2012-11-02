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

#-----------------------------------------------------------------------------
# OPENIGTLINK
#-----------------------------------------------------------------------------

# Sanity checks
IF(DEFINED OpenIGTLink_DIR AND NOT EXISTS ${OpenIGTLink_DIR})
  MESSAGE(FATAL_ERROR "OpenIGTLink_DIR variable is defined but corresponds to non-existing directory \"${OpenIGTLink_DIR}\".")
ENDIF()

SET(proj OPENIGTLINK)
SET(proj_DEPENDENCIES )
SET(OPENIGTLINK_DEPENDS ${proj})
 
IF(NOT DEFINED OpenIGTLink_DIR)

    SET(revision_tag development)
    IF(${proj}_REVISION_TAG)
        SET(revision_tag ${${proj}_REVISION_TAG})
    ENDIF()

    ExternalProject_Add(${proj}
    GIT_REPOSITORY ${NIFTYLINK_OPENIGTLINK_LOCATION}
    GIT_TAG ${revision_tag}
    BINARY_DIR ${proj}-build
    INSTALL_COMMAND ""
    CMAKE_GENERATOR ${GEN}
    CMAKE_ARGS
        ${EP_COMMON_ARGS}
        -DCMAKE_INSTALL_PREFIX:PATH=${EP_BASE}/Install/${proj}
		    -DOpenIGTLink_PROTOCOL_VERSION_2:BOOL=ON
    DEPENDS ${proj_DEPENDENCIES}
    )

  SET(OpenIGTLink_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
  SET(OpenIGTLink_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/CMakeExternals/Source/OPENIGTLINK)
  
  MESSAGE("SuperBuild loading OpenIGTLink from ${OpenIGTLink_DIR}")

ELSE(NOT DEFINED OpenIGTLink_DIR)

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

ENDIF(NOT DEFINED OpenIGTLink_DIR)
