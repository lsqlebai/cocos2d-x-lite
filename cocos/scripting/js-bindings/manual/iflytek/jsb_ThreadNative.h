
#ifndef __jsb_thread_native__
#define __jsb_thread_native__

#include "jsapi.h"
#include "jsfriendapi.h"

void register_jsb_thread_native(JSContext* cx, JS::HandleObject global);

#endif /* defined(__jsb_thread_native__) */