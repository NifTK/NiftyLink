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

    #1273 - OpenIGTLink version update due to bugfixes.
    SET(revision_tag 9beb7581f6)
    IF(${proj}_REVISION_TAG)
        SET(revision_tag ${${proj}_REVISION_TAG})
    ENDIF()

    ExternalProject_Add(${proj}
    GIT_REPOSITORY git://github.com/zomboir/OpenIGTLink.git
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
