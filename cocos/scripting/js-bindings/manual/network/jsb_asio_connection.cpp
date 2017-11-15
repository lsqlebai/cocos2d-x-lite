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
    }

    ~JSB_AsioConnection()
    {
    }

    void setJSDelegate(JS::HandleObject pJSDelegate)
    {
		_JSDelegate = pJSDelegate;
    }
public:
    JS::PersistentRootedObject _JSDelegate;
};

JSClass  *js_cocos2dx_asioconnection_class;
JSObject *js_cocos2dx_asioconnection_prototype;


void runOnCocosThread(const std::function<void()>& fun)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread(fun);
}


void js_cocos2dx_AsioConnection_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOG("jsbindings: finalizing JS object %p (AsioConnection)", obj);

	/*JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JS::RootedObject jsObj(cx, obj);
	js_proxy_t *proxy = jsb_get_js_proxy(cx, jsObj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);
	if (cobj)
	{
		auto ref = cobj->getRefPtr();
		CC_SAFE_DELETE(ref);
		CC_SAFE_DELETE(cobj);
	}*/

	TcpConnection *cobj = static_cast<TcpConnection *>(JS_GetPrivate(obj));
	if (cobj)
	{
		js_proxy_t * p = jsb_get_native_proxy(cobj);
		if (p)
		{
#if not CC_ENABLE_GC_FOR_NATIVE_OBJECTS
			auto copy = &p->obj;
			JS::RemoveObjectRoot(cx, copy);
#endif
			jsb_remove_proxy(p);
		}

		ScriptingCore::getInstance()->setFinalizing(true);
		
		// check connecting status,do disconnect
		if (cobj->isConnecting())
		{
			cobj->disconnect();
		}
		JSB_AsioConnection* jsbObj = static_cast<JSB_AsioConnection*>(cobj->getRefPtr());

		CC_SAFE_DELETE(jsbObj);
		CC_SAFE_DELETE(cobj);
		
		ScriptingCore::getInstance()->setFinalizing(false);
	}

}


bool js_cocos2dx_extension_AsioConnection_asynConnect(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO asynConnect");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject jsObj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, jsObj);
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


					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));

					JS::RootedValue errorCodeJS(cx, JS::Int32Value(errorCode));
					JS_SetProperty(cx, jsobj, "errorCode", errorCodeJS);


					JS::RootedValue errorMsgJS(cx);
					c_string_to_jsval(cx, errorMsg, &errorMsgJS);
					JS_SetProperty(cx, jsobj, "errorMsg", errorMsgJS);

					JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
					JS::HandleValueArray args(jsobjVal);

					JS::RootedValue owner(cx, JS::ObjectOrNullValue(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate));

					ScriptingCore::getInstance()->executeFunctionWithOwner(owner, "onConnectResult", args);
				});
				
			});
	    }
	    else
	    {
	        JS_ReportErrorUTF8(cx, "wrong host or port");
	        return false;
	    }

		argv.rval().setUndefined();
		

	

	    return true;
	}


	JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return true;
}

bool js_cocos2dx_extension_AsioConnection_disconnect(JSContext *cx, uint32_t argc, JS::Value *vp)
{

	CCLOG("ASIO disconnect");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	cobj->disconnect();

	argv.rval().setUndefined();
	return true;
	
}

bool js_cocos2dx_extension_AsioConnection_isConnected(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO isConnected");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	JS::RootedValue jsret(cx, JS::BooleanValue(cobj->isConnected()));
	argv.rval().set(jsret);
	return true;
}


bool js_cocos2dx_extension_AsioConnection_isConnecting(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO isConnecting");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	JS::RootedValue jsret(cx, JS::BooleanValue(cobj->isConnecting()));
	argv.rval().set(jsret);

	return true;
}

bool js_cocos2dx_extension_AsioConnection_getVersion(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO getVersion");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	JS::RootedValue versionJS(cx, JS::Int32Value(TcpConnection::getVersion()));

	argv.rval().set(versionJS);

	return true;
}

bool js_cocos2dx_extension_AsioConnection_setEnableCrypt(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO setEnableCrypt");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
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
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}


bool js_cocos2dx_extension_AsioConnection_setEnableDecodeProto(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO setEnableDecodeProto");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
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
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_extension_AsioConnection_setEnableZlib(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO setEnableDecodeProto");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
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
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}


bool js_cocos2dx_extension_AsioConnection_setProxy(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ASIO setProxy");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
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
			JS_ReportErrorUTF8(cx, "wrong arguments format, arg1 must be string, arg2 must be int");
			return false;
		}
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
		return false;
	}


	return true;
}

