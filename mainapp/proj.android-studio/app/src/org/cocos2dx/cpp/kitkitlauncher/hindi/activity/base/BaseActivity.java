package org.cocos2dx.cpp.kitkitlauncher.hindi.activity.base;

import android.app.Activity;
import android.os.Bundle;
import android.os.Message;

import com.maq.kitkitlogger.KitKitLoggerActivity;

import org.cocos2dx.cpp.kitkitlauncher.hindi.utility.RecycleUtils;
import org.cocos2dx.cpp.kitkitlauncher.hindi.utility.Util;
import org.cocos2dx.cpp.kitkitlauncher.hindi.utility.UtilHandlerThread;

public class BaseActivity extends KitKitLoggerActivity implements UtilHandlerThread.Callback, Util.HandlerInterface {
    protected Activity mThisActivity;
    protected Util.StaticHandler mHandler = new Util.StaticHandler(this);
    private UtilHandlerThread mUtilHandlerThread = new UtilHandlerThread();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mThisActivity = this;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mUtilHandlerThread.dispose();
        RecycleUtils.recycle(getWindow().getDecorView());
    }

    public void handleThreadMessage(Message msg) {

    }

    @Override
    public void handleUIMessage(Message msg) {

    }

    public void startHandlerThread() {
        mUtilHandlerThread.setup(this.getClass().getName(), this);
    }

    public void sendBackgroundCommand(int command) {
        mUtilHandlerThread.sendBackgroundCommand(command);
    }
}
