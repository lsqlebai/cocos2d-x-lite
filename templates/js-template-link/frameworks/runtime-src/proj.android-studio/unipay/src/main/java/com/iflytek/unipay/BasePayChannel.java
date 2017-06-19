package com.iflytek.unipay;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.widget.Toast;

import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.model.SDKParams;

/**
 * Created by lsq on 2017/1/21.
 */
public abstract class BasePayChannel implements PayChannel{
    @Override
    public void onAppInit() {

    }

    @Override
    public void initParamUpdate(SDKParams params) {

    }

    @Override
    public void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent createOrder) {
        Toast.makeText(activity, "暂不支持包月支付", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onApplicationAttachBaseContext(Application application, Context base) {

    }
}
