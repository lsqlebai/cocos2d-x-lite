package com.iflytek.config;

import android.content.Context;

import com.iflytek.app.BaseApplication;
import com.iflytek.utils.common.ResourceUtil;

import java.io.*;
import java.util.Properties;

/**
 * 提供基于Properties的配置器方案
 *
 * @author wpfang
 */
public class ProtertiesConfig implements IConfig {
    private Properties mProperties;
    private String configname;
    private Context context;

    /**
     * 默认构造，创建空的配置项
     */
    public ProtertiesConfig() {
        mProperties = new Properties();
    }

    /**
     * 根据rawResId创建配置项
     */
    public ProtertiesConfig(int rawResId) {
        this(ResourceUtil.getResources().openRawResource(rawResId));
    }

    /**
     * 根据openRawResource创建配置项
     */
    public ProtertiesConfig(InputStream openRawResource) {
        mProperties = new Properties();
        try {
            mProperties.load(openRawResource);
            openRawResource.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 打开配置项文件configname，若文件存在将覆盖构造中给的默认值
     */
    @Override
    public boolean open(String configname) {
        this.configname = configname;
        this.context = BaseApplication.getAppInstance().getApplicationContext();
        File configFile = getConfigFile(configname);
        if (configFile.exists()) {
            try {
                FileInputStream fileInputStream = new FileInputStream(configFile);
                mProperties.load(fileInputStream);
                fileInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
                return false;
            }
        }
        return true;
    }

    /**
     * 获得配置项的保存文件，可由子类覆盖为SD上的文件
     *
     * @param configname
     * @return
     */
    protected File getConfigFile(String configname) {
        return context.getFileStreamPath(configname);
    }

    /**
     * 关闭配置器，保存配置项到文件configname中
     */
    @Override
    public boolean close() {
        File configFile = getConfigFile(configname);
        try {
            if (!configFile.exists()) {
                configFile.createNewFile();
            }
            FileOutputStream outputStream = context.openFileOutput(configname, Context.MODE_PRIVATE);
            mProperties.store(outputStream, null);
            outputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    @Override
    public boolean clear() {
        mProperties.clear();
        return true;
    }

    @Override
    public boolean putString(String key, String value) {
        mProperties.put(key, value);
        return true;
    }

    @Override
    public boolean putInt(String key, int value) {
        mProperties.put(key, String.valueOf(value));
        return true;
    }

    @Override
    public boolean putBoolean(String key, Boolean value) {
        mProperties.put(key, String.valueOf(value));
        return true;
    }

    @Override
    public String getString(String key, String defaultValue) {
        return mProperties.getProperty(key, defaultValue);
    }

    @Override
    public int getInt(String key, int defaultValue) {
        try {
            return Integer.parseInt(mProperties.getProperty(key));
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
        return defaultValue;
    }

    @Override
    public boolean getBoolean(String key, Boolean defaultValue) {
        try {
            return Boolean.valueOf(mProperties.getProperty(key));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return defaultValue;
    }

    @Override
    public boolean remove(String key) {
        return mProperties.remove(key) != null;
    }
}
