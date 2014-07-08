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

######################################################################
# Our version number. Edit this to generate a new version.
# However, be warned, you need to set CMAKE_INSTALL_PREFIX manually.
######################################################################
SET(NIFTYLINK_VERSION_MAJOR 14 CACHE STRING "Major version number" )
SET(NIFTYLINK_VERSION_MINOR 07 CACHE STRING "Minor version number" )
SET(NIFTYLINK_VERSION_PATCH 0 CACHE STRING "Patch version number" )
MARK_AS_ADVANCED(NIFTYLINK_VERSION_MAJOR)
MARK_AS_ADVANCED(NIFTYLINK_VERSION_MINOR)
MARK_AS_ADVANCED(NIFTYLINK_VERSION_PATCH)

######################################################################
# Set the minimum CMake version.
######################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

# If CMake is the version we're expecting, don't show the
# CMAKE_BACKWARDS_COMPATIBILITY option. If the version is higher than the
# minimal version required, then show the backward compatibility option.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" MATCHES "^2\\.8\\.4$")
  MARK_AS_ADVANCED(FORCE CMAKE_BACKWARDS_COMPATIBILITY)
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" MATCHES "^2\\.8\\.4$")
  MARK_AS_ADVANCED(CLEAR CMAKE_BACKWARDS_COMPATIBILITY)
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" MATCHES "^2\\.8\\.4$")

######################################################################
# Setting supported build types. Should ONLY be Debug or Release.
######################################################################

IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Valid options are Debug/Release")
ELSE()
   SET(CMAKE_BUILD_TYPE Release CACHE STRING "Setting build type to Release (default). Valid options: Debug/Release.")
ENDIF()

