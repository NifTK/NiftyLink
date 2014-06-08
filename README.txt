Welcome
-----------------------------

Welcome to NiftyLink version @NIFTYLINK_VERSION_STRING@, a small messaging library
to facilitate communication over OpenIGTLink.
 
For all questions, please join:

@NIFTYLINK_USER_CONTACT@


Funding
---------------

This project was funded by the Comprehensive Biomedical 
Research Centre at University College London (UCL) and 
University College London Hospitals (UCLH), the
Technology Strategy Board (TSB) and the Engineering And
Physical Science Research Council (EPSRC).


Additional Libraries
-----------------------------

This platform uses these libraries/projects listed below. 

In addition, note that we use a modified OpenIGTLink.

Library  : OpenIGTLink
Website  : http://www.na-mic.org/Wiki/index.php/OpenIGTLink
Purpose  : Network protocol, somewhat tailored to medical imaging.
License  : BSD
Download : @NIFTYLINK_OPENIGTLINK_URL@

Library  : Qt
Website  : http://qt-project.org/
Purpose  : Cross platform general purpose library. 
License  : LGPL 2.1
Download : qt-project website, or here: 
           http://cmic.cs.ucl.ac.uk/platform/dependencies/qt-everywhere-opensource-src-4.7.4.tar.gz

Library  : QsLog
Website  : https://bitbucket.org/razvanpetru/qt-components/wiki/QsLog
Purpose  : Logging library.
License  : BSD
Download : Checked into this code base under NiftyLink/Code/Libraries/Common

In addition, you can view the nightly generated @NIFTYLINK_DOCUMENTATION_TEXT@ here:

@NIFTYLINK_DOCUMENTATION_URL@

which contains copies of the licenses. These licences are distributed within the source
tree at NiftyLink/Doc/ and also get copied to the build directory at compile time.
There is currently no "make install" task.
