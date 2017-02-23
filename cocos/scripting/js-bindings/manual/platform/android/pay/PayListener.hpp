#include "base/ccConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifndef __custom_h__
#define __custom_h__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_PayListener_class;
extern JSObject *jsb_PayListener_prototype;

bool js_custom_PayListener_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_custom_PayListener_finalize(JSContext *cx, JSObject *obj);
void js_register_PayListener(JSContext *cx, JS::HandleObject global);
void register_all_pay_listener(JSContext* cx, JS::HandleObject obj);
bool js_PayListener_addListener(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __custom_h__
#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
