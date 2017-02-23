package com.iflytek.utils.phone;


import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;


public class APNMgr
{
	public enum NetworkType
	{
		NET, WAP, _3GWAP, WIFI
	}
	
	public class ApnInfo {
		private String mApn;
		private String mHost;
		private int mPort;
		public String getApn() {
			return mApn;
		}
		
		public void setApn(String apn) {
			this.mApn = apn;
		}
		
		public String getHost() {
			return mHost;
		}
		
		public void setHost(String host) {
			this.mHost = host;
		}
		
		public int getPort() {
			return mPort;
		}
		
		public void setPort(int port) {
			this.mPort = port;
		}
	}
	
	private Context _context;
	private NetworkMgr.SimType _simType;
	private ApnInfo mApnInfo;
	
	static APNMgr mInstance = null;
	
	public static APNMgr getInstance(Context c) {
		if (null == mInstance) {
			mInstance = new APNMgr(c);
		}
		return mInstance;
	}
	
	private APNMgr(Context context)
	{
		_context = context;
		_simType = NetworkMgr.getInstance().getSimType();
	}
	
	public NetworkType getAPNType()
	{
		getDefaultAPN();
		String apn = mApnInfo.getApn();
			switch (_simType)
			{
				case China_Mobile :
					if(null != apn && apn.startsWith("cmwap"))
					{
						return NetworkType.WAP;
					}
					
				case China_Unicom :
					if(null != apn && apn.startsWith("uniwap")) {
						return NetworkType.WAP;
					} else if (null != apn && apn.startsWith("3gwap")) {
						return NetworkType._3GWAP;
					}
					
				case China_Telecom :
					if(null != apn && apn.startsWith("ctwap"))
					{
						return NetworkType.WIFI;
					}
					
				default :
					break;
			}
		return NetworkType.NET;
	}
	
	public ApnInfo getDefaultAPN()
	{
		if (mApnInfo != null) {
			return mApnInfo;
		} else {
			refreshApnInfo();
		}
		return mApnInfo;
	}
	
	public void refreshApnInfo() {
		if(mApnInfo == null){
			mApnInfo = new ApnInfo();
		}
		try {
			ConnectivityManager mgr = (ConnectivityManager) _context.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo networkInfo = mgr.getActiveNetworkInfo();
			if (networkInfo.getType() == ConnectivityManager.TYPE_WIFI) {
				mApnInfo.setApn("wifi");
			} else {
				mApnInfo.setApn(networkInfo.getExtraInfo());
				mApnInfo.setHost(android.net.Proxy.getDefaultHost());
				mApnInfo.setPort(android.net.Proxy.getDefaultPort());
				correctApn(mApnInfo);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private boolean isNeedCorrect(String apnString) {
		return !("3gwap".equals(apnString) || "3gnet".equals(apnString)
				|| "uniwap".equals(apnString) || "uninet".equals(apnString)
				|| "cmwap".equals(apnString) || "cmnet".equals(apnString)
				|| "ctwap".equals(apnString) || "ctnet".equals(apnString));
	}
	
	private void correctApn(ApnInfo apnInfo) {
		String apnString = apnInfo.getApn();
		if (apnString != null && !isNeedCorrect(apnString.toLowerCase())) {
			return;
		}
		switch (_simType) {
		case China_Mobile:
			if (apnInfo.getHost() == null) {
				apnInfo.setApn("cmnet");
			} else {
				apnInfo.setApn("cmwap");
			}
			break;
		case China_Unicom:
			if (apnInfo.getHost() == null) {
				apnInfo.setApn("3gnet");
			} else {
				apnInfo.setApn("3gwap");
			}
			break;
		case China_Telecom:
			if (apnInfo.getHost() == null) {
				apnInfo.setApn("ctnet");
			} else {
				apnInfo.setApn("ctwap");
			}
			break;
		default:
			if (apnInfo.getHost() != null && ("10.0.0.200".equals(apnInfo.getHost()) || 
					"010.000.000.200".equals(apnInfo.getHost()))) {
				apnInfo.setApn("ctwap");
			}
			break;
		}
	}
	
	/**
	 * 是否为电信CTWAP接入点
	 * @return
	 */
	public boolean isCTWap(){
		ApnInfo apnInfo = getDefaultAPN();
		if(apnInfo != null){
			String apn = apnInfo.getApn();
			if (_simType == NetworkMgr.SimType.China_Telecom) {
				if ((null != apn && apn.startsWith("ctwap"))
						|| ("10.0.0.200".equals(apnInfo.getHost()) || 
								"010.000.000.200".equals(apnInfo.getHost()))) {
					return true;
				}
			}
		}
		
		return false;
	}
}
