#include "jsb_GameLogic.h"

#include "GameLogic.h"

#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"

class JSB_GameLogic
{
public:
	JSB_GameLogic()
	{
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		_JSDelegate.construct(cx);
	}

	~JSB_GameLogic()
	{
		_JSDelegate.destroyIfConstructed();
	}

	void setJSDelegate(JS::HandleObject pJSDelegate)
	{
		_JSDelegate.ref() = pJSDelegate;
	}
protected:
private:

	mozilla::Maybe<JS::PersistentRootedObject> _JSDelegate;
};

JSClass  *js_cocos2dx_gamelogic_class;
JSObject *js_cocos2dx_gamelogic_prototype;


#include "cocos2d.h"
USING_NS_CC;


bool js_cocos2dx_gamelogic_addNode(JSContext *cx, uint32_t argc, jsval *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	CCLOG("gamelogic addObj argc:%d", argc);


	bool isObj = argv[0].isObject();
	

	JSObject* objj = argv[0].toObjectOrNull();
	JS::RootedObject jsobj1(cx, argv[0].toObjectOrNull());

	js_proxy_t *proxy1 = jsb_get_js_proxy(jsobj1);
	Node* cobj1 = (Node*)(proxy1 ? proxy1->ptr : NULL);
	cocos2d::Vector<Texture2D*> textures = TextureCache::getInstance()->getAllTextures();
	if (cobj1)
	{

		JSObject* objFood = argv[1].toObjectOrNull();
		JS::RootedObject jsobjFood(cx, objFood);

		js_proxy_t *proxyFood = jsb_get_js_proxy(jsobjFood);

		auto* cobj2Food = (SpriteFrame*)(proxyFood ? proxyFood->ptr : NULL);

		int count = argv[2].toInt32();

		bool isAddScreen = argv[3].toBoolean();

		Size size = Director::getInstance()->getVisibleSize();
		for (int i = 0; i < count; ++i)
		{
			auto sp = Sprite::createWithSpriteFrame(cobj2Food);
			sp->setAnchorPoint(Vec2(0, 0));
			if (isAddScreen)
			{
				sp->setPosition(CCRANDOM_MINUS1_1() * size.width /2, CCRANDOM_MINUS1_1() * size.height /2);
			}
			else
			{
				sp->setPosition((CCRANDOM_MINUS1_1() + 10) * size.width, CCRANDOM_MINUS1_1() * size.height / 2);
			}

			cobj1->addChild(sp);
		}

	}

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().setUndefined();
	return true;
}

#include <math.h>