bool js_cocos2dx_extension_AsioConnection_asynSend(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	//CCLOG("ASIO asynSend");
    JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
	TcpConnection* cobj = (TcpConnection *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "Invalid Native Object");

    if(argc == 1 || argc == 2)
    {
        

		if (argv[0].isObject() && argv[1].isInt32())
        {
            uint8_t *bufdata = NULL;
            uint32_t len = 0;

            JS::RootedObject jsobj(cx, argv[0].toObjectOrNull());
			bool flag;
            if (JS_IsArrayBufferObject(jsobj))
            {
				bufdata = JS_GetArrayBufferData(jsobj, &flag, JS::AutoCheckCannotGC());
                len = JS_GetArrayBufferByteLength(jsobj);
            }
            else if (JS_IsArrayBufferViewObject(jsobj))
            {
                bufdata = (uint8_t*)JS_GetArrayBufferViewData(jsobj, &flag, JS::AutoCheckCannotGC());
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


					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));

					JS::RootedValue sendIdJS(cx, JS::Int32Value(sendId));
					JS_SetProperty(cx, jsobj, "sendId", sendIdJS);

					JS::RootedValue errorCodeJS(cx, JS::Int32Value(sendResult));
					JS_SetProperty(cx, jsobj, "errorCode", errorCodeJS);

					JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
					JS::HandleValueArray args(jsobjVal);
					JS::RootedValue owner(cx, JS::ObjectOrNullValue(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate));
					
					ScriptingCore::getInstance()->executeFunctionWithOwner(owner, "onSendResult", args);
				});

			}, argv[1].toInt32());
        }
		else if (argv[0].isString())
		{

			JS_ReportErrorUTF8(cx, "data type string to be sent is unsupported.");

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
            JS_ReportErrorUTF8(cx, "data type to be sent is unsupported.");
            return false;
        }

        argv.rval().setUndefined();

        return true;
    }

    JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d or %d", argc, 1, 2);
    return true;
}


bool js_cocos2dx_extension_AsioConnection_constructor(JSContext *cx, uint32_t argc, JS::Value *vp)
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
        JS::RootedObject obj(cx, JS_NewObjectWithGivenProto(cx, js_cocos2dx_asioconnection_class, proto));

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

				JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));

				JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
				JS::HandleValueArray args(jsobjVal);

				if (tempData)
				{

					// data is binary
					JS::RootedObject buffer(cx, JS_NewArrayBuffer(cx, dataLen));

					bool flag;
					if (dataLen > 0)
					{
						uint8_t* bufdata = JS_GetArrayBufferData(buffer, &flag, JS::AutoCheckCannotGC());
						memcpy((void*)bufdata, tempData, dataLen);
					}
					JS::RootedValue dataVal(cx, JS::ObjectOrNullValue(buffer));
					JS_SetProperty(cx, jsobj, "data", dataVal);
				}

				JS::RootedValue errorMsgJS(cx);
				c_string_to_jsval(cx, jsonStr.c_str(), &errorMsgJS);
				JS_SetProperty(cx, jsobj, "protoObjJson", errorMsgJS);

				JS::RootedValue owner(cx, JS::ObjectOrNullValue(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate));

				ScriptingCore::getInstance()->executeFunctionWithOwner(owner, "onMessage", args);
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

				JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));

				JS::RootedValue isByMyselfJS(cx, JS::BooleanValue(isBySelf));
				JS_SetProperty(cx, jsobj, "isBySelf", isByMyselfJS);

				JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
				JS::HandleValueArray args(jsobjVal);

				JS::RootedValue owner(cx, JS::ObjectOrNullValue(((JSB_AsioConnection*)cobj->getRefPtr())->_JSDelegate));

				ScriptingCore::getInstance()->executeFunctionWithOwner(owner, "onDisconnect", args);
			});
		});

		JSB_AsioConnection* delegate = new JSB_AsioConnection();
		delegate->setJSDelegate(obj);
		cobj->setRefPtr(delegate);
        //js_proxy_t *p = jsb_new_proxy(cx, cobj, obj);
        //JS::AddNamedObjectRoot(cx, &p->obj, "AsioConnection");

		jsb_new_proxy(cx, cobj, obj);
		JS_SetPrivate(obj.get(), cobj);

		JS::RootedValue ret(cx, JS::ObjectOrNullValue(obj));
		args.rval().set(ret);
		return true;
    //}
	//JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	//return false;
	
}

