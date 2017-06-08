package com.iflytek.unipay.js;

import android.os.Handler;
import android.os.Looper;

import com.google.gson.Gson;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.unipay.AuthCallBack;
import com.iflytek.unipay.AuthComponent;
import com.iflytek.unipay.AuthResultEntity;
import com.iflytek.unipay.PayComponent;
import com.iflytek.unipay.UnicomVipOrder;
import com.iflytek.unipay.UnityOrder;
import com.iflytek.utils.common.ToastUtil;

import java.util.Map;

/**
 * Created by lsq on 2017/1/22.
 */
public class UniPay {
    static Handler MainThreadHandler = new Handler(Looper.getMainLooper());

    public static void pay(final String order, final String payMode) {
        MainThreadHandler.post(new Runnable() {
            @Override
            public void run() {
                PayComponent.getInstance().pay(
                        CocoActivityHelper.getActivity(),
                        new Gson().fromJson(order, UnityOrder.class),
                        new Callback(),
                        payMode
                );
            }
        });

    }

    public static void payMonth(final String order, final String payMode) {
        MainThreadHandler.post(new Runnable() {
            @Override
            public void run() {
                PayComponent.getInstance().payMonth(
                        CocoActivityHelper.getActivity(),
                        new Gson().fromJson(order, UnicomVipOrder.class),
                        new Callback(),
                        payMode
                );
            }
        });

    }

    public static void checkAuth(final String productId) {
        MainThreadHandler.post(new Runnable() {
            @Override
            public void run() {
                AuthComponent.getInstance().checkAuthPermission(productId, new AuthCallBack() {
                            @Override
                            public void checkReuslt(AuthResultEntity authResultEntity) {
                                PayCallBack.payCallback("Auth", new Gson().toJson(authResultEntity));
                            }
                        }
                );
            }
        });

    }


    static class Callback implements IPayCallback {

        @Override
        public void onSuccess(Map<String, Object> map) {
            PayCallBack.payCallback("Pay", "success");
        }

        @Override
        public void onFailed(Map<String, Object> map) {
            PayCallBack.payCallback("Pay", "fail");
        }

        @Override
        public void onCancel(Map<String, Object> map) {
            PayCallBack.payCallback("Pay", "cancel");
        }
    }

}
