/*
 * Created by James Chen
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "scripting/js-bindings/manual/network/jsb_websocket.h"

#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "network/WebSocket.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"

#include "iflytek/net/TcpConnection.hpp"

#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::network;

/*
 [Constructor(in DOMString url, in optional DOMString protocols)]
 [Constructor(in DOMString url, in optional DOMString[] protocols)]
 interface WebSocket {
 readonly attribute DOMString url;

 // ready state
 const unsigned short CONNECTING = 0;
 const unsigned short OPEN = 1;
 const unsigned short CLOSING = 2;
 const unsigned short CLOSED = 3;
 readonly attribute unsigned short readyState;
 readonly attribute unsigned long bufferedAmount;

 // networking
 attribute Function onopen;
 attribute Function onmessage;
 attribute Function onerror;
 attribute Function onclose;
 readonly attribute DOMString protocol;
 void send(in DOMString data);
 void close();
 };
 WebSocket implements EventTarget;
 */

class JSB_AsioConnection
{
public:

    JSB_AsioConnection()
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        _JSDelegate.construct(cx);
    }

    ~JSB_AsioConnection()
    {
        _JSDelegate.destroyIfConstructed();
    }

    //virtual void onOpen(WebSocket* ws)
    //{
    //    js_proxy_t * p = jsb_get_native_proxy(ws);
    //    if (!p) return;

    //    if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
    //        return;

    //    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    //    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    //    JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
    //    JS::RootedValue vp(cx);
    //    vp = c_string_to_jsval(cx, "open");
    //    JS_SetProperty(cx, jsobj, "type", vp);

    //    jsval args = OBJECT_TO_JSVAL(jsobj);

    //    ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate.ref()), "onopen", 1, &args);
    //}

    //virtual void onMessage(WebSocket* ws, const WebSocket::Data& data)
    //{
    //    js_proxy_t * p = jsb_get_native_proxy(ws);
    //    if (p == nullptr) return;

    //    if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
    //        return;

    //    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    //    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    //    JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
    //    JS::RootedValue vp(cx);
    //    vp = c_string_to_jsval(cx, "message");
    //    JS_SetProperty(cx, jsobj, "type", vp);

    //    JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));
    //    if (data.isBinary)
    //    {// data is binary
    //        JS::RootedObject buffer(cx, JS_NewArrayBuffer(cx, static_cast<uint32_t>(data.len)));
    //        if (data.len > 0)
    //        {
    //            uint8_t* bufdata = JS_GetArrayBufferData(buffer);
    //            memcpy((void*)bufdata, (void*)data.bytes, data.len);
    //        }
    //        JS::RootedValue dataVal(cx);
    //        dataVal = OBJECT_TO_JSVAL(buffer);
    //        JS_SetProperty(cx, jsobj, "data", dataVal);
    //    }
    //    else
    //    {// data is string
    //        JS::RootedValue dataVal(cx);
    //        if (strlen(data.bytes) == 0 && data.len > 0)
    //        {// String with 0x00 prefix
    //            dataVal = STRING_TO_JSVAL(JS_NewStringCopyN(cx, data.bytes, data.len));
    //        }
    //        else
    //        {// Normal string
    //            dataVal = c_string_to_jsval(cx, data.bytes);
    //        }
    //        if (dataVal.isNullOrUndefined())
    //        {
    //            ws->closeAsync();
    //            return;
    //        }
    //        JS_SetProperty(cx, jsobj, "data", dataVal);
    //    }

    //    ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate.ref()), "onMessage", 1, args.address());
    //}

    //virtual void onClose(WebSocket* ws)
    //{
    //    js_proxy_t * p = jsb_get_native_proxy(ws);
    //    if (!p) return;

    //    if (cocos2d::Director::getInstance() != nullptr && cocos2d::Director::getInstance()->getRunningScene() && cocos2d::ScriptEngineManager::getInstance() != nullptr)
    //    {
    //        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    //        
    //        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    //        JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
    //        JS::RootedValue vp(cx);
    //        vp = c_string_to_jsval(cx, "close");
    //        JS_SetProperty(cx, jsobj, "type", vp);
    //        
    //        JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));
    //        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate.ref()), "onclose", 1, args.address());
    //        
    //        auto copy = &p->obj;
    //        JS::RemoveObjectRoot(cx, copy);
    //        jsb_remove_proxy(p);
    //    }
    //    
    //    // Delete WebSocket instance
    //    CC_SAFE_DELETE(ws);
    //    // Delete self at last while websocket was closed.
    //    delete this;
    //}

    //virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error)
    //{
    //    js_proxy_t * p = jsb_get_native_proxy(ws);
    //    if (!p) return;

    //    if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
    //        return;

    //    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    //    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
    //    JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
    //    JS::RootedValue vp(cx);
    //    vp = c_string_to_jsval(cx, "error");
    //    JS_SetProperty(cx, jsobj, "type", vp);

    //    JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

    //    ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(_JSDelegate.ref()), "onerror", 1, args.address());
    //}

    void setJSDelegate(JS::HandleObject pJSDelegate)
    {
        _JSDelegate.ref() = pJSDelegate;
    }
