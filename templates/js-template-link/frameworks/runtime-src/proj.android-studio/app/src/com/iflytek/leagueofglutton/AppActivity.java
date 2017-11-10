
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
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
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
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

// For JS and JAVA reflection test, you can delete it if it's your own project

public class AppActivity extends Cocos2dxActivity {
    private static final String TAG = "AppActivity";

    private static final boolean IS_ENABLE_LOCAL_SERVER = true; // 是否启用本地游戏服务

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
        println("isAppUpgrade:" + isAppUpgrade);

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

        if(IS_ENABLE_LOCAL_SERVER)
        {
            startService(new Intent(this, LocalSocketServerService.class));
        }
    }

    /**
     * 上报错误日志到友盟
     * @param errorMsg
     */
    public static void reportError(String errorMsg)
    {
        println("errorMsg:" + errorMsg);
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

    /**
     * 拷贝so库
     */
    private void copyNewLib()
    {

        println("doLib copyNewLib");
        String downloadPath = null;

        File downloadFile = getExternalFilesDir("download");

        if(downloadFile == null)
        {
            println("downloadFile is null");

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
                        println("downloadFile is create success");
                        downloadPath = tmpDir.getAbsolutePath();
                    } else
                    {
                        println("downloadFile is create failed");
                        Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                    }
                }
            }
            catch (Exception e)
            {
                println("downloadFile is create failed2");
                Toast.makeText(this, "系统空间不足", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        } else
        {

            println("doLib downloadFile is not null");

            downloadPath = downloadFile.getAbsolutePath();

            println("doLib downloadFile path:" + downloadPath);
        }

        if(null == downloadPath)
        {
            println("downloadPath is null");
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

        println("doLib libCopyDirFile:" + libCopyDirFile.getAbsolutePath());
        if(libCopyDirFile.exists() && libCopyDirFile.isFile())
        {
            BufferedReader br =  null;
            try {
                br =  new BufferedReader(new FileReader(libCopyDirFile));
                String filePath = br.readLine();

                println("doLib readLine:" + filePath);
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
                        println("doLib soFiles is valid");
                        File dstDir = getFilesDir();
                        SharedPreferences sp = getPreferences(MODE_PRIVATE);

                        for(File soFile : soFiles)
                        {
                            String key = "KEY_LIB_LAST_TIME_" + soFile.getName();

                            File dstFile = new File(dstDir.getAbsolutePath() + "/" + soFile.getName());

                            println("doLib prepare copy so, soFile:" + soFile.getAbsolutePath() + ",dstFile:" + dstFile.getAbsolutePath());

                            long lastTime = sp.getLong(key, 0);
                            if(!dstFile.exists() || dstFile.length() != soFile.length() || lastTime != soFile.lastModified()) // so库发生过变化，则进行覆盖
                            {
                                FileUtil.copyFile(soFile, dstFile);
                                sp.edit().putLong(key, soFile.lastModified()).commit(); // 保存本次拷贝库的时间
                            }
                            else
                            {
                                println("doLib prepare copy so, no change");
                            }
                        }
                    }else
                    {
                        println("doLib soFiles is null or length is 0");
                    }
                } else
                {
                    println("doLib readLine path is not exists or is not dir");
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
            println("doLib is not exists or is not file");
        }
    }

    private void copyOriLib()
    {

        String oriLibPath = getApplicationInfo().nativeLibraryDir;
        File srcDir;
        println("doLib oriLibPath:" + oriLibPath);
        if(TextUtils.isEmpty(oriLibPath))
        {
            println("doLib oriLibPath is empty");
            File tmpDir = getFilesDir();

            if(null != tmpDir)
            {
                println("doLib tmpDir is not null, path:" + tmpDir.getAbsolutePath());

                srcDir = new File(tmpDir.getParentFile().getAbsoluteFile() + "/lib");
            }
            else
            {
                println("doLib tmpDir is null");
                srcDir = new File("/data/data/" + getPackageName() + "/lib");
            }
        }
        else
        {
            println("doLib oriLibPath is valid");
            srcDir = new File(oriLibPath);
        }


        println("doLib srcDir, path:" + srcDir.getAbsolutePath());
        File dstDir = getFilesDir();

        File[] srcFiles = srcDir.listFiles(new FileFilter() {
            @Override
            public boolean accept(File file) {

                println("doLib accept file:" + file.getAbsolutePath());

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

                println("doLib dstFile is :" + dstFile.getAbsolutePath());

                if(!dstFile.exists() || isAppUpgrade) // 文件不存在或者发生过升级，都需要拷贝
                {
                    try {

                        println("doLib dstFile is not exists, do copy");

                        FileUtil.copyFile(srcFile, dstFile);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else
                {
                    println("doLib dstFile is exists, no need copy");
                }
            }
        }else
        {
            println("doLib srcFiles is null");
        }
    }


    /**
     * 检测apk合法性
     * @param archiveFilePath
     * @return
     */
    private boolean checkAPKValid(String archiveFilePath)
    {
        boolean result = false;
        try {
            PackageManager pm = getPackageManager();
            Log.e("archiveFilePath", archiveFilePath);
            PackageInfo info = pm.getPackageArchiveInfo(archiveFilePath,
                    PackageManager.GET_ACTIVITIES);
            if (info != null) {
                result = true;
            }
        } catch (Exception e) {
            result = false;
        }
        return result;
    }

    private void prepareLocalServerDex() {
        copyOriDex();

        copyDownloadedDex(getExternalFilesDir("download"));
    }

    private void copyOriDex()
    {
        File dexFile = new File(DexLoaderManager.getInstance().getExtractedDexPath());

        println("dolib copyOriDex: " + dexFile.getPath());
        File destDir = new File(DexLoaderManager.getInstance().getExtractedDexDirPath());

        if(!destDir.exists())
        {
            println("dolib destDir mkdirs");
            destDir.mkdirs();
        }

        InputStream inputStream = null;
        OutputStream outputStream = null;
        try {
            inputStream = getAssets().open(DexLoaderManager.DEX_FILE_INNER_PATH
                    + DexLoaderManager.DEX_FILE_NAME);

            boolean isCanCopy;
            if(dexFile.exists()) // 目标文件已经存在，判断文件完整性
            {
                if(!checkAPKValid(dexFile.getAbsolutePath())) // apk不完整，重新拷贝
                {
                    println("dolib dexFile apk is broken, rewrite");
                    isCanCopy = true;
                    dexFile.delete();
                }else {
                    isCanCopy = false;
                }
            }else
            {
                isCanCopy = true;
            }

            if(isAppUpgrade)
            {
                isCanCopy = true; // 发生过升级，拷贝
            }

            if(isCanCopy)
            {
                println("dolib dexFile copy ori");
                outputStream = new FileOutputStream(dexFile);
                byte[] arrayOfByte = new byte[1024];
                while (true) {
                    int i = inputStream.read(arrayOfByte);
                    if (i == -1) {
                        break;
                    }
                    outputStream.write(arrayOfByte, 0, i);
                }
                outputStream.flush();
            }

            println("dolib dexFile copy over,fileSize:" + dexFile.length());
        } catch (IOException e) {
            println("dolib dexFile copy exception");
            e.printStackTrace();
        } finally {
            if (inputStream != null)
                try {
                inputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if(outputStream != null) try {
                outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Copy dex files from hotupdate folder to dex dir
     */
    private void copyDownloadedDex(File downloadFile) {
        if (downloadFile == null || !downloadFile.exists()) return;
        String dexFilePath = null;
        File downloadRecord = new File(downloadFile, "dexCopyDir");
        BufferedReader bufferedReader = null;
        if (downloadRecord.exists() && downloadRecord.isFile()) {
            try {
                bufferedReader = new BufferedReader(new FileReader(downloadRecord));
                dexFilePath = bufferedReader.readLine();
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (bufferedReader != null) try {
                    bufferedReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (dexFilePath != null) {
                File searchPath = new File(dexFilePath);
                if (searchPath.exists() && searchPath.isDirectory()) {
                    File[] dexFiles = searchPath.listFiles(new FileFilter() {
                        @Override
                        public boolean accept(File pathname) {
                            return pathname.getName().endsWith(DexLoaderManager.DEX_FILE_NAME);
                        }
                    });
                    for(File dexFile : dexFiles) {
                        try {
                            Log.d(TAG, "copyDownloadedDex: " + dexFile.getPath()
                                    + "->" + DexLoaderManager.getInstance().getExtractedDexPath());
                            FileUtil.copyFile(dexFile,
                                    new File(DexLoaderManager.getInstance().getExtractedDexPath()));
                            dexFile.delete();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
    }

    @Override
    protected void onLoadNativeLibraries() {
        println("onLoadNativeLibraries");

        File internalDir = getFilesDir();
        File externalDir = getExternalFilesDir("");

        String path1 = internalDir != null ? internalDir.getAbsolutePath() : "null";
        String path2 = externalDir != null ? externalDir.getAbsolutePath() : "null";
        println("dolib internalDir:" + path1 + ",externalDir:" + path2);

        copyOriLib();
        copyNewLib();
        println("onLoadNativeLibraries copy over");

        if(IS_ENABLE_LOCAL_SERVER)
        {
            prepareLocalServerDex();

            println("onLoadNativeLibraries server prepare over");
        }


        //super.onLoadNativeLibraries();

        try {
//            System.load(getFilesDir() + "/libcocos2djs.so");

            System.load(getFilesDir() + "/libiflyteknet.so");
            System.loadLibrary("cocos2djs");


            println("onLoadNativeLibraries over");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void println(String msg)
    {
        System.out.println("msg:" + msg + ",time:" + System.currentTimeMillis());
    }
}
