package com.iflytek.cache;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;

import com.iflytek.utils.common.ApiVersionUtil;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class DiskCacheUtil {
    /**
     * Get a usable cache directory (external if available, internal otherwise).
     *
     * @param context    The context to use
     * @param uniqueName A unique directory name to append to the cache dir
     * @return The cache dir
     */
    public static File getDiskCacheDir(Context context, String uniqueName) {
        // Check if media is mounted or storage is built-in, if so, try and use external cache dir
        // otherwise use internal cache dir
        String cachePath;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState()) || !Environment.isExternalStorageRemovable()) {
            File cacheDir = context.getExternalFilesDir(null);
            if (cacheDir != null) {
                cachePath = context.getExternalFilesDir(null).getPath();
            } else {
                cachePath = context.getFilesDir().getPath();
            }
        } else {
            cachePath = context.getFilesDir().getPath();
        }
        return new File(cachePath + File.separator + uniqueName);
//        try {
//            final String cachePath;
//            if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState()) || !isExternalStorageRemovable()){
//                cachePath = getExternalCacheDir(context).getPath();
//            }else {
//                cachePath = context.getFilesDir().getPath();
//            }
//            return new File(cachePath + File.separator + uniqueName);
//        } catch (Exception e) {
//            return new File(Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + uniqueName);
//        }
    }

    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public static boolean isExternalStorageRemovable() {
        return ApiVersionUtil.hasGingerbread() && Environment.isExternalStorageRemovable();
    }

    /**
     * A hashing method that changes a string (like a URL) into a hash suitable for using as a
     * disk filename.
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
        // http://stackoverflow.com/questions/332079
        StringBuilder sb = new StringBuilder();
        for (byte aByte : bytes) {
            String hex = Integer.toHexString(0xFF & aByte);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    /**
     * Get the external app cache directory.
     *
     * @param context The context to use
     * @return The external cache dir
     */
    @TargetApi(Build.VERSION_CODES.FROYO)
    public static File getExternalCacheDir(Context context) {
        if (ApiVersionUtil.hasFroyo()) {
            return context.getExternalFilesDir(null);
        }
        final String cacheDir = "/Android/data/" + context.getPackageName() + "/files/";
        return new File(Environment.getExternalStorageDirectory().getPath() + cacheDir);
    }

    /**
     * Check how much usable space is available at a given path.
     *
     * @param path The path to check
     * @return The space available in bytes
     */
    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    @SuppressWarnings("deprecation")
    public static long getUsableSpace(File path) {
        try {
            if (ApiVersionUtil.hasGingerbread()) {
                return path.getUsableSpace();
            }
            final StatFs stats = new StatFs(path.getPath());
            return (long) stats.getBlockSize() * (long) stats.getAvailableBlocks();
        } catch (Exception e) {
            return 0;
        }
    }

}
