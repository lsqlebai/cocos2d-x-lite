#include "CallbackManager.h"

#include "scripting/js-bindings/manual/js_manual_conversions.h"

CallbackManager::CallbackManager()
{
}

bool CallbackManager::callJS(std::string domain, std::string arg) {

	if (callbackMap.find(domain) == callbackMap.end())
		return false;
	else {
		auto callback = callbackMap[domain];

		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		JS::RootedValue ret(cx);
		std_string_to_jsval(cx, arg, &ret);

		JS::HandleValueArray args(ret);

		JS::RootedValue owner(cx, JS::ObjectOrNullValue(*(callback.object)));

		return ScriptingCore::getInstance()->executeFunctionWithOwner(owner, callback.functionName.c_str(), args);
	}

}

void CallbackManager::addListener(std::string domain, JS::RootedObject* object, std::string functionName) {

	callbackMap[domain] = callbackBundle(object, functionName);
}

CallbackManager* CallbackManager::getInstance()
{
	static CallbackManager* instance = nullptr;
	if (instance == nullptr)
		instance = new (std::nothrow) CallbackManager();

	return instance;
}

void CallbackManager::registerCallBack() {

}
CallbackManager::~CallbackManager()
{
}
