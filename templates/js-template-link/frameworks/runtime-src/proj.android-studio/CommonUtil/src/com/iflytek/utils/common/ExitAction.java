package com.iflytek.utils.common;

import android.os.SystemClock;

public class ExitAction{

	public static ExitAction instance = null;
	
	private static final int EXIT_RESET_TIME_OUT = 3500;
	private long lastCallTime = 0;
	
	public static ExitAction getInstance(){
		if(null == instance){
			instance = new ExitAction();
		}
		return instance;
	}
	
	public boolean excute(Runnable r){
		boolean exit = exitEnable();
		if(!exit){
			r.run();
		}
		return exit;
	}
	
	private boolean exitEnable(){
		long timeout = SystemClock.elapsedRealtime() - lastCallTime;
		lastCallTime = SystemClock.elapsedRealtime();
		return timeout < EXIT_RESET_TIME_OUT;
	}
}
