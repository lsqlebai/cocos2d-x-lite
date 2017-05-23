package com.iflytek.utils.image.blur;

import android.graphics.Bitmap;

public class JNIRender implements BaseRender {
	
	static {
        System.loadLibrary("blurjni");
    }
	
	private static native void Blur(Bitmap in, Bitmap out, int r);

	@Override
	public void blur(float radius, Bitmap in, Bitmap out) {
		Blur(in, out, (int)radius);
	}

	@Override
	public void destroy() {

	}

}
