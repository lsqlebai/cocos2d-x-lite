#pragma once
#ifndef  __PROXY_DOWNLOADER_H__
#define  __PROXY_DOWNLOADER_H__
#include "jsapi.h"
#include "scripting/js-bindings/manual/extension/jsb_cocos2dx_extension_manual.h"


class JSDownloaderDelegatorEx : public __JSDownloaderDelegator {
public:
	JSDownloaderDelegatorEx(JSContext *cx, JS::HandleObject obj, const std::string &url, const std::string& proxy, JS::HandleObject callback);
	void setProgressCallback(JS::HandleObject callback);
	virtual ~JSDownloaderDelegatorEx();
private:
	std::string _proxy;
	std::string getDownloadPath(std::string& url);
	JS::Heap<JSObject*> _jsProgressCallBack;
protected:
	virtual void startDownload();
	void onSuccess(const std::string& path);
	void onProgress(int64_t totalBytesReceived, int64_t totalBytesExpected);
	friend class DownloaderManager;
};

bool js_download(JSContext *cx, uint32_t argc, jsval *vp);
#endif