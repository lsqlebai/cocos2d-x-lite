package com.iflytek.payComponent;

import android.app.Activity;

import com.iflytek.pay.ubp.commonutils.log.Logger;
import com.iflytek.pay.ubp.sole.IParams;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.UBP;
import com.iflytek.ubplib.model.SDKParams;
import com.iflytek.unipay.BasePayChannel;
import com.iflytek.unipay.IPayComponent;
import com.iflytek.unipay.UnityOrder;

/**
 * Created by lsq on 2017/1/16.
 */

public class PayChannelImpl extends BasePayChannel {
    //private static final String APP_ID = "kdxfyylft";// 需要向iflytek申请
    //private static final String APP_KEY = "kdxfjf"; // 需要向iflytek申请
	private static final String APP_ID = "kdxfchdzz";// 需要向iflytek申请
    private static final String APP_KEY = "kdxfjf"; // 需要向iflytek申请
	
    final String proxy = "http://202.99.114.74:58005";

    @Override
    public void onAppInit() {
        // 天津联通代理
        UBP.setUBPHost(proxy);
        Logger.log().i("UBP.setUBPHost");
    }

    @Override
    public void initParamUpdate(SDKParams params) {
        params.setKeyValue(IParams.INIT_APP_KEY, APP_KEY);
        params.setKeyValue(IParams.INIT_APP_ID, APP_ID);
    }

    @Override
    public void pay(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
        params.setFeecodeId(order.getUnicomFeeId())
                .setKeyValue(IParams.PAY_IS_SUBS_PAY, false)// 包月计费
                .setKeyValue(IParams.PAY_PRODUCT_ID, order.getUnicomProductId())// 计费sdk需要申请
                .setPayPrice(order.getPrice())// 计费金额（分）
                .setProductName(order.getName())// 道具名称
                .setCustom(payComponent.createCustom(order.getOrderId(), "UnicomPay"));
        payComponent.createOrder(params, callback);

    }

    @Override
    public void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent createOrder) {
        params
                .setFeecodeId(order.getUnicomFeeId())
                .setKeyValue(IParams.PAY_IS_SUBS_PAY, true)// 包月计费
                .setKeyValue(IParams.PAY_PRODUCT_ID, order.getUnicomProductId())// 计费sdk需要申请
                .setCustom(createOrder.createCustom(order.getOrderId(), "UnicomVipPay"));
        createOrder.createOrder(params, callback);
    }
}
