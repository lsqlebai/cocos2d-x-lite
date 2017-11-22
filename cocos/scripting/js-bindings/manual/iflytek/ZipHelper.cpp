//
//  ZipHelper.cpp
//
//  Created by SULEI on 17/2/20.
//  Copyright © 2017年 SULEI. All rights reserved.
//

#include "ZipHelper.h"

#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"

#include "stdio.h"
#include "sys/stat.h"
#include "unzip/unzip.h"

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

/************************************************************************/
/* zip解压辅助类                                                                     */
/************************************************************************/
class ZipHelper {

public:

	ZipHelper() :_JSDelegate(nullptr){}

	typedef std::function<void(bool, string, string)> UnzipCallback;

	/**
	 * 异步解压
	 */
	static void unzipFileAsyn(const string& inputFilePath, const string& outFilePathOrDirPath, const string& password, UnzipCallback callback)
	{
		std::thread t([=]()
		{
			bool result = unzipFile(inputFilePath, outFilePathOrDirPath, password);
			callback(result, inputFilePath, outFilePathOrDirPath);
		});
		t.detach();
	}

	/**
	 *  同步解压
	 */

	static bool unzipFile(const string& inputFilePath, const string& outFilePathOrDirPath, const string& password = "")
	{
		auto unzFile = cocos2d::unzOpen(inputFilePath.c_str());

		if (!unzFile)
		{
			return false;
		}

		if (outFilePathOrDirPath.empty())
		{
			return false;
		}

		bool isOutDirectory = false;
		if (outFilePathOrDirPath.at(outFilePathOrDirPath.length() - 1) == '/' || 
			outFilePathOrDirPath.at(outFilePathOrDirPath.length() - 1) == '\\')
		{
			isOutDirectory = true;
		}

		if (isOutDirectory)
		{
			bool createOutDirResult = FileUtils::getInstance()->createDirectory(outFilePathOrDirPath); // 创建输出目录
			if (!createOutDirResult)
			{
				return false;
			}
		}

		
		unz_global_info globalInfo = { 0 };
		if (unzFile) {

			if (unzGetGlobalInfo(unzFile, &globalInfo) != UNZ_OK)
			{
				cocos2d::unzClose(unzFile);
				return false;
			}
		}

		bool success = true;
		int ret = unzGoToFirstFile(unzFile);


		unsigned char buffer[4096] = { 0 };
		FileUtils* fman = FileUtils::getInstance();
		if (ret == UNZ_OK) {

			do {
				if (password.size() == 0)
					ret = unzOpenCurrentFile(unzFile);
				else
					ret = unzOpenCurrentFilePassword(unzFile, password.c_str());

				if (ret != UNZ_OK) {

					success = false;
					break;
				}

				// 开始解压数据
				int read;
				unz_file_info fileInfo = { 0 };
				ret = unzGetCurrentFileInfo(unzFile, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0);
				if (ret != UNZ_OK) {
					success = false;
				}

				char* filename = (char*)malloc(fileInfo.size_filename + 1);
				unzGetCurrentFileInfo(unzFile, &fileInfo, filename, fileInfo.size_filename + 1, nullptr, 0, nullptr, 0);
				filename[fileInfo.size_filename] = '\0';

				bool isDirectory = false;
				if (filename[fileInfo.size_filename - 1] == '/' || filename[fileInfo.size_filename - 1] == '\\')
					isDirectory = true;

				std::string fullPath;
				if (isOutDirectory)
				{
					fullPath = outFilePathOrDirPath + StringUtils::format("/%s", filename);
				}
				else
				{
					fullPath = outFilePathOrDirPath;
				}

				free(filename);

				if (isDirectory)
				{
					bool createResult = FileUtils::getInstance()->createDirectory(fullPath); // 创建目录
					if (!createResult)
					{
						success = false;
						break;
					}
				}

				FILE* fp = fopen(fullPath.c_str(), "wb");
				while (fp) {
					read = unzReadCurrentFile(unzFile, buffer, 4096);
					if (read > 0) {
						fwrite(buffer, read, 1, fp);
					}
					else if (read < 0)
					{
						break;
					}
					else
					{
						break;
					}
				}

				if (fp) {
					fclose(fp);
				}

				ret = unzGoToNextFile(unzFile);

			} while (ret == UNZ_OK && UNZ_OK != UNZ_END_OF_LIST_OF_FILE);
		}

		if (unzFile)
		{
			cocos2d::unzClose(unzFile);// 关闭输入文件
		}

		return success;

	}

	void setJSDelegate(JS::HandleObject pJSDelegate)
	{
		if (_JSDelegate) {
			CC_SAFE_DELETE(_JSDelegate);
		}
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		_JSDelegate = new (std::nothrow) JS::PersistentRootedObject(cx, pJSDelegate);
	}
public:

