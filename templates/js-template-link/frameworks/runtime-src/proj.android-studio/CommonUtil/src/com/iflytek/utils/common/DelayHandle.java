package com.iflytek.utils.common;

import android.os.Handler;
import android.os.Looper;

public class DelayHandle {
	private static final Handler mHandle = new Handler(Looper.getMainLooper());
	
	public static final void delay(long delay, Runnable runnable) {
		mHandle.postDelayed(runnable, delay);
	}

    public static final void runOnUiThread(Runnable runnable) {
        mHandle.post(runnable);
    }

    public static final void remove(Runnable runnable) {
        mHandle.removeCallbacks(runnable);
    }
	
	public static final void delay(long delay, long period, int times, Runnable runnable) {
		for (int i = 0; i < times; i++) {
			mHandle.postDelayed(runnable, delay + period * i);
		}
	}
}
