#pragma once
#ifndef  __PROXY_DOWNLOADER_H__
#define  __PROXY_DOWNLOADER_H__
#include "jsapi.h"
#include "scripting/js-bindings/manual/extension/jsb_cocos2dx_extension_manual.h"


class JSDownloaderDelegatorEx : public __JSDownloaderDelegator {
public:
	JSDownloaderDelegatorEx(JSContext *cx, JS::HandleObject obj, const std::string &url, const std::string& proxy, JS::HandleObject callback, const bool download_data);
private:
	std::string _proxy;
	bool _downloadData;
	std::string getDownloadPath(std::string& url);
	void startDownloadFile();
	void startDownloadData();
protected:
	virtual void startDownload();
	void onSuccess(const std::string& path);
	
	friend class DownloaderManager;
};

bool js_download(JSContext *cx, uint32_t argc, jsval *vp);
bool js_downloadData(JSContext *cx, uint32_t argc, jsval *vp);
#endif