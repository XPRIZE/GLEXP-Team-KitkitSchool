package com.enuma.blackboard.activity;

import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Display;
import android.view.View;
import android.view.ViewGroup;

import com.enuma.blackboard.R;
import com.enuma.blackboard.utility.Log;
import com.enuma.blackboard.utility.Util;
import com.enuma.blackboard.view.ViewDrawingColoring;

public class MainActivity extends AppCompatActivity {

    private View mDecorView;
    private int mUiOption;
    private ViewDrawingColoring mVDrawing;
    private float mScale;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Point size = Util.getWindowSize(this);
        if (size.x <= 1280) {
            setContentView(R.layout.activity_main_s);

        } else {
            setContentView(R.layout.activity_main);
        }

        setImmersiveMode();
        setupView();
        setData();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mVDrawing.stopSound();
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
        mVDrawing = (ViewDrawingColoring) findViewById(R.id.v_drawing);
        findViewById(R.id.v_back).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        setScale(findViewById(R.id.layout_root));
    }

    private void setScale(View rootView) {
        Point size = Util.getWindowSize(this);
        float BASIC_HEIGHT = (size.x <= 1280) ?  900.f : 1800.f;

        float fixedSizeWidth = BASIC_HEIGHT * size.x / size.y;
        float fixedSizeHeight = BASIC_HEIGHT;
        mScale = size.y / BASIC_HEIGHT;

        Log.i("display width : " + size.x);
        Log.i("display height : " + size.y);
        Log.i("fixed width : " + fixedSizeWidth);
        Log.i("fixed height : " + fixedSizeHeight);
        Log.i("scale : " + mScale);

        ViewGroup.LayoutParams params = rootView.getLayoutParams();
        params.width = (int)(fixedSizeWidth + 0.5f);
        params.height = (int)(fixedSizeHeight + 0.5f);
        rootView.setPivotX(0);
        rootView.setPivotY(0);
        rootView.setScaleX(mScale);
        rootView.setScaleY(mScale);
    }

    private void setData() {
        mVDrawing.setPenColor(Color.WHITE);
    }
}
