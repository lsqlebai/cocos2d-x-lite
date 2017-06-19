package com.iflytek.unipay;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.res.Configuration;
import android.text.TextUtils;
import android.widget.Toast;

import com.google.gson.Gson;
import com.iflytek.pay.SDKPayType;
import com.iflytek.pay.ubp.commonutils.log.Logger;
import com.iflytek.pay.ubp.commonutils.phone.PhoneUtil;
import com.iflytek.pay.ubpcallback.IInitCallback;
import com.iflytek.pay.ubpcallback.IPayCallback;
import com.iflytek.pay.ubpcallback.IPrepareCallback;
import com.iflytek.pay.utils.ParamsUtils;
import com.iflytek.ubplib.UBP;
import com.iflytek.ubplib.model.SDKParams;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by lsq on 2017/1/16.
 */

public class PayComponent implements IPayComponent {

    private static PayComponent instance;
    private PayStatus payStatus = new PayStatus();
    private String userId;
    private PayChannel payChannel;
    private String payMode;
    private String channel;

    public static PayComponent getInstance() {
        if (null == instance) {
            instance = new PayComponent();
        }
        return instance;
    }

    private PayComponent() {

    }

    public void initChannel(String channel) {

        this.channel = channel;
        Class classType = null;
        try {
            classType = Class.forName("com.iflytek.payComponent.PayChannelImpl");
            payChannel = (PayChannel) classType.newInstance();
            Logger.log().i("init channel success!");
        } catch (Exception e) {
            Logger.log().i("init channel fail!");
            e.printStackTrace();
        }

    }

    public void appInit(Context appContext, String channel) {
        Logger.log().i("app init success!");
        initChannel(channel);
        if (payChannel == null) return;
        payChannel.onAppInit();
        Logger.log().i("payChannel.onAppInit");
        UBP.init(appContext);
        UBP.setDebug(true);
        Logger.log().i("UBP.init!");

        SDKParams params = new SDKParams().setContext(appContext);
        Logger.log().i("UBP.getPrepareCtrl" + params.build());
        UBP.getPrepareCtrl(SDKPayType.P_SDK_Sole).prepare(params.build()
                , new IPrepareCallback() {
                    @Override
                    public void onSuccess(Map<String, Object> result) {
                        Logger.log().i("UBP.getPrepareCtrl success");
                        payStatus.appInit(true);
                    }

                    @Override
                    public void onFailed(Map<String, Object> error) {
                        Logger.log().i("UBP.getPrepareCtrl fail");
                        payStatus.appInit(false);
                    }
                });
    }

    public void init(final Context appContext) {
        init(appContext, null);
    }

    public void init(final Context appContext, final PayInitCallBack payInitCallBack) {
        Logger.log().i("init");
        if (payChannel == null) return;
        SDKParams sdkParams = new SDKParams().setContext(appContext);
        Logger.log().i("sdkParams" + sdkParams.build().toString());
        payChannel.initParamUpdate(sdkParams);
        Logger.log().i("sdkParams initParamUpdate" + sdkParams.build().toString());
        UBP.getPayCtrl(SDKPayType.P_SDK_Sole).init(sdkParams.build(), new IInitCallback() {
            @Override
            public void onSuccess(Map<String, Object> map) {
                Logger.log().i("init onSuccess" + map.toString());
                payStatus.payInit(true);
                userId = ParamsUtils.getUserId(map);
                if (TextUtils.isEmpty(userId)) {
                    Logger.log().i("userId isEmpty");
                    userId = PhoneUtil.getDeviceUniqueId();
                }
                Logger.log().d("userId:" + userId);
                if (payInitCallBack != null) {
                    payInitCallBack.onSuccess(map);
                }
            }

            @Override
            public void onFailed(Map<String, Object> map) {
                Logger.log().e("init Failed:" + map);
                payStatus.payInit(false);
                if (payInitCallBack != null) {
                    payInitCallBack.onFailed(map);
                }
            }
        });
    }


    public void createOrder(SDKParams sdkParams, IPayCallback callback) {
        if (payChannel == null) return;
        UBP.getPayCtrl(SDKPayType.P_SDK_Sole).pay(sdkParams.build(), callback);
    }

    /**
     * 创建透传参数 透传给业务端
     *
     * @return
     */
    public String createCustom(final String orderId, final String payType) {
        if (payChannel == null) return null;
        HashMap<String, String> data = new HashMap<>();
        data.put("orderId", orderId);
        data.put("payType", payType);
        data.put("channel", channel);
        data.put("payMode", payMode);
        return new Gson().toJson(data);
    }


    public void release() {
        if (payChannel == null) return;
        UBP.getPayCtrl(SDKPayType.P_SDK_Sole).release(ParamsUtils.createEmptyParams());
    }

    public void pay(final Activity activity, final UnityOrder unityOrder, final IPayCallback callback, String payMode) {
        Logger.log().i("pay:" + unityOrder.toString());
        if (payChannel == null) return;
        if (!payStatus.canPay()) {
            Logger.log().e("payStatus error:" + payStatus);
            Toast.makeText(activity, "支付模块初始化失败", Toast.LENGTH_LONG).show();
            return;
        }
        this.payMode = payMode;
        final SDKParams sdkParams = new SDKParams()
                .setActivity(activity)
                .setUserId(userId)
                .setNotify(unityOrder.isNotify());

        Logger.log().i("pay sdkParams:" + sdkParams.build().toString());
        payChannel.pay(activity, sdkParams, unityOrder, callback, this);
    }

    public void payMonth(Activity activity, UnityOrder unicomVipOrder, final IPayCallback callback, String payMode) {
        Logger.log().i("payMonth:" + unicomVipOrder.toString());
        if (payChannel == null) return;
        if (!payStatus.canPay()) {
            Logger.log().e("payStatus error:" + payStatus);
            Toast.makeText(activity, "支付模块初始化失败", Toast.LENGTH_LONG).show();
            return;
        }
        this.payMode = payMode;
        final SDKParams sdkParams = new SDKParams()
                .setActivity(activity)
                .setUserId(userId)
                .setNotify(unicomVipOrder.isNotify());
        Logger.log().i("payMonth sdkParams:" + sdkParams.build().toString());
        payChannel.payMonth(activity, sdkParams, unicomVipOrder, callback, this);
    }

    public void attachBaseContext(Application application, Context base) {
        //UBP.getAppCtrl().attachBaseContext(application, base);
    }

    public void onConfigChanged(Configuration newConfig) {
        UBP.getAppCtrl().onConfigurationChanged(newConfig);
    }

    public void onLowMemory() {
        UBP.getAppCtrl().onLowMemory();
    }
}
