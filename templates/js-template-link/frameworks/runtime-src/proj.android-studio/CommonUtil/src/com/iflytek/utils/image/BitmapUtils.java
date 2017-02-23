package com.iflytek.utils.image;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.View;

import java.io.FileDescriptor;

public class BitmapUtils {

    public static final float computeImageScale(int srcWidth, int srcHeight, int targetWidth, int targetHeight) {
        float widthScale = (float) targetWidth / srcWidth;
        float heightScale = (float) targetHeight / srcHeight;
        return Math.min(widthScale, heightScale);
    }
    
    public static Bitmap takeScreenShot(View view){
		view.setDrawingCacheEnabled(true);
		view.buildDrawingCache();
		Bitmap bitmap = view.getDrawingCache();
		Bitmap bitmap2 = Bitmap.createBitmap(bitmap);
		view.destroyDrawingCache();
		return bitmap2;
	}

	public static int calculateInSampleSize(BitmapFactory.Options options, int reqWidth, int reqHeight)
	{
		// 源图片的高度和宽度
		final int height = options.outHeight;
		final int width = options.outWidth;
		int inSampleSize = 1;
		if (height > reqHeight || width > reqWidth)
		{
			// 计算出实际宽高和目标宽高的比率
			final int heightRatio = Math.round((float) height / (float) reqHeight);
			final int widthRatio = Math.round((float) width / (float) reqWidth);
			if (reqWidth == 0 && reqHeight == 0)
			{
				inSampleSize = 1;
			} else if (reqWidth == 0)
			{
				// 只指定了图片的高度
				inSampleSize = heightRatio;
			} else if (reqHeight == 0)
			{
				// 只指定了图片的宽度
				inSampleSize = widthRatio;
			} else
			{
				// 选择宽和高中最小的比率作为inSampleSize的值，这样可以保证最终图片的宽和高
				// 一定都会大于等于目标的宽和高。
				inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
			}
		}
		return inSampleSize;
	}

	/**
	 * 根据指定比例缩放图片
	 *
	 * @author blue
	 * @param fileDescriptor
	 * @param reqWidth
	 * @return
	 */
	public static Bitmap decodeSampledBitmapFromResource(FileDescriptor fileDescriptor, int reqWidth, int reqHeight) {
		// 第一次解析将inJustDecodeBounds设置为true，来获取图片大小
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeFileDescriptor(fileDescriptor, null, options);
		// 调用上面定义的方法计算inSampleSize值
		options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);
		// 使用获取到的inSampleSize值再次解析图片
		options.inJustDecodeBounds = false;
		return BitmapFactory.decodeFileDescriptor(fileDescriptor, null, options);
	}
}
