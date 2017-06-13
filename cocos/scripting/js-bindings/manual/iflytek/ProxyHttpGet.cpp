#include "ProxyHttpGet.h"

#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_auto.hpp"
#include "cocos2d.h"
#include "platform/CCFileUtils.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"

USING_NS_CC;

ProxyHttpGet::ProxyHttpGet(
	JSContext *cx,
	JS::HandleObject obj,
	const std::string &url,
	const std::string& proxy,
	JS::HandleObject callback)
	: __JSDownloaderDelegator(cx, obj, url, callback)
	, _proxy(proxy)
{

}

void ProxyHttpGet::onSuccess(const std::string& response)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, response]
	{
		JS::RootedObject global(_cx, ScriptingCore::getInstance()->getGlobalObject());
		JSAutoCompartment ac(_cx, global);

		jsval valArr[2];

		valArr[0] = BOOLEAN_TO_JSVAL(true);
		valArr[1] = std_string_to_jsval(_cx, response);

		JS::RootedValue callback(_cx, OBJECT_TO_JSVAL(_jsCallback));
		if (!callback.isNull())
		{
			JS::RootedValue retval(_cx);
			JS_CallFunctionValue(_cx, global, callback, JS::HandleValueArray::fromMarkedLocation(2, valArr), &retval);
		}
		release();
	});
}


void ProxyHttpGet::startDownload()
{
	_downloader = std::make_shared<cocos2d::network::Downloader>();
	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		CCLOG("JSDownloaderDelegatorEx:download: %s fails: %s", _url.c_str(), errorStr.c_str());
		FileUtils::getInstance()->removeFile(task.storagePath);
		this->onError();

	};

	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task, std::vector<unsigned char>& data)
	{
		const char *p = (char *)data.data();
		std::string char_data(p, data.size());

		this->onSuccess(char_data.c_str());

	};
	_downloader->createDownloadDataTask(_url, "", _proxy);

}

// jsb.downloadFile(url, proxy, function(succeed, result) {}, fuction(totalBytesReceived, totalBytesExpected) {})
bool proxy_http_get(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
	if (argc != 3) {
		JS_ReportError(cx, "proxy_http_get : wrong number of arguments");
		return false;
	}

	std::string url;
	bool ok = jsval_to_std_string(cx, args.get(0), &url);
	JSB_PRECONDITION2(ok, cx, false, "js_download : Error processing arguments");

	std::string proxy;
	ok = jsval_to_std_string(cx, args.get(1), &proxy);
	JSB_PRECONDITION2(ok, cx, false, "js_download : Error processing arguments");

	JS::RootedObject callback(cx, args.get(2).toObjectOrNull());
	ProxyHttpGet* delegate;
	delegate = new ProxyHttpGet(cx, obj, url, proxy, callback);

	delegate->autorelease();
	delegate->downloadAsync();
	args.rval().setUndefined();
	return true;
}