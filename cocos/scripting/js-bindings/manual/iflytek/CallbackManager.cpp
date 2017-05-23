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
		auto value = std_string_to_jsval(ScriptingCore::getInstance()->getGlobalContext(), arg);
		return ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(*(callback.object)), callback.functionName.c_str(), 1, &value);
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
