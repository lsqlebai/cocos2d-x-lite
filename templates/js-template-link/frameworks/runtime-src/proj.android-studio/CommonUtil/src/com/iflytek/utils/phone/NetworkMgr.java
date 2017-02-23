package com.iflytek.utils.phone;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.telephony.TelephonyManager;
import com.iflytek.utils.string.StringUtil;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * 网络连接类信息
 * 接口未经有效测试过。使用中注意
 */
public class NetworkMgr
{
    public enum SimType {
        China_Mobile, // 中国移动
        China_Unicom, // 中国联通
        China_Telecom,// 中国电信
        Unknown // 未知类型
    }

    public enum SimCardType {
        UNKNOWN, SIM, UIM, USIM
    }

    private TelephonyManager mTelMgr;
	private ConnectivityManager _conMgr;
	private NetworkInfo _networkInfo;
	private static NetworkMgr mInstance = null;

	public static NetworkMgr getInstance() {
		if (null == mInstance) {
			mInstance = new NetworkMgr();
		}
		return mInstance;
	}

	private NetworkMgr()
	{
		_conMgr = PhoneUtil.getSystemService(Context.CONNECTIVITY_SERVICE);
        mTelMgr = PhoneUtil.getSystemService(Context.TELEPHONY_SERVICE);
	}

	public void Connect()
	{
		// (1, "GPRS"), (2, "EDGE") and (3, "UMTS).
		int result = _conMgr.startUsingNetworkFeature(ConnectivityManager.TYPE_MOBILE, "GPRS");
		if (result == -1)
		{
			Logger.getLogger("Somusic").log(Level.INFO, "连接结果:" + result);
		}
	}

	public int getCurrentNetworkType()
	{
		int networkType = -1;
		NetworkInfo[] infos = _conMgr.getAllNetworkInfo();
		if (infos != null && infos.length > 0)
		{
			for (NetworkInfo info : infos)
			{
				if (info.isConnected())
				{
					if (networkType == ConnectivityManager.TYPE_WIFI)
					{
						return networkType;
					}
					networkType = info.getType();
				}
			}
		}

		return networkType;
	}

	public boolean isNetworkAvailable()
	{
		_networkInfo = _conMgr.getActiveNetworkInfo();
        return _networkInfo != null && _networkInfo.isAvailable();
    }

    /**
     * 判断当前使用的是否是手机网络，且网络连通状态正常
     * @return
     */
    public boolean isUserMobileNetWorkAndAvailable() {
        _networkInfo = _conMgr.getActiveNetworkInfo();
        return _networkInfo != null
                && _networkInfo.getType() == ConnectivityManager.TYPE_MOBILE
                && _networkInfo.isAvailable();
    }

	public boolean isNetworkConnected()
	{
		_networkInfo = _conMgr.getActiveNetworkInfo();
        return _networkInfo != null && _networkInfo.isConnected();
    }

	public String getNetworkExtraInfo()
	{
		_networkInfo = _conMgr.getActiveNetworkInfo();
        if (_networkInfo == null) {
            return StringUtil.EMPTY;
        }
        return _networkInfo.getExtraInfo();
    }

	public String getNetworkSubtypeName()
	{
		_networkInfo = _conMgr.getActiveNetworkInfo();
		if (_networkInfo != null)
		{
			return _networkInfo.getSubtypeName();
		}
		return StringUtil.EMPTY;
	}

    // 取得网络供应商的名称
    public String getNetworkOperatorName()
    {
        return mTelMgr.getNetworkOperatorName();
    }

    // 取得sim卡状态
    public int getSimState()
    {
        return mTelMgr.getSimState();
    }

    // 取得sim卡供货商名称
    public String getSimOperatorName()
    {
        return mTelMgr.getSimOperatorName();
    }

    // 取得sim卡供货商代码
    public String getSimOperator()
    {
        return mTelMgr.getSimOperator();
    }

    public String getMCCNumber()
    {
        String simOperator = getSimOperator();
        if (simOperator != null && simOperator.length() >= 5)
        {
            return simOperator.substring(0, 3);
        }
        return StringUtil.EMPTY;
    }

    public String getMNCNumber()
    {
        String simOperator = getSimOperator();
        if (simOperator != null && simOperator.length() >= 5)
        {
            return simOperator.substring(3, 5);
        }
        return StringUtil.EMPTY;
    }

    // 取得sim是联通或是移动卡
    public SimType getSimType()
    {
        String mnc = getMNCNumber();
        if (mnc != null)
        {
            if (mnc.equals("00") || mnc.equals("02"))
            {
                return SimType.China_Mobile;
            }
            else if (mnc.equals("01"))
            {
                return SimType.China_Unicom;
            }
            else if(mnc.equals("03") || mnc.equals("05"))
            {
                return SimType.China_Telecom;
            }
        }
        return SimType.Unknown;
    }

    /**
     * 判断卡的类型
     * @return 卡的类型
     */
    public SimCardType getNetworkType() {
        if (null == mTelMgr) {
            return SimCardType.UNKNOWN;
        }
        int type = mTelMgr.getNetworkType();
        switch (type) {
            case TelephonyManager.NETWORK_TYPE_UMTS:
                return SimCardType.USIM;
            case TelephonyManager.NETWORK_TYPE_GPRS:
            case TelephonyManager.NETWORK_TYPE_EDGE:
                return SimCardType.SIM;
            default:
                return SimCardType.UIM;
        }
    }

    public int getNetworkSubType() {
        if (null == mTelMgr) {
            return -1;
        }
        return mTelMgr.getNetworkType();
    }
}
