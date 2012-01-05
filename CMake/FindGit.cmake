#/*=========================================================================
# 
# Copyright (c) 2003-2010 German Cancer Research Center, Division of Medical
# and Biological Informatics
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
#  * The name of the German Cancer Research Center, nor the names of any
#    contributors, may be used to endorse or promote products derived from this
#    software without specific prior written permission.
# 
#  * Modified source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
#=========================================================================*/

#
# FindGit
#

SET(git_candidates git eg)

IF(WIN32)
  LIST(APPEND git_candidates eg.cmd)
  IF(NOT MSYS)
    # We don't append git.cmd, since it does not correctly return
    # exit codes, see http://code.google.com/p/msysgit/issues/detail?id=428
    LIST(APPEND git_candidates git.cmd)
  ENDIF()
ENDIF()

FIND_PROGRAM(GIT_EXECUTABLE ${git_candidates}
  PATHS
    "C:/Program Files/Git/bin"
    "C:/Program Files (x86)/Git/bin"
  DOC "git command line client")
MARK_AS_ADVANCED(GIT_EXECUTABLE)
 
IF(GIT_EXECUTABLE)

  MACRO(GIT_IS_REPO dir result_var)
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} status
       WORKING_DIRECTORY ${dir}
       RESULT_VARIABLE GIT_error
       OUTPUT_QUIET
       ERROR_QUIET)
    if(GIT_error EQUAL 0)
      SET(${result_var} 1)
    ELSE()
      SET(${result_var} 0)
    ENDIF()
  ENDMACRO()
  
  MACRO(GIT_WC_INFO dir prefix)
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} rev-list -n 1 HEAD
       WORKING_DIRECTORY ${dir}
       ERROR_VARIABLE GIT_error
       OUTPUT_VARIABLE ${prefix}_WC_REVISION_HASH
       OUTPUT_STRIP_TRAILING_WHITESPACE)
    IF(NOT ${GIT_error} EQUAL 0)
      MESSAGE(SEND_ERROR "Command \"${GIT_EXECUTBALE} rev-list -n 1 HEAD\" in directory ${dir} failed with output:\n${GIT_error}")
    ELSE()
      EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} name-rev ${${prefix}_WC_REVISION_HASH}
         WORKING_DIRECTORY ${dir}
         OUTPUT_VARIABLE ${prefix}_WC_REVISION_NAME
          OUTPUT_STRIP_TRAILING_WHITESPACE)
    ENDIF()

    SET(${prefix}_WC_GITSVN 0)
    # In case, git-svn is used, attempt to extract svn info
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} svn info
      WORKING_DIRECTORY ${dir}
      ERROR_VARIABLE git_svn_info_error
      OUTPUT_VARIABLE ${prefix}_WC_INFO
      RESULT_VARIABLE git_svn_info_result
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    IF(${git_svn_info_result} EQUAL 0)
      SET(${prefix}_WC_GITSVN 1)
      STRING(REGEX REPLACE "^(.*\n)?URL: ([^\n]+).*"
        "\\2" ${prefix}_WC_URL "${${prefix}_WC_INFO}")
      STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
        "\\2" ${prefix}_WC_REVISION "${${prefix}_WC_INFO}")
      STRING(REGEX REPLACE "^(.*\n)?Last Changed Author: ([^\n]+).*"
        "\\2" ${prefix}_WC_LAST_CHANGED_AUTHOR "${${prefix}_WC_INFO}")
      STRING(REGEX REPLACE "^(.*\n)?Last Changed Rev: ([^\n]+).*"
        "\\2" ${prefix}_WC_LAST_CHANGED_REV "${${prefix}_WC_INFO}")
      STRING(REGEX REPLACE "^(.*\n)?Last Changed Date: ([^\n]+).*"
        "\\2" ${prefix}_WC_LAST_CHANGED_DATE "${${prefix}_WC_INFO}")
    ENDIF()
    
  ENDMACRO()
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Git DEFAULT_MSG GIT_EXECUTABLE)
