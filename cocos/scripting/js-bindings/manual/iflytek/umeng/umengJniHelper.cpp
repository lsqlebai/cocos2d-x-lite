#include "umengJniHelper.h"
#include "umenglog.h"
#include <string.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define JAVAVM    umengJniHelper::getJavaVM()

using namespace std;

#define JAVA_CLASS_MOBCLICKAGENT                "com/umeng/analytics/MobclickAgent"
#define JAVA_CLASS_MOBCLICKCPPHELPER            "com/umeng/analytics/mobclick/game/MobClickCppHelper"
#define JAVA_CLASS_UMGAMEAGENT                  "com/umeng/analytics/game/UMGameAgent"


jclass gClass_MobClickAgent;
jclass gClass_MobClickCppHelper;
jclass gClass_UMGameAgent;


extern "C" {
    static bool getEnv(JNIEnv **env) {

        bool bRet = false;
        do {
            if (JAVAVM) {
//              LOGD("JAVAVM->GetEnv");
                jint ret = JAVAVM->GetEnv((void**) env, JNI_VERSION_1_4);
                if (ret != JNI_OK) {
//                  LOGD("AttachCurrentThread env");
                    jint status = JAVAVM->AttachCurrentThread(env, NULL);
                    if (status < 0) {
                        LOGE("getEnv: failed to attach current thread");
                        env = NULL;
                        break;
                    }
//                  LOGD("AttachCurrentThread env success");
                }
//              LOGD("JAVAVM->GetEnv return true");
            } else {
                LOGE("!!!!!! Not set javavm. Please Call umengJniHelper::setJavaVM() in JNI_OnLoad.");
                break;
            }
            bRet = true;
        } while (0);        

        return bRet;
    }

    static bool getStaticMethodInfo_(umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;
        do {
            if (! getEnv(&pEnv)) {
                break;
            }

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID) {
                LOGD("Failed to find static method id of %s", methodName);
                break;
            }

            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;
            bRet = true;
        } while (0);

        return bRet;
    }

    static bool getMethodInfo_(umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do {
            if (! getEnv(&pEnv)) {
                break;
            }

            methodID = pEnv->GetMethodID(classID, methodName, paramCode);
            if (! methodID) {
                LOGD("Failed to find method id of %s", methodName);
                break;
            }

            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static string jstring2string_(jstring jstr) {
        if (jstr == NULL) {
            return "";
        }
        
        JNIEnv *env = 0;

        if (! getEnv(&env)) {
            return 0;
        }

        const char* chars = env->GetStringUTFChars(jstr, NULL);
        string ret(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        return ret;
    }
}


JavaVM* umengJniHelper::m_psJavaVM = NULL;

bool umengJniHelper::getJNIEnv(JNIEnv **env) {
    return getEnv(env);
}

void umengJniHelper::detachEnv() {
    if (JAVAVM) {
        JAVAVM->DetachCurrentThread();
    }
}

JavaVM* umengJniHelper::getJavaVM() {
    return m_psJavaVM;
}

void umengJniHelper::globalDeInit() {
    JNIEnv *env = 0;
    getEnv(&env);

    env->DeleteGlobalRef(gClass_MobClickAgent);
    gClass_MobClickAgent = NULL;

    env->DeleteGlobalRef(gClass_MobClickCppHelper);
    gClass_MobClickCppHelper = NULL;

    env->DeleteGlobalRef(gClass_UMGameAgent);
    gClass_UMGameAgent = NULL;

}

void umengJniHelper::setJavaVM(JavaVM *javaVM) {
    m_psJavaVM = javaVM;

    JNIEnv *env = 0;
    getEnv(&env);

    jclass c = env->FindClass(JAVA_CLASS_MOBCLICKAGENT);
    gClass_MobClickAgent = (jclass)env->NewGlobalRef(c);

    c = env->FindClass(JAVA_CLASS_MOBCLICKCPPHELPER);
    gClass_MobClickCppHelper = (jclass)env->NewGlobalRef(c);

    c = env->FindClass(JAVA_CLASS_UMGAMEAGENT);
    gClass_UMGameAgent = (jclass)env->NewGlobalRef(c);
}

string umengJniHelper::m_externalAssetPath;

const char* umengJniHelper::getExternalAssetPath() {
    return m_externalAssetPath.c_str();
}

void umengJniHelper::setExternalAssetPath(const char * externalAssetPath) {
    m_externalAssetPath = externalAssetPath;
}

bool umengJniHelper::getStaticMethodInfo(umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode)
{
    return getStaticMethodInfo_(methodinfo, classID, methodName, paramCode);
}

bool umengJniHelper::getMethodInfo(umengJniMethodInfo &methodinfo, jclass classID, const char *methodName, const char *paramCode)
{
    return getMethodInfo_(methodinfo, classID, methodName, paramCode);
}

string umengJniHelper::jstring2string(jstring str)
{
    return jstring2string_(str);
}

#endif