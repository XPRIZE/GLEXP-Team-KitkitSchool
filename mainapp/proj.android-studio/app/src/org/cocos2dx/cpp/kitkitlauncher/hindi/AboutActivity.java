package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.app.DialogFragment;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.maq.kitkitlogger.KitKitLoggerActivity;
import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.KitkitSchoolApplication;


/**
 * Created by ingtellect on 1/10/17.
 */

public class AboutActivity extends KitKitLoggerActivity implements PasswordDialogFragment.PasswordDialogListener {
    private View mVBack;
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);
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

        final String kitkitSchoolPackageName = "com.maq.xprize.kitkitschool.hindi";
        try {
            PackageInfo kitkitPInfo = pm.getPackageInfo(kitkitSchoolPackageName, 0);
            ApplicationInfo kitkitAInfo = pm.getApplicationInfo(kitkitSchoolPackageName, 0);

            String appName = (String) (kitkitAInfo != null ? pm.getApplicationLabel(kitkitAInfo) : "");
            String versionName = kitkitPInfo.versionName;

            versionTextView.setText(appName + " " + versionName);
        } catch (PackageManager.NameNotFoundException ex) {

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

    public void onClickSetting(View v) {
        Intent i = new Intent(AboutActivity.this, SettingActivity.class);
        startActivity(i);
    }

    public void onClickSelectUser(View v) {
        SelectNumberDialog selectNumberDialog = new SelectNumberDialog(this, SelectNumberDialog.MODE.USER_NO, new SelectNumberDialog.Callback() {
            @Override
            public void onSelectedNumber(int number) {
                KitkitDBHandler dbHandler = ((KitkitSchoolApplication) getApplication()).getDbHandler();
                User user = dbHandler.findUser("user" + number);
                if (user != null) {
                    dbHandler.setCurrentUser(user);
                    displayCurrentUser();
                }
            }
        });

        selectNumberDialog.show();
    }

    public void onClickAdministration(View v) {
        DialogFragment dialog = new PasswordDialogFragment();
        Bundle bundle = new Bundle();
        bundle.putSerializable("userobject", "ADMINISTRATION");
        dialog.setArguments(bundle);
        dialog.show(getFragmentManager(), "PasswordDialogFragment");
    }

    public void onClickCredit(View v) {
        Intent i = new Intent(AboutActivity.this, CreditActivity.class);
        startActivity(i);
    }

    @Override
    public void onDialogPositiveClick(DialogFragment dialog, String redirectTo) {
        dialog.dismiss();
        if (redirectTo.equals("ADMINISTRATION")) {
            Intent i = new Intent(AboutActivity.this, AdministrationActivity.class);
            startActivity(i);
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
        KitKitLogger logger = ((KitkitSchoolApplication) getApplication()).getLogger();
        logger.tagScreen("AboutActivity");
        displayCurrentUser();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    private void displayCurrentUser() {
        Util.displayUserName(this, (TextView) findViewById(R.id.textView_currentUserId_about));
    }
}
