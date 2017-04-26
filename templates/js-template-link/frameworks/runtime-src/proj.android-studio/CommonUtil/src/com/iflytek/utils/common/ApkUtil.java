package com.iflytek.utils.common;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

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
	 * @param context
	 * @param delayTime
	 *            延迟时间，单位：毫秒
	 */
	public static void restartApp() {
		restartApp(0);
	}
	
	/**
	 * 重启app
	 * 
	 * @param context
	 * @param delayTime
	 *            延迟时间，单位：毫秒
	 */
	public static void restartApp(long delayTime) {
		Context ctx = sContext;
		Intent mStartActivity = IchangApplication
				.getInstance()
				.getPackageManager()
				.getLaunchIntentForPackage(
						IchangApplication.getInstance().getPackageName());
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
}
