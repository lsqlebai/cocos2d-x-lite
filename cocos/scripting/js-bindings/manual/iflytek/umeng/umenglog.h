#ifndef __UMENGLog_h__
#define __UMENGLog_h__

#if ANDROID
	#include <android/log.h>
#elif IOS
	#import <Foundation/Foundation.h>
#endif

#undef LOG_UMENG
#undef LOGD
#undef LOGW
#undef LOGE
#define LOG_UMENG "libmobclickcpp"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_UMENG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_UMENG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_UMENG,__VA_ARGS__)

#endif // __TDGALog_h__
