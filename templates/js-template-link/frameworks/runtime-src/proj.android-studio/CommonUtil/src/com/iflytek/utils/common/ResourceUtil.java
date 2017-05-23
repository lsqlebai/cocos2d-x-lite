package com.iflytek.utils.common;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.iflytek.app.BaseApplication;

import java.io.IOException;
import java.io.InputStream;

/**
 * 资源工具
 *
 * @author wpfang
 */
public class ResourceUtil {
    private static Resources appResources;

    /**
     * 获得Resources，用于没有Context的环境中
     *
     * @return
     */
    public static Resources getResources() {
        if (appResources == null) {
            appResources = BaseApplication.getAppInstance().getApplicationContext()
                    .getResources();
        }
        return appResources;
    }

    /**
     * 获得ID的String资源，用于没有Context的环境
     *
     * @param id
     * @return
     */
    public static String getString(int id) {
        return getResources().getString(id);
    }

    /**
     * 获得ID的dimen资源，用于没有Context的环境
     *
     * @param id
     * @return
     */
    public static int getDimenPx(int id) {
        return getResources().getDimensionPixelSize(id);
    }

    public static int getDimenFhdPx(int fhdPx) {
        int id = getResources().getIdentifier("fhd_" + fhdPx, "dimen", BaseApplication.getAppInstance().getPackageName());
        return getResources().getDimensionPixelSize(id);
    }
    /**
     * 获得Assert中文件流
     *
     * @param filename
     * @return
     * @throws IOException
     */
    public static InputStream getAssertFile(String filename) throws IOException {
        InputStream stream = getResources().getAssets().open(filename);
        return stream;
    }

    /**
     * 获得Assert文件中的Text
     *
     * @param filename
     * @return
     */
    public static String getAssertResText(String filename) {
        try {
            byte[] buffer = new byte[1024];
            InputStream stream = getAssertFile(filename);
            StringBuffer out = new StringBuffer();
            byte[] b = new byte[4096];
            for (int n; (n = stream.read(b)) != -1; ) {
                out.append(new String(b, 0, n));
            }
            return out.toString();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获得Assert中Bitmap
     *
     * @param filename
     * @return
     */
    public static Bitmap getAssertBitmap(String filename) {
        Bitmap bitmap = null;
        try {
            InputStream stream = getAssertFile(filename);
            bitmap = BitmapFactory.decodeStream(stream);
            stream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return bitmap;
    }

    public static int getDrawableId(String name) {
        return BaseApplication.getAppInstance().getResources().getIdentifier(name, "drawable", BaseApplication.getAppInstance().getPackageName());
    }

    public static int getWidgetId(String name) {
        return BaseApplication.getAppInstance().getResources().getIdentifier(name, "id", BaseApplication.getAppInstance().getPackageName());
    }

    public static int getMipmapId(String name) {
        return BaseApplication.getAppInstance().getResources().getIdentifier(name, "mipmap", BaseApplication.getAppInstance().getPackageName());
    }
}
