#include "jsb_ThreadNative.h"

#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"

#include <thread>

JSClass  *js_cocos2dx_threanative_class;
JSObject *js_cocos2dx_threadnative_prototype;

class JSB_ThreadNative
{
public:
	JSB_ThreadNative()
	{
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		_JSDelegate.construct(cx);
	}

	~JSB_ThreadNative()
	{
		_JSDelegate.destroyIfConstructed();
	}

	void setJSDelegate(JS::HandleObject pJSDelegate)
	{
		_JSDelegate.ref() = pJSDelegate;
	}
protected:
public:

	mozilla::Maybe<JS::PersistentRootedObject> _JSDelegate;
};

class ThreadNativeImpl
{

public:
	ThreadNativeImpl()
	{
		this->_p = nullptr;
	}

	void* getRefPtr()
	{
		return this->_p;
	}

	void setRefPtr(void* p)
	{
		this->_p = p;
	}

	void *_p;
};

#include "cocos2d.h"

// 异步执行一个js函数
bool js_cocos2dx_threanative_asynRun(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	ThreadNativeImpl* cobj = (ThreadNativeImpl*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");
	CCLOG("ThreadNative js_cocos2dx_threanative_asynRun argc:%d", argc);


	std::thread t([=]()
	{

		/*cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
		{

			
		});*/

		
		if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
			return;

		JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

			JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		auto params = JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr());
		JS::RootedObject jsobj(cx, params);
		jsval args = OBJECT_TO_JSVAL(jsobj);


		auto obj1 = ((JSB_ThreadNative*)cobj->getRefPtr());
		auto obj2 = obj1->_JSDelegate.ref();



		ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj2), "run", 1, &args);

	});
	t.detach();

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_threanative_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("jsbindings: constructor JS object (ThreadNative)");
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_threadnative_prototype);
	JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_threanative_class, proto, JS::NullPtr()));


	ThreadNativeImpl* cobj = new ThreadNativeImpl();

	JSB_ThreadNative* delegate = new JSB_ThreadNative();
	delegate->setJSDelegate(obj);

	cobj->setRefPtr(delegate);
	js_proxy_t *p = jsb_new_proxy(cobj, obj);
	JS::AddNamedObjectRoot(cx, &p->obj, "ThreadNative");



	args.rval().set(OBJECT_TO_JSVAL(obj));
	return true;
	//}
	//JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	//return false;

}


void js_cocos2dx_threanative_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (ThreadNative)", obj);

	JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JS::RootedObject jsObj(cx, obj);
	js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
	ThreadNativeImpl* cobj = (ThreadNativeImpl *)(proxy ? proxy->ptr : NULL);
	//if (cobj)
	//{
	//	auto ref = cobj->getRefPtr();
	//	CC_SAFE_DELETE(ref);
	//	CC_SAFE_DELETE(cobj);
	//}
}



void register_jsb_thread_native(JSContext* cx, JS::HandleObject global)
{
	js_cocos2dx_threanative_class = (JSClass *)calloc(1, sizeof(JSClass));
	js_cocos2dx_threanative_class->name = "ThreadNative";
	js_cocos2dx_threanative_class->addProperty = JS_PropertyStub;
	js_cocos2dx_threanative_class->delProperty = JS_DeletePropertyStub;
	js_cocos2dx_threanative_class->getProperty = JS_PropertyStub;
	js_cocos2dx_threanative_class->setProperty = JS_StrictPropertyStub;
	js_cocos2dx_threanative_class->enumerate = JS_EnumerateStub;
	js_cocos2dx_threanative_class->resolve = JS_ResolveStub;
	js_cocos2dx_threanative_class->convert = JS_ConvertStub;
	js_cocos2dx_threanative_class->finalize = js_cocos2dx_threanative_finalize;
	js_cocos2dx_threanative_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		// JS_PSG("readyState", js_cocos2dx_extension_WebSocket_get_readyState, JSPROP_ENUMERATE | JSPROP_PERMANENT),
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("asynRun", js_cocos2dx_threanative_asynRun, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),

		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FS_END
	};

	js_cocos2dx_threadnative_prototype = JS_InitClass(
		cx, global,
		JS::NullPtr(),
		js_cocos2dx_threanative_class,
		js_cocos2dx_threanative_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
}
