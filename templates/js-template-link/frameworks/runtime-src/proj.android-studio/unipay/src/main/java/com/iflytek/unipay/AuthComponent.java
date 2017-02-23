package com.iflytek.unipay;

/**
 * Created by Administrator on 2017/2/6.
 */

public class AuthComponent {
    private static AuthComponent instance;

    private AuthChannel authChannel;

    public static AuthComponent getInstance() {
        if (null == instance) {
            instance = new AuthComponent();
        }
        return instance;
    }

    private AuthComponent() {
        init();
    }

    public void init() {
        Class classType = null;
        try {
            classType = Class.forName("com.iflytek.payComponent.AuthChannelImpl");
            authChannel = (AuthChannel) classType.newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 用户鉴权
     */
    public void checkAuthPermission(String productId, AuthCallBack callBack) {
        AuthResultEntity authResultEntity;
        if (authChannel == null) {
            authResultEntity = new AuthResultEntity();
            authResultEntity.result = false;
            if (callBack != null) {
                callBack.checkReuslt(authResultEntity);
            }
        } else {
            authChannel.checkAuthPermission(productId, callBack);
        }
    }
}
