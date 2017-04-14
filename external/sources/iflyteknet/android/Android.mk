LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  

LOCAL_MODULE := iflyteknet_shared
LOCAL_MODULE_FILENAME := libiflyteknet
   
LOCAL_SRC_FILES := 	../src/iflytek/net/TcpConnection.cpp \
					../src/iflytek/net/RSAUtil.cpp					  

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src/boost/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../src/
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../src/boost/

LOCAL_LDLIBS := -pthread -lz -L$(LOCAL_PATH)/prebuilt -lcrypto -lssl

LOCAL_CFLAGS := -DASIO_STANDALONE

#导入proto库
LOCAL_WHOLE_STATIC_LIBRARIES += protobuf_static

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

$(call import-module,sources/protobuf/src)