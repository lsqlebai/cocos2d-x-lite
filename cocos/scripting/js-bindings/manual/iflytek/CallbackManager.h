#pragma once
#include<map>
#include<string>

#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"

class callbackBundle {
public:
	JS::RootedObject* object;
	std::string functionName;
	callbackBundle() {};
	callbackBundle(JS::RootedObject* obj, std::string func) : object(obj), functionName(func) {};

};

class CallbackManager
{
public:
	CallbackManager();
	~CallbackManager();
	static CallbackManager* getInstance();
	std::map<std::string, callbackBundle> callbackMap;
	void addListener(std::string domain, JS::RootedObject* object, std::string functionName);
	bool callJS(std::string domain, std::string arg);
	void registerCallBack();


};