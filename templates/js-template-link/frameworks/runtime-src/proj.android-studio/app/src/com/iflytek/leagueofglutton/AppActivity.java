
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

import android.content.SharedPreferences;
import android.os.Bundle;

import com.iflytek.unipay.PayComponent;
import com.iflytek.unipay.js.CocoActivityHelper;
import com.iflytek.utils.common.ApkUtil;

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


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ApkUtil.init(this);

        CocoActivityHelper.setActivity(this);
        PayComponent.getInstance().init(this);
    }
	
    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // TestCpp should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    public static void copyFile(File srcFile, File destFile) throws IOException {
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
        File libCopyDirFile = new File(getExternalFilesDir("download").getAbsolutePath() + "/libCopyDir");
        if(libCopyDirFile.exists() && libCopyDirFile.isFile())
        {
            BufferedReader br =  null;
            try {
                br =  new BufferedReader(new FileReader(libCopyDirFile));
                String filePath = br.readLine();
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
                        File dstDir = getFilesDir();
                        SharedPreferences sp = getPreferences(MODE_PRIVATE);

                        for(File soFile : soFiles)
                        {
                            String key = "KEY_LIB_LAST_TIME_" + soFile.getName();

                            File dstFile = new File(dstDir.getAbsolutePath() + "/" + soFile.getName());

                            long lastTime = sp.getLong(key, 0);
                            if(!dstFile.exists() || dstFile.length() != soFile.length() || lastTime != soFile.lastModified()) // so库发生过变化，则进行覆盖
                            {
                                copyFile(soFile, dstFile);
                                sp.edit().putLong(key, soFile.lastModified()).commit(); // 保存本次拷贝库的时间

                            }
                        }
                    }
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
        }
    }

    private void copyOriLib()
    {
        File srcDir = new File("/data/data/" + getPackageName() + "/lib");
        File dstDir = getFilesDir();

        File[] srcFiles = srcDir.listFiles();
        if(null != srcFiles)
        {
            for(File srcFile : srcFiles)
            {

                File dstFile = new File(dstDir.getAbsolutePath() + "/" + srcFile.getName());

                if(!dstFile.exists())
                {
                    try {
                        copyFile(srcFile, dstFile);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    @Override
    protected void onLoadNativeLibraries() {

        copyOriLib();

        copyNewLib();

        //super.onLoadNativeLibraries();

        try {
            System.load(getFilesDir() + "/libiflyteknet.so");
            System.load(getFilesDir() + "/libcocos2djs.so");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}
