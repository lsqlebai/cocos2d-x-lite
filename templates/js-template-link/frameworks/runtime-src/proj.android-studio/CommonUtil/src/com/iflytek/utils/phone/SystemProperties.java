package com.iflytek.utils.phone;

import java.lang.reflect.Method;

public class SystemProperties {
    // String SystemProperties.get(String key){}
    public static String set(String key,String value) {
        try {
            mSetMethod.invoke(mClassType, key,value);
        } catch (Exception ignored) {
        }
        return value;
    }

    //int SystemProperties.get(String key, int def){}
    public static int getInt(String key, int def) {
        int value = def;
        try {
            value = (Integer) mGetIntMethod.invoke(mClassType, key, def);
        } catch (Exception ignored) {
        }
        return value;
    }

    //int SystemProperties.get(String key, int def){}
    public static String getString(String key, String def) {
        try {
            return (String) mGetTextMethod.invoke(mClassType, key, def);
        } catch (Exception ignored) {
        }
        return null;
    }

    //-------------------------------------------------------------------
    private static Class<?> mClassType;
    private static Method mSetMethod;
    private static Method mGetIntMethod;
    private static Method mGetTextMethod;

    static {
        try {
            mClassType = Class.forName("android.os.SystemProperties");
            mSetMethod = mClassType.getDeclaredMethod("set", String.class, String.class);
            mGetIntMethod = mClassType.getDeclaredMethod("getInt", String.class, int.class);
            mGetTextMethod = mClassType.getDeclaredMethod("get", String.class, String.class);
        } catch (Exception e) {
            mClassType = null;
            mSetMethod = null;
            mGetIntMethod = null;
            mGetTextMethod = null;
        }
    }
}
