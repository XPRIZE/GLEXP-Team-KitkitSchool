package org.cocos2dx.cpp.drum.activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.drum.utility.EffectSound;
import org.cocos2dx.cpp.drum.utility.Log;

public class DrumActivity extends AppCompatActivity {

    private EffectSound mEffectSound;

    private ImageView mIvDrum;
    private Bitmap mColorInfoBitmap;

    private Matrix mInvertMatrix = new Matrix();
    private float[] mTransPosition = new float[2];

    private View mDecorView;
    private int mUiOption;
    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            int id = v.getId();

            if (id == R.id.v_back) {
                finish();

            }
        }
    };
    private View.OnTouchListener mOnTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            switch (event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    int touchIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                    doTouch(event.getX(touchIndex), event.getY(touchIndex));
                    break;
            }

            return true;
        }
    };

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drum);
        setImmersiveMode();
        setupView();

        mEffectSound = EffectSound.getInstance(this);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (hasFocus) {
            mDecorView.setSystemUiVisibility(mUiOption);
        }
    }

    private void setImmersiveMode() {
        mDecorView = getWindow().getDecorView();
        mUiOption = getWindow().getDecorView().getSystemUiVisibility();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
            mUiOption |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            mUiOption |= View.SYSTEM_UI_FLAG_FULLSCREEN;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
            mUiOption |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        mDecorView.setSystemUiVisibility(mUiOption);
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    private void setupView() {
        findViewById(R.id.v_back).setOnClickListener(mOnClickListener);

        mIvDrum = (ImageView) findViewById(R.id.iv_drum);
        mIvDrum.setOnTouchListener(mOnTouchListener);
        mColorInfoBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.drum_color_info);
        setScale(findViewById(R.id.layout_root));
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    private void setScale(View rootView) {
        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getRealSize(size);

        float fixedSizeWidth = 1800.f * size.x / size.y;
        float fixedSizeHeight = 1800.f;
        float mScale = size.y / 1800.f;

        Log.i("display width : " + size.x);
        Log.i("display height : " + size.y);
        Log.i("fixed width : " + fixedSizeWidth);
        Log.i("fixed height : " + fixedSizeHeight);
        Log.i("scale : " + mScale);

        ViewGroup.LayoutParams params = rootView.getLayoutParams();
        params.width = (int) (fixedSizeWidth + 0.5f);
        params.height = (int) (fixedSizeHeight + 0.5f);
        rootView.setPivotX(0);
        rootView.setPivotY(0);
        rootView.setScaleX(mScale);
        rootView.setScaleY(mScale);
    }

    private void startAnimation(View v) {
        Animation animation = AnimationUtils.loadAnimation(this, R.anim.scale);
        v.startAnimation(animation);
    }

    private void doTouch(float x, float y) {
        mIvDrum.getImageMatrix().invert(mInvertMatrix);
        mTransPosition[0] = x;
        mTransPosition[1] = y;
        mInvertMatrix.mapPoints(mTransPosition);
        x = mTransPosition[0];
        y = mTransPosition[1];

        if (x < 0 || x > mColorInfoBitmap.getWidth() - 1) {
            return;
        }

        if (y < 0 || y > mColorInfoBitmap.getHeight() - 1) {
            return;
        }

        int pixel = mColorInfoBitmap.getPixel((int) x, (int) y);
        int r = Color.red(pixel);
        int g = Color.green(pixel);
        int b = Color.blue(pixel);

        Log.i("rgb : " + r + "," + g + "," + b);
        if (r != 0 || g != 0 || b != 0) {
            int max = Math.max(Math.max(r, g), b);

            if (max == r) {
                mEffectSound.startSoundPool(EffectSound.SOUND_BASS);

            } else if (max == g) {
                mEffectSound.startSoundPool(EffectSound.SOUND_TONE);

            } else {
                mEffectSound.startSoundPool(EffectSound.SOUND_SLAP);

            }

            startAnimation(mIvDrum);
        }
    }
}