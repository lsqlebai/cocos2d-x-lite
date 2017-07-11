#pragma once
#ifndef __DOWNLOAD_MANAGER_H__
#define __DOWNLOAD_MANAGER_H__
#include<string>
#include<set>
#include <map>
#include <vector>
#include "ProxyDownloader.h"

class DownloaderManager
{
public:
	DownloaderManager();
	~DownloaderManager();
	static DownloaderManager* getInstance();
	bool addTask(const std::string& url, JSDownloaderDelegatorEx* downloaderDelegator);
	void allOnError(const std::string& url);
	void allOnSuccess(const std::string& url, const std::string& path);

private:
	std::set<std::string> _downloadingUrls;
	std::map<std::string, std::vector<JSDownloaderDelegatorEx*>> _waitingMap;
	std::vector<JSDownloaderDelegatorEx*> getWaitingList(const std::string& url);
	bool isDownloading(const std::string& url);
	

};

bool js_set_global_proxy(JSContext *cx, uint32_t argc, jsval *vp);

bool js_set_hot_update_url(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __DOWNLOAD_MANAGER_H__
