package com.iflytek.payComponent;

import android.app.Activity;
import android.app.Application;
import android.content.Context;

import com.google.gson.Gson;
import com.iflytek.pay.ubp.commonutils.phone.NetWorkUtil;
import com.iflytek.pay.ubp.sole.IParams;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.UBP;
import com.iflytek.ubplib.model.SDKParams;
import com.iflytek.unipay.BasePayChannel;
import com.iflytek.unipay.IPayComponent;
import com.iflytek.unipay.UnityOrder;

import java.util.HashMap;

/**
 * Created by lsq on 2017/1/16.
 */

public class PayChannelImpl extends BasePayChannel {

    @Override
    public void onAppInit() {
        UBP.getAppCtrl().onCreate();
    }

    @Override
    public void pay(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
        HashMap<String, String> data = new HashMap<>();
        data.put("trade_type", "NATIVE");
        data.put("spbill_create_ip", NetWorkUtil.getIpv4());
        params.setFeeExtend(new Gson().toJson(data))
                .setFeecodeId(order.getXiaowoFeeId())
                .setKeyValue(IParams.PROP_NAME, order.getName())
                .setKeyValue(IParams.PROP_PRICE, order.getPrice())
                .setCustom(payComponent.createCustom(order.getOrderId(), "XiaoWoPay"));
        payComponent.createOrder(params, callback);
    }

    @Override
    public void onApplicationAttachBaseContext(Application application, Context base) {
        UBP.getAppCtrl().attachBaseContext(application, base);
    }
}
