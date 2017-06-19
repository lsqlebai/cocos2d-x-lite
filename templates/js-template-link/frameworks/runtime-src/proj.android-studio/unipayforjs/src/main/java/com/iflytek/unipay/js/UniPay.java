package com.iflytek.unipay.js;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;

import com.google.gson.Gson;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.unipay.AuthCallBack;
import com.iflytek.unipay.AuthComponent;
import com.iflytek.unipay.AuthResultEntity;
import com.iflytek.unipay.PayComponent;
import com.iflytek.unipay.PayInitCallBack;
import com.iflytek.unipay.UnityOrder;

import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

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
                        new Gson().fromJson(order, UnityOrder.class),
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

    public static void getInitMap() {
        synchronized (UniPay.class) {
            hasInitCallBack.set(true);
            if (initMap != null) {
                initCallBack();
            }
        }
    }
    public static void initCallBack() {
        if (hasInitCallBack.get()) {
            PayCallBack.payCallback("Init", new Gson().toJson(initMap));
        }
    }


    public static Map<String, Object> initMap = null;
    public static AtomicBoolean hasInitCallBack = new AtomicBoolean(false);
    public static void init(Activity activity) {
        PayComponent.getInstance().init(activity, new PayInitCallBack() {
            @Override
            public void onSuccess(Map<String, Object> param) {
                synchronized (UniPay.class) {
                    initMap = param;
                    initMap.put("initStatus", "success");
                    initCallBack();
                }
            }

            @Override
            public void onFailed(Map<String, Object> param) {
                synchronized (UniPay.class) {
                    initMap = param;
                    initMap.put("initStatus", "fail");
                    initCallBack();
                }

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
