#include"JS_Binder.h"
#include"scripting/js-bindings/manual/iflytek/CallbackManager.h"
#include"cocos2d.h"
using namespace cocos2d;
void runOnCocosThread(const std::function<void()>& fun)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread(fun);
}

bool JSBinder::callJS(std::string domain, std::string arg) {
	runOnCocosThread( [=]() {
		CallbackManager::getInstance()->callJS(domain, arg); 
	});
	return true;
}