void register_jsb_asio_connection(JSContext *cx, JS::HandleObject global)
{

	JSClass  *js_cocos2dx_asioconnection_class;
	JSObject *js_cocos2dx_asioconnection_prototype;


	static const JSClassOps asioconnection_classOps = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		js_cocos2dx_AsioConnection_finalize,
		nullptr, nullptr, nullptr, nullptr
	};

	static JSClass asioconnection_class = {
		"AsioConnection",
		JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
		&asioconnection_classOps
	};

	js_cocos2dx_asioconnection_class = &asioconnection_class;

	static JSPropertySpec properties[] = {
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

	JS::RootedObject parent_proto(cx, nullptr);
	js_cocos2dx_asioconnection_prototype = JS_InitClass(
		cx, global,
		parent_proto,
		js_cocos2dx_asioconnection_class,
		js_cocos2dx_extension_AsioConnection_constructor, 0, // constructor
		properties,
		funcs,
		nullptr, // no static properties
		nullptr);




  //  js_cocos2dx_asioconnection_class = (JSClass *)calloc(1, sizeof(JSClass));
  //  js_cocos2dx_asioconnection_class->name = "AsioConnection";
  //  js_cocos2dx_asioconnection_class->addProperty = JS_PropertyStub;
  //  js_cocos2dx_asioconnection_class->delProperty = JS_DeletePropertyStub;
  //  js_cocos2dx_asioconnection_class->getProperty = JS_PropertyStub;
  //  js_cocos2dx_asioconnection_class->setProperty = JS_StrictPropertyStub;
  //  js_cocos2dx_asioconnection_class->enumerate = JS_EnumerateStub;
  //  js_cocos2dx_asioconnection_class->resolve = JS_ResolveStub;
  //  js_cocos2dx_asioconnection_class->convert = JS_ConvertStub;
  //  js_cocos2dx_asioconnection_class->finalize = js_cocos2dx_AsioConnection_finalize;
  //  js_cocos2dx_asioconnection_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

  //  static JSPropertySpec properties[] = {
  //     // JS_PSG("readyState", js_cocos2dx_extension_WebSocket_get_readyState, JSPROP_ENUMERATE | JSPROP_PERMANENT),
  //      JS_PS_END
  //  };

  //  static JSFunctionSpec funcs[] = {
		//JS_FN("asynConnect", js_cocos2dx_extension_AsioConnection_asynConnect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("disconnect", js_cocos2dx_extension_AsioConnection_disconnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("isConnected", js_cocos2dx_extension_AsioConnection_isConnected, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("isConnecting", js_cocos2dx_extension_AsioConnection_isConnecting, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("asynSend", js_cocos2dx_extension_AsioConnection_asynSend, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),		
		//JS_FN("setEnableCrypt", js_cocos2dx_extension_AsioConnection_setEnableCrypt, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("setEnableDecodeProto", js_cocos2dx_extension_AsioConnection_setEnableDecodeProto, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("setEnableZlib", js_cocos2dx_extension_AsioConnection_setEnableZlib, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("setProxy", js_cocos2dx_extension_AsioConnection_setProxy, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("getVersion", js_cocos2dx_extension_AsioConnection_getVersion, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//
  //      JS_FS_END
  //  };

  //  static JSFunctionSpec st_funcs[] = {
  //      JS_FS_END
  //  };

  //  js_cocos2dx_asioconnection_prototype = JS_InitClass(
  //                                              cx, global,
  //                                              JS::NullPtr(),
  //                                              js_cocos2dx_asioconnection_class,
  //                                              js_cocos2dx_extension_AsioConnection_constructor, 0, // constructor
  //                                              properties,
  //                                              funcs,
  //                                              NULL, // no static properties
  //                                              st_funcs);

  //  //JS::RootedObject jsclassObj(cx, anonEvaluate(cx, global, "(function () { return AsioConnection; })()").toObjectOrNull());

  //  //JS_DefineProperty(cx, jsclassObj, "CONNECTING", (int)WebSocket::State::CONNECTING, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
  //  //JS_DefineProperty(cx, jsclassObj, "OPEN", (int)WebSocket::State::OPEN, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
  //  //JS_DefineProperty(cx, jsclassObj, "CLOSING", (int)WebSocket::State::CLOSING, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
  //  //JS_DefineProperty(cx, jsclassObj, "CLOSED", (int)WebSocket::State::CLOSED, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY);
}
