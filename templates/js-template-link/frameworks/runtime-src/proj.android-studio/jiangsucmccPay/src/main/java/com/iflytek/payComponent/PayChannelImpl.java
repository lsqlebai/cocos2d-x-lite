package com.iflytek.payComponent;

import android.app.Activity;

import com.google.gson.Gson;
import com.iflytek.pay.ubp.commonutils.log.Logger;
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


    final String proxy = "http://60.205.235.66/ttdjserver";

    @Override
    public void onAppInit() {
        // 江苏移动代理
        UBP.setUBPHost(proxy);
        Logger.log().i("UBP.setUBPHost");
    }

    @Override
    public void initParamUpdate(SDKParams params) {

    }

    @Override
    public void pay(Activity activity, SDKParams sdkParams, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {

        HashMap<String, String> map = new HashMap<>();
        map.put("appid", order.getUnicomProductId());
        sdkParams.setFeeExtend(new Gson().toJson(map));
        //购买结果通知接口
        sdkParams.setKeyValue("notifyurl", order.getNotifyUrl());
        //二次订单查询接口，确认已购买成功
        payComponent.createOrder(sdkParams, callback);
    }

    @Override
    public void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent createOrder) {
        pay(activity, params, order, callback, createOrder);
    }
}
