package com.iflytek.unipay;

import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.ubplib.model.SDKParams;

/**
 * Created by lsq on 2017/1/16.
 */

public interface IPayComponent {
    void createOrder(SDKParams sdkParams, IPayCallback callback);
    String createCustom(final String orderId, final String payType);
}
