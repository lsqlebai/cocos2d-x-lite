package com.iflytek.utils.lang;

import com.iflytek.utils.string.StringUtil;

import java.text.DecimalFormat;

public class NumberUtil {
    public static final int parseInt(String val, int defVal) {
        int ret;
        if (val == null) {
            return 0;
        }
        try {
            ret = java.lang.Integer.parseInt(val);
        } catch (NumberFormatException e) {
            ret = defVal;
        }
        return ret;
    }

    /**
     * 万
     *
     * @param data
     * @return
     */
    public static String dataFormat(String data) {
        if (StringUtil.isNotBlank(data)) {
            double d = Double.parseDouble(data);
            if (d > 10000) {
                return dataFormat("0.0", d / 10000) + "万";
            } else {
                return (int) d + "";
            }
        } else {
            return "";
        }
    }

    /**
     * 数据格式化（0和#都是占位符，不同的是用0表示占位符，不足的地方会用0补上，而#不会）
     *
     * @param data
     * @return
     */
    public static String dataFormat(String pattern, Object data) {
        DecimalFormat decimalFormat = new DecimalFormat(pattern);
        String dataString = decimalFormat.format(data);
        return dataString;
    }
}
