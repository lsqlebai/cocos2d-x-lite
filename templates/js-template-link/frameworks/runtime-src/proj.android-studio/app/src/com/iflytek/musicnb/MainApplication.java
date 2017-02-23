package com.iflytek.musicnb;

import android.content.Context;

import com.google.gson.Gson;
import com.iflytek.app.BaseApplication;
import com.iflytek.unipay.PayComponent;

import java.io.IOException;
import java.io.InputStream;

public class MainApplication extends BaseApplication {

    static class Config {
        String channel;
    }

    @Override
    public void onCreate() {
        super.onCreate();
//        FrescoHelper.initialize(this);

//        x.Ext.init(this);
//        x.Ext.setDebug(BuildConfig.DEBUG);

//        AppUtil.readLocalProperties();
//        MobclickAgent.startWithConfigure(new MobclickAgent.UMAnalyticsConfig(this, "57b66b9267e58e55b0002b8f", AppUtil.getAppId()));
        // 支付组件初始化
        PayComponent.getInstance().appInit(this, getChannelId());
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        MultiDex.install(this);
    }

    private String getChannelId() {
        InputStream is = null;
        try {
            is = getAssets().open("res/raw-assets/resources/config.json");
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            Config config = new Gson().fromJson(new String(buffer), Config.class);
            return config.channel;
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
