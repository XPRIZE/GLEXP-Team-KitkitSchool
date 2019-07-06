package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.graphics.Rect;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

import com.maq.kitkitlogger.KitKitLogger;
import com.maq.kitkitlogger.KitKitLoggerActivity;
import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.KitkitSchoolApplication;

/**
 * Created by ingtellect on 1/11/17.
 */

public class CreditActivity extends KitKitLoggerActivity {
    private View mVClose;
    private Rect mTempRect = new Rect();
    private boolean mbPressed = false;
    private View.OnTouchListener mBlockViewTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                mVClose.getGlobalVisibleRect(mTempRect);
                if (mTempRect.contains((int) event.getX(), (int) event.getY())) {
                    mbPressed = true;
                    mVClose.dispatchTouchEvent(event);
                } else {
                    mbPressed = false;
                }
            } else {
                if (mbPressed) {
                    mVClose.dispatchTouchEvent(event);
                }

                if (event.getAction() == MotionEvent.ACTION_UP) {
                    mbPressed = false;
                }
            }

            return true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);
        setContentView(R.layout.activity_credit);
        Util.setScale(this, findViewById(R.id.root_container));

        mVClose = findViewById(R.id.close_btn);
    }

    @Override
    protected void onPause() {
        super.onPause();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(null);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((KitkitSchoolApplication) getApplication()).getLogger();
        logger.tagScreen("CreditActivity");

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    public void onClickClose(View v) {
        finish();
    }
}
