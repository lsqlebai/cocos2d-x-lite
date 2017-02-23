package com.iflytek.app;

import android.content.Context;

import com.iflytek.log.FileLogger;
import com.iflytek.log.Logger;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;

/**
 * @author wpfang
 */
public class AppException implements UncaughtExceptionHandler {
    public static final String TAG = "CrashHandler";
    private static AppException instance;
    private UncaughtExceptionHandler mDefaultHandler;

    private AppException(Context context) {
        init(context);
    }

    public static AppException getInstance(Context context) {
        if (instance == null) {
            instance = new AppException(context);
        }
        return instance;
    }

    private void init(Context context) {
        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        if (!handleException(ex) && mDefaultHandler != null) {
            mDefaultHandler.uncaughtException(thread, ex);
        } else {
            exit();
        }
    }

    private void exit() {
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    private boolean handleException(Throwable ex) {
        if (ex == null) {
            return false;
        }
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        ex.printStackTrace(pw);
        String msg = sw.toString();

        new FileLogger("exitlog", "appexit").println(msg);
        Logger.log().e(msg);

        BaseApplication.getAppInstance().onAppException(msg);

        return true;
    }
}