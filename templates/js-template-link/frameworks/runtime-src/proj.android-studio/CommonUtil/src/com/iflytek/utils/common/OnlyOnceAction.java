package com.iflytek.utils.common;

public class OnlyOnceAction {
    private boolean isCalled = false;
    private Runnable r;

    public OnlyOnceAction(Runnable r) {
        this.r = r;
    }

    public void act() {
        if (isCalled) {
            return;
        }
        isCalled = true;
        r.run();
    }

    public void reset() {
        isCalled = false;
    }
}
