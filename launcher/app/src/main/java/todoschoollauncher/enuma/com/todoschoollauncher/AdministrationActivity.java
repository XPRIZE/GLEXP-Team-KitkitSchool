package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;


/**
 * Created by ingtellect on 1/10/17.
 */

public class AdministrationActivity extends KitKitLoggerActivity {
    private View mVBack;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);
        setContentView(R.layout.activity_administration);

        Util.setScale(this, findViewById(R.id.root_container));
        mVBack = findViewById(R.id.v_back);
        mVBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        Switch reviewModeSwitch = (Switch)findViewById(R.id.reviewModeSwitch);
        reviewModeSwitch.setChecked(getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS).getBoolean("review_mode_on", false));
        reviewModeSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                SharedPreferences.Editor editor = getSharedPreferences("sharedPref",Context.MODE_MULTI_PROCESS).edit();
                editor.putBoolean("review_mode_on", isChecked);
                editor.commit();
            }
        });

        Switch signLanguageSwitch = (Switch)findViewById(R.id.signLanguageSwitch);
        signLanguageSwitch.setChecked(getSharedPreferences("sharedPref",Context.MODE_MULTI_PROCESS).getBoolean("sign_language_mode_on", false));
        signLanguageSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                SharedPreferences.Editor editor = getSharedPreferences("sharedPref",Context.MODE_MULTI_PROCESS).edit();
                editor.putBoolean("sign_language_mode_on", isChecked);
                editor.commit();
            }
        });

        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.KITKAT) {
            reviewModeSwitch.setSwitchTextAppearance(this, R.style.SwitchTextAppearance);
            signLanguageSwitch.setSwitchTextAppearance(this, R.style.SwitchTextAppearance);
        }

        displayCurrentUser();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            Util.hideSystemUI(this);
        }
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
        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
        logger.tagScreen("AdministrationActivity");
        displayCurrentUser();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    private void displayCurrentUser() {
        Util.displayUserName(this, (TextView) findViewById(R.id.textView_currentUserId));
    }

    public void onClickReviewMode(View v) {
        v.findViewById(R.id.reviewModeSwitch).performClick();
    }

    public void onClickSignLanguageMode(View v) {
        v.findViewById(R.id.signLanguageSwitch).performClick();
    }

    private Rect mTempRect = new Rect();
    private boolean mbPressed = false;
    private View.OnTouchListener mBlockViewTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                mVBack.getGlobalVisibleRect(mTempRect);
                if (mTempRect.contains((int) event.getX(), (int) event.getY())) {
                    mbPressed = true;
                    mVBack.dispatchTouchEvent(event);
                } else {
                    mbPressed = false;
                }
            } else {
                if (mbPressed) {
                    mVBack.dispatchTouchEvent(event);
                }

                if (event.getAction() == MotionEvent.ACTION_UP) {
                    mbPressed = false;
                }
            }

            return true;
        }
    };
}