IF (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release")) 
  MESSAGE(FATAL_ERROR "Build type ${CMAKE_BUILD_TYPE} is not supported.")
ENDIF()

##################################################################################
# Set some CMake Policies.
# See http://cmake.org/cmake/help/cmake-2-8-docs.html#section_Policies for details
##################################################################################

SET(project_policies
  CMP0001 # NEW: CMAKE_BACKWARDS_COMPATIBILITY should no longer be used.
  CMP0002 # NEW: Logical target names must be globally unique.
  CMP0003 # NEW: Libraries linked via full path no longer produce linker search paths.
  CMP0004 # NEW: Libraries linked may NOT have leading or trailing whitespace.
  CMP0005 # NEW: Preprocessor definition values are now escaped automatically.
  CMP0006 # NEW: Installing MACOSX_BUNDLE targets requires a BUNDLE DESTINATION.
  CMP0007 # NEW: List command no longer ignores empty elements.
  CMP0008 # NEW: Libraries linked by full-path must have a valid library file name.
  CMP0009 # NEW: FILE GLOB_RECURSE calls should not follow symlinks by default.
  CMP0010 # NEW: Bad variable reference syntax is an error.
  CMP0011 # NEW: Included scripts do automatic cmake_policy PUSH and POP.
  CMP0012 # NEW: if() recognizes numbers and boolean constants.
  CMP0013 # NEW: Duplicate binary directories are not allowed.
  CMP0014 # NEW: Input directories must have CMakeLists.txt
  )
FOREACH(policy ${project_policies})
  IF(POLICY ${policy})
    CMAKE_POLICY(SET ${policy} NEW)
  ENDIF()
ENDFOREACH()

######################################################################
# We have a super-build option. 
######################################################################

OPTION(BUILD_SUPERBUILD "Build NiftyLink and the projects it depends on via SuperBuild.cmake." ON)

If(BUILD_SUPERBUILD)
  PROJECT(NIFTYLINK-SUPERBUILD)
ELSE(BUILD_SUPERBUILD)
  PROJECT(NIFTYLINK)
ENDIF(BUILD_SUPERBUILD)

######################################################################
# Setting build name based on local system details
######################################################################

MESSAGE(STATUS "Cmake generator: ${CMAKE_GENERATOR}")

IF(CMAKE_GENERATOR MATCHES Make AND NOT (CMAKE_GENERATOR MATCHES NMake) )
  find_program(UNAME NAMES uname)
  macro(getuname name flag)
    exec_program("${UNAME}" ARGS "${flag}" OUTPUT_VARIABLE "${name}")
  endmacro(getuname)

  getuname(osname -s)
  getuname(cpu    -m)

  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

  #MESSAGE(STATUS "gcc version: ${GCC_VERSION}")

  set(CTBN "${osname}-${cpu}-gcc-${GCC_VERSION}")
  string(REPLACE "\n" "" CTBN ${CTBN})
  MESSAGE(STATUS "Buildname: ${CTBN}")
  SET(BUILDNAME ${CTBN} CACHE STRING "${CTBN}")
  SET(CTEST_BUILD_NAME ${CTBN} CACHE STRING "${CTBN}")
ELSE()
  MESSAGE(STATUS "Buildname: ${CMAKE_GENERATOR}")
  SET(BUILDNAME ${CMAKE_GENERATOR} CACHE STRING "${CMAKE_GENERATOR}")
  SET(CTEST_BUILD_NAME ${CMAKE_GENERATOR} CACHE STRING "${CMAKE_GENERATOR}")
ENDIF()

######################################################################
# Options. These are set up front, so are available when configuring
# the SuperBuild, and hence they must also be passed to the normal
# build. So, look in CMake/Superbuild.cmake to see where they are
# passed to the main build of NiftyLink when doing the Superbuild.
######################################################################
OPTION(BUILD_SHARED_LIBS "Build NiftyLink with shared libraries." ON)
IF (NOT BUILD_SHARED_LIBS) 
  MESSAGE(FATAL_ERROR "Static Build is not supported.")
ENDIF()

OPTION(BUILD_TESTING "Build Unit tests in NiftyLink/Testing/Code" ON)
OPTION(NIFTYLINK_OPENIGTLINK_DEV "Build the development version of OpenIGTLink" ON)
OPTION(NIFTYLINK_CHECK_COVERAGE "Enable/Disable code coverage checking." OFF)

######################################################################
# Variables that get compiled into C++ code. 
######################################################################
SET(NIFTYLINK_NAME "NiftyLink" CACHE STRING "Full name of platform." FORCE )
SET(NIFTYLINK_COPYRIGHT "Copyright (C) 2008-2014 University College London" CACHE STRING "Copyright string." FORCE )
SET(NIFTYLINK_ORIGIN_URL "http://cmic.cs.ucl.ac.uk/" CACHE STRING "URL of originating institution." FORCE )
SET(NIFTYLINK_ORIGIN_SHORT_TEXT "CMIC" CACHE STRING "Short name of originating institution." FORCE)
SET(NIFTYLINK_ORIGIN_LONG_TEXT "Centre For Medical Image Computing" CACHE STRING "Full name of originating institution." FORCE)
SET(NIFTYLINK_WIKI_URL "https://cmicdev.cs.ucl.ac.uk/trac/wiki/MidasPlatform" CACHE STRING "Wiki URL" FORCE )
SET(NIFTYLINK_WIKI_TEXT "wiki" CACHE STRING "Wiki text" FORCE )
SET(NIFTYLINK_DASHBOARD_URL "https://cmicdev.cs.ucl.ac.uk/cdash/index.php?project=UCLToolkit" CACHE STRING "Dashboard URL" FORCE)
SET(NIFTYLINK_DASHBOARD_TEXT "dashboard" CACHE STRING "Dashboard text" FORCE)
SET(NIFTYLINK_DOCUMENTATION_URL "http://cmic.cs.ucl.ac.uk/platform/niftk/current/NiftyLink/index.html" CACHE STRING "Documentation URL" FORCE)
SET(NIFTYLINK_DOCUMENTATION_TEXT "doxygen" CACHE STRING "Documentation text" FORCE)
SET(NIFTYLINK_USER_CONTACT "https://www.mailinglists.ucl.ac.uk/mailman/listinfo/niftk-users" CACHE STRING "Contact address for users." FORCE )
SET(NIFTYLINK_VERSION_STRING "${NIFTYLINK_VERSION_MAJOR}.${NIFTYLINK_VERSION_MINOR}.${NIFTYLINK_VERSION_PATCH}" CACHE STRING "String to describe fully named version" FORCE)
SET(NIFTYLINK_DEPLOY_NAME "${NIFTYLINK_NAME}-${NIFTYLINK_VERSION_STRING}" CACHE STRING "String to describe deployed name" FORCE)
SET(NIFTYLINK_OPENIGTLINK_VERSION "a0f1cc6a32"  CACHE STRING "OpenIGTLink version" FORCE )
SET(NIFTYLINK_OPENIGTLINK_MD5 "f489dedfb35a2dc25cef720b461aa751" CACHE STRING "OpenIGTLink MD5" FORCE )
SET(NIFTYLINK_OPENIGTLINK_LOCATION "http://cmic.cs.ucl.ac.uk/platform/dependencies/NifTK-OpenIGTLink-${NIFTYLINK_OPENIGTLINK_VERSION}.tar.gz" CACHE STRING "Location of OpenIGTLink .tar.gz" FORCE )
SET(NIFTYLINK_OPENIGTLINK_LOCATION_DEV "git://github.com/NifTK/OpenIGTLink.git" CACHE STRING "Development version of OpenIGTLink." )
SET(NIFTYLINK_OPENIGTLINK_LOCATION_URL "https://github.com/NifTK/OpenIGTLink" CACHE STRING "OpenIGTLink on Github" FORCE )
SET(NIFTYLINK_QT_URL "http://cmic.cs.ucl.ac.uk/platform/dependencies/qt-everywhere-opensource-src-4.7.4.tar.gz" CACHE STRING "Example Qt download" FORCE )

######################################################################
# Hide these variables from the user, unless they are 'advanced' :-)
######################################################################
MARK_AS_ADVANCED(NIFTYLINK_NAME)
MARK_AS_ADVANCED(NIFTYLINK_COPYRIGHT)
MARK_AS_ADVANCED(NIFTYLINK_ORIGIN_URL)
MARK_AS_ADVANCED(NIFTYLINK_ORIGIN_SHORT_TEXT)
MARK_AS_ADVANCED(NIFTYLINK_ORIGIN_LONG_TEXT)
MARK_AS_ADVANCED(NIFTYLINK_WIKI_URL)
MARK_AS_ADVANCED(NIFTYLINK_WIKI_TEXT)
MARK_AS_ADVANCED(NIFTYLINK_DASHBOARD_URL)
MARK_AS_ADVANCED(NIFTYLINK_DASHBOARD_TEXT)
MARK_AS_ADVANCED(NIFTYLINK_DOCUMENTATION_URL)
MARK_AS_ADVANCED(NIFTYLINK_DOCUMENTATION_TEXT)
MARK_AS_ADVANCED(NIFTYLINK_USER_CONTACT)
MARK_AS_ADVANCED(NIFTYLINK_VERSION_STRING)
MARK_AS_ADVANCED(NIFTYLINK_DEPLOY_NAME)
MARK_AS_ADVANCED(NIFTYLINK_CHECK_COVERAGE)
MARK_AS_ADVANCED(NIFTYLINK_OPENIGTLINK_VERSION)
MARK_AS_ADVANCED(NIFTYLINK_OPENIGTLINK_MD5)
MARK_AS_ADVANCED(NIFTYLINK_OPENIGTLINK_LOCATION)
MARK_AS_ADVANCED(NIFTYLINK_OPENIGTLINK_LOCATION_DEV)
MARK_AS_ADVANCED(UNAME)

######################################################################
# Make sure Git is available.
######################################################################
find_package(Git REQUIRED)
if (WIN32)
  set(GITCOMMAND ${GIT_EXECUTABLE})
endif()

######################################################################
# Sort out which version of OpenIGTLink we are using
######################################################################
IF(NIFTYLINK_OIGTLINK_DEV)
  SET(OIGTL_OPTIONS
    GIT_REPOSITORY ${NIFTYLINK_OPENIGTLINK_LOCATION_DEV}
    GIT_TAG development
  )
ELSE()
  SET(OIGTL_OPTIONS
    URL ${NIFTYLINK_OPENIGTLINK_LOCATION}
    URL_MD5 ${NIFTYLINK_OPENIGTLINK_MD5}
  )
ENDIF()

######################################################################
# Setup the path to load CMake macros, and extra CMake files.
######################################################################
SET(CMAKE_MODULE_PATH 
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/CMake/CMakeExternals
    ${CMAKE_MODULE_PATH}
   )

######################################################################
# Add in any functions/macros. 
######################################################################
INCLUDE(mitkFunctionGetVersion)
INCLUDE(mitkMacroEmptyExternalProject)

######################################################################
# NiftyLink uses KWStyle for checking the coding style
######################################################################
INCLUDE(${CMAKE_SOURCE_DIR}/Utilities/KWStyle/NiftyLinkKWStyle.cmake)

###########################################################################
# Set these compiler flags early, so it can be applied to all dependencies.
###########################################################################

SET(NIFTYLINK_ADDITIONAL_C_FLAGS "" CACHE STRING "Additional C Flags")
MARK_AS_ADVANCED(NIFTYLINK_ADDITIONAL_C_FLAGS)
SET(NIFTYLINK_ADDITIONAL_CXX_FLAGS "" CACHE STRING "Additional CXX Flags")
MARK_AS_ADVANCED(NIFTYLINK_ADDITIONAL_CXX_FLAGS)

######################################################################
# Find Qt. 
######################################################################

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtXml QtGui QtNetwork REQUIRED)

