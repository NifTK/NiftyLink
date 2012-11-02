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

SET(CTEST_PROJECT_NAME "NiftyLink")
SET(CTEST_NIGHTLY_START_TIME "22:00:00 GMT")
SET(CTEST_TEST_TIMEOUT "3600")

IF(NOT DEFINED CTEST_DROP_METHOD)
  SET(CTEST_DROP_METHOD "http")
ENDIF(NOT DEFINED CTEST_DROP_METHOD)

IF(CTEST_DROP_METHOD STREQUAL "http")
  SET(CTEST_DROP_SITE "cmicdev.cs.ucl.ac.uk")
  SET(CTEST_DROP_LOCATION "/cdash/submit.php?project=NiftyLink")
  SET(CTEST_DROP_SITE_CDASH TRUE)
ENDIF(CTEST_DROP_METHOD STREQUAL "http")


