#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <jni.h>
#include <assert.h>
#include "scripting/js-bindings/manual/platform/android/JS_Binder.h"


#define  LOG_TAG    "pay"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

extern "C"
{
	JNIEXPORT void Java_com_iflytek_unipay_js_PayCallBack_payCallback(JNIEnv *env, jobject thiz, jstring type, jstring result) {
		const char* typeStr = env->GetStringUTFChars(type, NULL);
		const char* resultStr = env->GetStringUTFChars(result, NULL);

		LOGD("type = %s, result = %s", typeStr, resultStr);
		JSBinder::callJS(typeStr, resultStr);
		env->ReleaseStringUTFChars(type, typeStr);
		env->ReleaseStringUTFChars(result, resultStr);
		
	}
}

