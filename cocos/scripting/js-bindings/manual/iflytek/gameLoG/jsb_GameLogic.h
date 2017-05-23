/*!
 * \file GameLogicNative.h
 *
 * \author sulei
 * \date 三月 2017
 *
 * 吃货大联盟游戏逻辑底层实现
 */
#ifndef __jsb_game_logic__
#define __jsb_game_logic__

#include "jsapi.h"
#include "jsfriendapi.h"

void register_jsb_game_logic_native(JSContext* cx, JS::HandleObject global);

#endif /* defined(__jsb_game_logic__) */