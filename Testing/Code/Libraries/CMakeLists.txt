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

# --------------------------------------------------------------------------
# Sources

SET(TestNL_Utils_SRCS
Common/NiftyLinkUtilsTests.cpp
)

SET(TestNL_Utils_HDRS
Common/NiftyLinkUtilsTests.h
)

QT4_WRAP_CPP(UTILSTEST_QT_MOC_FILES ${TestNL_Utils_HDRS})
SOURCE_GROUP("Generated Files" FILES ${UTILSTEST_QT_MOC_FILES})
SET(TEST_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_Utils ${TestNL_Utils_SRCS} ${TestNL_Utils_HDRS} ${UTILSTEST_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_Utils ${TEST_LIBRARIES} )
ADD_TEST(TestNL_Utils ${EXECUTABLE_OUTPUT_PATH}/TestNL_Utils )

###########################################################################
###             TEST MESSAGE TYPE CONSTRUCTOR / DESTRUCTOR
###########################################################################

SET(MsgType_ConstrDestr_SRCS
NLTest_MsgTypes_ConstrDestr.cpp
TestMsgTypes_ConstrDestr.cpp
)

SET(MsgType_ConstrDestr_HDRS
TestMsgTypes_ConstrDestr.h
)

QT4_WRAP_CPP(MsgType_ConstrDestr_QT_MOC_FILES ${MsgType_ConstrDestr_HDRS})
SOURCE_GROUP("Generated Files" FILES ${MsgType_ConstrDestr_QT_MOC_FILES})

SET(MsgType_ConstrDestr_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_MsgTypes_ConstrDestr ${MsgType_ConstrDestr_SRCS} ${MsgType_ConstrDestr_HDRS} ${MsgType_ConstrDestr_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_MsgTypes_ConstrDestr ${MsgType_ConstrDestr_LIBRARIES} )
ADD_TEST(TestNL_MsgTypes_ConstrDestr ${EXECUTABLE_OUTPUT_PATH}/TestNL_MsgTypes_ConstrDestr )

###########################################################################
###             TEST IMAGEMESSAGE GENERAL GET / SET
###########################################################################

SET(ImgMsgType_General_SRCS
NLTest_ImgMsg_General.cpp
TestImgMsg_General.cpp
)

SET(ImgMsgType_General_HDRS
TestImgMsg_General.h
)

QT4_WRAP_CPP(ImgMsgType_General_QT_MOC_FILES ${ImgMsgType_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${ImgMsgType_General_QT_MOC_FILES})

SET(ImgMsgType_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_ImgMsg_General ${ImgMsgType_General_SRCS} ${ImgMsgType_General_HDRS} ${ImgMsgType_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_ImgMsg_General ${ImgMsgType_General_LIBRARIES} )
ADD_TEST(TestNL_ImgMsg_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_ImgMsg_General )

###########################################################################
###             TEST IMAGEMESSAGE LOAD / SAVE / INIT
###########################################################################

SET(ImgMsgType_LoadSaveInit_SRCS
NLTest_ImgMsg_LoadSaveInit.cpp
TestImgMsg_LoadSaveInit.cpp
)

SET(ImgMsgType_LoadSaveInit_HDRS
TestImgMsg_LoadSaveInit.h
)

QT4_WRAP_CPP(ImgMsgType_LoadSaveInit_QT_MOC_FILES ${ImgMsgType_LoadSaveInit_HDRS})
SOURCE_GROUP("Generated Files" FILES ${ImgMsgType_LoadSaveInit_QT_MOC_FILES})

SET(ImgMsgType_LoadSaveInit_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_ImgMsg_LoadSaveInit ${ImgMsgType_LoadSaveInit_SRCS} ${ImgMsgType_LoadSaveInit_HDRS} ${ImgMsgType_LoadSaveInit_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_ImgMsg_LoadSaveInit ${ImgMsgType_LoadSaveInit_LIBRARIES} )
ADD_TEST(TestNL_ImgMsg_LoadSaveInit ${EXECUTABLE_OUTPUT_PATH}/TestNL_ImgMsg_LoadSaveInit )

###########################################################################
###             TEST TRANSFORMMESSAGE GENERAL GET / SET
###########################################################################

SET(TransMsgType_General_SRCS
NLTest_TransMsg_General.cpp
TestTransMsg_General.cpp
)

SET(TransMsgType_General_HDRS
TestTransMsg_General.h
)

QT4_WRAP_CPP(TransMsgType_General_QT_MOC_FILES ${TransMsgType_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${TransMsgType_General_QT_MOC_FILES})

SET(TransMsgType_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_TransMsg_General ${TransMsgType_General_SRCS} ${TransMsgType_General_HDRS} ${TransMsgType_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_TransMsg_General ${TransMsgType_General_LIBRARIES} )
ADD_TEST(TestNL_TransMsg_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_TransMsg_General )

###########################################################################
###             TEST SENDERTHREAD GENERAL
###########################################################################

SET(SenderThread_General_SRCS
NLTest_SenderThread_General.cpp
TestSenderThread_General.cpp
)

SET(SenderThread_General_HDRS
TestSenderThread_General.h
)

QT4_WRAP_CPP(SenderThread_General_QT_MOC_FILES ${SenderThread_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${SenderThread_General_QT_MOC_FILES})

SET(SenderThread_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_SenderThread_General ${SenderThread_General_SRCS} ${SenderThread_General_HDRS} ${SenderThread_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_SenderThread_General ${SenderThread_General_LIBRARIES} )
ADD_TEST(TestNL_SenderThread_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_SenderThread_General )

###########################################################################
###             TEST LISTENERTHREAD GENERAL
###########################################################################

SET(ListenerThread_General_SRCS
NLTest_ListenerThread_General.cpp
TestListenerThread_General.cpp
)

SET(ListenerThread_General_HDRS
TestListenerThread_General.h
)

QT4_WRAP_CPP(ListenerThread_General_QT_MOC_FILES ${ListenerThread_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${ListenerThread_General_QT_MOC_FILES})

SET(ListenerThread_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_ListenerThread_General ${ListenerThread_General_SRCS} ${ListenerThread_General_HDRS} ${ListenerThread_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_ListenerThread_General ${ListenerThread_General_LIBRARIES} )
ADD_TEST(TestNL_ListenerThread_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_ListenerThread_General )

###########################################################################
###             TEST SEND / RECEIVE MESSAGES
###########################################################################

SET(SendReceive_Basic_SRCS
NLTest_SendReceive_Basic.cpp
TestSendReceive_Basic.cpp
)

SET(SendReceive_Basic_HDRS
TestSendReceive_Basic.h
)

QT4_WRAP_CPP(SendReceive_Basic_QT_MOC_FILES ${SendReceive_Basic_HDRS})
SOURCE_GROUP("Generated Files" FILES ${SendReceive_Basic_QT_MOC_FILES})

SET(SendReceive_Basic_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_SendReceive_Basic ${SendReceive_Basic_SRCS} ${SendReceive_Basic_HDRS} ${SendReceive_Basic_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_SendReceive_Basic ${SendReceive_Basic_LIBRARIES} )
ADD_TEST(TestNL_SendReceive_Basic ${EXECUTABLE_OUTPUT_PATH}/TestNL_SendReceive_Basic )

###########################################################################
###             TEST SEND / RECEIVE MESSAGES
###########################################################################

SET(SendReceiveTiming_SRCS
NLTest_SendReceive_Timing.cpp
TestSendReceive_Timing.cpp
)

SET(SendReceiveTiming_HDRS
TestSendReceive_Timing.h
)

QT4_WRAP_CPP(SendReceiveTiming_QT_MOC_FILES ${SendReceiveTiming_HDRS})
SOURCE_GROUP("Generated Files" FILES ${SendReceiveTiming_QT_MOC_FILES})

SET(SendReceiveTiming_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_SendReceive_Timing ${SendReceiveTiming_SRCS} ${SendReceiveTiming_HDRS} ${SendReceiveTiming_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_SendReceive_Timing ${SendReceiveTiming_LIBRARIES} )
ADD_TEST(TestNL_SendReceive_Timing ${EXECUTABLE_OUTPUT_PATH}/TestNL_SendReceive_Timing )

###########################################################################
###             TEST PTP SEND / RECEIVE MESSAGES
###########################################################################

SET(PTPSendReceive_Basic_SRCS
NLTest_PTPSendReceive_Basic.cpp
TestPTPSendReceive_Basic.cpp
)

SET(PTPSendReceive_Basic_HDRS
TestPTPSendReceive_Basic.h
)

QT4_WRAP_CPP(PTPSendReceive_Basic_QT_MOC_FILES ${PTPSendReceive_Basic_HDRS})
SOURCE_GROUP("Generated Files" FILES ${PTPSendReceive_Basic_QT_MOC_FILES})

SET(PTPSendReceive_Basic_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_PTPSendReceive_Basic ${PTPSendReceive_Basic_SRCS} ${PTPSendReceive_Basic_HDRS} ${PTPSendReceive_Basic_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_PTPSendReceive_Basic ${PTPSendReceive_Basic_LIBRARIES} )
ADD_TEST(TestNL_PTPSendReceive_Basic ${EXECUTABLE_OUTPUT_PATH}/TestNL_PTPSendReceive_Basic )