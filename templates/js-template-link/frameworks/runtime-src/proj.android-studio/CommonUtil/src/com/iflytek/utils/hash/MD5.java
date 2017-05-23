package com.iflytek.utils.hash;

import com.iflytek.utils.string.StringUtil;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MD5 {
    public static String generate(String data) {
        String cacheKey;
        try {
            final MessageDigest mDigest = MessageDigest.getInstance("MD5");
            mDigest.update(data.getBytes());
            cacheKey = StringUtil.bytesToHexString(mDigest.digest());
        } catch (NoSuchAlgorithmException e) {
            cacheKey = String.valueOf(data.hashCode());
        }
        return cacheKey;
    }
}