package com.iflytek.utils.common;

import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * @author wpfang
 * @Description 时间戳操作类分享的类
 */
public class TimestampUtil {

    private static String defaultFormat = "yyyy-MM-dd HH:mm:ss";

    /**
     * 获得当前时间戳，默认格式"yyyy-MM-dd HH:mm:ss"
     *
     * @return
     */
    public static String getCurrentTimeLable() {
        return getCurrentTimeLable(defaultFormat);
    }

    /**
     * 获得当前时间戳
     *
     * @param format 格式"yyyy-MM-dd HH:mm:ss"
     * @return
     */
    public static String getCurrentTimeLable(String format) {
        SimpleDateFormat sdf = new SimpleDateFormat(format);
        return sdf.format(getIntTimestamp());
    }

    /**
     * 获得当前时间戳
     *
     * @return
     */
    public static String getTimestamp() {
        String unixTimeGMT;
        try {
            long unixTime = System.currentTimeMillis();
            unixTimeGMT = unixTime + "";
        } catch (Exception e) {
            // TODO: handle exception
            unixTimeGMT = "";
        }
        return unixTimeGMT;

    }

    /**
     * 获得当前时间戳
     *
     * @return
     */
    public static long getIntTimestamp() {
        long unixTimeGMT = 0;
        try {
            unixTimeGMT = System.currentTimeMillis();
        } catch (Exception e) {
            // TODO: handle exception
        }
        return unixTimeGMT;

    }

    /**
     * 返回时间戳间隔
     *
     * @return
     */
    public static boolean compareTimestamp(long currentTimestap,
                                           long oldTimestap) {
        Boolean isExceed = false;
        long day = 7;
        if (gapTimestamp(currentTimestap, oldTimestap) > 86400 * day) {
            isExceed = true;
        }
        return isExceed;
    }

    public static long gapTimestamp(long currentTimestap, long oldTimestap) {
        return (currentTimestap - oldTimestap);
    }

    /**
     * 对时间戳格式进行格式化，保证时间戳长度为13位
     *
     * @param timestamp 时间戳
     * @return 返回为13位的时间戳
     */
    public static String formatTimestamp(String timestamp) {
        if (timestamp == null || "".equals(timestamp)) {
            return "";
        }
        String tempTimeStamp = timestamp + "00000000000000";
        StringBuffer stringBuffer = new StringBuffer(tempTimeStamp);
        return tempTimeStamp = stringBuffer.substring(0, 13);
    }

    /**
     * 根据 timestamp 生成各类时间状态串
     *
     * @param timestamp 距1970 00:00:00 GMT的秒数
     * @param format    格式
     * @return 时间状态串(如：刚刚5分钟前)
     */
    public static String getTimeState(String timestamp, String format) {
        if (timestamp == null || "".equals(timestamp)) {
            return "";
        }

        try {
            timestamp = formatTimestamp(timestamp);
            long _timestamp = Long.parseLong(timestamp);
            if (System.currentTimeMillis() - _timestamp < 1 * 60 * 1000) {
                return "刚刚";
            } else if (System.currentTimeMillis() - _timestamp < 30 * 60 * 1000) {
                return ((System.currentTimeMillis() - _timestamp) / 1000 / 60)
                        + "分钟前";
            } else {
                Calendar now = Calendar.getInstance();
                Calendar c = Calendar.getInstance();
                c.setTimeInMillis(_timestamp);
                if (c.get(Calendar.YEAR) == now.get(Calendar.YEAR)
                        && c.get(Calendar.MONTH) == now.get(Calendar.MONTH)
                        && c.get(Calendar.DATE) == now.get(Calendar.DATE)) {
                    SimpleDateFormat sdf = new SimpleDateFormat("今天 HH:mm");
                    return sdf.format(c.getTime());
                }
                if (c.get(Calendar.YEAR) == now.get(Calendar.YEAR)
                        && c.get(Calendar.MONTH) == now.get(Calendar.MONTH)
                        && c.get(Calendar.DATE) == now.get(Calendar.DATE) - 1) {
                    SimpleDateFormat sdf = new SimpleDateFormat("昨天 HH:mm");
                    return sdf.format(c.getTime());
                } else if (c.get(Calendar.YEAR) == now.get(Calendar.YEAR)) {
                    SimpleDateFormat sdf = null;
                    if (format != null && !format.equalsIgnoreCase("")) {
                        sdf = new SimpleDateFormat(format);

                    } else {
                        sdf = new SimpleDateFormat("M月d日 HH:mm:ss");
                    }

                    return sdf.format(c.getTime());
                } else {
                    SimpleDateFormat sdf = null;
                    if (format != null && !format.equalsIgnoreCase("")) {
                        sdf = new SimpleDateFormat(format);

                    } else {
                        sdf = new SimpleDateFormat("yyyy年M月d日 HH:mm:ss");
                    }
                    return sdf.format(c.getTime());
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }
}
