package com.iflytek.unipay;

import android.app.Activity;
import android.content.Context;
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
import com.iflytek.protobuf.ProtoTools;
import com.iflytek.ubplib.UBP;
import com.iflytek.ubplib.model.SDKParams;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by lsq on 2017/1/16.
 */

public class PayComponent implements IPayComponent {

    private static PayComponent instance;
    private boolean payStatus = false;
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

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void appInit(Context appContext, String channel) {
        initChannel(channel);
        if (payChannel == null) return;

        UBP.setDebug(true);
        UBP.init(appContext);
        payChannel.onAppInit();

        SDKParams params = new SDKParams().setContext(appContext);
        UBP.getPrepareCtrl(SDKPayType.P_SDK_Sole).prepare(params.build()
                , new IPrepareCallback() {
                    @Override
                    public void onSuccess(Map<String, Object> result) {
                        payStatus = true;
                    }

                    @Override
                    public void onFailed(Map<String, Object> error) {
                        payStatus = false;
                    }
                });
    }

    public void init(final Context appContext) {
        init(appContext, null);
    }

    public void init(final Context appContext, final PayInitCallBack payInitCallBack) {
        if (payChannel == null) return;
        SDKParams sdkParams = new SDKParams().setContext(appContext);
        payChannel.initParamUpdate(sdkParams);
        UBP.getPayCtrl(SDKPayType.P_SDK_Sole).init(sdkParams.build(), new IInitCallback() {
            @Override
            public void onSuccess(Map<String, Object> map) {
                payStatus = true;
                userId = ParamsUtils.getUserId(map);
                if (TextUtils.isEmpty(userId)) {
                    userId = PhoneUtil.getDeviceUniqueId();
                }
                Logger.log().d("userId:" + userId);
                if (payInitCallBack != null) {
                    payInitCallBack.onSuccess();
                }
            }

            @Override
            public void onFailed(Map<String, Object> map) {
                Logger.log().e("init Failed:" + map);
                payStatus = false;
                if (payInitCallBack != null) {
                    payInitCallBack.onFailed();
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
        if (payChannel == null) return;
        if (!payStatus) {
            Toast.makeText(activity, "支付模块初始化失败", Toast.LENGTH_LONG).show();
            return;
        }
        this.payMode = payMode;
        final SDKParams sdkParams = new SDKParams()
                .setActivity(activity)
                .setUserId(userId)
                .setNotify(unityOrder.isNotify());


        payChannel.pay(activity, sdkParams, unityOrder, callback, this);
    }

    public void payMonth(Activity activity, UnicomVipOrder unicomVipOrder, final IPayCallback callback, String payMode) {
        if (payChannel == null) return;
        if (!payStatus) {
            Toast.makeText(activity, "支付模块初始化失败", Toast.LENGTH_LONG).show();
            return;
        }
        this.payMode = payMode;
        final SDKParams sdkParams = new SDKParams()
                .setActivity(activity)
                .setUserId(userId);

        payChannel.payMonth(activity, sdkParams, ProtoTools.createPOJO(UnicomVipOrder.class, unicomVipOrder), callback, this);
    }
}
