package com.iflytek.payComponent;

import android.app.Activity;
import android.app.Dialog;

import com.google.gson.Gson;
import com.iflytek.pay.ubp.commonutils.phone.NetWorkUtil;
import com.iflytek.pay.ubp.sole.PayProxy;
import com.iflytek.pay.ubp.ui.DialogHelper;
import com.iflytek.pay.ubpcallback.IPayCallback;
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
    public void pay(Activity activity, final SDKParams params, final UnityOrder order, final IPayCallback callback, final IPayComponent payComponent) {
        HashMap<String, String> data = new HashMap<>();
        data.put("trade_type", "NATIVE"); //微信支付官方要求的字段，表示扫码支付
        data.put("spbill_create_ip", NetWorkUtil.getIpv4());

        params.setFeeExtend(new Gson().toJson(data));
        DialogHelper.showWeixin_AliDialog(activity, true, new DialogHelper.DialogCallBack() {
            @Override
            public void onCancel(Dialog dialog) {
                params.setCustom(payComponent.createCustom(order.getOrderId(), PayProxy.WEIXINPAY));
                params.setFeecodeId(order.getWeixinFeeId());
                params.setKeyValue(PayProxy.PAY_TYPE, PayProxy.WEIXINPAY);
                payComponent.createOrder(params, callback);
                dialog.dismiss();
            }

            @Override
            public void onConfirm(Dialog dialog) {
                params.setCustom(payComponent.createCustom(order.getOrderId(), PayProxy.ALIPAY));
                params.setFeecodeId(order.getAliFeeId());
                params.setKeyValue(PayProxy.PAY_TYPE, PayProxy.ALIPAY);
                payComponent.createOrder(params, callback);
                dialog.dismiss();
            }
        });
    }
}