SET(QT_USE_QTXML 1)
SET(QT_USE_QTXMLPATTERNS 1)
SET(QT_USE_PHONON 0)
SET(QT_USE_QTTEST 1)    
INCLUDE(${QT_USE_FILE})
  
########################################################################
# Now, if required, do the SuperBuild
# If we are doing SuperBuild 
#   We configure up to this point (see the return() statement)
#   and then we call SuperBuild.cmake, which builds all the 
#   dependencies as CMake ExternalProjects, and then also builds
#   NiftyLink as an ExternalProject. However instead of downloading
#   a tar file, you set the SOURCE_DIR to be THIS project, and force
#   the BUILD_SUPERBUILD flag to be off (to avoid infinite loop).
#
# If we are NOT doing superbuild, then the next statement has no effect.
########################################################################

IF(BUILD_SUPERBUILD)
  INCLUDE("CMake/SuperBuild.cmake")
  RETURN()
ENDIF(BUILD_SUPERBUILD)

######################################################################
# End of SuperBuild.
######################################################################

######################################################################
# Find dependent packages. Qt is already found above.
######################################################################

FIND_PACKAGE(OpenIGTLink REQUIRED)
IF(OpenIGTLink_FOUND)
  MESSAGE("Found OpenIGTLink")    
  INCLUDE(${OpenIGTLink_USE_FILE}) 
  IF(NIFTYLINK_NiftyLinkINK_DEV)
    mitkFunctionGetVersion(${OpenIGTLink_SOURCE_DIR} OPENIGTLINK)
    MESSAGE("OpenIGTLink git version=${OPENIGTLINK_REVISION_ID}")   
  ELSE()
    SET(OPENIGTLINK_REVISION_ID ${NIFTYLINK_OPENIGTLINK_VERSION})
    MESSAGE("OpenIGTLink version=${OPENIGTLINK_REVISION_ID}")
  ENDIF()
