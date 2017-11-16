package com.iflytek.leagueofglutton.components;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;

import com.iflytek.leagueofglutton.dex.DexLoaderManager;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class LocalSocketServerService extends Service {

    private static Object sServerObj = null;
    private static Method sStopMethod = null;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
//        DexLoaderManager.getInstance().setApkClassLoader(this);
//        ClassLoader cl = getClassLoader();

        new Thread(){
            @Override
            public void run() {
                try {
                    Class serverClass = DexLoaderManager.getInstance().loadClass(LocalSocketServerService.this.getApplicationContext());
                    Method getInstanceMethod = serverClass.getMethod("getInstance");
                    sServerObj = getInstanceMethod.invoke(null);

                    Method initMethod = serverClass.getMethod("init", Context.class);
                    initMethod.invoke(sServerObj, LocalSocketServerService.this.getApplicationContext());

                    Method startMethod = serverClass.getMethod("start");
                    startMethod.invoke(sServerObj);

                    sStopMethod = serverClass.getMethod("stop");

                } catch (Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onStart(Intent intent, int startId) {
        super.onStart(intent, startId);
    }

    @Override
    public void onDestroy() {
        try {
            sStopMethod.invoke(sServerObj);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        super.onDestroy();
    }
}
