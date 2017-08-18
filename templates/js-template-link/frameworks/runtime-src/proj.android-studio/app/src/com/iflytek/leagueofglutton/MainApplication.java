package com.iflytek.leagueofglutton;

import android.content.Context;
import android.content.res.Configuration;
import android.support.multidex.MultiDex;
import android.support.multidex.MultiDexApplication;

import com.google.gson.Gson;
import com.iflytek.leagueofglutton.dex.DexLoaderManager;
import com.iflytek.unipay.PayComponent;

import java.io.IOException;
import java.io.InputStream;

public class MainApplication extends MultiDexApplication {

    static class Config {
        String channel;
    }

    public static String channel = "";

    private static Context _context;
    public static final Context geContext()
    {
        return _context;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        _context = this;

//        FrescoHelper.initialize(this);

//        x.Ext.init(this);
//        x.Ext.setDebug(BuildConfig.DEBUG);

//        AppUtil.readLocalProperties();
//        MobclickAgent.startWithConfigure(new MobclickAgent.UMAnalyticsConfig(this, "57b66b9267e58e55b0002b8f", AppUtil.getAppId()));
        // 支付组件初始化
        PayComponent.getInstance().appInit(this, getChannelId());
        DexLoaderManager.getInstance().copyDexLocked();
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        MultiDex.install(this);
        PayComponent.getInstance().attachBaseContext(this, base);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // TODO Auto-generated method stub
        super.onConfigurationChanged(newConfig);
        PayComponent.getInstance().onConfigChanged(newConfig);

    }

    @Override
    public void onLowMemory() {
        // TODO Auto-generated method stub
        super.onLowMemory();
        PayComponent.getInstance().onLowMemory();
    }


    private String getChannelId() {
        InputStream is = null;
        try {
            is = getAssets().open("res/raw-assets/resources/config.json");
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            Config config = new Gson().fromJson(new String(buffer), Config.class);

            channel = config.channel;
            return channel;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return "014BDA10";
    }
}
