package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.DialogFragment;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;


/**
 * Created by ingtellect on 1/10/17.
 */

public class AboutActivity extends KitKitLoggerActivity implements PasswordDialogFragment.PasswordDialogListener {

    private static final String TAG = "AboutActivity";
//    private Context cntx = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        hideStatusbar();
        setContentView(R.layout.activity_about);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });


        TextView versionTextView = (TextView)findViewById(R.id.version_textView);
        final PackageManager pm = getApplicationContext().getPackageManager();

        final String kitkitSchoolPackageName = "com.enuma.xprize";
        try {
            PackageInfo kitkitPInfo = pm.getPackageInfo(kitkitSchoolPackageName,0);
            ApplicationInfo kitkitAInfo = pm.getApplicationInfo(kitkitSchoolPackageName,0);

            String appName = (String) (kitkitAInfo != null ? pm.getApplicationLabel(kitkitAInfo) : "");
            String versionName = kitkitPInfo.versionName;

            versionTextView.setText(appName + " "+ versionName);
        }
        catch (PackageManager.NameNotFoundException ex) {

        }

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
        hideStatusbar();
    }

    public void onClickSetting(View v) {
        Log.d("onclickSetting", "show setting dialog");
        DialogFragment dialog = new PasswordDialogFragment();
        dialog.show(getFragmentManager() ,"PasswordDialogFragment");
    }

    public void onClickCredit(View v) {
        Intent i = new Intent(AboutActivity.this, CreditActivity.class);
        startActivity(i);
    }

    @Override
    public void onDialogPositiveClick(DialogFragment dialog) {
//        Intent i = getPackageManager().getLaunchIntentForPackage("com.android.settings");
        Intent i = new Intent(AboutActivity.this, SettingActivity.class);
        startActivity(i);
    }

    @Override
    public void onDialogNegativeClick(DialogFragment dialog) {
        dialog.dismiss();
    }

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
        logger.tagScreen("AboutActivity");
    }

}
