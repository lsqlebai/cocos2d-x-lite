package com.iflytek.utils.hash;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import com.iflytek.utils.string.StringUtil;

public class SHA1 {
	public static byte[] createSha1(File file) throws NoSuchAlgorithmException, IOException {
		InputStream fis = new BufferedInputStream(new FileInputStream(file));
	    return createSha1(fis);
	}

	public static byte[] createSha1(InputStream inputStream) throws NoSuchAlgorithmException, IOException {
		MessageDigest digest = MessageDigest.getInstance("SHA-1");
		InputStream fis = new BufferedInputStream(inputStream);
		int n = 0;
		byte[] buffer = new byte[8192];
		while (n != -1) {
			n = fis.read(buffer);
			if (n > 0) {
				digest.update(buffer, 0, n);
			}
		}
		fis.close();
		return digest.digest();
	}
	
	public static String createSha1String(File file)  throws NoSuchAlgorithmException, IOException {
		byte[] sha1 = createSha1(file);
		return StringUtil.bytesToHexString(sha1);
	}

	public static byte[] createSha1(String str) throws NoSuchAlgorithmException {
		MessageDigest digest = MessageDigest.getInstance("SHA-1");
		digest.update(str.getBytes());
		return digest.digest();
	}

	public static String createSha1String(String str)  throws NoSuchAlgorithmException {
		byte[] sha1 = createSha1(str);
		return StringUtil.bytesToHexString(sha1);
	}
}
