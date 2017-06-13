package com.iflytek.utils.phone;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;

import com.iflytek.app.BaseApplication;
import com.iflytek.log.Logger;
import com.iflytek.utils.common.ApiVersionUtil;
import com.iflytek.utils.common.ResourceUtil;
import com.iflytek.utils.string.StringUtil;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.regex.Pattern;

/**
 * 手机环境相关信息，存储类见storage包
 *
 * @author wpfang
 */
public class PhoneUtil {
    private static String ethMacAddress;
    private static String wlanMacAddress;

    private static final boolean isSingleCore;
    private static final boolean isLowMem;
    private static final int lowMemFlag = 512;

    private static TelephonyManager mTelMgr;

    static {
        isSingleCore = 1 == getNumCores();

        isLowMem = lowMemFlag >= getTotalMemory();
    }

    public static boolean isSingleCore() {
        return isSingleCore;
    }

    public static boolean isMemoryLessThan512MB() {
        return isLowMem;
    }

    /**
     * 获得屏幕高度
     *
     * @return
     */
    public static int getScreenHeight() {
        return ResourceUtil.getResources().getDisplayMetrics().heightPixels;
    }

    /**
     * 获得屏幕宽度
     *
     * @return
     */
    public static int getScreenWidth() {
        return ResourceUtil.getResources().getDisplayMetrics().widthPixels;
    }

    /**
     * 获得 SystemService
     *
     * @param name SystemService名称,如 Context.WIFI_SERVICE
     * @return
     */
    @SuppressWarnings("unchecked")
    public static <T> T getSystemService(String name) {
        return (T) BaseApplication.getAppInstance()
                .getApplicationContext().getSystemService(name);
    }

    /**
     * 获得机器Mac地址
     *
     * @return
     */
    public static String getWlanMacAddress() {
        if (StringUtil.isBlank(wlanMacAddress)) {
            if (ApiVersionUtil.hasGingerbread()) {
                wlanMacAddress = getMacAddress("wlan");
            } else {
                WifiManager systemService = (WifiManager) BaseApplication.getAppInstance().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
                wlanMacAddress = StringUtil.defaultString(
                        systemService.getConnectionInfo().getMacAddress(),
                        StringUtil.EMPTY);
            }
        }
        return StringUtil.defaultIfEmpty(wlanMacAddress, StringUtil.EMPTY);
    }

    public static String getEthMacAddress() {
        if (StringUtil.isBlank(ethMacAddress)) {
            if (ApiVersionUtil.hasGingerbread()) {
                ethMacAddress = getMacAddress("eth");
            } else {
                ethMacAddress = StringUtil.EMPTY;
            }
        }
        return StringUtil.defaultIfEmpty(ethMacAddress, StringUtil.EMPTY);
    }

    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public static String getMacAddress(String name) {
        if (ApiVersionUtil.hasGingerbread()) {
            try {
                Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
                while (en.hasMoreElements()) {
                    NetworkInterface networkInterface = en.nextElement();
                    byte[] mac = networkInterface.getHardwareAddress();
                    if (!networkInterface.getName().startsWith(name) || mac == null) {
                        continue;
                    }
                    return formatMac(mac);
                }
            } catch (SocketException ignored) {
            }
        }
        return null;
    }

