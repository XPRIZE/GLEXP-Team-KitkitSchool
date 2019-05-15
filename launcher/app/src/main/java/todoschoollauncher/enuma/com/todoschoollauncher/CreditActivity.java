package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

/**
 * Created by ingtellect on 1/11/17.
 */

public class CreditActivity extends KitKitLoggerActivity {
    private View mVClose;

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
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
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

    public void onClickClose(View v) {
        finish();
    }
}
