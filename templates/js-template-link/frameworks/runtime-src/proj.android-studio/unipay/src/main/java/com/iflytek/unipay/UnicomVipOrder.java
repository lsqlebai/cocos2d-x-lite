package com.iflytek.unipay;

import com.iflytek.protobuf.ProtoField;

/**
 * Created by lsq on 2017/1/16.
 */

public class UnicomVipOrder {
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
