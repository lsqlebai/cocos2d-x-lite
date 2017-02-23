package com.iflytek.utils.image.blur;

import android.graphics.Bitmap;

public interface BaseRender {
	void blur(float radius, Bitmap in, Bitmap out);
	void destroy() ;
}
