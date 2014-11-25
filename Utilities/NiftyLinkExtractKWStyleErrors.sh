#!/bin/sh

# This script to be run from the build directory.
ctest -R KWStyleCodeTest

CWD=`pwd`
cd ../../NiftyLink

# Method 1. Older versions of KWStyle used to print the file name with each error.
find . -name "*.h" -exec grep {} ${CWD}/Testing/Temporary/LastTest.log \;
find . -name "*.cxx" -exec grep {} ${CWD}/Testing/Temporary/LastTest.log \;

# Method 2. Newer versions just seem to print the error code.
find . -name "*.h" -print -exec grep "Error #" ${CWD}/Testing/Temporary/LastTest.log \;
find . -name "*.cxx" -print -exec grep "Error #" ${CWD}/Testing/Temporary/LastTest.log \;

cd ${CWD}



