#include"JS_Binder.h"
#include"scripting/js-bindings/manual/iflytek/CallbackManager.h"
#include"cocos2d.h"
using namespace cocos2d;


bool JSBinder::callJS(std::string domain, std::string arg) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
		CallbackManager::getInstance()->callJS(domain, arg); 
	});
	return true;
}

