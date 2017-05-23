package com.iflytek.app;


import android.app.Application;
import android.content.Context;

import com.iflytek.listener.CatchExceptionListener;

/**
 * @author wpfang
 */
public class BaseApplication extends Application {
    private static BaseApplication appInstance;
    CatchExceptionListener mCatchExceptionListener;
    /**
     * @return 全局Context
     */
    public static Context globalContext() {
        return getAppInstance().getApplicationContext();
    }

    public static BaseApplication getAppInstance() {
        return appInstance;
    }

    public static void setAppInstance(BaseApplication appInstance) {
        BaseApplication.appInstance = appInstance;
    }

    /**
     * 应用异常捕捉后的回调
     */
    public void onAppException(String msg) {
    	if(mCatchExceptionListener != null){
    		mCatchExceptionListener.catchException(msg);
    	}
    }

    @Override
    public void onCreate() {
        super.onCreate();
        setAppInstance(this);
        Thread.setDefaultUncaughtExceptionHandler(
                AppException.getInstance(getAppInstance()));
    }
    
    
    public void setCatchExceptionListener(CatchExceptionListener catchExceptionListener){
    	mCatchExceptionListener = catchExceptionListener;
    }
   
}
