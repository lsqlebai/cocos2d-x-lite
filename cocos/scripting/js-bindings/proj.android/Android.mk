LOCAL_PATH := $(call my-dir)
#==============================================================
include $(CLEAR_VARS)

#添加预编译的网络通信库
include $(CLEAR_VARS)
LOCAL_MODULE := libiflyteknet
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../external/sources/iflyteknet/android/libs/$(TARGET_ARCH_ABI)/libiflyteknet.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_MODULE := cocos2d_js_static
LOCAL_MODULE_FILENAME := libjscocos2d

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := ../auto/jsb_cocos2dx_extension_auto.cpp \
                   ../auto/jsb_cocos2dx_experimental_webView_auto.cpp \
                   ../auto/jsb_cocos2dx_experimental_video_auto.cpp \
                   ../auto/jsb_cocos2dx_spine_auto.cpp \
                   ../auto/jsb_cocos2dx_dragonbones_auto.cpp \
                   ../auto/jsb_cocos2dx_auto.cpp \
                   ../auto/jsb_cocos2dx_audioengine_auto.cpp \
                   ../auto/jsb_cocos2dx_studio_auto.cpp \
                   ../auto/jsb_cocos2dx_builder_auto.cpp \
                   ../auto/jsb_cocos2dx_ui_auto.cpp \
                   ../auto/jsb_cocos2dx_network_auto.cpp \
                   ../auto/jsb_creator_auto.cpp \
                   ../auto/js_bindings_chipmunk_auto_classes.cpp \
                   ../auto/js_bindings_chipmunk_functions.cpp \
                   ../manual/ScriptingCore.cpp \
                   ../manual/cocos2d_specifics.cpp \
                   ../manual/js_manual_conversions.cpp \
                   ../manual/js_bindings_core.cpp \
                   ../manual/js_bindings_opengl.cpp \
                   ../manual/jsb_opengl_functions.cpp \
                   ../manual/jsb_opengl_manual.cpp \
                   ../manual/jsb_opengl_registration.cpp \
                   ../manual/jsb_event_dispatcher_manual.cpp \
                   ../manual/js_module_register.cpp \
                   ../manual/component/CCComponentJS.cpp \
                   ../manual/experimental/jsb_cocos2dx_experimental_video_manual.cpp \
                   ../manual/experimental/jsb_cocos2dx_experimental_webView_manual.cpp \
                   ../manual/chipmunk/js_bindings_chipmunk_manual.cpp \
                   ../manual/chipmunk/js_bindings_chipmunk_registration.cpp \
                   ../manual/cocosbuilder/js_bindings_ccbreader.cpp \
                   ../manual/cocostudio/jsb_cocos2dx_studio_conversions.cpp \
                   ../manual/cocostudio/jsb_cocos2dx_studio_manual.cpp \
                   ../manual/extension/jsb_cocos2dx_extension_manual.cpp \
                   ../manual/localstorage/js_bindings_system_functions.cpp \
                   ../manual/localstorage/js_bindings_system_registration.cpp \
                   ../manual/network/jsb_socketio.cpp \
                   ../manual/network/jsb_websocket.cpp \
				   ../manual/network/jsb_asio_connection.cpp \
                   ../manual/network/XMLHTTPRequest.cpp \
                   ../manual/network/js_network_manual.cpp \
                   ../manual/spine/jsb_cocos2dx_spine_manual.cpp \
                   ../manual/dragonbones/jsb_cocos2dx_dragonbones_manual.cpp \
                   ../manual/ui/jsb_cocos2dx_ui_manual.cpp \
                   ../manual/platform/android/CCJavascriptJavaBridge.cpp \
				   ../manual/iflytek/CallbackManager.cpp \
				   ../manual/iflytek/md5.c \
				   ../manual/iflytek/DownloaderManager.cpp \
				   ../manual/iflytek/ProxyDownloader.cpp \
				   ../manual/iflytek/ZipHelper.cpp \
				   ../manual/iflytek/ProxyHttpGet.cpp \
				   ../manual/iflytek/gameLoG/jsb_GameLogic.cpp \
				   ../manual/iflytek/gameLoG/GameLogic.cpp \
				   ../manual/iflytek/umeng/jsb_umeng.cpp \
				   ../manual/iflytek/umeng/MobClickCpp.cpp \
				   ../manual/iflytek/umeng/umengJniHelper.cpp \
				   ../manual/platform/android/pay/PayListener.cpp \
				   ../manual/platform/android/JS_Binder.cpp  \
				   
LOCAL_CFLAGS := -DCOCOS2D_JAVASCRIPT -DASIO_STANDALONE

				   #../manual/iflytek/proto/game.pb.cc \
				   #../manual/iflytek/proto/json_format.cc \
LOCAL_EXPORT_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../manual \
					$(LOCAL_PATH)/../manual/network/boost \
                    $(LOCAL_PATH)/../manual/cocostudio \
                    $(LOCAL_PATH)/../manual/spine \
                    $(LOCAL_PATH)/../manual/platform/android \
                    $(LOCAL_PATH)/../auto \
                    $(LOCAL_PATH)/../../../2d \
                    $(LOCAL_PATH)/../../../base \
                    $(LOCAL_PATH)/../../../ui \
                    $(LOCAL_PATH)/../../../audio/include \
                    $(LOCAL_PATH)/../../../storage \
                    $(LOCAL_PATH)/../../../../extensions \
                    $(LOCAL_PATH)/../../../editor-support/spine \
                    $(LOCAL_PATH)/../../../editor-support/cocosbuilder \
                    $(LOCAL_PATH)/../../../editor-support/cocostudio \
                    $(LOCAL_PATH)/../../../editor-support/creator \
					$(LOCAL_PATH)/../../../../external/sources/iflyteknet/src \
					$(LOCAL_PATH)/../../../../external/sources/iflyteknet/src/boost

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../manual \
                           $(LOCAL_PATH)/../auto \
                           $(LOCAL_PATH)/../../../audio/include

#LOCAL_EXPORT_LDLIBS := -lz -lcrypto -lssl 
LOCAL_EXPORT_LDLIBS := -lz

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2d_js_android_static

#依赖网络通信库编译
LOCAL_SHARED_LIBRARIES += libiflyteknet

#导入proto库
#LOCAL_WHOLE_STATIC_LIBRARIES += protobuf_static

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += spidermonkey_static

include $(BUILD_STATIC_LIBRARY)
#==============================================================
$(call import-module,cocos)

#$(call import-module,sources/protobuf/src)
