package com.iflytek.cache;

import java.io.File;
import java.io.IOException;

import android.content.Context;
import android.util.Log;

import com.iflytek.cache.DiskCacheUtil;
import com.iflytek.cache.DiskLruCache;

public class SimpleDiskLruCache {
    private static final String TAG = "SimpleDiskLruCache";
    private final int mCacheSize;
    private final int mValueCount;
	private DiskLruCache mHttpDiskCache;
    private File mHttpCacheDir;
    private boolean mHttpDiskCacheStarting = true;
    private final Object mHttpDiskCacheLock = new Object();
    
    protected SimpleDiskLruCache() {
    	mCacheSize = 0;
    	mValueCount = 0;
	}
    
	public SimpleDiskLruCache(Context context, String cacheDir, int cacheSize) {
		this(context, 1, cacheDir, cacheSize);
	}
    
	public SimpleDiskLruCache(Context context, int valueCount, String cacheDir, int cacheSize) {
		mHttpCacheDir = DiskCacheUtil.getDiskCacheDir(context, cacheDir);
		mCacheSize = cacheSize;
		mValueCount = valueCount;
		initCache();
	}
	
	@Override
	protected void finalize() throws Throwable {
		closeCache();
		super.finalize();
	}
	
    private void initCache() {
        if (!mHttpCacheDir.exists()) {
            mHttpCacheDir.mkdirs();
        }
        int cacheSize = mCacheSize;
        synchronized (mHttpDiskCacheLock) {
        	long usableSpace = DiskCacheUtil.getUsableSpace(mHttpCacheDir);
        	while (usableSpace > 0 && usableSpace < cacheSize) {
        		cacheSize /= 2;
        	}
            try {
                mHttpDiskCache = DiskLruCache.open(mHttpCacheDir, 1, mValueCount, cacheSize);
            } catch (IOException e) {
                mHttpDiskCache = null;
            }
            mHttpDiskCacheStarting = false;
            mHttpDiskCacheLock.notifyAll();
        }
    }
    
    private void waitUntilCacheInitialied() {
    	synchronized (mHttpDiskCacheLock) {
            // Wait for disk cache to initialize
            while (mHttpDiskCacheStarting) {
                try {
                    mHttpDiskCacheLock.wait();
                } catch (InterruptedException ignored) {}
            }
        }
    }
    
    public boolean avaliable() {
    	waitUntilCacheInitialied();
    	return (mHttpDiskCache != null);
    }
    
    public DiskLruCache.Snapshot get(String key) throws IOException {
    	waitUntilCacheInitialied();
    	return (mHttpDiskCache != null) ? mHttpDiskCache.get(key) : null;
    }
    
    public DiskLruCache.Editor edit(String key) throws IOException {
    	waitUntilCacheInitialied();
    	return (mHttpDiskCache != null) ? mHttpDiskCache.edit(key) : null;
    }
    
    public boolean hasSnapshot(String key) throws IOException {
    	waitUntilCacheInitialied();
    	return (mHttpDiskCache != null) && mHttpDiskCache.get(key) != null;
    }
    
    public void remove(String key) throws IOException {
    	waitUntilCacheInitialied();
    	if (mHttpDiskCache != null) {
    		mHttpDiskCache.remove(key);
    	}
    }

    public void clearCache() {
        synchronized (mHttpDiskCacheLock) {
            if (mHttpDiskCache != null && !mHttpDiskCache.isClosed()) {
                try {
                    mHttpDiskCache.delete();
                } catch (IOException e) {
                    Log.e(TAG, "clearCacheInternal - " + e);
                }
                mHttpDiskCache = null;
                mHttpDiskCacheStarting = true;
                initCache();
            }
        }
    }

    public void flushCache() {
        synchronized (mHttpDiskCacheLock) {
            if (mHttpDiskCache != null) {
                try {
                    mHttpDiskCache.flush();
                } catch (IOException e) {
                    Log.e(TAG, "flush - " + e);
                }
            }
        }
    }

    public void closeCache() {
        synchronized (mHttpDiskCacheLock) {
            if (mHttpDiskCache != null) {
                try {
                    if (!mHttpDiskCache.isClosed()) {
                        mHttpDiskCache.close();
                        mHttpDiskCache = null;
                    }
                } catch (IOException e) {
                    Log.e(TAG, "closeCacheInternal - " + e);
                }
            }
        }
    }
}