	JS::PersistentRootedObject *_JSDelegate;

};


JSClass  *js_cocos2dx_ziphelper_class;
JSObject *js_cocos2dx_ziphelper_prototype;

void js_cocos2dx_ZipHelper_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (ZipHelper)", obj);

	//JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	//JS::RootedObject jsObj(cx, obj);
	//js_proxy_t *proxy = jsb_get_js_proxy(cx, jsObj);
	//ZipHelper* cobj = (ZipHelper *)(proxy ? proxy->ptr : NULL);
	//CC_SAFE_DELETE(cobj);


	ZipHelper *cobj = static_cast<ZipHelper *>(JS_GetPrivate(obj));
	if (cobj)
	{
		js_proxy_t * p = jsb_get_native_proxy(cobj);
		if (p)
		{
#if not CC_ENABLE_GC_FOR_NATIVE_OBJECTS
			auto copy = &p->obj;
			JS::RemoveObjectRoot(cx, copy);
#endif
			jsb_remove_proxy(p);
		}
	}

}

bool js_cocos2dx_extension_ZipHelper_constructor(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_ziphelper_prototype);
	JS::RootedObject obj(cx, JS_NewObjectWithGivenProto(cx, js_cocos2dx_ziphelper_class, proto));

	ZipHelper* cobj = new ZipHelper();

	js_proxy_t *p = jsb_new_proxy(cx, cobj, obj);
	//JS::AddNamedObjectRoot(cx, &p->obj, "ZipHelper");
	cobj->setJSDelegate(obj);

	jsb_new_proxy(cx, cobj, obj);
	JS_SetPrivate(obj.get(), cobj);

	args.rval().set(JS::ObjectOrNullValue(obj));
	return true;
}

bool js_cocos2dx_extension_ZipHelper_unzipFile(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ZipHelper_unzipFile");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject jsObj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, jsObj);
	ZipHelper* cobj = (ZipHelper *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if (argc >= 2)
	{
		if (argv[0].isString() && argv[1].isString())
		{
			
			std::string inputFile;
			jsval_to_std_string(cx, argv[0], &inputFile);

			std::string outputFileDir;
			jsval_to_std_string(cx, argv[1], &outputFileDir);
			if (inputFile.empty() || outputFileDir.empty())
			{
				JS_ReportErrorUTF8(cx, "wrong inputFile or outputFileDir");
				return false;
			}
			
			std::string password;
			if (argc == 3)
			{
				if (argv[2].isString())
				{
					jsval_to_std_string(cx, argv[2], &password);
				}
				else
				{
					JS_ReportErrorUTF8(cx, "wrong arg 3, must be String type");
					return false;
				}
			}

			bool result = cobj->unzipFile(inputFile, outputFileDir, password);
			
			JS::RootedValue jsret(cx, JS::BooleanValue(result));
			argv.rval().set(jsret);
		}
		else
		{
			JS_ReportErrorUTF8(cx, "wrong inputFile or outputFileDir");
			return false;
		}
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, need more than %d", argc, 2);
		return false;
	}
	
	return true;
}


bool js_cocos2dx_extension_ZipHelper_unzipFileAsyn(JSContext *cx, uint32_t argc, JS::Value *vp)
{
	CCLOG("ZipHelper_unzipFile");

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject jsObj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(cx, jsObj);
	ZipHelper* cobj = (ZipHelper *)(proxy ? proxy->ptr : NULL);

	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if (argc >= 2)
	{
		if (argv[0].isString() && argv[1].isString())
		{

			std::string inputFile;
			jsval_to_std_string(cx, argv[0], &inputFile);

			std::string outputFileDir;
			jsval_to_std_string(cx, argv[1], &outputFileDir);
			if (inputFile.empty() || outputFileDir.empty())
			{
				JS_ReportErrorUTF8(cx, "wrong inputFile or outputFileDir");
				return false;
			}

			std::string password;
			if (argc == 3)
			{
				if (argv[2].isString())
				{
					jsval_to_std_string(cx, argv[2], &password);
				}
				else
				{
					JS_ReportErrorUTF8(cx, "wrong arg 3, must be String type");
					return false;
				}
			}

			cobj->unzipFileAsyn(inputFile, outputFileDir, password, [=](bool unzipResult, string input, string output)
			{

				//js_proxy_t * p = jsb_get_native_proxy(ws);
				//if (!p) return;

				//if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
				//	return;

				//JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();

				//// Set the protocol which server selects.
				//JS::RootedValue jsprotocol(cx);
				//std_string_to_jsval(cx, ws->getProtocol(), &jsprotocol);
				//JS::RootedObject wsObj(cx, p->obj);
				//JS_SetProperty(cx, wsObj, "protocol", jsprotocol);

				//JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));
				//JS::RootedValue vp(cx);
				//c_string_to_jsval(cx, "open", &vp);
				//JS_SetProperty(cx, jsobj, "type", vp);

				//JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
				//JS::HandleValueArray args(jsobjVal);
				//JS::RootedValue owner(cx, JS::ObjectOrNullValue(_JSDelegate));

				//ScriptingCore::getInstance()->executeFunctionWithOwner(owner, "onopen", args);
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
				{
					if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
						return;


					// 回调
					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewPlainObject(cx));

					JS::RootedValue resultJS(cx, JS::BooleanValue(unzipResult));
					JS_SetProperty(cx, jsobj, "result", resultJS);


					JS::RootedValue inputJS(cx);
					c_string_to_jsval(cx, input.c_str(), &inputJS);
					JS_SetProperty(cx, jsobj, "inputFilePath", inputJS);

					JS::RootedValue outputJS(cx);
					c_string_to_jsval(cx, output.c_str(), &outputJS);
					JS_SetProperty(cx, jsobj, "outputFilePath", outputJS);

					JS::RootedValue jsobjVal(cx, JS::ObjectOrNullValue(jsobj));
					JS::HandleValueArray args(jsobjVal);

					JS::RootedValue delegate(cx, JS::ObjectOrNullValue(cobj->_JSDelegate->get()));

					if (delegate.isObject())
					{
						ScriptingCore::getInstance()->executeFunctionWithOwner(delegate, "onUnzipResult", args);
					}
					
				});
			});
		}
		else
		{
			JS_ReportErrorUTF8(cx, "wrong inputFile or outputFileDir");
			return false;
		}
	}
	else
	{
		JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, need more than %d", argc, 2);
		return false;
	}

	return true;
}


