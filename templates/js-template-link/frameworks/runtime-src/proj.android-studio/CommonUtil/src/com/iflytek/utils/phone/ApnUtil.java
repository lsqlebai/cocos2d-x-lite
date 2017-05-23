package com.iflytek.utils.phone;

import android.content.Context;
import android.net.ConnectivityManager;
import android.telephony.TelephonyManager;

import com.iflytek.utils.phone.APNMgr.ApnInfo;
import com.iflytek.utils.string.StringUtil;

public class ApnUtil {
	public static final int Wifi = 0;
	public static final int Phone_2G = 1;
	public static final int Phone_3G = 2;
	private static final String[] NETWORK_TYPE_STR = new String[] {"wifi", "2G", "3G"};
	
	public static final String getApn(Context c) {
	    ApnInfo apn = APNMgr.getInstance(c).getDefaultAPN();
	    if (null == apn || StringUtil.isEmpty(apn.getApn())) {
	    	return "unknown";
	    }
	    return apn.getApn();
	}
	
	public static String getNetworkTypeString(Context c) {
		int type = getNetworkType(c);
		return NETWORK_TYPE_STR[type];
	}
	
	public static int getNetworkType(Context c) {
		if (null == c) {
			return Phone_2G;
		}
		NetworkMgr conMgr = NetworkMgr.getInstance();
		int type = conMgr.getCurrentNetworkType();
		int subType = conMgr.getNetworkSubType();
		
		if(type == ConnectivityManager.TYPE_WIFI) {
			return Wifi;
		} else {
			switch(subType) {
			case TelephonyManager.NETWORK_TYPE_1xRTT:
				return Phone_2G; // ~ 50-100 kbps
			case TelephonyManager.NETWORK_TYPE_CDMA:
				return Phone_2G; // ~ 14-64 kbps
			case TelephonyManager.NETWORK_TYPE_EDGE:
				return Phone_2G; // ~ 50-100 kbps
			case TelephonyManager.NETWORK_TYPE_EVDO_0:
				return Phone_3G; // ~ 400-1000 kbps
			case TelephonyManager.NETWORK_TYPE_EVDO_A:
				return Phone_3G; // ~ 600-1400 kbps
			case TelephonyManager.NETWORK_TYPE_GPRS:
				return Phone_2G; // ~ 100 kbps
			case TelephonyManager.NETWORK_TYPE_UMTS:
				return Phone_3G; // ~ 400-7000 kbps
			case 8: //TelephonyManager.NETWORK_TYPE_HSDPA:
				return Phone_3G;
			case 10: //TelephonyManager.NETWORK_TYPE_HSPA:
				return Phone_3G;
			case 9: //TelephonyManager.NETWORK_TYPE_HSUPA:
				return Phone_3G;
			case 12: //TelephonyManager.NETWORK_TYPE_EVDO_B:
				return Phone_3G;
			case 14: // TelephonyManager.NETWORK_TYPE_EHRPD:
				return Phone_3G;
			case 13: // TelephonyManager.NETWORK_TYPE_LTE:
				return Phone_3G;
            // NOT AVAILABLE YET IN API LEVEL 7
//            case Connectivity.NETWORK_TYPE_EHRPD:
//                return true; // ~ 1-2 Mbps
//            case Connectivity.NETWORK_TYPE_EVDO_B:
//                return true; // ~ 5 Mbps
//            case Connectivity.NETWORK_TYPE_HSPAP:
//                return true; // ~ 10-20 Mbps
//            case Connectivity.NETWORK_TYPE_IDEN:
//                return false; // ~25 kbps
//            case Connectivity.NETWORK_TYPE_LTE:
//                return true; // ~ 10+ Mbps
            // Unknown
			case TelephonyManager.NETWORK_TYPE_UNKNOWN:
				return Phone_2G;
			default:
				return Phone_2G;
			}
		}
	}
}
