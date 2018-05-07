package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.DialogFragment;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;


/**
 * Created by ingtellect on 1/10/17.
 */

public class AboutActivity extends KitKitLoggerActivity implements PasswordDialogFragment.PasswordDialogListener {

    private static final String TAG = "AboutActivity";
    private View mVBack;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);
//        hideStatusbar();
        setContentView(R.layout.activity_about);

        Util.setScale(this, findViewById(R.id.root_container));
        mVBack = findViewById(R.id.v_back);
        mVBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });


        TextView versionTextView = (TextView) findViewById(R.id.version_textView);
        final PackageManager pm = getApplicationContext().getPackageManager();

        final String kitkitSchoolPackageName = "com.enuma.xprize";
        try {
            PackageInfo kitkitPInfo = pm.getPackageInfo(kitkitSchoolPackageName, 0);
            ApplicationInfo kitkitAInfo = pm.getApplicationInfo(kitkitSchoolPackageName, 0);

            String appName = (String) (kitkitAInfo != null ? pm.getApplicationLabel(kitkitAInfo) : "");
            String versionName = kitkitPInfo.versionName;

            versionTextView.setText(appName + " " + versionName);
        } catch (PackageManager.NameNotFoundException ex) {

        }

        displayCurrentUser();

//        cntx = this.getBaseContext();
        //listLogFiles();

    }

    private void hideStatusbar() {
        View decorView = getWindow().getDecorView();
        // Hide the status bar.
        int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);

    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
//        hideStatusbar();
        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    public void onClickSetting(View v) {


        DialogFragment dialog = new PasswordDialogFragment();
        Bundle bundle = new Bundle();
        if (v.getId() == R.id.setting_button) {
            bundle.putSerializable("userobject", "SETTINGS");

        } else {
            bundle.putSerializable("userobject", "USERS");
        }
        dialog.setArguments(bundle);
        dialog.show(getFragmentManager(), "PasswordDialogFragment");

    }

    public void onClickCredit(View v) {
        Intent i = new Intent(AboutActivity.this, CreditActivity.class);
        startActivity(i);
    }

    @Override
    public void onDialogPositiveClick(DialogFragment dialog, String redirectTo) {
//        Intent i = getPackageManager().getLaunchIntentForPackage("com.android.settings")
        Log.d("received arg", redirectTo);
        if (redirectTo.equals("SETTINGS")) {
            Intent i = new Intent(AboutActivity.this, SettingActivity.class);
            startActivity(i);
        } else {
            DialogFragment userdialog = new SelectUserDialogFragment();
            Bundle userbundle = new Bundle();
            userbundle.putSerializable("user", "ABOUT");
            userdialog.setArguments(userbundle);
            userdialog.show(getFragmentManager(), "SelectUserDialogFragment");
        }

    }

    @Override
    public void onDialogNegativeClick(DialogFragment dialog) {
        dialog.dismiss();
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
        logger.tagScreen("AboutActivity");
        displayCurrentUser();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    private void displayCurrentUser(){
        User user = ((LauncherApplication)getApplication()).getDbHandler().getCurrentUser();
        String currentUsername = user.getUserName();
        TextView textViewUsername = (TextView)findViewById(R.id.textView_currentUserId_about);

        if ("user0".equalsIgnoreCase(currentUsername) == false) {
            textViewUsername.setText(currentUsername);

        } else {
            textViewUsername.setText("");

        }
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
