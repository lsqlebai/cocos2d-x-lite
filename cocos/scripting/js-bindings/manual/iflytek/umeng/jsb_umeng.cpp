#include "jsb_umeng.h"


#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"
#include "cocos2d.h"
#include "MobClickCpp.h"

USING_NS_CC;
JSClass  *js_cocos2dx_umeng_class;
JSObject *js_cocos2dx_umeng_prototype;

class JSB_UmengNative
{
public:
	JSB_UmengNative()
	{
	}

	~JSB_UmengNative()
	{
	}

	void setJSDelegate(JS::HandleObject pJSDelegate)
	{
		_JSDelegate = pJSDelegate;
	}
protected:
public:

	JS::PersistentRootedObject _JSDelegate;
};


bool js_cocos2dx_umeng_beginLogPageView(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 1)
	{
		if (argv[0].isString())
		{
			std::string pageName;
			jsval_to_std_string(cx, argv[0], &pageName);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			umeng::MobClickCpp::beginLogPageView(pageName.c_str());
#endif
		}
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_endLogPageView(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 1)
	{
		if (argv[0].isString())
		{
			std::string pageName;
			jsval_to_std_string(cx, argv[0], &pageName);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			umeng::MobClickCpp::endLogPageView(pageName.c_str());
#endif
		}
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_payForGold(JSContext *cx, uint32_t argc, JS::Value *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 2)
	{
		double cash = 0;
		int32_t coin = 0;
		if (argv[0].isNumber())
		{
			cash = argv[0].toDouble();
		}
		if (argv[1].isNumber())
		{
			coin = argv[1].toInt32();
		}
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		umeng::MobClickCpp::pay(cash, 0, coin);
#endif
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}
bool js_cocos2dx_umeng_payForGoods(JSContext *cx, uint32_t argc, JS::Value *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 4)
	{
		double cash = 0;
		string goodsId = "";
		int32_t amount = 0;
		int32_t coin = 0;
		if (argv[0].isNumber())
		{
			cash = argv[0].toDouble();
		}
		if (argv[1].isString())
		{
			jsval_to_std_string(cx, argv[1], &goodsId);
		}
		if (argv[2].isNumber())
		{
			amount = argv[2].toInt32();
		}
		if (argv[3].isNumber())
		{
			coin = argv[3].toInt32();
		}
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		umeng::MobClickCpp::pay(cash, 0,goodsId.c_str(),amount,coin);
#endif
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_buyGoods(JSContext *cx, uint32_t argc, JS::Value *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 3)
	{
		string goodsId = "";
		int32_t amount = 0;
		int32_t coin = 0;
		
		if (argv[0].isString())
		{
			jsval_to_std_string(cx, argv[0], &goodsId);
		}
		if (argv[1].isNumber())
		{
			amount = argv[1].toInt32();
		}
		if (argv[2].isNumber())
		{
			coin = argv[2].toInt32();
		}
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		umeng::MobClickCpp::buy(goodsId.c_str(), amount, coin);
#endif
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_constructor(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("jsbindings: constructor JS object (UmnegNative)");
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_umeng_prototype);
	JS::RootedObject obj(cx, JS_NewObjectWithGivenProto(cx, js_cocos2dx_umeng_class, proto));

	args.rval().set(JS::ObjectOrNullValue(obj));
	return true;
}


void js_cocos2dx_umeng_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (UmnegNative)", obj);

}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "umengJniHelper.h"
#endif

void register_jsb_umeng_native(JSContext* cx, JS::HandleObject global)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	umengJniHelper::setJavaVM(JniHelper::getJavaVM());
#endif

	static const JSClassOps umengnative_classOps = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		js_cocos2dx_umeng_finalize,
		nullptr, nullptr, nullptr, nullptr
	};
	static JSClass umengnative_class = {
		"GameLogicNative",
		JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
		&umengnative_classOps
	};

	js_cocos2dx_umeng_class = &umengnative_class;

	static JSPropertySpec properties[] = {
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("beginLogPageView", js_cocos2dx_umeng_beginLogPageView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("endLogPageView", js_cocos2dx_umeng_endLogPageView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("payForGold", js_cocos2dx_umeng_payForGold, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("payForGoods", js_cocos2dx_umeng_payForGoods, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("buyGoods", js_cocos2dx_umeng_buyGoods, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),

		JS_FS_END
	};

	JS::RootedObject parent_proto(cx, nullptr);
	js_cocos2dx_umeng_prototype = JS_InitClass(
		cx, global,
		parent_proto,
		js_cocos2dx_umeng_class,
		js_cocos2dx_umeng_constructor, 0, // constructor
		properties,
		funcs,
		nullptr, // no static properties
		nullptr);


	//js_cocos2dx_umeng_class = (JSClass *)calloc(1, sizeof(JSClass));
	//js_cocos2dx_umeng_class->name = "UmengNative";
	//js_cocos2dx_umeng_class->addProperty = JS_PropertyStub;
	//js_cocos2dx_umeng_class->delProperty = JS_DeletePropertyStub;
	//js_cocos2dx_umeng_class->getProperty = JS_PropertyStub;
	//js_cocos2dx_umeng_class->setProperty = JS_StrictPropertyStub;
	//js_cocos2dx_umeng_class->enumerate = JS_EnumerateStub;
	//js_cocos2dx_umeng_class->resolve = JS_ResolveStub;
	//js_cocos2dx_umeng_class->convert = JS_ConvertStub;
	//js_cocos2dx_umeng_class->finalize = js_cocos2dx_umeng_finalize;
	//js_cocos2dx_umeng_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	//static JSPropertySpec properties[] = {
	//	JS_PS_END
	//};
	///*static void beginLogPageView(const char *pageName);
	//static void endLogPageView(const char *pageName);*/
	//static JSFunctionSpec funcs[] = {
	//	JS_FN("beginLogPageView", js_cocos2dx_umeng_beginLogPageView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	JS_FN("endLogPageView", js_cocos2dx_umeng_endLogPageView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	JS_FN("payForGold", js_cocos2dx_umeng_payForGold, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	JS_FN("payForGoods", js_cocos2dx_umeng_payForGoods, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	JS_FN("buyGoods", js_cocos2dx_umeng_buyGoods, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	//	JS_FS_END
	//};

	//static JSFunctionSpec st_funcs[] = {
	//	JS_FS_END
	//};

	//js_cocos2dx_umeng_prototype = JS_InitClass(
	//	cx, global,
	//	JS::NullPtr(),
	//	js_cocos2dx_umeng_class,
	//	js_cocos2dx_umeng_constructor, 0, // constructor
	//	properties,
	//	funcs,
	//	NULL, // no static properties
	//	st_funcs);
}
