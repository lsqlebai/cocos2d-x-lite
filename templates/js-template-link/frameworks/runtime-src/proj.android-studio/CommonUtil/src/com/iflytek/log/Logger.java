package com.iflytek.log;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import com.iflytek.app.BaseApplication;
import com.iflytek.utils.string.StringUtil;

import java.util.HashMap;
import java.util.Hashtable;

public class Logger {
    private final static String TAG = "ISING";
    public final static boolean VERBOSE;
    public final static boolean DEBUG;
    public final static boolean INFO;
    public final static boolean WARN;
    public final static boolean ERROR = true;
    public final static boolean ASSERT = true;
    private static boolean forceEnableLog;

    public final static String tag = BaseApplication.getAppInstance().getPackageName();
    private static Hashtable<String, Logger> sLoggerTable = new Hashtable<String, Logger>();
    private String mTagName;
    private FileLogger mFileLogger;
    private static final String COMMON = "@";
    private static final String CLLU = "@cllu@ ";
    private static final String XLZHANG4 = "@xlzhang4@ ";
    private static long sLogTime = 0L;
    static {

//        try {
//            forceEnableLog = "verbose".equalsIgnoreCase(get(BaseApplication.getAppInstance(), "droidparts_log_level"));
//        } catch (Exception e) {
            forceEnableLog = true;
//        }

        VERBOSE = Log.isLoggable(TAG, Log.VERBOSE);
        DEBUG = Log.isLoggable(TAG, Log.DEBUG) | forceEnableLog;
        INFO = Log.isLoggable(TAG, Log.INFO) | forceEnableLog;
        WARN = Log.isLoggable(TAG, Log.WARN) | forceEnableLog;
    }

    public static String get(Context ctx, String key) throws PackageManager.NameNotFoundException {
        PackageManager pm = ctx.getPackageManager();
        Bundle metaData = pm.getApplicationInfo(ctx.getPackageName(), 128).metaData;
        return metaData.getString(key);
    }

    public static boolean isDebugVersion() {
        return forceEnableLog;
    }

    private Logger(String tagName, String logFileName) {
        mTagName = tagName;
        String filename = StringUtil.trimToEmpty(logFileName);
        if (StringUtil.isNotBlank(filename)) {
            mFileLogger = new FileLogger("Log", filename);
        }
    }

    /**
     * @param tagName
     * @return
     */
    private static Logger getLogger(String tagName, String logFileName) {
        Logger classLogger = sLoggerTable.get(tagName);
        if (classLogger == null) {
            classLogger = new Logger(tagName, logFileName);
            sLoggerTable.put(tagName, classLogger);
        }
        return classLogger;
    }

    public static Logger getLogger(String tagName) {
        return Logger.getLogger(tagName, StringUtil.EMPTY);
    }

    public static Logger getLogger(Object object) {
        return Logger.getLogger(object.getClass().getSimpleName(), StringUtil.EMPTY);
    }

    public static Logger getLogger(Class<?> cls) {
        return Logger.getLogger(cls.getSimpleName(), StringUtil.EMPTY);
    }

    /**
     * Purpose:Mark user two
     *
     * @return
     */
    public static Logger log() {
        return getLogger(COMMON);
    }

    public static Logger clluLog() {
        return getLogger(CLLU);
    }

    public static Logger xlz4Log() {
        return getLogger(XLZHANG4);
    }

    /**
     * 获得一个带写入到文件的Logger
     * 生成的Log文件路径为/sdcard/android/data/包/cache/log/tag×××
     *
     * @param tag Log的tag 及 文件名前缀，
     * @return
     */
    public static Logger log2File(String tag) {
        return getLogger(tag, tag);
    }

    /**
     * Get The Current Function Name
     *
     * @return
     */
    private String getFunctionName() {
        StackTraceElement[] sts = Thread.currentThread().getStackTrace();
        if (sts == null) {
            return null;
        }
        for (StackTraceElement st : sts) {
            if (st.isNativeMethod()) {
                continue;
            }
            if (st.getClassName().equals(Thread.class.getName())) {
                continue;
            }
            if (st.getClassName().equals(this.getClass().getName())) {
                continue;
            }
            return mTagName + "[ " + Thread.currentThread().getName() + ": "
                    + st.getFileName() + ":" + st.getLineNumber() + " "
                    + st.getMethodName() + " ]";
        }
        return null;
    }

