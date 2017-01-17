package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

/**
 * Created by ingtellect on 1/3/17.
 */

public class ToolsActivity extends AppCompatActivity {

    final String cameraPackageName = "com.android.camera2";
    final String galleryPackageName = "com.android.gallery3d";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });


        ImageButton cameraButton = (ImageButton) findViewById(R.id.button_camera);
        final PackageManager pm = getApplicationContext().getPackageManager();
        ApplicationInfo ai;
        try {
            ai = pm.getApplicationInfo( cameraPackageName, 0);
        } catch (final PackageManager.NameNotFoundException e) {
            ai = null;
        }
        final String cameraAppName = (String) (ai != null ? pm.getApplicationLabel(ai) : "(unknown)");
        TextView cameraTextView = (TextView) findViewById(R.id.camera_textView);
        cameraTextView.setText(cameraAppName);

        cameraButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = getPackageManager().getLaunchIntentForPackage(cameraPackageName);
                startActivity(i);
            }
        });


        final ImageButton gallaryButton = (ImageButton) findViewById(R.id.button_gallery);
        try {
            ai = pm.getApplicationInfo( galleryPackageName, 0);
        } catch (final PackageManager.NameNotFoundException e) {
            ai = null;
        }
        final String galleryAppName = (String) (ai != null ? pm.getApplicationLabel(ai) : "(unknown)");
        TextView galleryTextView = (TextView) findViewById(R.id.gallery_textView);
        galleryTextView.setText(galleryAppName);

        gallaryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = getPackageManager().getLaunchIntentForPackage(galleryPackageName);
                startActivity(i);
            }
        });


//        ImageButton settingButton = (ImageButton) findViewById(R.id.button_setting);
//        try {
//            Drawable settingIcon = getPackageManager().getApplicationIcon("com.android.settings");
//            settingButton.setImageDrawable(settingIcon);
//        }
//        catch (PackageManager.NameNotFoundException ex) {
//
//        }
//
//        settingButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent i = getPackageManager().getLaunchIntentForPackage("com.android.settings");
//                startActivity(i);
//            }
//        });

    }
}
