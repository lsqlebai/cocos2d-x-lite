package org.cocos2dx.lib;

import android.annotation.TargetApi;
import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.URL;
import java.util.HashMap;

public class Cocos2dxDownloader {
    private final static String TAG = "Cocos2dxDownloader";
    private int id;
    private String tempFileNameSufix;
    private int timeout;
    private int countOfMaxProcessingTasks;
    private HashMap<Integer, DownloadTask> taskMap = new HashMap();

    public static Cocos2dxDownloader createDownloader(int id, int timeoutInSeconds, String tempFileNameSufix, int countOfMaxProcessingTasks) {
        Cocos2dxDownloader downloader = new Cocos2dxDownloader();
        downloader.id = id;
        downloader.timeout = timeoutInSeconds * 1000;
        downloader.tempFileNameSufix = tempFileNameSufix;
        downloader.countOfMaxProcessingTasks = countOfMaxProcessingTasks;
        Log.d(TAG, "createDownloader: " + id + ", countOfMaxProcessingTasks: " + countOfMaxProcessingTasks);
        return downloader;
    }

    public static void createTask(final Cocos2dxDownloader downloader, final int id_
            , final String url_, final String path_, final String proxy_) {
        Log.d(TAG, "createTask: " + id_ + ", task count:" + downloader.taskMap.size() + ", url:" + url_ + ", path:" + path_);
        if (downloader.countOfMaxProcessingTasks <= downloader.taskMap.size()) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    downloader.nativeOnFinish(downloader.id, id_, -1
                            , "too many download task: " + downloader.countOfMaxProcessingTasks, null);
                }
            });
            return;
        }
        Cocos2dxHelper.getActivity().runOnUiThread(new Runnable() {
            @TargetApi(Build.VERSION_CODES.HONEYCOMB)
            @Override
            public void run() {
                DownloadTask task = downloader.new DownloadTask(id_);
                task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, url_, path_, proxy_);
                downloader.taskMap.put(id_, task);
            }
        });
    }

    public static void cancelAllRequests(final Cocos2dxDownloader downloader) {
        Cocos2dxHelper.getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                for (DownloadTask task : downloader.taskMap.values()) {
                    task.cancel(false);
                }
                downloader.taskMap.clear();
            }
        });
    }

    native void nativeOnProgress(int id, int taskId, long dl, long dlnow, long dltotal);
    native void nativeOnFinish(int id, int taskId, int errCode, String errStr, final byte[] data);

    private class DownloadTask extends AsyncTask<String, Integer, String> {

        private String httpUrl;
        private String httpProxy;
        private String filePath;
        private int id;
        private ByteArrayOutputStream byteArrayOutputStream;
        private int resultCode;

        public DownloadTask(int id) {
            this.id = id;
        }

        @Override
        protected void onProgressUpdate(final Integer... progress) {
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    nativeOnProgress(Cocos2dxDownloader.this.id
                            , id, progress[0], progress[1], progress[2]);
                }
            });
        }

        @Override
        protected void onPostExecute(final String result) {
            Log.d(TAG, "onPostExecute: " + id + " : " + result + ", code:" + resultCode);
            Cocos2dxHelper.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    if (result != null) {
                        nativeOnFinish(Cocos2dxDownloader.this.id, id, resultCode, result, null);
                    } else {
                        nativeOnFinish(Cocos2dxDownloader.this.id, id, resultCode, null,
                                byteArrayOutputStream != null ? byteArrayOutputStream.toByteArray() : null);
                    }
                    taskMap.remove(id);
                }
            });
        }

        @Override
        protected String doInBackground(String... args) {
            InputStream input = null;
            OutputStream output = null;
            httpUrl = args[0];
            if (args.length >= 2) filePath = args[1];
            if (args.length >= 3) httpProxy = args[2];
            String[] proxy_port = null;
            if (httpProxy != null && httpProxy.length() > 0) {
                if (httpProxy.contains(":"))
                    proxy_port = httpProxy.split(":");
                else
                    Log.d(TAG, "doInBackground: proxy address must be format as : HOST:port");
            }
            Log.d(TAG, "doInBackground: " + id + ", url: " + httpUrl + ", proxy: " + httpProxy + ", filePath:" + filePath);
            HttpURLConnection connection = null;
            File tmp = null;
            try {
                if (proxy_port != null) {
                    Proxy proxy = new Proxy(Proxy.Type.HTTP
                            , new InetSocketAddress(proxy_port[0], Integer.parseInt(proxy_port[1])));
                    connection = (HttpURLConnection) new URL(httpUrl).openConnection(proxy);
                } else {
                    connection = (HttpURLConnection) new URL(httpUrl).openConnection();
                }
                connection.setConnectTimeout(timeout);
                connection.setReadTimeout(timeout);
                connection.connect();

                // expect HTTP 200 OK, so we don't mistakenly save error report
                // instead of the file
                resultCode = connection.getResponseCode();
                if (resultCode != HttpURLConnection.HTTP_OK) {
                    return "Server returned HTTP " + resultCode
                            + ", " + connection.getResponseMessage();
                }
                // download the file
                int contentLength = connection.getContentLength();
                input = connection.getInputStream();
                if (filePath != null && filePath.length() > 0) {
                    tmp = new File(filePath + tempFileNameSufix);
                    if (!tmp.exists()) {
                        tmp.getParentFile().mkdirs();
                        tmp.createNewFile();
                    }
                    output = new FileOutputStream(tmp);
                } else {
                    byteArrayOutputStream = new ByteArrayOutputStream();
                }
                byte data[] = new byte[(filePath == null && filePath.length() > 0)
                        ? 4 * 1024 : 256 * 1024];
                int received = 0;
                int count;
                while ((count = input.read(data)) != -1) {
                    // allow canceling with back button
                    if (isCancelled()) {
                        input.close();
                        return null;
                    }
                    received += count;
                    // publishing the progress....
                    if (contentLength > 0) // only if total length is known
                        publishProgress(count, received, contentLength);
                    if (output != null) {
                        output.write(data, 0, count);
                    } else {
                        byteArrayOutputStream.write(data, 0, count);
                    }
                }
            } catch (Exception e) {
                Log.e(TAG, "doInBackground: ", e);
                return e.toString();
            } finally {
                try {
                    if (output != null)
                        output.close();
                    if (input != null)
                        input.close();
                } catch (IOException ignored) {
                }
                if (connection != null)
                    connection.disconnect();
            }
            if (tmp != null) {
                Log.d(TAG, " doInBackground: saveFile:" + filePath);
                tmp.renameTo(new File(filePath));
            }
            return null;
        }
    }
}
