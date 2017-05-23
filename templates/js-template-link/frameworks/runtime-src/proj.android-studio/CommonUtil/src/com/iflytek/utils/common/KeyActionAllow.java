package com.iflytek.utils.common;

import android.os.SystemClock;
import android.view.KeyEvent;

import java.util.*;

public class KeyActionAllow {

	private static Map<String, Long> times = new HashMap<String, Long>();

    public static boolean allowOperate(int interval) {
        return allowOperate("default", interval);
    }

	public static boolean allowOperate(String key, int interval) {
        long now = SystemClock.elapsedRealtime();
        Long timeObj = times.get(key);
        long time = timeObj == null ? 0 : timeObj.longValue();
        boolean ret = now - time > interval;
        if (ret) {
            times.put(key, now);
        }
        return ret;
	}

    public static boolean allowDPadDownRepeat(KeyEvent event){
        return allowDPadDownRepeat(event, 80);
    }

    public static boolean allowDPadDownRepeat(KeyEvent event, int interval){
        return !((event.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT ||
                event.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT ||
                event.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP ||
                event.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN) &&
                event.getAction() == KeyEvent.ACTION_DOWN &&
                event.getRepeatCount() >= 1 &&
                !allowOperate(interval));
    }
	
}
