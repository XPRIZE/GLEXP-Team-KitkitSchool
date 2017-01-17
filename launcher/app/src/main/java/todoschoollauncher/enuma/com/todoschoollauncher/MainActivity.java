package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.KeyguardManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.TextView;
import android.content.Intent;

import android.graphics.drawable.Drawable;
import android.widget.Toast;



import java.util.ArrayList;
import java.util.List;




public class MainActivity extends AppCompatActivity {

    public static String TAG = "TodoschoolLancher";

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_FULLSCREEN
                |View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                |View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //hideSystemUI();

        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {

            if (!Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Please give my app this permission!", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, OVERLAY_PERMISSION_REQ_CODE);
            } else {
                disableStatusBar();
            }
        }
        else {
            disableStatusBar();
        }

        loadApps();

        AppDetail todoschool = getAppDetail("com.enuma.xprize");
        ImageButton todoSchoolButton = (ImageButton) findViewById(R.id.button_todoschool);
        todoSchoolButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = manager.getLaunchIntentForPackage("com.enuma.xprize");
                startActivity(i);
            }
        });

        AppDetail library = getAppDetail("library.todoschool.enuma.com.todoschoollibrary");
        ImageButton todoLibraryButton = (ImageButton) findViewById(R.id.button_library);
        todoLibraryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = manager.getLaunchIntentForPackage("library.todoschool.enuma.com.todoschoollibrary");
                startActivity(i);
            }
        });

        ImageButton toolsButton = (ImageButton) findViewById(R.id.button_tool);
        toolsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(MainActivity.this, ToolsActivity.class);
                startActivity(i);
            }
        });

        TextView tv = (TextView)findViewById(R.id.launcher_title_textView);
        Typeface face = Typeface.createFromAsset(getAssets(),
                "TodoMainCurly.ttf");
        tv.setTypeface(face);

        ImageButton aboutButton = (ImageButton)findViewById(R.id.button_about);



        final Handler handle = new Handler();

        aboutButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View arg0, MotionEvent arg1) {
                switch (arg1.getAction()) {
                    case MotionEvent.ACTION_BUTTON_PRESS:
                    case MotionEvent.ACTION_DOWN:
                        //Log.d("onTouch", "touch started");

                        handle.postDelayed(run, 2000);
                        break;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                    case MotionEvent.ACTION_BUTTON_RELEASE:
                        //Log.d("onTouch", "touch canceled");
                        handle.removeCallbacks(run);

                    default:
                        break;

                }
                return true;
            }

            Runnable run = new Runnable() {
                @Override
                public void run() {
                    //Log.d("runnable", "start setting activity");
                    Intent i = new Intent(MainActivity.this, AboutActivity.class);
                    startActivity(i);
                }
            };
        });

        registerLockscreenReceiver();

    }
    /** code to post/handler request for permission */
    public static final int OVERLAY_PERMISSION_REQ_CODE = 4545;

    @Override
    protected void onDestroy() {

        unregisterReceiver(receiver);

        super.onDestroy();

        if (blockingView!=null) {
            WindowManager manager = ((WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE));
            manager.removeView(blockingView);
            Log.v("MainActivity", "blockingView destroyed");

        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == OVERLAY_PERMISSION_REQ_CODE) {
            if (!Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "User can access system settings without this permission!", Toast.LENGTH_SHORT).show();
            }
            else
            {
                disableStatusBar();
            }
        }
    }

    public static class CustomViewGroup extends ViewGroup {

        public CustomViewGroup(Context context) {
            super(context);
        }

        @Override
        protected void onLayout(boolean changed, int l, int t, int r, int b) {
        }

        @Override
        public boolean onInterceptTouchEvent(MotionEvent ev) {
            Log.v("customViewGroup", "**********Intercepted");
            return true;
        }
    }
    protected CustomViewGroup blockingView = null;

    protected void disableStatusBar() {

        WindowManager manager = ((WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE));

        WindowManager.LayoutParams localLayoutParams = new WindowManager.LayoutParams();
        localLayoutParams.type = WindowManager.LayoutParams.TYPE_SYSTEM_ERROR;
        localLayoutParams.gravity = Gravity.TOP;
        localLayoutParams.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |

                // this is to enable the notification to receive touch events
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |

                // Draws over status bar
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;

        localLayoutParams.width = WindowManager.LayoutParams.MATCH_PARENT;
        localLayoutParams.height = (int) (40 * getResources().getDisplayMetrics().scaledDensity);
        localLayoutParams.format = PixelFormat.TRANSPARENT;

        blockingView = new CustomViewGroup(this);
        manager.addView(blockingView, localLayoutParams);
    }



//    @Override
//    public void onWindowFocusChanged(boolean hasFocus) {
//
//        super.onWindowFocusChanged(hasFocus);
//
//        if (hasFocus) {
//            hideSystemUI();
//        }
//
//
//    }


    private static PackageManager manager;
    private List<AppDetail> apps;

    private void loadApps() {
        manager = getPackageManager();
        apps = new ArrayList<AppDetail>();

        Intent i = new Intent(Intent.ACTION_MAIN, null);
        i.addCategory(Intent.CATEGORY_LAUNCHER);

        List<ResolveInfo> availableActivities = manager.queryIntentActivities(i, 0);
        for (ResolveInfo ri : availableActivities) {
            AppDetail app = new AppDetail();
            app.label = ri.loadLabel(manager);
            app.name = ri.activityInfo.packageName;
            app.icon = ri.activityInfo.loadIcon(manager);
            apps.add(app);
            Log.d(TAG, app.name.toString());
        }
    }

    private String getAppListText() {
        String appListText = "";
        for (AppDetail app : apps) {
            appListText += app.name + "\n";
        }
        return appListText;
    }

    private AppDetail getAppDetail(String name) {
        for (AppDetail app : apps) {
            if (app.name.equals(name)) {
                return app;
            }
        }
        return null;
    }

    public static class AppDetail {
        CharSequence label;
        CharSequence name;
        Drawable icon;
    }

    private void closeApp() {
        this.finish();
    }

    @Override
    public  boolean onKeyDown(int keyCode, KeyEvent event) {
        return false;
    }

    BroadcastReceiver receiver;


    private void registerLockscreenReceiver() {
        KeyguardManager.KeyguardLock key;
        KeyguardManager km = (KeyguardManager)getSystemService(KEYGUARD_SERVICE);

        //This is deprecated, but it is a simple way to disable the lockscreen in code
        key = km.newKeyguardLock("IN");

        key.disableKeyguard();

        //Start listening for the Screen On, Screen Off, and Boot completed actions
        IntentFilter filter = new IntentFilter(Intent.ACTION_SCREEN_ON);
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        filter.addAction(Intent.ACTION_BOOT_COMPLETED);

        //Set up a receiver to listen for the Intents in this Service
        receiver = new LockScreenReceiver();
        registerReceiver(receiver, filter);
    }

}
