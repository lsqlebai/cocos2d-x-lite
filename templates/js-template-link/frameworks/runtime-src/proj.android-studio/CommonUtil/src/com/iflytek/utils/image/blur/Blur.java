package com.iflytek.utils.image.blur;

import android.content.Context;
import android.graphics.*;

public class Blur {
	
	private static final String TAG = "Blur";
	
	private static final float BLUR_RADIUS_DEFAULT = 10.0f;

	private BaseRender mRender;
	private Paint paint;
	
	public Blur(Context context) {
		mRender = new JNIRender();
        paint = new Paint();
        paint.setAntiAlias(true);
	}
	
	public Bitmap blur(Bitmap image, int maskColor, PorterDuff.Mode mode) {
		return blur(image, maskColor, mode, BLUR_RADIUS_DEFAULT);
	}

	public Bitmap blur(Bitmap image, int maskColor, PorterDuff.Mode mode, float radius) {
		int width = image.getWidth();
		int height = image.getHeight();

		Bitmap outputBitmap = image.copy(Bitmap.Config.ARGB_8888, true);
        Bitmap tmpBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(tmpBitmap);

        canvas.drawBitmap(image, 0, 0, paint);
        canvas.drawColor(maskColor, mode);

		mRender.blur(radius, tmpBitmap, outputBitmap);
		tmpBitmap.recycle();
		
		return outputBitmap;
	}
	
	public void Destroy() {
		if (mRender != null) mRender.destroy();
	}
}

