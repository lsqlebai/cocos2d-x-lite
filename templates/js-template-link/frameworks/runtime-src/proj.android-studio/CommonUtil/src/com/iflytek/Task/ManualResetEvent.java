package com.iflytek.Task;

/**
 * similar to ManualResetEvent in c#
 * details: http://msdn.microsoft.com/zh-cn/library/2ssskfws(v=vs.80).aspx
 */
public class ManualResetEvent {

	private final Object monitor = new Object();
	private volatile boolean open = false;

	public ManualResetEvent(boolean open) {
		this.open = open;
	}

	public void waitOne() throws InterruptedException {
		synchronized (monitor) {
			while (!open) {
				monitor.wait();
			}
		}
	}

	public boolean waitOne(long milliseconds) throws InterruptedException {
		synchronized (monitor) {
			if (open)
				return true;
			monitor.wait(milliseconds);
			return open;
		}
	}

	public void set() {// open start
		synchronized (monitor) {
			open = true;
			monitor.notifyAll();
		}
	}

	public void reset() {// close stop
		open = false;
	}

}
