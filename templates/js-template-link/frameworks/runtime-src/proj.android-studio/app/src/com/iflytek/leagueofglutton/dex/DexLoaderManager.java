package com.iflytek.leagueofglutton.dex;

import android.content.Context;
import android.util.Log;

import com.iflytek.leagueofglutton.MainApplication;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.locks.ReentrantLock;

import dalvik.system.DexClassLoader;

public class DexLoaderManager {
    private static final String TAG = DexLoaderManager.class.getSimpleName();
    public static final String DEX_FILE_INNER_PATH = "res/raw-assets/resources/server/";
    public static final String DEX_FILE_NAME = "localserver.apk";

    private static volatile DexLoaderManager instance = null;

    private ReentrantLock lock = null;

    private DexLoaderManager() {
        lock = new ReentrantLock();
    }

    public String getExtractedDexPath() {
        final Context ctx = MainApplication.geContext();
        return ctx.getDir("dex", 0) + "/" + DEX_FILE_NAME;
    }

    public String getExtractedDexDirPath() {
        final Context ctx = MainApplication.geContext();
        return ctx.getDir("dex", 0) + "/";
    }

    public static DexLoaderManager getInstance() {
        if (instance == null) {
            synchronized (DexLoaderManager.class) {
                instance = new DexLoaderManager();
            }
        }

        return instance;
    }

    public void copyDexLocked() {
        lock.lock();
        final Context ctx = MainApplication.geContext();
        File destFile = new File(getExtractedDexPath());
        if (destFile.exists()) {
            return;
        }
        File destDir = new File(getExtractedDexDirPath());
        destDir.mkdirs();
        try {
            InputStream localInputStream = ctx.getAssets().open(DEX_FILE_INNER_PATH + DEX_FILE_NAME);
            FileOutputStream localFileOutputStream = new FileOutputStream(destFile);
            byte[] arrayOfByte = new byte[1024];
            for (;;)
            {
                int i = localInputStream.read(arrayOfByte);
                if (i == -1) {
                    break;
                }
                localFileOutputStream.write(arrayOfByte, 0, i);
                localFileOutputStream.flush();
            }

            localFileOutputStream.close();
            localInputStream.close();
        } catch (IOException e) {
            Log.e(TAG, "exception in copyDex: " + e.toString());
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void copyDexAsync() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                copyDexLocked();
            }
        }).start();
    }

    public synchronized Class loadClass(Context ctx) {
        Class ret = null;
        ClassLoader cl = new DexClassLoader(
                getExtractedDexPath()
                , getExtractedDexDirPath()
                , ctx.getApplicationInfo().nativeLibraryDir
                , ctx.getClassLoader());

        try {
            ret = cl.loadClass("com.iflytek.localserver.game.main.ServerMain");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            return ret;
        }
    }

    public void setApkClassLoader(Context ctx) {
        lock.lock();
        try {
            ClassLoader paramClassLoader = new DexClassLoader(
                    getExtractedDexPath()
                    , getExtractedDexDirPath()
                    , ctx.getApplicationInfo().nativeLibraryDir, ctx.getClassLoader());
            Class localClass = Class.forName("android.app.ActivityThread");
            Object localObject1 = localClass.getMethod("currentActivityThread"
                    , new Class[0]).invoke(null, new Object[0]);
            Field localField1 = localClass.getDeclaredField("mPackages");
            localField1.setAccessible(true);
            Object localObject2 = localField1.get(localObject1);
            localField1.setAccessible(false);
            Method localMethod = localObject2.getClass().getMethod("get"
                    , new Class[] { Object.class });
            Object[] arrayOfObject = new Object[1];
            arrayOfObject[0] = ctx.getPackageName();
            Object localObject3 = ((WeakReference)localMethod.invoke(localObject2, arrayOfObject)).get();
            Field localField2 = localObject3.getClass().getDeclaredField("mClassLoader");
            localField2.setAccessible(true);
            localField2.set(localObject3, paramClassLoader);
            localField2.setAccessible(false);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }
}
