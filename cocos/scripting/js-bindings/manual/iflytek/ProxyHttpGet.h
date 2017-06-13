#pragma once
#ifndef  __PROXY_HTPP_GET_H__
#define  __PROXY_HTPP_GET_H__
#include "jsapi.h"
#include "scripting/js-bindings/manual/extension/jsb_cocos2dx_extension_manual.h"


class ProxyHttpGet : public __JSDownloaderDelegator {
public:
	ProxyHttpGet(JSContext *cx, JS::HandleObject obj, const std::string &url, const std::string& proxy, JS::HandleObject callback);

private:
	std::string _proxy;

protected:
	virtual void startDownload();
	void onSuccess(const std::string& response);
	

};

bool proxy_http_get(JSContext *cx, uint32_t argc, jsval *vp);
#endif