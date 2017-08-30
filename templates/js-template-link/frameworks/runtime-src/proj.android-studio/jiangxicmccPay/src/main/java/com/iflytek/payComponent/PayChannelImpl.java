package com.iflytek.payComponent;

import android.app.Activity;

import com.iflytek.pay.ubp.sole.PayParams;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.model.SDKParams;
import com.iflytek.unipay.BasePayChannel;
import com.iflytek.unipay.IPayComponent;
import com.iflytek.unipay.UnityOrder;

/**
 * Created by leisu on 2017/8/17
 */

public class PayChannelImpl extends BasePayChannel {

    final int GAME_ID = 1001803; // 游戏唯一id，由局方发放
    final String GAME_KEY = "890f7fb2178f019310a8e75cfb9ce0fd";// 游戏唯一key，由局方发放

    @Override
    public void onAppInit() {
    }

    @Override
    public void initParamUpdate(SDKParams params) {

        params.setKeyValue(PayParams.KEY_GAME_ID, GAME_ID);
        params.setKeyValue(PayParams.KEY_GAME_NAME, "吃货大作战");
        params.setKeyValue(PayParams.KEY_PROVIDER_COMPANY_NAME, "科大讯飞");
        params.setKeyValue(PayParams.KEY_SERVICE_PHONE_NUM, "QQ群-529066615");
    }

    @Override
    public void pay(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
        params.setKeyValue(PayParams.KEY_GAME_ID, GAME_ID);
        params.setKeyValue(PayParams.KEY_IS_PAY_MONTH, false);
        params.setKeyValue(PayParams.KEY_PRODUCT_ID,  order.getUnicomProductId());// 计费sdk需要申请
        params.setProductName(order.getName());// 道具名称
        params.setPayPrice(order.getPrice());// 计费金额（分）
        params.setCustom(payComponent.createCustom(order.getOrderId(), "JiangxiCMCCPay"));// 透传参数
        params.setKeyValue(PayParams.KEY_GAME_KEY, GAME_KEY);// 游戏唯一key，由局方发放
        params.setFeecodeId(order.getUnicomFeeId());
        payComponent.createOrder(params, callback);
    }

    @Override
    public void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
        params.setKeyValue(PayParams.KEY_IS_PAY_MONTH, true);
        params.setCustom(payComponent.createCustom(order.getOrderId(), "JiangxiCMCCPay"));// 透传参数
        payComponent.createOrder(params, callback);
    }
}
