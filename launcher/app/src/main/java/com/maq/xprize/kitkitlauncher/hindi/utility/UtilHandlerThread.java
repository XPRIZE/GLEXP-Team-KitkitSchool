package com.maq.xprize.kitkitlauncher.hindi.utility;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;

public class UtilHandlerThread {
    private HandlerThread mHandlerThread;
    private MainWorkerHandler mMainWorkerHandler;
    private Callback mCallback;
    private String mOwnerClassName;

    private class MainWorkerHandler extends Handler {
        public MainWorkerHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            try {
                if (mCallback != null) {
                    Log.i("command : " + msg.what);
                    mCallback.handleThreadMessage(msg);
                }

            } catch (Exception e) {
                Log.e("thread exception : " + e);
            }
        }
    }

    public interface Callback {
        void handleThreadMessage(Message message);
    }

    public void setup(String ownerClassName, Callback callback) {
        if (mHandlerThread == null) {
            mOwnerClassName = ownerClassName;
            Log.i("HandlerThread 시작 : " + mOwnerClassName);

            mCallback = callback;
            mHandlerThread = new HandlerThread(ownerClassName);
            mHandlerThread.start();
            mMainWorkerHandler = new MainWorkerHandler(mHandlerThread.getLooper());

        } else {
            throw new AssertionError("Handler Thread Setup Error : " + ownerClassName);

        }
    }

    /**
     * setup() 호출시 반드시 dispose() 호출
     */
    public void dispose() {
        if (mHandlerThread != null) {
            Log.i("HandlerThread 종료 : " + mOwnerClassName);
            mHandlerThread.quit();
            mHandlerThread.interrupt();
            mCallback = null;
            mOwnerClassName = null;
        }
    }

    public void sendNetworkCommand(int command) {
        if (mMainWorkerHandler != null) {
            mMainWorkerHandler.sendEmptyMessage(command);
        }
    }

    public void sendNetworkCommandDelayed(int command, int delayedTime) {
        if (mMainWorkerHandler != null) {
            mMainWorkerHandler.sendEmptyMessageDelayed(command, delayedTime);
        }
    }

    public void sendBackgroundCommand(int command) {
        sendNetworkCommand(command);
    }

    public void sendNetworkCommand(int command, int arg1, int arg2, Object object) {
        if (mMainWorkerHandler != null) {
            mMainWorkerHandler.sendMessage(mMainWorkerHandler.obtainMessage(command, arg1, arg2, object));
        }
    }

    public boolean hasNetworkCommand(int command) {
        if (mMainWorkerHandler != null) {
            return mMainWorkerHandler.hasMessages(command);
        }

        return false;
    }
}
