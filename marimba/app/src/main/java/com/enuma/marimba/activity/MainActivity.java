package com.enuma.marimba.activity;

import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import com.enuma.marimba.R;
import com.enuma.marimba.utility.EffectSound;
import com.enuma.marimba.utility.Log;
import com.enuma.marimba.utility.Util;

import java.util.HashMap;

public class MainActivity extends AppCompatActivity {

    ////////////////////////////////////////////////////////////////////////////////

    private final int MAX_BAR_COUNT = 8;

    ////////////////////////////////////////////////////////////////////////////////

    private View[] mVBars = new View[MAX_BAR_COUNT];
    private Rect mTempRect = new Rect();
    private EffectSound mEffectSound;
    private Handler mHandler = new Handler();

    private View mDecorView;
    private int mUiOption;

    ////////////////////////////////////////////////////////////////////////////////

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setImmersiveMode();
        DisplayMetrics displayMetrics = Util.getWindowInfo(this);
        Log.i("width dp : " + Util.getDip(this, displayMetrics.widthPixels));
        Log.i("height dp : " + Util.getDip(this, displayMetrics.heightPixels));
        mEffectSound = EffectSound.getInstance(this);

        setupView();

        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                mEffectSound.startSoundPool(EffectSound.SOUND_INTRO);
            }
        }, 300);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (hasFocus == true) {
            mDecorView.setSystemUiVisibility(mUiOption);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setImmersiveMode() {
        mDecorView = getWindow().getDecorView();
        mUiOption = getWindow().getDecorView().getSystemUiVisibility();
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH )
            mUiOption |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN )
            mUiOption |= View.SYSTEM_UI_FLAG_FULLSCREEN;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT )
            mUiOption |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        mDecorView.setSystemUiVisibility(mUiOption);
    }

    private void setupView() {
        findViewById(R.id.v_back).setOnClickListener(mOnClickListener);
        findViewById(R.id.layout_root).setOnTouchListener(mOnTouchListener);

        String packageName = getPackageName();
        for (int i = 0; i < MAX_BAR_COUNT; ++i) {
            mVBars[i] = findViewById(Util.getResourceId(this, "v_bar_" + i, "id", packageName));
        }
    }

    private void selectBar(int barIndex) {
        mEffectSound.startSoundPool(barIndex);
        startAnimation(mVBars[barIndex]);
    }

    private void startAnimation(View v) {
        Animation animation = AnimationUtils.loadAnimation(this, R.anim.scale);
        v.startAnimation(animation);
    }

    ////////////////////////////////////////////////////////////////////////////////

    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            int id = v.getId();

            if (id == R.id.v_back) {
                finish();
            }
        }
    };

    HashMap<Integer, Integer> mTouchInfo = new HashMap<>();
    private View.OnTouchListener mOnTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {

            switch (event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN: {
                    int touchIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                    mTouchInfo.put(event.getPointerId(touchIndex), -1);
                }
                break;

                case MotionEvent.ACTION_MOVE:
                    break;

                case MotionEvent.ACTION_POINTER_UP: {
                    int touchIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                    mTouchInfo.remove(event.getPointerId(touchIndex));
                }
                    return true;
                case MotionEvent.ACTION_UP:
                    mTouchInfo.clear();
                    return true;
            }

            int touchCount = event.getPointerCount();
            float x;
            float y;

            for (int i = 0; i < touchCount; ++i) {
                int touchId = event.getPointerId(i);
                x = event.getX(i);
                y = event.getY(i);

                boolean bSelect = false;
                for (int j = 0; j < mVBars.length; ++j) {
                    mVBars[j].getGlobalVisibleRect(mTempRect);
                    if (mTempRect.contains((int) x, (int) y) == true) {
                        if (mTouchInfo.get(touchId) != j) {
                            mTouchInfo.put(touchId, j);
                            selectBar(j);
                            startAnimation(mVBars[j]);
                        }
                        bSelect = true;
                        break;
                    }
                }

                if (bSelect == false) {
                    mTouchInfo.put(touchId, -1);
                }
            }
            return true;
        }
    };
}
