/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __UMENG_JNI_HELPER_H__
#define __UMENG_JNI_HELPER_H__

#include "cocos2d.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include <string>

typedef struct umengJniMethodInfo_ {
    JNIEnv *    env;
    jmethodID   methodID;
} umengJniMethodInfo;

class umengJniHelper {
public:
  static bool getJNIEnv(JNIEnv **env);
  static void detachEnv();
  static void globalDeInit();
    static JavaVM* getJavaVM();
    static void setJavaVM(JavaVM *javaVM);
    static const char* getExternalAssetPath();
    static void setExternalAssetPath(const char* externalAssetPath);
    static bool getStaticMethodInfo(umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode);
    static bool getMethodInfo      (umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode);
    static std::string jstring2string(jstring str);

private:
    static JavaVM *m_psJavaVM;
    static std::string m_externalAssetPath;
};

#endif

#endif // __ANDROID_JNI_HELPER_H__