public:
    mozilla::Maybe<JS::PersistentRootedObject> _JSDelegate;
};

JSClass  *js_cocos2dx_asioconnection_class;
JSObject *js_cocos2dx_asioconnection_prototype;


void runOnCocosThread(const std::function<void()>& fun)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread(fun);
}


void js_cocos2dx_AsioConnection_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOG("jsbindings: finalizing JS object %p (AsioConnection)", obj);

	JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JS::RootedObject jsObj(cx, obj);
	js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);
	if (cobj)
	{
		auto ref = cobj->getRefPtr();
		CC_SAFE_DELETE(ref);
		CC_SAFE_DELETE(cobj);
	}
}


bool js_cocos2dx_extension_AsioConnection_asynConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO asynConnect");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject jsObj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if(argc == 2)
	{
		if (argv[0].isString() && argv[1].isInt32())
	    {
	        ssize_t len = JS_GetStringLength(argv[0].toString());
	        std::string host;
	        jsval_to_std_string(cx, argv[0], &host);
			
	        if (host.empty() && len > 0)
	        {
	            CCLOGWARN("Text message to send is empty, but its length is greater than 0!");
	            //FIXME: Note that this text message contains '0x00' prefix, so its length calcuted by strlen is 0.
	            // we need to fix that if there is '0x00' in text message,
	            // since javascript language could support '0x00' inserted at the beginning or the middle of text message
	        }

			int port;
			jsval_to_int(cx, argv[1], &port);

			cobj->asynConnect(host, port, [=](int errorCode, const char* errorMsg)
			{

				CCLOG("ASIO connect result:%d, %s", errorCode, errorMsg);

				runOnCocosThread([=]()
				{

					// connect callback
					if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
						return;

					JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET


					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

					JS::RootedValue errorCodeJS(cx);
					errorCodeJS = INT_TO_JSVAL(errorCode);
					JS_SetProperty(cx, jsobj, "errorCode", errorCodeJS);


					JS::RootedValue errorMsgJS(cx);
					errorMsgJS = c_string_to_jsval(cx, errorMsg);
					JS_SetProperty(cx, jsobj, "errorMsg", errorMsgJS);


					JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

					ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate.ref()), "onConnectResult", 1, args.address());
				});
				
			});
	    }
	    else
	    {
	        JS_ReportError(cx, "wrong host or port");
	        return false;
	    }

		argv.rval().setUndefined();
		

	

	    return true;
	}


	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return true;
}

bool js_cocos2dx_extension_AsioConnection_disconnect(JSContext *cx, uint32_t argc, jsval *vp)
{

	CCLOG("ASIO disconnect");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	cobj->disconnect();

	argv.rval().setUndefined();
	return true;
	
}

bool js_cocos2dx_extension_AsioConnection_isConnected(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO isConnected");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().set(BOOLEAN_TO_JSVAL(cobj->isConnected()));
	return true;
}


bool js_cocos2dx_extension_AsioConnection_isConnecting(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO isConnecting");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().set(BOOLEAN_TO_JSVAL(cobj->isConnecting()));

	return true;
}

bool js_cocos2dx_extension_AsioConnection_getVersion(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO getVersion");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().set(INT_TO_JSVAL(TcpConnection::getVersion()));

	return true;
}