    private String getStackTrace() {
        StackTraceElement[] sts = Thread.currentThread().getStackTrace();
        if (sts == null) {
            return null;
        }
        StringBuilder sb = new StringBuilder();
        for (StackTraceElement st : sts) {
            if (st.isNativeMethod()) {
                continue;
            }
            if (st.getClassName().equals(Thread.class.getName())) {
                continue;
            }
            if (st.getClassName().equals(this.getClass().getName())) {
                continue;
            }
            sb.append(mTagName + "[ " + Thread.currentThread().getName() + ": "
                    + st.getFileName() + ":" + st.getLineNumber() + " "
                    + st.getMethodName() + " ]\n");
        }
        return sb.toString();
    }

    public void printStackTrace(Object str) {
        if (INFO) {
            String name = getStackTrace();
            if (name != null) {
                Log.i(tag, str + "\n" + name);
            } else {
                Log.i(tag, str.toString());
            }
        }
    }

    /**
     * The Log Level:i
     *
     * @param str
     */
    public void i(Object str) {
        if (INFO) {
            String name = getFunctionName();
            if (name != null) {
                Log.i(tag, name + " - " + str);
            } else {
                Log.i(tag, str.toString());
            }
            if (mFileLogger != null) {
                mFileLogger.println("[i] " + str.toString());
            }
        }
    }

    /**
     * The Log Level:d
     *
     * @param str
     */
    public void d(Object str) {
        if (DEBUG) {
            String name = getFunctionName();
            if (name != null) {
                Log.d(tag, name + " - " + str);
            } else {
                Log.d(tag, str.toString());
            }
            if (mFileLogger != null) {
                mFileLogger.println("[d] " + str.toString());
            }
        }
    }

    /**
     * The Log Level:V
     *
     * @param str
     */
    public void v(Object str) {
        if (VERBOSE) {
            String name = getFunctionName();
            if (name != null) {
                Log.v(tag, name + " - " + str);
            } else {
                Log.v(tag, str.toString());
            }
            if (mFileLogger != null) {
                mFileLogger.println("[v] " + str.toString());
            }
        }
    }

    /**
     * The Log Level:w
     *
     * @param str
     */
    public void w(Object str) {
        if (WARN) {
            String name = getFunctionName();
            if (name != null) {
                Log.w(tag, name + " - " + str);
            } else {
                Log.w(tag, str.toString());
            }
            if (mFileLogger != null) {
                mFileLogger.println("[w] " + str.toString());
            }
        }
    }

    public void w(String log, Throwable tr) {
        if (WARN) {
            String line = getFunctionName();
            Log.e(tag, "{Thread:" + Thread.currentThread().getName() + "}"
                    + "[" + mTagName + line + ":] " + log + "\n", tr);
            if (mFileLogger != null) {
                mFileLogger.println("[w] " + log);
            }
        }
    }

    /**
     * The Log Level:e
     *
     * @param str
     */
    public void e(Object str) {
        if (ERROR) {
            String name = getFunctionName();
            if (name != null) {
                Log.e(tag, name + " - " + str);
            } else {
                Log.e(tag, str.toString());
            }
            if (mFileLogger != null) {
                mFileLogger.println("[e] " + str.toString());
            }
        }
    }

    /**
     * The Log Level:e
     *
     * @param ex
     */
    public void e(Throwable ex) {
        if (ERROR) {
            Log.e(tag, "error", ex);
        }
        if (mFileLogger != null) {
            mFileLogger.println("[error] " + ex.toString());
        }
    }

    /**
     * The Log Level:e
     *
     * @param log
     * @param tr
     */
    public void e(String log, Throwable tr) {
        if (ERROR) {
            String line = getFunctionName();
            Log.e(tag, "{Thread:" + Thread.currentThread().getName() + "}"
                    + "[" + mTagName + line + ":] " + log + "\n", tr);
            if (mFileLogger != null) {
                mFileLogger.println("[E] " + log);
            }
        }
    }


    public void logMap(HashMap map, int level) {
        boolean hasLog = Log.isLoggable(TAG, level) | forceEnableLog;
        if (hasLog && map != null) {
            for (Object key : map.keySet()) {
                Log.d(tag, "key: " + key + " - value : " + map.get(key));
                if (mFileLogger != null) {
                    mFileLogger.println("[d] " + "key: " + key + " - value : " + map.get(key));
                }
            }

        }
    }

    public void logTime(String log) {
        if (INFO) {
            Log.i(tag, "[" + mTagName  + ":] delta time:" + (System.currentTimeMillis() - sLogTime) +"  " +  log + "\n");
            sLogTime = System.currentTimeMillis();
            if (mFileLogger != null) {
                mFileLogger.println("[E] " + log);
            }
        }
    }

}  

