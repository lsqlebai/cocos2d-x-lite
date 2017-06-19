//
//  MobClickCpp.m
//  MobClickCpp
//
//  Created by 张梓琦 on 14-2-24.
//  Copyright (c) 2014年 zhangziqi. All rights reserved.
//


#include "MobClickCpp.h"
#include "umengJniHelper.h"
using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

extern jclass gClass_MobClickAgent;
extern jclass gClass_MobClickCppHelper;
extern jclass gClass_UMGameAgent;

namespace umeng {

extern "C"
{
    static jobject createJavaMapObject(JNIEnv *env, std::map<std::string, std::string>* map) {
        jclass class_Hashtable = env->FindClass("java/util/HashMap");
        jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
        jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
        if (map != NULL) {
            jmethodID add_method= env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
            for (std::map<std::string, std::string>::const_iterator it = map->begin(); it != map->end(); ++it) {
                env->CallObjectMethod(obj_Map, add_method, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
            }
        }
        env->DeleteLocalRef(class_Hashtable);
        return obj_Map;
    }
}


    void MobClickCpp::setLogEnabled(bool value){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "setDebugMode", "(Z)V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,value);
        }
    }
    void MobClickCpp::setCheckDevice(bool value){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "setCheckDevice", "(Z)V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,value);
        }
    }
    void MobClickCpp::setSessionIdleLimit(int seconds){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "setSessionContinueMillis", "(J)V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,seconds);
        }
    }
    
    void MobClickCpp::doNotCallItFromYourCode(const char *appKey, const char * channelId){
        //for ios
    }
    void MobClickCpp::setAppVersion(const char *){
        //for ios
    }
    void MobClickCpp::event(const char * eventId, const char * label){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickCppHelper, "event", "(Ljava/lang/String;Ljava/lang/String;)V")){
            jstring eId = t.env->NewStringUTF(eventId);
            jstring la = t.env->NewStringUTF(label);
            t.env->CallStaticVoidMethod(gClass_MobClickCppHelper, t.methodID,eId,la);
            t.env->DeleteLocalRef(eId);
            t.env->DeleteLocalRef(la);
        }
    }
    void MobClickCpp::event(const char * eventId, eventDict * attributes, int counter){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickCppHelper, "event", "(Ljava/lang/String;Ljava/util/Map;I)V")){
            jstring eId = t.env->NewStringUTF(eventId);
            jobject jparamMap = createJavaMapObject(t.env, attributes);
            t.env->CallStaticVoidMethod(gClass_MobClickCppHelper, t.methodID,eId,jparamMap,counter);
            t.env->DeleteLocalRef(eId);
            t.env->DeleteLocalRef(jparamMap);
        }
    }
    void MobClickCpp::event(const char * eventId, eventDict * attributes){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickCppHelper, "event", "(Ljava/lang/String;Ljava/util/Map;)V")){
            jstring eId = t.env->NewStringUTF(eventId);
            jobject jparamMap = createJavaMapObject(t.env, attributes);
            t.env->CallStaticVoidMethod(gClass_MobClickCppHelper, t.methodID,eId,jparamMap);
            t.env->DeleteLocalRef(eId);
            t.env->DeleteLocalRef(jparamMap);
        }
    }
   
    void MobClickCpp::setUserLevel(int level) {
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "setPlayerLevel", "(I)V")){
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,level);
        }
    }
   
    
    void MobClickCpp::startLevel(const char *level){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "startLevel", "(Ljava/lang/String;)V")){
            jstring le = t.env->NewStringUTF(level);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,le);
            t.env->DeleteLocalRef(le);
        }
    }
    
    void MobClickCpp::finishLevel(const char *level){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "finishLevel", "(Ljava/lang/String;)V")){
            jstring le = t.env->NewStringUTF(level);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,le);
            t.env->DeleteLocalRef(le);
        }
    }
    
    void MobClickCpp::failLevel(const char *level){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "failLevel", "(Ljava/lang/String;)V")){
            jstring le = t.env->NewStringUTF(level);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,le);
            t.env->DeleteLocalRef(le);
        }
    }
    
    void MobClickCpp::pay(double cash, int source, double coin){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "pay", "(DDI)V")){
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,cash,coin,source);
        }
    }
    
    void MobClickCpp::pay(double cash, int source, const char *item, int amount, double price){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "pay", "(DLjava/lang/String;IDI)V")){
            jstring it = t.env->NewStringUTF(item);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,cash,it,amount,price,source);
            t.env->DeleteLocalRef(it);
        }
    }
    
    void MobClickCpp::buy(const char *item, int amount, double price){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "buy", "(Ljava/lang/String;ID)V")){
            jstring it = t.env->NewStringUTF(item);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,it,amount,price);
            t.env->DeleteLocalRef(it);
        }
    }
    
    void MobClickCpp::use(const char *item, int amount, double price){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "use", "(Ljava/lang/String;ID)V")){
            jstring it = t.env->NewStringUTF(item);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,it,amount,price);
            t.env->DeleteLocalRef(it);
        }
    }
    
    void MobClickCpp::bonus(double coin, int source){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "bonus", "(DI)V")){
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,coin,source);
        }
    }
    
    void MobClickCpp::bonus(const char *item, int amount, double price, int source){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_UMGameAgent, "bonus", "(Ljava/lang/String;IDI)V")){
            jstring it = t.env->NewStringUTF(item);
            t.env->CallStaticVoidMethod(gClass_UMGameAgent, t.methodID,it,amount,price,source);
            t.env->DeleteLocalRef(it);
        }
    }
    
    void MobClickCpp::beginLogPageView(const char *pageName){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "onPageStart", "(Ljava/lang/String;)V")){
            jstring page = t.env->NewStringUTF(pageName);
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,page);
            t.env->DeleteLocalRef(page);
        }
    }
    
    void MobClickCpp::endLogPageView(const char *pageName){
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "onPageEnd", "(Ljava/lang/String;)V")){
            jstring page = t.env->NewStringUTF(pageName);
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,page);
            t.env->DeleteLocalRef(page);
        }
    }
    
    void MobClickCpp::setEncryptEnabled(bool value) {
       umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "enableEncrypt", "(Z)V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,value);
        }
    }
    
    void MobClickCpp::profileSignIn(const char *puid, const char *provider) {
        umengJniMethodInfo t;
        if(provider == NULL){
            if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "onProfileSignIn", "(Ljava/lang/String;)V")){
                jstring uid = t.env->NewStringUTF(puid);
                t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,uid);
                t.env->DeleteLocalRef(uid);
            }
        }else{
            if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "onProfileSignIn", "(Ljava/lang/String;Ljava/lang/String;)V")){
                jstring uid = t.env->NewStringUTF(puid);
                jstring pro = t.env->NewStringUTF(provider);
                t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,uid,pro);
                t.env->DeleteLocalRef(uid);
                t.env->DeleteLocalRef(pro);
            }
        }
    }
    
    void MobClickCpp::profileSignOff() {
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "onProfileSignOff", "()V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID);
        }
    }
    
    void MobClickCpp::setLatency(unsigned int latency) {
        umengJniMethodInfo t;
        if (umengJniHelper::getStaticMethodInfo(t, gClass_MobClickAgent, "setLatencyWindow", "(J)V")){
            t.env->CallStaticVoidMethod(gClass_MobClickAgent, t.methodID,latency);
        }
    }
}

#endif