bool js_cocos2dx_extension_AsioConnection_setEnableCrypt(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO setEnableCrypt");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	if (argc == 1)
	{
		if (argv[0].isBoolean())
		{
			bool isEnable = argv[0].toBoolean();
			CCLOG("EnableCrypt:%d", isEnable);
			cobj->setEnableCrypt(isEnable);
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}


bool js_cocos2dx_extension_AsioConnection_setEnableDecodeProto(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO setEnableDecodeProto");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	if (argc == 1)
	{
		if (argv[0].isBoolean())
		{
			bool isEnable = argv[0].toBoolean();
			CCLOG("EnableDecodeProto:%d", isEnable);
			cobj->setEnableDecodeProto(isEnable);
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_extension_AsioConnection_setEnableZlib(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO setEnableDecodeProto");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if (argc == 1)
	{
		if (argv[0].isBoolean())
		{
			bool isEnable = argv[0].toBoolean();
			CCLOG("EnableZlib:%d", isEnable);
			cobj->setEnableZlib(isEnable);
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}


bool js_cocos2dx_extension_AsioConnection_setProxy(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("ASIO setProxy");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	if (argc == 2)
	{
		if (argv[0].isString() && argv[1].isInt32())
		{
			bool isEnable = argv[0].toBoolean();
			cobj->setEnableCrypt(isEnable);

			ssize_t len = JS_GetStringLength(argv[0].toString());
			std::string proxyHost;
			jsval_to_std_string(cx, argv[0], &proxyHost);

			int32_t proxyPort = argv[1].toInt32();

			cobj->setProxy(proxyHost, proxyPort); // 配置代理
		}
		else
		{
			JS_ReportError(cx, "wrong arguments format, arg1 must be string, arg2 must be int");
			return false;
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
		return false;
	}


	return true;
}

bool js_cocos2dx_extension_AsioConnection_asynSend(JSContext *cx, uint32_t argc, jsval *vp)
{
	//CCLOG("ASIO asynSend");
    JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "Invalid Native Object");

    if(argc == 1 || argc == 2)
    {
        

		if (argv[0].isObject() && argv[1].isInt32())
        {
            uint8_t *bufdata = NULL;
            uint32_t len = 0;

            JS::RootedObject jsobj(cx, argv[0].toObjectOrNull());
            if (JS_IsArrayBufferObject(jsobj))
            {
                bufdata = JS_GetArrayBufferData(jsobj);
                len = JS_GetArrayBufferByteLength(jsobj);
            }
            else if (JS_IsArrayBufferViewObject(jsobj))
            {
                bufdata = (uint8_t*)JS_GetArrayBufferViewData(jsobj);
                len = JS_GetArrayBufferViewByteLength(jsobj);
            }

			//CCLOG("ASIO asynSend data len:%d", len);
			cobj->asynSend(bufdata, len, [=](int sendId, int sendResult)
			{
				//CCLOG("ASIO asynSend result:%d, sendId:%d", sendResult, sendId);

				runOnCocosThread([=]()
				{

					// connect callback
					if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
						return;

					JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET


						JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

					JS::RootedValue sendIdJS(cx);
					sendIdJS = INT_TO_JSVAL(sendId);
					JS_SetProperty(cx, jsobj, "sendId", sendIdJS);

					JS::RootedValue errorCodeJS(cx);
					errorCodeJS = INT_TO_JSVAL(sendResult);
					JS_SetProperty(cx, jsobj, "errorCode", errorCodeJS);


					JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

					ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate.ref()), "onSendResult", 1, args.address());
				});

			}, argv[1].toInt32());
        }
		else if (argv[0].isString())
		{

			JS_ReportError(cx, "data type string to be sent is unsupported.");

			ssize_t len = JS_GetStringLength(argv[0].toString());
			std::string data;
			jsval_to_std_string(cx, argv[0], &data);

			if (data.empty() && len > 0)
			{
				CCLOGWARN("Text message to send is empty, but its length is greater than 0!");
				//FIXME: Note that this text message contains '0x00' prefix, so its length calcuted by strlen is 0.
				// we need to fix that if there is '0x00' in text message,
				// since javascript language could support '0x00' inserted at the beginning or the middle of text message
			}

			//cobj->asynSend(data);
		}
        else
        {
            JS_ReportError(cx, "data type to be sent is unsupported.");
            return false;
        }

        argv.rval().setUndefined();

        return true;
    }

    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d or %d", argc, 1, 2);
    return true;
}


bool js_cocos2dx_extension_AsioConnection_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

  //  if (argc == 1 || argc == 2)
  //  {
		//std::string url;

		//do {
		//	bool ok = jsval_to_std_string(cx, args.get(0), &url);
		//	JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
		//} while (0);

        JS::RootedObject proto(cx, js_cocos2dx_asioconnection_prototype);
        JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_asioconnection_class, proto, JS::NullPtr()));

		TcpConnection* cobj = new TcpConnection();

		// 注册接收函数
		cobj->registerReceiveCallback([=](void* data, const std::size_t& dataLen, const string& jsonStr)
		{

			//CCLOG("ASIO onMessage dataLen:%d", dataLen);

			int8_t* tempData = nullptr;
			
			if (jsonStr.empty())
			{
				tempData = new int8_t[dataLen];
				memcpy(tempData, data, dataLen);
			}

			runOnCocosThread([cobj, tempData, dataLen, jsonStr]()
			{
				if (tempData)
				{
					std::unique_ptr<int8_t[]> tempDataPtr(tempData);
				}
				

				if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
					return;

				JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

				JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				auto obj = JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr());
				JS::RootedObject jsobj(cx, obj);
				jsval args = OBJECT_TO_JSVAL(jsobj);

				if (tempData)
				{

					// data is binary
					JS::RootedObject buffer(cx, JS_NewArrayBuffer(cx, dataLen));

					if (dataLen > 0)
					{
						uint8_t* bufdata = JS_GetArrayBufferData(buffer);
						memcpy((void*)bufdata, tempData, dataLen);
					}
					JS::RootedValue dataVal(cx);
					dataVal = OBJECT_TO_JSVAL(buffer);
					JS_SetProperty(cx, jsobj, "data", dataVal);
				}

				JS::RootedValue errorMsgJS(cx);
				errorMsgJS = c_string_to_jsval(cx, jsonStr.c_str());
				JS_SetProperty(cx, jsobj, "protoObjJson", errorMsgJS);

				ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate.ref()), "onMessage", 1, &args);
			});
		});

		// 注册断开连接函数
		cobj->registerDisconnectCallback([=](bool isBySelf)
		{
			CCLOG("registerDisconnectCallback ondis");

			runOnCocosThread([=]()
			{
				if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
					return;

				JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
				jsval args = OBJECT_TO_JSVAL(jsobj);

				JS::RootedValue isByMyselfJS(cx);
				isByMyselfJS = BOOLEAN_TO_JSVAL(isBySelf);
				JS_SetProperty(cx, jsobj, "isBySelf", isByMyselfJS);


				ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate.ref()), "onDisconnect", 1, &args);
			});
		});

		JSB_AsioConnection* delegate = new JSB_AsioConnection();
		delegate->setJSDelegate(obj);
		cobj->setRefPtr(delegate);
        js_proxy_t *p = jsb_new_proxy(cobj, obj);
        JS::AddNamedObjectRoot(cx, &p->obj, "AsioConnection");



		args.rval().set(OBJECT_TO_JSVAL(obj));
		return true;
    //}
	//JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	//return false;
	
}

