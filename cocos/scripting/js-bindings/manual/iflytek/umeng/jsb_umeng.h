/*!
 * \file GameLogicNative.h
 *
 * \author sulei
 * \date 六月 2017
 *
 * 友盟统计jsb实现
 */
#ifndef jsb_umeng_native
#define jsb_umeng_native

#include "jsapi.h"
#include "jsfriendapi.h"

void register_jsb_umeng_native(JSContext* cx, JS::HandleObject global);

#endif /* defined(jsb_umeng_native) */