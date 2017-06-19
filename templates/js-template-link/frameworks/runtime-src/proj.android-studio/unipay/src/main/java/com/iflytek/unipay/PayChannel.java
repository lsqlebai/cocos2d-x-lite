package com.iflytek.unipay;

import android.app.Activity;
import android.app.Application;
import android.content.Context;

import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.model.SDKParams;

/**
 * Created by lsq on 2017/1/16.
 */

public interface PayChannel {
    void onAppInit();
    void onApplicationAttachBaseContext(Application application, Context base);
    void initParamUpdate(SDKParams params);
    void pay(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent createOrder);
    void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent createOrder);
}
