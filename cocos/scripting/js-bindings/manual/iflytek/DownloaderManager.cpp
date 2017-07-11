#include "DownloaderManager.h"

#include "cocos2d.h"
#include "extensions/assets-manager/AssetsManagerEx.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_auto.hpp"
USING_NS_CC;

DownloaderManager::DownloaderManager(){

}

bool DownloaderManager::isDownloading(const std::string& url) {
	auto iter = this->_downloadingUrls.find(url);
	return iter != this->_downloadingUrls.end();

}

void DownloaderManager::allOnError(const std::string& url) {

	std::vector<JSDownloaderDelegatorEx*> waitingList = getWaitingList(url);
	std::vector<JSDownloaderDelegatorEx*>::iterator iter;
	for (iter = waitingList.begin(); iter != waitingList.end(); iter++)
	{
		(*iter)->onError();
	}
	this->_downloadingUrls.erase(url);
}

void DownloaderManager::allOnSuccess(const std::string& url, const std::string& path) {
	
	std::vector<JSDownloaderDelegatorEx*> waitingList = getWaitingList(url);
	std::vector<JSDownloaderDelegatorEx*>::iterator iter;
	for (iter = waitingList.begin(); iter != waitingList.end(); iter++)
	{
		(*iter)->onSuccess(path);
	}
	this->_downloadingUrls.erase(url);
}


bool DownloaderManager::addTask(const std::string& url, JSDownloaderDelegatorEx* downloaderDelegator) {
	if (isDownloading(url)) {
		
		std::vector<JSDownloaderDelegatorEx*> delegators;

		auto itor = this->_waitingMap.find(url);
		if (itor != this->_waitingMap.end()) {
			delegators = itor->second;
		}
		delegators.push_back(downloaderDelegator);
		CCLOG("DownloaderManager:: %s mutidownloading : %d", url.c_str(), delegators.size());
		this->_waitingMap.insert(make_pair(url, delegators));
		return false;
	}
	this->_downloadingUrls.insert(url);
	return true;
}

std::vector<JSDownloaderDelegatorEx*> DownloaderManager::getWaitingList(const std::string& url) {
	std::vector<JSDownloaderDelegatorEx*> delegators;
	auto itor = this->_waitingMap.find(url);
	if (itor != this->_waitingMap.end()) {
		delegators = itor->second;
	}
	this->_waitingMap.erase(url);
	return delegators;
}

DownloaderManager* DownloaderManager::getInstance()
{
	static DownloaderManager* instance = nullptr;
	if (instance == nullptr)
		instance = new (std::nothrow) DownloaderManager();

	return instance;
}


