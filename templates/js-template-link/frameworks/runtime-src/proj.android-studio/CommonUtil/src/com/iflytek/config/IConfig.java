package com.iflytek.config;

/**
 * @author wpfang
 */
public interface IConfig {
    boolean open(String configname);

    boolean close();

    /**
     * 清除所有配置
     */
    boolean clear();

    /**
     * 设置String类型的配置值
     */
    boolean putString(String key, String value);

    /**
     * 设置int类型的配置值
     */
    boolean putInt(String key, int value);

    /**
     * 设置Boolean类型的配置值
     */
    boolean putBoolean(String key, Boolean value);

    /**
     * 返回String类型的配置值
     */
    String getString(String key, String defaultValue);

    /**
     * 返回int类型的配置值
     */
    int getInt(String key, int defaultValue);

    /**
     * 返回Boolean类型的配置值
     */
    boolean getBoolean(String key, Boolean defaultValue);

    /**
     * 删除配置值
     * @param key
     */
    boolean remove(String key);
}
