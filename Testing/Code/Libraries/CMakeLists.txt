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

# --------------------------------------------------------------------------
# Sources

SET(TestNL_Utils_SRCS
Common/NiftyLinkUtilsTests.cxx
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
NLTest_MsgTypes_ConstrDestr.cxx
TestMsgTypes_ConstrDestr.cxx
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
###             TEST MESSAGE TYPE DELIVERY
###########################################################################

SET(MsgTypes_Delivery_SRCS
NLTest_MsgTypes_Delivery.cxx
TestMsgTypes_Delivery.cxx
)

SET(MsgTypes_Delivery_HDRS
TestMsgTypes_Delivery.h
)

QT4_WRAP_CPP(MsgTypes_Delivery_QT_MOC_FILES ${MsgTypes_Delivery_HDRS})
SOURCE_GROUP("Generated Files" FILES ${MsgTypes_Delivery_QT_MOC_FILES})

SET(MsgTypes_Delivery_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_MsgTypes_Delivery ${MsgTypes_Delivery_SRCS} ${MsgTypes_Delivery_HDRS} ${MsgTypes_Delivery_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_MsgTypes_Delivery ${MsgTypes_Delivery_LIBRARIES} )
ADD_TEST(TestNL_MsgTypes_Delivery ${EXECUTABLE_OUTPUT_PATH}/TestNL_MsgTypes_Delivery )

###########################################################################
###             TEST IMAGEMESSAGE GENERAL GET / SET
###########################################################################

SET(ImgMsgType_General_SRCS
NLTest_ImgMsg_General.cxx
TestImgMsg_General.cxx
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
NLTest_ImgMsg_LoadSaveInit.cxx
TestImgMsg_LoadSaveInit.cxx
)

SET(ImgMsgType_LoadSaveInit_HDRS
TestImgMsg_LoadSaveInit.h
)

QT4_WRAP_CPP(ImgMsgType_LoadSaveInit_QT_MOC_FILES ${ImgMsgType_LoadSaveInit_HDRS})
SOURCE_GROUP("Generated Files" FILES ${ImgMsgType_LoadSaveInit_QT_MOC_FILES})

SET(ImgMsgType_LoadSaveInit_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_ImgMsg_LoadSaveInit ${ImgMsgType_LoadSaveInit_SRCS} ${ImgMsgType_LoadSaveInit_HDRS} ${ImgMsgType_LoadSaveInit_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_ImgMsg_LoadSaveInit ${ImgMsgType_LoadSaveInit_LIBRARIES} )
ADD_TEST(TestNL_ImgMsg_LoadSaveInit ${EXECUTABLE_OUTPUT_PATH}/TestNL_ImgMsg_LoadSaveInit ${CMAKE_SOURCE_DIR}/Testing/Data/niftkUltrasonix.png)

###########################################################################
###             TEST TRANSFORMMESSAGE GENERAL GET / SET
###########################################################################

SET(TransMsgType_General_SRCS
NLTest_TransMsg_General.cxx
TestTransMsg_General.cxx
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
###             TEST TRACKERDATAMESSAGE GENERAL GET / SET
###########################################################################

SET(TdataMsgType_General_SRCS
NLTest_TdataMsg_General.cxx
TestTdataMsg_General.cxx
)

SET(TdataMsgType_General_HDRS
TestTdataMsg_General.h
)

QT4_WRAP_CPP(TdataMsgType_General_QT_MOC_FILES ${TdataMsgType_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${TdataMsgType_General_QT_MOC_FILES})

SET(TdataMsgType_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_TdataMsg_General ${TdataMsgType_General_SRCS} ${TdataMsgType_General_HDRS} ${TdataMsgType_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_TdataMsg_General ${TdataMsgType_General_LIBRARIES} )
ADD_TEST(TestNL_TdataMsg_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_TdataMsg_General )

###########################################################################
###             TEST STATUSMESSAGE GENERAL GET / SET
###########################################################################

SET(StatusMsgType_General_SRCS
NLTest_StatusMsg_General.cxx
TestStatusMsg_General.cxx
)

SET(StatusMsgType_General_HDRS
TestStatusMsg_General.h
)

QT4_WRAP_CPP(StatusMsgType_General_QT_MOC_FILES ${StatusMsgType_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${StatusMsgType_General_QT_MOC_FILES})

SET(StatusMsgType_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_StatusMsg_General ${StatusMsgType_General_SRCS} ${StatusMsgType_General_HDRS} ${StatusMsgType_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_StatusMsg_General ${StatusMsgType_General_LIBRARIES} )
ADD_TEST(TestNL_StatusMsg_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_StatusMsg_General )

###########################################################################
###             TEST STRINGMESSAGE GENERAL GET / SET
###########################################################################

SET(StringMsgType_General_SRCS
NLTest_StringMsg_General.cxx
TestStringMsg_General.cxx
)

SET(StringMsgType_General_HDRS
TestStringMsg_General.h
)

QT4_WRAP_CPP(StringMsgType_General_QT_MOC_FILES ${StringMsgType_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${StringMsgType_General_QT_MOC_FILES})

SET(StringMsgType_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_StringMsg_General ${StringMsgType_General_SRCS} ${StringMsgType_General_HDRS} ${StringMsgType_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_StringMsg_General ${StringMsgType_General_LIBRARIES} )
ADD_TEST(TestNL_StringMsg_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_StringMsg_General )

###########################################################################
###             TEST SENDERPROCESS GENERAL
###########################################################################

SET(SenderProcess_General_SRCS
NLTest_SenderProcess_General.cxx
TestSenderProcess_General.cxx
)

SET(SenderProcess_General_HDRS
Network/NiftyLinkSenderProcessTester.h
TestSenderProcess_General.h
)

QT4_WRAP_CPP(SenderProcess_General_QT_MOC_FILES ${SenderProcess_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${SenderProcess_General_QT_MOC_FILES})

SET(SenderProcess_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_SenderProcess_General ${SenderProcess_General_SRCS} ${SenderProcess_General_HDRS} ${SenderProcess_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_SenderProcess_General ${SenderProcess_General_LIBRARIES} )
ADD_TEST(TestNL_SenderProcess_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_SenderProcess_General )

###########################################################################
###             TEST LISTENERPROCESS GENERAL
###########################################################################

SET(ListenerProcess_General_SRCS
NLTest_ListenerProcess_General.cxx
TestListenerProcess_General.cxx
)

SET(ListenerProcess_General_HDRS
Network/NiftyLinkListenerProcessTester.h
TestListenerProcess_General.h
)

QT4_WRAP_CPP(ListenerProcess_General_QT_MOC_FILES ${ListenerProcess_General_HDRS})
SOURCE_GROUP("Generated Files" FILES ${ListenerProcess_General_QT_MOC_FILES})

SET(ListenerProcess_General_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

ADD_EXECUTABLE(TestNL_ListenerProcess_General ${ListenerProcess_General_SRCS} ${ListenerProcess_General_HDRS} ${ListenerProcess_General_QT_MOC_FILES})
TARGET_LINK_LIBRARIES(TestNL_ListenerProcess_General ${ListenerProcess_General_LIBRARIES} )
ADD_TEST(TestNL_ListenerProcess_General ${EXECUTABLE_OUTPUT_PATH}/TestNL_ListenerProcess_General )

###########################################################################
###             TEST SEND / RECEIVE MESSAGES
###########################################################################

SET(SendReceive_Basic_SRCS
NLTest_SendReceive_Basic.cxx
TestSendReceive_Basic.cxx
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
NLTest_SendReceive_Timing.cxx
TestSendReceive_Timing.cxx
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

#SET(PTPSendReceive_Basic_SRCS
#NLTest_PTPSendReceive_Basic.cxx
#TestPTPSendReceive_Basic.cxx
#)

#SET(PTPSendReceive_Basic_HDRS
#TestPTPSendReceive_Basic.h
#)

#QT4_WRAP_CPP(PTPSendReceive_Basic_QT_MOC_FILES ${PTPSendReceive_Basic_HDRS})
#SOURCE_GROUP("Generated Files" FILES ${PTPSendReceive_Basic_QT_MOC_FILES})

#SET(PTPSendReceive_Basic_LIBRARIES niftylink ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

#ADD_EXECUTABLE(TestNL_PTPSendReceive_Basic ${PTPSendReceive_Basic_SRCS} ${PTPSendReceive_Basic_HDRS} ${PTPSendReceive_Basic_QT_MOC_FILES})
#TARGET_LINK_LIBRARIES(TestNL_PTPSendReceive_Basic ${PTPSendReceive_Basic_LIBRARIES} )
#ADD_TEST(TestNL_PTPSendReceive_Basic ${EXECUTABLE_OUTPUT_PATH}/TestNL_PTPSendReceive_Basic )
