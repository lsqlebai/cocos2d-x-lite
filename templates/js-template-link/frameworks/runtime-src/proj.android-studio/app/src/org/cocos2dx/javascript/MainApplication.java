package org.cocos2dx.javascript;

import android.content.Context;

import com.iflytek.app.BaseApplication;
import com.iflytek.unipay.PayComponent;

public class MainApplication extends BaseApplication {

    @Override
    public void onCreate() {
        super.onCreate();
//        FrescoHelper.initialize(this);

//        x.Ext.init(this);
//        x.Ext.setDebug(BuildConfig.DEBUG);

//        AppUtil.readLocalProperties();
//        MobclickAgent.startWithConfigure(new MobclickAgent.UMAnalyticsConfig(this, "57b66b9267e58e55b0002b8f", AppUtil.getAppId()));
        // 支付组件初始化
        PayComponent.getInstance().appInit(this, "014BDA10");
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        MultiDex.install(this);
    }

}
