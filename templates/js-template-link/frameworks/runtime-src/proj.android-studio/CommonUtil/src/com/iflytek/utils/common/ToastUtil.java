package com.iflytek.utils.common;

import android.util.DisplayMetrics;
import android.widget.Toast;

import com.iflytek.app.BaseApplication;
import com.iflytek.log.Logger;
import com.iflytek.utils.R;
import com.iflytek.utils.string.StringUtil;

/**
 * @author wpfang
 */
public class ToastUtil {

    public static void toast(int resId) {
        toast(ResourceUtil.getString(resId));
    }

    public static void toast(String msg) {
        int showTime = Toast.LENGTH_SHORT;
        if (msg != null && msg.length() > 5) {
            showTime = Toast.LENGTH_LONG;
        }
        Toast.makeText(BaseApplication.globalContext(),
                msg, showTime).show();
    }


    /*TV端 Toast工具*/
    public static void toastOnTV(int resId, boolean isOverride) {
        toastOnTV(ResourceUtil.getString(resId), isOverride);
    }

    /*TV端 Toast工具*/
    public static void toastOnTV(String msg, boolean isOverride) {
        if (!isOverride && IsShowing()) {
            return;
        }
        toastOnTV(msg);
    }
    public static void toastFailedOnTV(boolean hasToasted) {
        if (!hasToasted) {
            toastOnTV(ResourceUtil.getString(R.string.response_fail));
        }
    }


    /*TV端 Toast工具*/
    public static void toastOnTV(int resId) {
        toastOnTV(ResourceUtil.getString(resId));
    }

    /*TV端 Toast工具*/
    public static void toastOnTV(String msg) {
        Logger.getLogger(ToastUtil.class).d("msg : " + msg + "time: " + System.currentTimeMillis());
        int showTime = Toast.LENGTH_SHORT;
        if (msg != null && msg.length() > 5) {
            showTime = Toast.LENGTH_LONG;
        }
        toastOnTV(msg, showTime);
        prevShowTime = System.currentTimeMillis();
    }

    private static Toast sToast;
    private static String prev;
    private static long prevShowTime = 0;

    public static void toastOnTV(final String msg, final int showTime) {
        if (StringUtil.equals(prev, msg) && sToast != null) {
            sToast.cancel();
        }
        prev = msg;

        DelayHandle.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (sToast == null) {
                    synchronized (ToastUtil.class) {
                        if (sToast == null) {
                            sToast = Toast.makeText(BaseApplication.globalContext(), "", Toast.LENGTH_LONG);

//                            sToast.setGravity(Gravity.BOTTOM, 0, 98);
//
//                            TextView tv = new TextView(BaseApplication.globalContext());
//
//                            tv.setBackgroundColor(0xe6000000);
//                            if (getScreenWidthInDp() >= 720) {
//                                tv.setPadding(20, 10, 20, 10);
//                                tv.setTextSize(22);
//                            } else {
//                                tv.setPadding(15, 7, 15, 7);
//                                tv.setTextSize(17);
//                            }
//                            tv.setTextColor(Color.WHITE);
//
//                            sToast.setView(tv);
                        }
                    }
                }

                //TextView tv = (TextView) sToast.getView();
                //tv.setText(msg);
                sToast.setText(msg);
                sToast.setDuration(showTime);
                sToast.show();

//                if (sToast != null && StringUtil.equals(prev, msg)) {
//                    sToast.cancel();
//                }
//                prev = msg;
//                sToast = new Toast(BaseApplication.globalContext());
//                sToast.setGravity(Gravity.BOTTOM, 0, 98);
//
//                TextView tv = new TextView(BaseApplication.globalContext());
//
//                tv.setBackgroundColor(0xe6000000);
//                if (getScreenWidthInDp() >= 720) {
//                    tv.setPadding(20, 10, 20, 10);
//                    tv.setTextSize(22);
//                } else {
//                    tv.setPadding(15, 7, 15, 7);
//                    tv.setTextSize(17);
//                }
//                tv.setTextColor(Color.WHITE);
//                tv.setText(msg);
//                sToast.setView(tv);
//                sToast.setDuration(showTime);
//                sToast.show();
//                prevShowTime = android.os.SystemClock.elapsedRealtime();
            }
        });
    }

    public static boolean IsShowing() {
        if (sToast != null && System.currentTimeMillis() - prevShowTime < 4000) {
            return true;
        }
        return false;
    }

    private static int getScreenWidthInDp() {
        DisplayMetrics dm = BaseApplication.globalContext().getResources().getDisplayMetrics();
        int width = Math.min(dm.heightPixels, dm.widthPixels);
        return (int) (width / dm.density + 0.5);
    }

}