    private static String formatMac(byte[] mac) {
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < mac.length; i++) {
            String s = Integer.toHexString(mac[i] & 0xFF);
            sb.append(s.length() == 1 ? 0 + s : s);
        }
        return sb.toString().toUpperCase();
    }

    /**
     * 获得本地连接IP
     *
     * @return
     */
    public static String getWlanIpv4Address() {
        return StringUtil.defaultIfEmpty(getIpv4Address("wlan"), StringUtil.EMPTY);
    }

    public static String getEthIpv4Address() {
        return StringUtil.defaultIfEmpty(getIpv4Address("eth"), StringUtil.EMPTY);
    }

    public static String getIpv4Address(String name) {
        try {
            Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
            while (en.hasMoreElements()) {
                NetworkInterface networkInterface = en.nextElement();
                if (!networkInterface.getName().startsWith(name)) {
                    continue;
                }
                Enumeration<InetAddress> enumIpAddr = networkInterface.getInetAddresses();
                while (enumIpAddr.hasMoreElements()) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (inetAddress instanceof Inet4Address
                            && !inetAddress.isLoopbackAddress()) {
                        return inetAddress.getHostAddress();
                    }
                }
            }
        } catch (SocketException ignored) {
        }
        return null;
    }

    public static String getIMSI() {
        if (mTelMgr == null) {
            mTelMgr = getSystemService(Context.TELEPHONY_SERVICE);
        }
        return mTelMgr.getSubscriberId();
    }

    // 获取IMEI
    public static String getIMEI() {
        if (mTelMgr == null) {
            mTelMgr = getSystemService(Context.TELEPHONY_SERVICE);
        }
        String deviceId = mTelMgr.getDeviceId();
        if (StringUtil.isBlank(deviceId)) {
            deviceId = getDeviceId(BaseApplication.globalContext());
        }
        return deviceId;
    }

    public static String getDeviceId(Context ctx) {
        return Settings.Secure.getString(ctx.getContentResolver(), "android_id");
    }

    // 获取版本号
    public static String getPkgVersionName() {
        PackageManager packageManager = BaseApplication.globalContext().getPackageManager();
        if (packageManager != null) {
            try {
                PackageInfo packageInfo = packageManager.getPackageInfo(
                        BaseApplication.globalContext().getPackageName(), 0);
                if (packageInfo != null) {
                    return StringUtil.defaultString(packageInfo.versionName);
                }
            } catch (PackageManager.NameNotFoundException e) {
                e.printStackTrace();
            }
        }
        return StringUtil.EMPTY;
    }

    /**
     * 获取包名
     *
     * @return
     */
    public static String getPackageName() {
        PackageManager packageManager = BaseApplication.globalContext().getPackageManager();
        if (packageManager != null) {
            try {
                PackageInfo packageInfo = packageManager.getPackageInfo(
                        BaseApplication.globalContext().getPackageName(), 0);
                if (packageInfo != null) {
                    return StringUtil.defaultString(packageInfo.packageName);
                }
            } catch (PackageManager.NameNotFoundException e) {
                e.printStackTrace();
            }
        }
        return StringUtil.EMPTY;
    }

    public static String getUserAgent() {
        return StringUtil.defaultIfBlank(Build.MODEL, "unknown");
    }

    /**
     * 获取CPU个数
     *
     * @return
     */
    private static int getNumCores() {
        //Private Class to display only CPU devices in the directory listing
        class CpuFilter implements FileFilter {
            @Override
            public boolean accept(File pathname) {
                //Check if filename is "cpu", followed by a single digit number
                return Pattern.matches("cpu[0-9]", pathname.getName());
            }
        }

        try {
            //Get directory containing CPU info
            File dir = new File("/sys/devices/system/cpu/");
            //Filter to only list the devices we care about
            File[] files = dir.listFiles(new CpuFilter());
            //Log.d("CPU", "CPU Count: "+files.length);
            //Return the number of cores (virtual CPU devices)
            return files.length;
        } catch (Exception e) {
            //Print exception
            Logger.log().w("CPU Count: Failed.");
            e.printStackTrace();
            //Default to return 1 core
            return 1;
        }
    }

    /**
     * 内存总大小 单位M
     *
     * @return
     */
    private static int getTotalMemory() {
        String str1 = "/proc/meminfo";// 系统内存信息文件
        String str2;
        String[] arrayOfString;
        int totalMemSize = 0;
        BufferedReader localBufferedReader = null;
        try {
            localBufferedReader = new BufferedReader(new FileReader(str1), 8192);
            // 读取meminfo第一行，系统总内存大小
            str2 = localBufferedReader.readLine();
            // MemTotal: 962808 kB
            arrayOfString = str2.split("\\s+");
            // 获得系统总内存，默认单位是KB，改为M
            totalMemSize = Integer.valueOf(arrayOfString[1]).intValue() / 1024;
        } catch (Exception ignored) {
        } finally {
            if (localBufferedReader != null) {
                try {
                    localBufferedReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return totalMemSize;// Byte转换为KB或者MB，内存大小规格化
    }
}
