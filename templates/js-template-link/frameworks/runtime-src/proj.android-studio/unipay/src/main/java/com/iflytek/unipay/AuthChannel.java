package com.iflytek.unipay;

/**
 * Created by Administrator on 2017/2/6.
 */

public interface AuthChannel {
    void checkAuthPermission(String productId,AuthCallBack callBack);
}