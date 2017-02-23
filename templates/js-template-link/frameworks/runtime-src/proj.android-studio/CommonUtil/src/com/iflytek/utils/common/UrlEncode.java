package com.iflytek.utils.common;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class UrlEncode{
	public static String decode(String url) {
		try {
			return URLDecoder.decode(url, "utf-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static String encode(String url, String code) {
		Matcher matcher = Pattern.compile("[\\u4e00-\\u9fa5]").matcher(url);
		while(matcher.find())
		{
			String tmp = matcher.group();
			try
			{
				url = url.replaceAll(tmp, URLEncoder.encode(tmp, code));
			}
			catch (UnsupportedEncodingException e)
			{
				e.printStackTrace();
			}
		}
		return url;
	}
	
	public static String encodeGBK(String url) {
		String newUrl = encode(url, "gbk");
		newUrl = newUrl.replaceAll("（", "%A3%A8");
		newUrl = newUrl.replaceAll("）", "%A3%A9");
        newUrl = newUrl.replaceAll(" ", "%20");
		return newUrl;
	}
	
	public static String encodeUTF8(String url) {
		String newUrl = encode(url, "utf-8");
		newUrl = newUrl.replaceAll("（", "%EF%BC%88");
		newUrl = newUrl.replaceAll("）", "%EF%BC%89");
        newUrl = newUrl.replaceAll(" ", "%20");
		return newUrl;
	}
	
}
