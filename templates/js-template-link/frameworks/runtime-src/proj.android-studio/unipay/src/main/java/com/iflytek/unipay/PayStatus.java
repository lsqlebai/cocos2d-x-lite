package com.iflytek.unipay;


import com.iflytek.pay.ubp.commonutils.log.Logger;

/**
 * Created by iflytek on 2016/5/9.
 * 支付SDK状态
 */
public class PayStatus {
    private int payStatus = 0;
    public void appInit(boolean success) {
        if (success) {
            payStatus = 1;
        } else {
            Logger.log().d("PayStatus:appInit Failed");
        }
    }

    public void payInit(boolean success) {
        if (success && payStatus == 1) {
            payStatus = 2;
        } else {
            Logger.log().d("PayStatus:payInit Failed curPayStatus:" + payStatus);
        }
    }

    @Override
    public String toString() {
        return "payStatus:" + payStatus;
    }

    /**
     * 可以调用支付
     * @return
     */
    public boolean canPay() {
        return payStatus == 2;
    }
}