bool js_cocos2dx_gamelogic_removeNode(JSContext *cx, uint32_t argc, jsval *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	CCLOG("gamelogic addObj argc:%d", argc);


	bool isObj = argv[0].isObject();


	JSObject* objj = argv[0].toObjectOrNull();
	JS::RootedObject jsobj1(cx, argv[0].toObjectOrNull());

	js_proxy_t *proxy1 = jsb_get_js_proxy(jsobj1);
	Node* cobj1 = (Node*)(proxy1 ? proxy1->ptr : NULL);
	cocos2d::Vector<Texture2D*> textures = TextureCache::getInstance()->getAllTextures();
	if (cobj1)
	{

		int count = argv[1].toInt32();
		
		int minCount = count < cobj1->getChildrenCount() ? count : cobj1->getChildrenCount();

		auto children = cobj1->getChildren();
		for (int i = minCount - 1; i >= 0; --i)
		{
			Node* obj = children.at(i);
			obj->removeFromParent();
		}

//		cobj1

	}

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_gamelogic_updateFoodArea(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	if (argc == 4)
	{

		int32_t visibleX = argv[0].toInt32();
		int32_t visibleY = argv[1].toInt32();
		int32_t visibleWidth = argv[2].toInt32();
		int32_t visibleHeight = argv[3].toInt32();

		cobj->updateFoodArea(visibleX, visibleY, visibleWidth, visibleHeight);
	}
	else
	{
		JS_ReportError(cx, "js_cocos2dx_gamelogic_updateFoodArea wrong params count");
		return false;
	}

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_gamelogic_removeFoodWithAnim(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	if (argc == 4)
	{

		std::string foodsStr;
		jsval_to_std_string(cx, argv[0], &foodsStr);

		Vector<FoodAreaObj*> foodAreas;
		bool parseOk = true;
		parseOk &= GameLogic::parseJsonToFoodAreas(foodsStr, foodAreas);

		if (!parseOk) // 解析数据失败，无需继续处理
		{
			CCLOGERROR("js_cocos2dx_gamelogic_removeFoodWithAnim foodAreas data error");
			return false;
		}

		float animDuration = argv[1].toDouble(); // 动画持续时间
		float targetX = argv[2].toDouble(); // 目标点x
		float targetY = argv[3].toDouble(); // 目标点y

		if (!foodAreas.empty())
		{
			cobj->removeFoodWithAnim(foodAreas, animDuration, targetX, targetY); // 处理食物变化
		}

	}
	else
	{
		JS_ReportError(cx, "js_cocos2dx_gamelogic_removeFoodWithAnim wrong params count");
		return false;
	}

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_gamelogic_addOrRemoveFood(JSContext *cx, uint32_t argc, jsval *vp)
{
	
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");
	

	if (argc == 2)
	{
		bool isAdd = argv[0].toBoolean(); // 是否是新增食物


		std::string foodsStr;
		jsval_to_std_string(cx, argv[1], &foodsStr);

		Vector<FoodAreaObj*> foodAreas;
		bool parseOk = true;
		parseOk &= GameLogic::parseJsonToFoodAreas(foodsStr, foodAreas);

		if (!parseOk) // 解析数据失败，无需继续处理
		{
			CCLOGERROR("js_cocos2dx_gamelogic_addOrRemoveFood foodAreas data error");
			return false;
		}
		if (!foodAreas.empty())
		{
			cobj->addOrRemoveFood(isAdd, foodAreas); // 处理食物变化
		}
		
	}
	else
	{
		JS_ReportError(cx, "js_cocos2dx_gamelogic_addOrRemoveFood wrong params count");
		return false;
	}

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_gamelogic_releaseFoodLayer(JSContext *cx, uint32_t argc, jsval *vp)
{

	CCLOG("gamelogic releaseFoodLayer");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic*)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	CCLOG("gamelogic releaseFoodLayer argc:%d", argc);


	// 释放食物图层
	cobj->releaseFoodLayer();

	if (cobj)
	{
		CC_SAFE_DELETE(cobj);
	}

	argv.rval().setUndefined();
	return true;
}

bool jsval_to_vector_string(JSContext* cx, JS::HandleValue v, vector<std::string>* ret)
{
	JS::RootedObject jsobj(cx);

	bool ok = v.isObject() && JS_ValueToObject(cx, v, &jsobj);
	JSB_PRECONDITION3(ok, cx, false, "Error converting value to object");
	JSB_PRECONDITION3(jsobj && JS_IsArrayObject(cx, jsobj), cx, false, "Object must be an array");

	uint32_t len = 0;
	JS_GetArrayLength(cx, jsobj, &len);

	for (uint32_t i = 0; i < len; i++)
	{
		JS::RootedValue value(cx);
		if (JS_GetElement(cx, jsobj, i, &value))
		{
			std::string data;
			jsval_to_std_string(cx, value, &data);
			ret->push_back(data);
		}
	}

	return true;
}

bool js_cocos2dx_gamelogic_initFoodLayer(JSContext *cx, uint32_t argc, jsval *vp)
{

	CCLOG("gamelogic initFoodLayer");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	GameLogic* cobj = (GameLogic*)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	CCLOG("gamelogic initFoodLayer argc:%d", argc);
	
	
	if (argc == 6)
	{
		
		JS::RootedObject foodLayerJObj(cx, argv[0].toObjectOrNull());
		js_proxy_t *proxyFoodLayer = jsb_get_js_proxy(foodLayerJObj);
		Node* foodLayer = (Node*)(proxyFoodLayer ? proxyFoodLayer->ptr : NULL); // 获取食物图层

		JSB_PRECONDITION2(foodLayer, cx, false, "js_cocos2dx_gamelogic_initFoodLayer : Error processing arguments, foodLayer error");
		
		
		JS::RootedObject foodAnimLayerJObj(cx, argv[1].toObjectOrNull());
		js_proxy_t *proxyFoodAnimLayer = jsb_get_js_proxy(foodAnimLayerJObj);
		Node* foodAnimLayer = (Node*)(proxyFoodAnimLayer ? proxyFoodAnimLayer->ptr : NULL); // 获取食物动画图层

		JSB_PRECONDITION2(foodAnimLayer, cx, false, "js_cocos2dx_gamelogic_initFoodLayer : Error processing arguments, foodAnimLayer error");

		bool ok = true;
		cocos2d::Vector<SpriteFrame*> foodSkins;
		ok &= jsval_to_ccvector(cx, argv.get(2), &foodSkins);
		JSB_PRECONDITION2(ok, cx, false, "js_cocos2dx_gamelogic_initFoodLayer : Error processing arguments, foodSkins error");


		std::string foodsStr;
		jsval_to_std_string(cx, argv[3], &foodsStr);

		Vector<FoodAreaObj*> foodAreas;
		bool parseOk = true;
		parseOk &= GameLogic::parseJsonToFoodAreas(foodsStr, foodAreas);
		
		if (!parseOk) // 解析数据失败，无需继续处理
		{
			CCLOGERROR("js_cocos2dx_gamelogic_initFoodLayer foodAreas data error");
			return false;
		}
		
		int32_t foodRadius = argv[4].toInt32(); // 食物初始化半径

		int32_t foodPreCount = argv[5].toInt32(); // 预制食物总数
		

		// 初始化食物图层
		cobj->initFoodLayer(foodLayer, foodAnimLayer, foodSkins, foodAreas, foodRadius, foodPreCount);
	}
	else
	{
		JS_ReportError(cx, "js_cocos2dx_gamelogic_initFoodLayer wrong params count");
		return false;
	}

	argv.rval().setUndefined();
	return true;
}

bool js_cocos2dx_gamelogic_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("jsbindings: constructor JS object (gamelogic)");
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_gamelogic_prototype);
	JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_gamelogic_class, proto, JS::NullPtr()));


	GameLogic* cobj = new GameLogic();

	JSB_GameLogic* delegate = new JSB_GameLogic();
	delegate->setJSDelegate(obj);
	//cobj->setRefPtr(delegate);
	js_proxy_t *p = jsb_new_proxy(cobj, obj);
	JS::AddNamedObjectRoot(cx, &p->obj, "GameLogicNative");



	args.rval().set(OBJECT_TO_JSVAL(obj));
	return true;
	//}
	//JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	//return false;

}

