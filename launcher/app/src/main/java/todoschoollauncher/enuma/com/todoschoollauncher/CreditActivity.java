package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.TextView;

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

/**
 * Created by ingtellect on 1/11/17.
 */

public class CreditActivity extends KitKitLoggerActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_credit);
    }

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
        logger.tagScreen("CreditActivity");
    }

    public void onClickClose(View v) {
        finish();
    }
}
