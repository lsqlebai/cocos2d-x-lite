package com.iflytek.utils.storage;

import android.annotation.TargetApi;
import android.app.ActivityManager;
import android.content.Context;
import android.os.Build;
import android.os.Environment;
import com.iflytek.utils.common.ApiVersionUtil;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @Description 缓存的工具类, Android 2.2以上版本使用
 * @author wpfang
 */
@TargetApi(9)
public class ExternalUtils {
    /**
     * 判断是否存在外部存储设备
     *
     * @return 如果不存在返回false
     */
    public static boolean hasExternalStorage() {
        Boolean externalStorage = Environment.getExternalStorageState().equals(
                android.os.Environment.MEDIA_MOUNTED);
        return externalStorage;
    }

    /**
     * 获取目录使用的空间大小
     *
     * @param path 检查的路径路径
     * @return 在字节的可用空间
     */
    public static long getUsableSpace(File path) {
            return path.getUsableSpace();
    }

    /**
     * 获得外部应用程序缓存目录
     *
     * @param context 上下文信息
     * @return 外部缓存目录
     */
    public static File getExternalCacheDir(Context context) {
        return context.getExternalCacheDir();
    }

    /**
     * 检查如果外部存储器是内置的或是可移动的。
     *
     * @return 如果外部存储是可移动的(就像一个SD卡)返回为 true,否则false。
     */
    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public static boolean isExternalStorageRemovable() {
        return ApiVersionUtil.hasGingerbread() && Environment.isExternalStorageRemovable();
    }

    /**
     * 一个散列方法,改变一个字符串(如URL)到一个散列适合使用作为一个磁盘文件名。
     */
    public static String hashKeyForDisk(String key) {
        String cacheKey;
        try {
            final MessageDigest mDigest = MessageDigest.getInstance("MD5");
            mDigest.update(key.getBytes());
            cacheKey = bytesToHexString(mDigest.digest());
        } catch (NoSuchAlgorithmException e) {
            cacheKey = String.valueOf(key.hashCode());
        }
        return cacheKey;
    }

    private static String bytesToHexString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < bytes.length; i++) {
            String hex = Integer.toHexString(0xFF & bytes[i]);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    /**
     * 得到一个可用的缓存目录(如果外部可用使用外部,否则内部)。
     *
     * @param context    上下文信息
     * @param uniqueName 目录名字
     * @return 返回目录名字
     */
    public static File getDiskCacheDir(Context context, String uniqueName) {
        // 检查是否安装或存储媒体是内置的,如果是这样,试着使用
        // 外部缓存 目录
        // 否则使用内部缓存 目录
        try {
            final String cachePath = Environment.MEDIA_MOUNTED.equals(Environment
                    .getExternalStorageState()) || !isExternalStorageRemovable() ? getExternalCacheDir(
                    context).getPath()
                    : context.getCacheDir().getPath();

            return new File(cachePath + File.separator + uniqueName);
        } catch (Exception e) {
            return new File(context.getCacheDir().getPath() + File.separator + uniqueName);
        }
    }

    /**
     * 得到一个可用的缓存目录(如果外部可用使用外部,否则内部)。
     *
     * @param context    上下文信息
     * @return 返回目录名字
     */
    public static File getSystemDiskCacheDir(Context context) {
        // 检查是否安装或存储媒体是内置的,如果是这样,试着使用
        // 外部缓存 目录
        // 否则使用内部缓存 目录
        try {
            final String cachePath = Environment.MEDIA_MOUNTED.equals(Environment
                    .getExternalStorageState()) || !isExternalStorageRemovable() ? getExternalCacheDir(
                    context).getPath()
                    : context.getCacheDir().getPath();

            return new File(cachePath);
        } catch (Exception e) {
            return new File(context.getCacheDir().getPath());
        }
    }

    public static int getMemoryClass(Context context) {
        return ((ActivityManager) context
                .getSystemService(Context.ACTIVITY_SERVICE)).getMemoryClass();
    }

}
