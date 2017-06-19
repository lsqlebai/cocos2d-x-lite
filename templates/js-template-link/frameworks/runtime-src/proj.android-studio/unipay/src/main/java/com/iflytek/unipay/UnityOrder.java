package com.iflytek.unipay;

import com.iflytek.protobuf.ProtoField;

/**
 * Created by lsq on 2017/1/16.
 */

public class UnityOrder {

    @ProtoField
    String aliFeeId;
    @ProtoField
    String weixinFeeId;
    @ProtoField
    String xiaowoFeeId;
    @ProtoField
    String unicomFeeId;
    @ProtoField
    String unicomProductId;
    @ProtoField
    boolean notify;
    @ProtoField
    String orderId;
    @ProtoField
    int price;
    @ProtoField
    String name;

    public String getNotifyUrl() {
        return notifyUrl;
    }

    public void setNotifyUrl(String notifyUrl) {
        this.notifyUrl = notifyUrl;
    }
    //new for jiangsu
    String notifyUrl;

    public String getAliFeeId() {
        return aliFeeId;
    }

    public void setAliFeeId(String aliFeeId) {
        this.aliFeeId = aliFeeId;
    }

    public String getWeixinFeeId() {
        return weixinFeeId;
    }

    public void setWeixinFeeId(String weixinFeeId) {
        this.weixinFeeId = weixinFeeId;
    }

    public String getXiaowoFeeId() {
        return xiaowoFeeId;
    }

    public void setXiaowoFeeId(String xiaowoFeeId) {
        this.xiaowoFeeId = xiaowoFeeId;
    }

    public String getUnicomFeeId() {
        return unicomFeeId;
    }

    public void setUnicomFeeId(String unicomFeeId) {
        this.unicomFeeId = unicomFeeId;
    }

    public String getUnicomProductId() {
        return unicomProductId;
    }

    public void setUnicomProductId(String unicomProductId) {
        this.unicomProductId = unicomProductId;
    }

    public boolean isNotify() {
        return notify;
    }

    public void setNotify(boolean notify) {
        this.notify = notify;
    }

    public String getOrderId() {
        return orderId;
    }

    public void setOrderId(String orderId) {
        this.orderId = orderId;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