ENDIF(OpenIGTLink_FOUND)

######################################################################
# Retrieve version info.
######################################################################
mitkFunctionGetVersion(${CMAKE_SOURCE_DIR} NIFTYLINK_GIT)
MESSAGE("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
MESSAGE("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
MESSAGE("Qt version=${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
MESSAGE("NiftyLink git version=${NIFTYLINK_GIT_REVISION_ID}")

######################################################################
# Test for some required system information. 
######################################################################
INCLUDE(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)

######################################################################
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
######################################################################
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
ENABLE_TESTING()
IF(BUILD_TESTING)
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/Images/NiftyLinkLogo.gif
                 ${CMAKE_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif
                 COPYONLY IMMEDIATE)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
  
  # Setup file for setting custom ctest vars
  CONFIGURE_FILE(CMake/CTestCustom.cmake.in ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)
  
ENDIF(BUILD_TESTING)

######################################################################
# Stuff that needs generating. For example before we compile.
######################################################################

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Config/NiftyLinkConfig.h.in ${CMAKE_BINARY_DIR}/NiftyLinkConfig.h)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/MainPage.dox.in ${CMAKE_BINARY_DIR}/Doxygen/MainPage.dox)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/License.dox.in ${CMAKE_BINARY_DIR}/Doxygen/License.dox)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/Doxygen/niftylinkdoxygen.pl.in ${CMAKE_BINARY_DIR}/niftylinkdoxygen.pl)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/Doxygen/doxygen.config.in ${CMAKE_BINARY_DIR}/doxygen.config)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/License_OpenIGTLink.txt ${CMAKE_BINARY_DIR}/LICENSE_OpenIGTLink.txt @ONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/License_Qt.txt ${CMAKE_BINARY_DIR}/LICENSE_QT.txt @ONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doc/License_QsLog.txt ${CMAKE_BINARY_DIR}/LICENSE_QsLog.txt @ONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/LICENSE.txt ${CMAKE_BINARY_DIR}/LICENSE.txt @ONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/README.txt ${CMAKE_BINARY_DIR}/README.txt @ONLY)