void js_cocos2dx_gamelogic_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (gamelogic)", obj);

	// 在js_cocos2dx_gamelogic_releaseFoodLayer中释放
	//JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	//JS::RootedObject jsObj(cx, obj);
	//js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
	//GameLogic* cobj = (GameLogic *)(proxy ? proxy->ptr : NULL);
	//if (cobj)
	//{
	//	/*auto ref = cobj->getRefPtr();
	//	CC_SAFE_DELETE(ref);*/
	//	CC_SAFE_DELETE(cobj);
	//}
}

void register_jsb_game_logic_native(JSContext* cx, JS::HandleObject global)
{
	js_cocos2dx_gamelogic_class = (JSClass *)calloc(1, sizeof(JSClass));
	js_cocos2dx_gamelogic_class->name = "GameLogicNative";
	js_cocos2dx_gamelogic_class->addProperty = JS_PropertyStub;
	js_cocos2dx_gamelogic_class->delProperty = JS_DeletePropertyStub;
	js_cocos2dx_gamelogic_class->getProperty = JS_PropertyStub;
	js_cocos2dx_gamelogic_class->setProperty = JS_StrictPropertyStub;
	js_cocos2dx_gamelogic_class->enumerate = JS_EnumerateStub;
	js_cocos2dx_gamelogic_class->resolve = JS_ResolveStub;
	js_cocos2dx_gamelogic_class->convert = JS_ConvertStub;
	js_cocos2dx_gamelogic_class->finalize = js_cocos2dx_gamelogic_finalize;
	js_cocos2dx_gamelogic_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		// JS_PSG("readyState", js_cocos2dx_extension_WebSocket_get_readyState, JSPROP_ENUMERATE | JSPROP_PERMANENT),
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("initFoodLayer", js_cocos2dx_gamelogic_initFoodLayer, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("releaseFoodLayer", js_cocos2dx_gamelogic_releaseFoodLayer, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("addNode", js_cocos2dx_gamelogic_addNode, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("removeNode", js_cocos2dx_gamelogic_removeNode, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("addOrRemoveFood", js_cocos2dx_gamelogic_addOrRemoveFood, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("removeFoodWithAnim", js_cocos2dx_gamelogic_removeFoodWithAnim, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("updateFoodArea", js_cocos2dx_gamelogic_updateFoodArea, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FS_END
	};

	js_cocos2dx_gamelogic_prototype = JS_InitClass(
		cx, global,
		JS::NullPtr(),
		js_cocos2dx_gamelogic_class,
		js_cocos2dx_gamelogic_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
}