void register_jsb_zip_helper(JSContext *cx, JS::HandleObject global)
{
	static const JSClassOps ziphelper_classOps = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		js_cocos2dx_ZipHelper_finalize,
		nullptr, nullptr, nullptr, nullptr
	};
	static JSClass ziphelper_class = {
		"ZipHelper",
		JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
		&ziphelper_classOps
	};

	js_cocos2dx_ziphelper_class = &ziphelper_class;

	static JSPropertySpec properties[] = {
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("unzipFile", js_cocos2dx_extension_ZipHelper_unzipFile, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("unzipFileAsyn", js_cocos2dx_extension_ZipHelper_unzipFileAsyn, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),

		JS_FS_END
	};

	JS::RootedObject parent_proto(cx, nullptr);
	js_cocos2dx_ziphelper_prototype = JS_InitClass(
		cx, global,
		parent_proto,
		js_cocos2dx_ziphelper_class,
		js_cocos2dx_extension_ZipHelper_constructor, 0, // constructor
		properties,
		funcs,
		nullptr, // no static properties
		nullptr);

	//js_cocos2dx_ziphelper_class = (JSClass *)calloc(1, sizeof(JSClass));
	//js_cocos2dx_ziphelper_class->name = "ZipHelper";
	//js_cocos2dx_ziphelper_class->addProperty = JS_PropertyStub;
	//js_cocos2dx_ziphelper_class->delProperty = JS_DeletePropertyStub;
	//js_cocos2dx_ziphelper_class->getProperty = JS_PropertyStub;
	//js_cocos2dx_ziphelper_class->setProperty = JS_StrictPropertyStub;
	//js_cocos2dx_ziphelper_class->enumerate = JS_EnumerateStub;
	//js_cocos2dx_ziphelper_class->resolve = JS_ResolveStub;
	//js_cocos2dx_ziphelper_class->convert = JS_ConvertStub;
	//js_cocos2dx_ziphelper_class->finalize = js_cocos2dx_ZipHelper_finalize;
	//js_cocos2dx_ziphelper_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	//static JSPropertySpec properties[] = {
	//	JS_PS_END
	//};

	//static JSFunctionSpec funcs[] = {
	//	JS_FN("unzipFile", js_cocos2dx_extension_ZipHelper_unzipFile, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	JS_FN("unzipFileAsyn", js_cocos2dx_extension_ZipHelper_unzipFileAsyn, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	//	
	//	JS_FS_END
	//};

	//static JSFunctionSpec st_funcs[] = {
	//	JS_FS_END
	//};

	//js_cocos2dx_ziphelper_prototype = JS_InitClass(
	//	cx, global,
	//	JS::NullPtr(),
	//	js_cocos2dx_ziphelper_class,
	//	js_cocos2dx_extension_ZipHelper_constructor, 0, // constructor
	//	properties,
	//	funcs,
	//	NULL, // no static properties
	//	st_funcs);

}