######################################################################
# Compilation specific stuff:
######################################################################

######################################################################
# Compilation specific stuff: Include directories.
######################################################################
SET(NIFTYLINK_INCLUDE_DIRS_BUILD_TREE ${NIFTYLINK_INCLUDE_DIRS_BUILD_TREE}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/Code/Libraries
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Common
)
INCLUDE_DIRECTORIES(
  ${NIFTYLINK_INCLUDE_DIRS_BUILD_TREE}
  )
  
######################################################################
# Compilation specific stuff: Flags
######################################################################
IF(NIFTYLINK_CHECK_COVERAGE)
  IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(NIFTYLINK_COVERAGE_FLAGS "-g -fprofile-arcs -ftest-coverage  -O0 -DNDEBUG" )
    SET(NIFTYLINK_COVERAGE_C_FLAGS ${NIFTYLINK_COVERAGE_FLAGS})
    SET(NIFTYLINK_COVERAGE_CXX_FLAGS ${NIFTYLINK_COVERAGE_FLAGS})
  ENDIF()
ENDIF()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${NIFTYLINK_COVERAGE_C_FLAGS} ${NIFTYLINK_ADDITIONAL_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NIFTYLINK_COVERAGE_CXX_FLAGS} ${NIFTYLINK_ADDITIONAL_CXX_FLAGS}")

######################################################################
# Compilation specific stuff: Compilation output directories.
######################################################################
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})              # Used to control, where to put unit test binaries.
SET(UCLTK_DATA_ROOT "${CMAKE_SOURCE_DIR}/Testing/Data")   # Used in unit tests to refer to the input data directory.

FOREACH(type LIBRARY RUNTIME ARCHIVE)
  SET(output_dir ${CMAKE_BINARY_DIR}/bin)
  SET(CMAKE_${type}_OUTPUT_DIRECTORY ${output_dir} CACHE INTERNAL "Single output directory for building all libraries.")
  MARK_AS_ADVANCED(CMAKE_${type}_OUTPUT_DIRECTORY)
ENDFOREACH()

######################################################################
# Decide what subdirectories we are building, and go and build them.
# Note: We have no INSTALL or PACKAGE support. The library should
# be used from the Build directory.
######################################################################

SUBDIRS(Code)

IF(BUILD_TESTING) 
  SUBDIRS(Testing)
ENDIF(BUILD_TESTING)   

##################################################################################
# Generate "Use" and "Configure" files so external projects can link to NiftyLink.
# These go at or near the end, as all the variables need to be defined.
##################################################################################

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/UseNiftyLink.cmake.in ${CMAKE_BINARY_DIR}/UseNiftyLink.cmake @ONLY IMMEDIATE)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/NiftyLinkConfig.cmake.in ${CMAKE_BINARY_DIR}/NiftyLinkConfig.cmake @ONLY IMMEDIATE)
