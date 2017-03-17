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
}