void register_jsb_asio_connection(JSContext *cx, JS::HandleObject global)
{
    js_cocos2dx_asioconnection_class = (JSClass *)calloc(1, sizeof(JSClass));
    js_cocos2dx_asioconnection_class->name = "AsioConnection";
    js_cocos2dx_asioconnection_class->addProperty = JS_PropertyStub;
    js_cocos2dx_asioconnection_class->delProperty = JS_DeletePropertyStub;
    js_cocos2dx_asioconnection_class->getProperty = JS_PropertyStub;
    js_cocos2dx_asioconnection_class->setProperty = JS_StrictPropertyStub;
    js_cocos2dx_asioconnection_class->enumerate = JS_EnumerateStub;
    js_cocos2dx_asioconnection_class->resolve = JS_ResolveStub;
    js_cocos2dx_asioconnection_class->convert = JS_ConvertStub;
    js_cocos2dx_asioconnection_class->finalize = js_cocos2dx_AsioConnection_finalize;
    js_cocos2dx_asioconnection_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
       // JS_PSG("readyState", js_cocos2dx_extension_WebSocket_get_readyState, JSPROP_ENUMERATE | JSPROP_PERMANENT),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
		JS_FN("asynConnect", js_cocos2dx_extension_AsioConnection_asynConnect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("disconnect", js_cocos2dx_extension_AsioConnection_disconnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("isConnected", js_cocos2dx_extension_AsioConnection_isConnected, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("isConnecting", js_cocos2dx_extension_AsioConnection_isConnecting, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("asynSend", js_cocos2dx_extension_AsioConnection_asynSend, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),		
		JS_FN("setEnableCrypt", js_cocos2dx_extension_AsioConnection_setEnableCrypt, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setEnableDecodeProto", js_cocos2dx_extension_AsioConnection_setEnableDecodeProto, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setEnableZlib", js_cocos2dx_extension_AsioConnection_setEnableZlib, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setProxy", js_cocos2dx_extension_AsioConnection_setProxy, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getVersion", js_cocos2dx_extension_AsioConnection_getVersion, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FS_END
    };

    js_cocos2dx_asioconnection_prototype = JS_InitClass(
                                                cx, global,
                                                JS::NullPtr(),
                                                js_cocos2dx_asioconnection_class,
                                                js_cocos2dx_extension_AsioConnection_constructor, 0, // constructor
                                                properties,
                                                funcs,
                                                NULL, // no static properties
                                                st_funcs);

    //JS::RootedObject jsclassObj(cx, anonEvaluate(cx, global, "(function () { return AsioConnection; })()").toObjectOrNull());

    //JS_DefineProperty(cx, jsclassObj, "CONNECTING", (int)WebSocket::State::CONNECTING, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
    //JS_DefineProperty(cx, jsclassObj, "OPEN", (int)WebSocket::State::OPEN, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
    //JS_DefineProperty(cx, jsclassObj, "CLOSING", (int)WebSocket::State::CLOSING, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
    //JS_DefineProperty(cx, jsclassObj, "CLOSED", (int)WebSocket::State::CLOSED, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
}
