package com.iflytek.payComponent;

import android.app.Activity;

import com.iflytek.pay.model.PayParamConstant;
import com.iflytek.pay.ubp.sole.PayParams;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.model.SDKParams;
import com.iflytek.unipay.BasePayChannel;
import com.iflytek.unipay.IPayComponent;
import com.iflytek.unipay.UnityOrder;


/**
 * 广东电信计费
 */
public class PayChannelImpl extends BasePayChannel {
    //private static final String APP_ID = "kdxfyylft";// 需要向iflytek申请
    //private static final String APP_KEY = "kdxfjf"; // 需要向iflytek申请
	private static final String APP_ID = "kdxfchdzz";// 需要向iflytek申请
    private static final String APP_KEY = "kdxfjf"; // 需要向iflytek申请
	

    @Override
    public void onAppInit() {
    }

    @Override
    public void initParamUpdate(SDKParams params) {
    }

    @Override
    public void pay(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
//        params.setFeecodeId(order.getUnicomFeeId())
//                .setKeyValue(IParams.PAY_IS_SUBS_PAY, false)// 包月计费
//                .setKeyValue(IParams.PAY_PRODUCT_ID, order.getUnicomProductId())// 计费sdk需要申请
//                .setPayPrice(order.getPrice())// 计费金额（分）
//                .setProductName(order.getName())// 道具名称
//                .setCustom(payComponent.createCustom(order.getOrderId(), "UnicomPay"));

        params.setKeyValue(PayParamConstant.P_Notify, true);
        params.setKeyValue(PayParamConstant.PayPrice, order.getPrice());
        params.setKeyValue(PayParamConstant.Pay_ProductName,order.getName());
        //params.setFeecodeId("1_27_1");
        params.setFeecodeId(order.getUnicomFeeId());
        params.setCustom(payComponent.createCustom(order.getOrderId(), "gddxPay"));

        payComponent.createOrder(params, callback);

    }

    @Override
    public void payMonth(Activity activity, SDKParams params, UnityOrder order, IPayCallback callback, IPayComponent payComponent) {
        params.setKeyValue(PayParams.KEY_IS_PAY_MONTH, true);
        params.setCustom(payComponent.createCustom(order.getOrderId(), "JiangxiCMCCPay"));// 透传参数
        payComponent.createOrder(params, callback);
    }
}
