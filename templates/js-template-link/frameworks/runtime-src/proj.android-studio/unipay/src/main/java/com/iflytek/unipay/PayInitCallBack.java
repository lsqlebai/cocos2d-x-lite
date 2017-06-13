package com.iflytek.unipay;

import java.util.Map;

/**
 * Created by Administrator on 2017/2/5.
 */

public interface PayInitCallBack {
    void onSuccess(Map<String, Object> param);
    void onFailed(Map<String, Object> param);
}
