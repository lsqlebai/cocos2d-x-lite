package com.iflytek.utils.io;

/**
 * Created by cllu on 4/28/15.
 */
public class NetUtils {
    public static String host2Ip(String host) {
        try {
            return java.net.InetAddress.getByName(host).getHostAddress();//得到字符串形式的ip地址
        } catch (Exception e) {
            return null;
        }
    }
}
