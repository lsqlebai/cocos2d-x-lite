#include "ProxyDownloader.h"

#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_auto.hpp"
#include "cocos2d.h"
#include "platform/CCFileUtils.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"
extern "C" {
#include "md5.h"
}
#include "DownloaderManager.h"
USING_NS_CC;

JSDownloaderDelegatorEx::JSDownloaderDelegatorEx(
	JSContext *cx, 
	JS::HandleObject obj, 
	const std::string &url, 
	const std::string& proxy, 
	JS::HandleObject callback)
	: __JSDownloaderDelegator(cx, obj, url, callback)
	, _proxy(proxy)
	, _jsProgressCallBack(nullptr)
{

}

void JSDownloaderDelegatorEx::setProgressCallback(JS::HandleObject callback) {
	_jsProgressCallBack = callback;

	JS::RootedValue target(_cx, OBJECT_TO_JSVAL(_jsProgressCallBack));
	if (!target.isNullOrUndefined())
	{
		js_add_object_root(target);
	}
}

JSDownloaderDelegatorEx::~JSDownloaderDelegatorEx()
{
	JS::RootedValue target(_cx, OBJECT_TO_JSVAL(_jsProgressCallBack));
	if (!target.isNullOrUndefined())
	{
		js_remove_object_root(target);
	}
	if (_downloader != nullptr) {
		_downloader->onTaskProgress = (nullptr);
		_downloader->onFileTaskSuccess = (nullptr);
	}
}
#define MD5_BUFFER_LENGTH 16
void MD5(void* input, int inputLength, unsigned char* output)
{
	MD5_CTX ctx;
	MD5_Init_(&ctx);
	MD5_Update_(&ctx, input, inputLength);
	MD5_Final_(output, &ctx);
}

std::string dumpHex(unsigned char* bin, int binLength)
{
	static const char* hextable = "0123456789abcdef";
	//const int strLength = binLength * 2 + 1;
	std::string result;
	char str[100];
	memset(str, 0, 100);
	int ci = 0;
	for (int i = 0; i < binLength; ++i)
	{
		unsigned char c = bin[i];
		str[ci++] = hextable[(c >> 4) & 0x0f];
		str[ci++] = hextable[c & 0x0f];
	}
	result = str;
	return result;
}

std::string getStringUseMD5(std::string input)
{
	unsigned char buffer[MD5_BUFFER_LENGTH];
	MD5((void*)input.c_str(), input.length(), buffer);
	std::string str = dumpHex(buffer, MD5_BUFFER_LENGTH);
	return str;
}

//test code
std::string getSuffix(std::string& url) {
	std::string suffix;
	int index = url.find_last_of(".");

	if (index >= 0) {
		suffix = url.substr(index);
	}
	return suffix;
}

std::string JSDownloaderDelegatorEx::getDownloadPath(std::string& url) 
{
	return FileUtils::getInstance()->getWritablePath() + getStringUseMD5(url) + getSuffix(url);
}

void JSDownloaderDelegatorEx::onSuccess(const std::string& path)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, path]
	{
		JS::RootedObject global(_cx, ScriptingCore::getInstance()->getGlobalObject());
		JSAutoCompartment ac(_cx, global);

		jsval valArr[2];

		valArr[0] = BOOLEAN_TO_JSVAL(true);
		valArr[1] = std_string_to_jsval(_cx, path);

		JS::RootedValue callback(_cx, OBJECT_TO_JSVAL(_jsCallback));
		if (!callback.isNull())
		{
			JS::RootedValue retval(_cx);
			JS_CallFunctionValue(_cx, global, callback, JS::HandleValueArray::fromMarkedLocation(2, valArr), &retval);
		}
		release();
	});
}


void JSDownloaderDelegatorEx::onProgress(int64_t totalBytesReceived, int64_t totalBytesExpected)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, totalBytesReceived, totalBytesExpected]
	{
		JS::RootedObject global(_cx, ScriptingCore::getInstance()->getGlobalObject());
		JSAutoCompartment ac(_cx, global);

		jsval valArr[2];

		valArr[0] = INT_TO_JSVAL((int32_t)totalBytesReceived);
		valArr[1] = INT_TO_JSVAL((int32_t)totalBytesExpected);

		JS::RootedValue callback(_cx, OBJECT_TO_JSVAL(this->_jsProgressCallBack));
		if (!callback.isNullOrUndefined())
		{
			JS::RootedValue retval(_cx);
			JS_CallFunctionValue(_cx, global, callback, JS::HandleValueArray::fromMarkedLocation(2, valArr), &retval);
		}
	});
}

void JSDownloaderDelegatorEx::startDownload() {
	std::string downloadPath = getDownloadPath(_url);

	if (FileUtils::getInstance()->isFileExist(downloadPath)) {
		onSuccess(downloadPath);
	}
	
	else {
		if (!DownloaderManager::getInstance()->addTask(_url, this)) {
			return;
		}
		_downloader = std::make_shared<cocos2d::network::Downloader>();
		
		_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
			int errorCode,
			int errorCodeInternal,
			const std::string& errorStr)
		{
			CCLOG("JSDownloaderDelegatorEx:download: %s fails: %s", _url.c_str() , errorStr.c_str());
			FileUtils::getInstance()->removeFile(task.storagePath);
			this->onError();
			DownloaderManager::getInstance()->allOnError(_url);
		};

		_downloader->onTaskProgress = [this](const cocos2d::network::DownloadTask& task,
			int64_t bytesReceived,
			int64_t totalBytesReceived,
			int64_t totalBytesExpected)
		{
			if (this->_jsProgressCallBack != nullptr)
				this->onProgress(totalBytesReceived, totalBytesExpected);

		};
		_downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
		{

			if (task.storagePath.empty())
			{
				this->onError();
				DownloaderManager::getInstance()->allOnError(_url);
			}
			else
			{
				this->onSuccess(task.storagePath);
				DownloaderManager::getInstance()->allOnSuccess(_url, task.storagePath);

			}
		};
		
		_downloader->createDownloadFileTask(_url, downloadPath, "", _proxy);
	}
}



// jsb.downloadFile(url, proxy, function(succeed, result) {}, fuction(totalBytesReceived, totalBytesExpected) {})
bool js_download(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
	if (argc != 3 && argc != 4) {
		JS_ReportError(cx, "js_download : wrong number of arguments");
		return false;
	}

	std::string url;
	bool ok = jsval_to_std_string(cx, args.get(0), &url);
	JSB_PRECONDITION2(ok, cx, false, "js_download : Error processing arguments");

	std::string proxy;
	ok = jsval_to_std_string(cx, args.get(1), &proxy);
	JSB_PRECONDITION2(ok, cx, false, "js_download : Error processing arguments");

	JS::RootedObject callback(cx, args.get(2).toObjectOrNull());
	JSDownloaderDelegatorEx* delegate;
	delegate = new JSDownloaderDelegatorEx(cx, obj, url, proxy, callback);
	if (argc == 4) {
		JS::RootedObject progressCallback(cx, args.get(3).toObjectOrNull());
		delegate->setProgressCallback(progressCallback);
	}

	delegate->autorelease();
	delegate->downloadAsync();
	args.rval().setUndefined();
	return true;
}