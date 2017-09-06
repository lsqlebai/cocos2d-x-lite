
/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.iflytek.leagueofglutton;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.widget.Toast;

import com.iflytek.leagueofglutton.components.LocalSocketServerService;
import com.iflytek.leagueofglutton.dex.DexLoaderManager;
import com.iflytek.unipay.PayComponent;
import com.iflytek.unipay.js.CocoActivityHelper;
import com.iflytek.unipay.js.UniPay;
import com.iflytek.utils.common.ApkUtil;
import com.iflytek.utils.common.FileUtil;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.mobclick.game.MobClickCppHelper;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.nio.channels.FileChannel;

// For JS and JAVA reflection test, you can delete it if it's your own project

public class AppActivity extends Cocos2dxActivity {

    boolean isAppUpgrade = false; // 程序是否发生过升级

    private static final String KEY_APP_VERSION_CODE = "KEY_APP_VERSION_CODE";
    private void initInfo()
    {
        SharedPreferences sp = getPreferences(MODE_PRIVATE);
        int preCode = sp.getInt(KEY_APP_VERSION_CODE, -1);
        int curCode = ApkUtil.getAppVersionCode();
        if(preCode == -1)
        {
            sp.edit().putInt(KEY_APP_VERSION_CODE, curCode).commit(); // 保存当前程序版本号
        }
        else
        {
           if(preCode != curCode)
           {
               isAppUpgrade = true; // 程序发生过升级
               sp.edit().putInt(KEY_APP_VERSION_CODE, curCode).commit(); // 保存当前程序版本号
           }
        }
        System.out.println("isAppUpgrade:" + isAppUpgrade);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // 联通渠道，设置代理
        if("018TJLT".equals(MainApplication.channel))
        {
            System.setProperty("http.proxySet", "true");    //设置使用网络代理
            System.setProperty("http.proxyHost", "202.99.114.28");  //设置代理服务器地址
            System.setProperty("http.proxyPort", "10011");    //设置代理服务器端口号

            // 针对https也开启代理
            System.setProperty("https.proxyHost", "202.99.114.28");
            System.setProperty("https.proxyPort", "10011");
        }

        ApkUtil.init(this);
        initInfo();
        super.onCreate(savedInstanceState);

        CocoActivityHelper.setActivity(this);

        if("018JXYD".equals(MainApplication.channel))
        {
            Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    UniPay.init(AppActivity.this); // 初始化支付
                }
            }, 1000);
        }else {
            UniPay.init(AppActivity.this); // 初始化支付
        }

        // 初始化友盟统计
        MobClickCppHelper.init(this,"59506351bbea835a61000f4f", MainApplication.channel);

        MobclickAgent.setCatchUncaughtExceptions(true);
		startService(new Intent(this, LocalSocketServerService.class));
    }

    /**
     * 上报错误日志到友盟
     * @param errorMsg
     */
    public static void reportError(String errorMsg)
    {
        System.out.println("errorMsg:" + errorMsg);
        MobclickAgent.reportError(MainApplication.geContext(), errorMsg);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        PayComponent.getInstance().release();
    }

    @Override
    protected void onResume() {
        super.onResume();
        MobClickCppHelper.onResume(this);
        MobclickAgent.onResume(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        MobClickCppHelper.onPause(this);
        MobclickAgent.onPause(this);
    }

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // TestCpp should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    public static void copyFile(File srcFile, File destFile) throws IOException {

        System.out.println("doLib do copy file:" + srcFile.getAbsolutePath() + "," + destFile.getAbsolutePath());

        if (!destFile.exists()) {
            destFile.createNewFile();
        }

        FileChannel source = null;
        FileChannel destination = null;

        try {
            source = new FileInputStream(srcFile).getChannel();
            destination = new FileOutputStream(destFile).getChannel();
            destination.transferFrom(source, 0, source.size());
        }catch (Exception e)
        {
            e.printStackTrace();
        }
        finally {
            if (source != null) {
                source.close();
            }
            if (destination != null) {
                destination.close();
            }
        }
    }

    /**
     * 拷贝so库
     */
    private void copyNewLib()
    {

        System.out.println("doLib copyNewLib");
        String downloadPath = null;

        File downloadFile = getExternalFilesDir("download");

        if(downloadFile == null)
        {
            System.out.println("downloadFile is null");

            try
            {
                File tmpDir = new File("/sdcard/Android/data/" +getPackageName() +"/files/download");

                if(tmpDir.exists())
                {
                    downloadPath = tmpDir.getAbsolutePath();
                }
                else
                {
                    boolean mkResult = tmpDir.mkdirs();
                    if(mkResult)
                    {
                        System.out.println("downloadFile is create success");
                        downloadPath = tmpDir.getAbsolutePath();
                    } else
                    {
                        System.out.println("downloadFile is create failed");
                        Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                    }
                }
            }
            catch (Exception e)
            {
                System.out.println("downloadFile is create failed2");
                Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        } else
        {

            System.out.println("doLib downloadFile is not null");

            downloadPath = downloadFile.getAbsolutePath();

            System.out.println("doLib downloadFile path:" + downloadPath);
        }

        if(null == downloadPath)
        {
            System.out.println("downloadPath is null");
            return;
        }

        if(isAppUpgrade)
        {

            // 程序发生过升级，把之前热更新数据全部清空
            File fileDir =  new File(downloadPath);
            if(fileDir.exists())
            {
                FileUtil.Delete(fileDir);
            }
            return;
        }

        File libCopyDirFile = new File(downloadPath + "/libCopyDir");

        System.out.println("doLib libCopyDirFile:" + libCopyDirFile.getAbsolutePath());
        if(libCopyDirFile.exists() && libCopyDirFile.isFile())
        {
            BufferedReader br =  null;
            try {
                br =  new BufferedReader(new FileReader(libCopyDirFile));
                String filePath = br.readLine();

                System.out.println("doLib readLine:" + filePath);
                File libDir = new File(filePath);
                if(libDir.exists() && libDir.isDirectory())
                {
                    File[] soFiles = libDir.listFiles(new FileFilter() {
                        @Override
                        public boolean accept(File pathname) {
                            return pathname.getName().endsWith(".so");
                        }
                    });


                    if(null != soFiles && soFiles.length>0)
                    {
                        System.out.println("doLib soFiles is valid");
                        File dstDir = getFilesDir();
                        SharedPreferences sp = getPreferences(MODE_PRIVATE);

                        for(File soFile : soFiles)
                        {
                            String key = "KEY_LIB_LAST_TIME_" + soFile.getName();

                            File dstFile = new File(dstDir.getAbsolutePath() + "/" + soFile.getName());

                            System.out.println("doLib prepare copy so, soFile:" + soFile.getAbsolutePath() + ",dstFile:" + dstFile.getAbsolutePath());

                            long lastTime = sp.getLong(key, 0);
                            if(!dstFile.exists() || dstFile.length() != soFile.length() || lastTime != soFile.lastModified()) // so库发生过变化，则进行覆盖
                            {
                                copyFile(soFile, dstFile);
                                sp.edit().putLong(key, soFile.lastModified()).commit(); // 保存本次拷贝库的时间
                            }
                            else
                            {
                                System.out.println("doLib prepare copy so, no change");
                            }
                        }
                    }else
                    {
                        System.out.println("doLib soFiles is null or length is 0");
                    }
                } else
                {
                    System.out.println("doLib readLine path is not exists or is not dir");
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if(null != br)
                {
                    try {
                        br.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }else {
            System.out.println("doLib is not exists or is not file");
        }
    }

    private void copyOriLib()
    {

        String oriLibPath = getApplicationInfo().nativeLibraryDir;
        File srcDir;
        System.out.println("doLib oriLibPath:" + oriLibPath);
        if(TextUtils.isEmpty(oriLibPath))
        {
            System.out.println("doLib oriLibPath is empty");
            File tmpDir = getFilesDir();

            if(null != tmpDir)
            {
                System.out.println("doLib tmpDir is not null, path:" + tmpDir.getAbsolutePath());

                srcDir = new File(tmpDir.getParentFile().getAbsoluteFile() + "/lib");
            }
            else
            {
                System.out.println("doLib tmpDir is null");
                srcDir = new File("/data/data/" + getPackageName() + "/lib");
            }
        }
        else
        {
            System.out.println("doLib oriLibPath is valid");
            srcDir = new File(oriLibPath);
        }


        System.out.println("doLib srcDir, path:" + srcDir.getAbsolutePath());
        File dstDir = getFilesDir();

        File[] srcFiles = srcDir.listFiles(new FileFilter() {
            @Override
            public boolean accept(File file) {

                System.out.println("doLib accept file:" + file.getAbsolutePath());

                if(file.getName().indexOf("cocos2djs") != -1) // 不拷贝引擎库
                {
                    return false;
                }
                return true;
            }
        });
        if(null != srcFiles)
        {
            for(File srcFile : srcFiles)
            {
                File dstFile = new File(dstDir.getAbsolutePath() + "/" + srcFile.getName());

                System.out.println("doLib dstFile is :" + dstFile.getAbsolutePath());

                if(!dstFile.exists())
                {
                    try {

                        System.out.println("doLib dstFile is not exists, do copy");

                        copyFile(srcFile, dstFile);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else
                {
                    System.out.println("doLib dstFile is exists, no need copy");
                }
            }
        }else
        {
            System.out.println("doLib srcFiles is null");
        }
    }

    /**
     * Copy dex files from hotupdate folder to dex dir
     */
    private void copyNewDex() {
        String downloadPath = null;
        File downloadFile = getExternalFilesDir("download");
        if(downloadFile == null){
            System.out.println("downloadFile is null");
            try {
                File tmpDir = new File("/sdcard/Android/data/" +getPackageName() +"/files/download");

                if(tmpDir.exists()) {
                    downloadPath = tmpDir.getAbsolutePath();
                } else {
                    boolean mkResult = tmpDir.mkdirs();
                    if(mkResult) {
                        System.out.println("downloadFile is create success");
                        downloadPath = tmpDir.getAbsolutePath();
                    } else {
                        System.out.println("downloadFile is create failed");
                        Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                    }
                }
            } catch (Exception e) {
                System.out.println("downloadFile is create failed2");
                Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        } else {
            downloadPath = downloadFile.getAbsolutePath();
        }

        if(null == downloadPath) {
            System.out.println("downloadPath is null");
            return;
        }

//        if(isAppUpgrade) {
//
//            // 程序发生过升级，把之前热更新数据全部清空
//            File fileDir =  new File(downloadPath);
//            if(fileDir.exists())
//            {
//                FileUtil.Delete(fileDir);
//            }
//            return;
//        }

        File dexCopyDirFile = new File(downloadPath + "/dexCopyDir");
        if(dexCopyDirFile.exists() && dexCopyDirFile.isFile()) {
            BufferedReader br =  null;
            try {
                br =  new BufferedReader(new FileReader(dexCopyDirFile));
                String filePath = br.readLine();
                File dexDir = new File(filePath);
                if(dexDir.exists() && dexDir.isDirectory()) {
                    File[] dexFiles = dexDir.listFiles(new FileFilter() {
                        @Override
                        public boolean accept(File pathname) {
                            return pathname.getName().endsWith(".apk")
                                    || pathname.getName().endsWith(".properties");
                        }
                    });

                    if(null != dexFiles && dexFiles.length>0) {
                        SharedPreferences sp = getPreferences(MODE_PRIVATE);

                        for(File dexFile : dexFiles) {
                            String key = "KEY_DEX_LAST_TIME_" + dexFile.getName();
                            File dstFile = new File(
                                    DexLoaderManager.getInstance().getExtractedDexPath());
                            long lastTime = sp.getLong(key, 0);
                            if(!dstFile.exists()
                                    || dstFile.length() != dexFile.length()
                                    || lastTime != dexFile.lastModified()) {
                                copyFile(dexFile, dstFile);
                                sp.edit().putLong(key, dexFile.lastModified()).commit();
                            }
                        }
                    }
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if(null != br) {
                    try {
                        br.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    @Override
    protected void onLoadNativeLibraries() {
        System.out.println("onLoadNativeLibraries");

        File internalDir = getFilesDir();
        File externalDir = getExternalFilesDir("");

        String path1 = internalDir != null ? internalDir.getAbsolutePath() : "null";
        String path2 = internalDir != null ? externalDir.getAbsolutePath() : "null";
        System.out.println("dolib internalDir:" + path1 + ",externalDir:" + path2);

        copyOriLib();
        copyNewDex();
        copyNewLib();

        //super.onLoadNativeLibraries();

        try {
//            System.load(getFilesDir() + "/libcocos2djs.so");

            System.load(getFilesDir() + "/libiflyteknet.so");
            System.loadLibrary("cocos2djs");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
