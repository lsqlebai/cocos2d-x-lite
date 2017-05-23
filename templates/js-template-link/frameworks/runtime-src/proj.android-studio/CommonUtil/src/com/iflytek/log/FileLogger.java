package com.iflytek.log;

import com.iflytek.app.BaseApplication;
import com.iflytek.utils.common.TimestampUtil;
import com.iflytek.utils.storage.ExternalUtils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.Writer;

/**
 * 打印到sdcard上面的日志类
 * sd卡可用保存于/sdcard/android/data/包/cache下，
 * 不可用在系统内置空间的cache目录下
 * @author wpfang 
 */
public class FileLogger {
    private Writer mWriter;
    private String logfilename;

    /**
     * @param basepath    默认日志目录，/sdcard/android/data/包/cache/abcd
     * @param logfilename /sdcard/android/data/cache/abcd/filename@2012-03-04***.log
     */
    public FileLogger(String basepath, String logfilename) {
        File logDir = ExternalUtils.getDiskCacheDir(BaseApplication
                .getAppInstance().getApplicationContext(), basepath);

        if (!logDir.exists()) {
            logDir.mkdirs();
            try {
                new File(logDir, ".nomedia").createNewFile();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        this.logfilename = logDir.getAbsolutePath() + "/"
                + logfilename + "@" + TimestampUtil.getCurrentTimeLable("yyyyMMdd_HHmmss") + ".log";
    }

    private void openInput() {
        if (mWriter != null){
            return;
        }
        try {
            File file = new File(logfilename);
            mWriter = new BufferedWriter(new FileWriter(file.getAbsolutePath(),true));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 将日志写入到文件中。
     * 日志默认提供时间戳
     * @param message
     */
    public synchronized void println(String message) {
        openInput();
        print(message);
//        close();
    }

    /**
     * 将日志写入到文件中。
     * 日志默认提供时间戳
     * @param message
     */
    protected void print(String message) {
        try {
            mWriter.write('\n');
            StringBuilder sb = new StringBuilder("[")
                    .append(TimestampUtil.getCurrentTimeLable("yyyy-MM-ddHH:mm:ss:SSS"))
                    .append(']')
                    .append(message)
                    .append('\n');
            mWriter.write(sb.toString());
            mWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
            mWriter = null;
        }
    }

    /**
     * 结束时需要把日志文件关闭
     */
    protected void close() {
        try {
            mWriter.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        mWriter = null;
    }
}
