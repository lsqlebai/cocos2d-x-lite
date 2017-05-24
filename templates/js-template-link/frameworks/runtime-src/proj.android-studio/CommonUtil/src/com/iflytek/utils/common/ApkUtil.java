package com.iflytek.utils.common;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Build;

import com.iflytek.app.BaseApplication;

import java.io.File;

public class ApkUtil {
    // Android获取一个用于打开APK文件的intent
    public static Intent getApkFileIntent(String filePath) {
        Intent intent = new Intent();
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(Intent.ACTION_VIEW);
        Uri uri = Uri.fromFile(new File(filePath));
        intent.setDataAndType(uri, "application/vnd.android.package-archive");
        return intent;
    }

    public static void startInstallApk(String apkPath) {
        Intent intent = ApkUtil.getApkFileIntent(apkPath);
        /* 使用Application的Context在切换界面时有黑屏情况 */
        Context context = BaseApplication.getAppInstance();
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

        context.startActivity(intent);
    }
	
	private static Context sContext;
	
	public static void init(Context context)
	{
		sContext = context;
	}
	
	
	/**
	 * 重启app
	 * 
	 */
	public static void restartApp() {
		restartApp(0);
	}
	
	/**
	 * 重启app
	 * 
	 * @param delayTime
	 *            延迟时间，单位：毫秒
	 */
	public static void restartApp(long delayTime) {
		Context ctx = sContext;
		Intent mStartActivity = ctx
				.getPackageManager()
				.getLaunchIntentForPackage(
						ctx.getPackageName());
		mStartActivity.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		mStartActivity.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
			mStartActivity.addFlags(0x8000); // 相当于Intent.FLAG_ACTIVITY_CLEAR_TASK
		int mPendingIntentId = 123456;
		PendingIntent mPendingIntent = PendingIntent.getActivity(ctx,
				mPendingIntentId, mStartActivity,
				PendingIntent.FLAG_CANCEL_CURRENT);
		AlarmManager mgr = (AlarmManager) ctx
				.getSystemService(Context.ALARM_SERVICE);
		mgr.set(AlarmManager.RTC, System.currentTimeMillis() + delayTime,
				mPendingIntent);

		if (ctx instanceof Activity) {
			((Activity) ctx).moveTaskToBack(true); // 切换到后台
		}
		 android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}

	/**
	 * 是否是平板设备
	 * @return
     */
	public static boolean isPad() {
		Context context = sContext;
		return (context.getResources().getConfiguration().screenLayout
				& Configuration.SCREENLAYOUT_SIZE_MASK)
				>= Configuration.SCREENLAYOUT_SIZE_LARGE;
	}

	/**
	 * 设备是否支持多点触摸
	 * @return
     */
	public static boolean isSupportMultiTouch() {
		Context context = sContext;
		PackageManager pm = context.getPackageManager();
		boolean isSupportMultiTouch = pm.hasSystemFeature(PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH);
		return isSupportMultiTouch;
	}

	public static String getAppVersionName()
	{
		try
		{
			// 获取packagemanager的实例
			PackageManager packageManager = sContext.getPackageManager();
			// getPackageName()是你当前类的包名，0代表是获取版本信息
			PackageInfo packInfo = packageManager.getPackageInfo(sContext.getPackageName(),0);
			String version = packInfo.versionName;
			return version;
		}catch (Exception e)
		{
			e.printStackTrace();
		}
		return "";
	}

	public static int getAppVersionCode()
	{
		try
		{
			// 获取packagemanager的实例
			PackageManager packageManager = sContext.getPackageManager();
			// getPackageName()是你当前类的包名，0代表是获取版本信息
			PackageInfo packInfo = packageManager.getPackageInfo(sContext.getPackageName(),0);
			return packInfo.versionCode;
		}catch (Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
}
