#include "PayListener.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include"scripting/js-bindings/manual/iflytek/CallbackManager.h"


class PayListener
{
public:
	PayListener() {};

};

static bool js_is_native_obj(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
JSClass  *jsb_PayListener_class;
JSObject *jsb_PayListener_prototype;

bool js_PayListener_addListener(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	if (argc == 3) {
		JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
		std::string type;
		bool ok = jsval_to_std_string(cx, args.get(0), &type);
		JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

		JS::RootedObject* obj = new JS::RootedObject(cx, args.get(1).toObjectOrNull());
		std::string callbackName;

		do {
			bool ok = jsval_to_std_string(cx, args.get(2), &callbackName);
			JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
		} while (0);
		CallbackManager::getInstance()->addListener(type, obj, callbackName);
		return true;
	}
	JS_ReportErrorUTF8(cx, "Wrong number of arguments");
	return false;
}


void js_register_PayListener(JSContext *cx, JS::HandleObject global) {


	static const JSClassOps PayListener_classOps = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		nullptr,
		nullptr, nullptr, nullptr, nullptr
	};

	static JSClass PayListener_class = {
		"PayListener",
		JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
		&PayListener_classOps
	};

	jsb_PayListener_class = &PayListener_class;

	static JSPropertySpec properties[] = {
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("addListener", js_PayListener_addListener, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	JS::RootedObject parent_proto(cx, nullptr);
	jsb_PayListener_prototype = JS_InitClass(
		cx, global,
		parent_proto,
		jsb_PayListener_class,
		dummy_constructor<PayListener>, 0, // constructor
		properties,
		funcs,
		nullptr, // no static properties
		st_funcs);


    //jsb_PayListener_class = (JSClass *)calloc(1, sizeof(JSClass));
    //jsb_PayListener_class->name = "PayListener";
    //jsb_PayListener_class->addProperty = JS_PropertyStub;
    //jsb_PayListener_class->delProperty = JS_DeletePropertyStub;
    //jsb_PayListener_class->getProperty = JS_PropertyStub;
    //jsb_PayListener_class->setProperty = JS_StrictPropertyStub;
    //jsb_PayListener_class->enumerate = JS_EnumerateStub;
    //jsb_PayListener_class->resolve = JS_ResolveStub;
    //jsb_PayListener_class->convert = JS_ConvertStub;
    //jsb_PayListener_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    //static JSPropertySpec properties[] = {
    //    JS_PS_END
    //};

    //static JSFunctionSpec funcs[] = {
    //    JS_FS_END
    //};

    //static JSFunctionSpec st_funcs[] = {
    //    JS_FN("addListener", js_PayListener_addListener, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
    //    JS_FS_END
    //};

    //jsb_PayListener_prototype = JS_InitClass(
    //    cx, global,
    //    JS::NullPtr(),
    //    jsb_PayListener_class,
    //    dummy_constructor<PayListener>, 0, // no constructor
    //    properties,
    //    funcs,
    //    NULL, // no static properties
    //    st_funcs);

    //JS::RootedObject proto(cx, jsb_PayListener_prototype);
    //JS::RootedValue className(cx, std_string_to_jsval(cx, "PayListener"));
    //JS_SetProperty(cx, proto, "_className", className);
    //JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    //JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    //// add the proto and JSClass to the type->js info hash table
    //jsb_register_class<PayListener>(cx, jsb_PayListener_class, proto, JS::NullPtr());
}

void register_all_pay_listener(JSContext* cx, JS::HandleObject obj) {
    // Get the ns
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "cc", &ns);

	js_register_PayListener(cx, ns);
}

#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

