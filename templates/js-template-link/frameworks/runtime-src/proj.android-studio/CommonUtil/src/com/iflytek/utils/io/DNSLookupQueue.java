package com.iflytek.utils.io;

import com.iflytek.Task.AsyncTask;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Executor;

/**
 * Created by cllu on 4/28/15.
 */
public class DNSLookupQueue {
    private static DNSLookupQueue instance;
    private Executor executor;
    private Map<String, String> dnsCache;
    private DNSLookupQueue() {
        executor = AsyncTask.THREAD_POOL_EXECUTOR;
        dnsCache = Collections.synchronizedMap(new HashMap<String, String>());
    }

    public static DNSLookupQueue getInstance() {
        if (instance == null) {
            instance = new DNSLookupQueue();
        }
        return instance;
    }

    public String lookup(String host) {
        String cache = dnsCache.get(host);
        if (cache == null) {
            lookupAsync(host);
        }
        return cache;
    }

    public String lookupUrl(String urlStr) {
        try {
            URL url = new URL(urlStr);
            String host = url.getHost();
            String ip = lookup(host);
            return ip;
        } catch (MalformedURLException e) {
            return null;
        }
    }

    public void lookupAsync(final String host) {
        executor.execute(new Runnable() {
            @Override
            public void run() {
                String ip = NetUtils.host2Ip(host);
                if (ip == null) return;
                dnsCache.put(host, ip);
            }
        });
    }

    public String convertUrl(String urlStr) {
        try {
            URL url = new URL(urlStr);
            String host = url.getHost();
            String ip = lookup(host);
            return ip == null ? urlStr : urlStr.replace(host, ip);
        } catch (MalformedURLException e) {
            return urlStr;
        }
    }

}
