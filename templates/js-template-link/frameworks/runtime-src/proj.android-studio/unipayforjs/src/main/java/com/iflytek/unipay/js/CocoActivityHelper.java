package com.iflytek.unipay.js;

import android.app.Activity;

/**
 * Created by lsq on 2017/1/22.
 */
public class CocoActivityHelper {

    public static Activity getActivity() {
        return activity;
    }

    public static void setActivity(Activity activity) {

        CocoActivityHelper.activity = activity;

    }

    static Activity activity;

}
