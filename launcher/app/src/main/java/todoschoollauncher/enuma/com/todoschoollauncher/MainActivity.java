package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.KeyguardManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.content.Intent;

import android.graphics.drawable.Drawable;
import android.widget.Toast;


import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

import org.apache.commons.io.filefilter.WildcardFileFilter;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.List;




public class MainActivity extends KitKitLoggerActivity {

    public static String TAG = "TodoschoolLancher";
    private FtpClient ftpclient = null;
    private long lastLogSentTimeInMillis = 0;
    private static final long logCooldownTimeInMillis = 5 * 60 * 1000; // 5 minues
    private static Thread ftpConnector = null;
    private static Thread logUploader = null;

    private Context cntx = null;

    private  PowerConnectionReceiver _batteryinfo = new PowerConnectionReceiver() {
        @Override
        public  void onReceive(Context context, Intent intent) {
            int chargePlug = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, -1);
            boolean usbCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_USB;
            boolean acCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_AC;

//            Log.d("PowerConnectionReceiver", usbCharge ? "usb charging true" : "usb charging not true");
//            Log.d("PowerConnectionReceiver", acCharge ? "ac charging true" : "ac charging not true");
            if (usbCharge|acCharge) {
                connectToWifi();
            }

        }
    };

    private WifiReceiver _wifiInfo = new WifiReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION)) {

                ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = cm.getActiveNetworkInfo();

                if (networkInfo != null && networkInfo.getType() == ConnectivityManager.TYPE_WIFI &&
                        networkInfo.isConnected()) {
                    // Wifi is connected
                    WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
                    WifiInfo wifiInfo = wifiManager.getConnectionInfo();
                    String ssid = wifiInfo.getSSID();

                    Log.d(TAG, " -- Wifi connected --- " + " SSID " + ssid );
                    connectToFTPAddress();

                }
            }
            else if (intent.getAction().equalsIgnoreCase(WifiManager.WIFI_STATE_CHANGED_ACTION))
            {
                int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, WifiManager.WIFI_STATE_UNKNOWN);
                if (wifiState == WifiManager.WIFI_STATE_DISABLED)
                {
                    Log.e(TAG, " ----- Wifi  Disconnected ----- ");
                }

            }
        }
    };


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
        cntx = getBaseContext();

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
        ImageButton libraryButton = (ImageButton) findViewById(R.id.button_library);
        libraryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (view.isEnabled()) {
                    Intent i = manager.getLaunchIntentForPackage("library.todoschool.enuma.com.todoschoollibrary");
                    startActivity(i);
                }
            }
        });

        ImageButton toolsButton = (ImageButton) findViewById(R.id.button_tool);
        toolsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (view.isEnabled()) {
                    Intent i = new Intent(MainActivity.this, ToolsActivity.class);
                    startActivity(i);
                }
            }
        });

        Button title = (Button)findViewById(R.id.launcher_title_button);
        Typeface face = Typeface.createFromAsset(getAssets(),
                "TodoMainCurly.ttf");
        title.setTypeface(face);

        final Handler handle = new Handler();

        title.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View arg0, MotionEvent arg1) {
                switch (arg1.getAction()) {
                    case MotionEvent.ACTION_BUTTON_PRESS:
                    case MotionEvent.ACTION_DOWN:
                        //Log.d("onTouch", "touch started");

                        handle.postDelayed(run, 5*1000);
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


//        ImageButton aboutButton = (ImageButton)findViewById(R.id.button_about);
//
//
//
//        final Handler handle = new Handler();
//
//        aboutButton.setOnTouchListener(new View.OnTouchListener() {
//
//            @Override
//            public boolean onTouch(View arg0, MotionEvent arg1) {
//                switch (arg1.getAction()) {
//                    case MotionEvent.ACTION_BUTTON_PRESS:
//                    case MotionEvent.ACTION_DOWN:
//                        //Log.d("onTouch", "touch started");
//
//                        handle.postDelayed(run, 10);
//                        break;
//                    case MotionEvent.ACTION_UP:
//                    case MotionEvent.ACTION_CANCEL:
//                    case MotionEvent.ACTION_BUTTON_RELEASE:
//                        //Log.d("onTouch", "touch canceled");
//                        handle.removeCallbacks(run);
//
//                    default:
//                        break;
//
//                }
//                return true;
//            }
//
//            Runnable run = new Runnable() {
//                @Override
//                public void run() {
//                    //Log.d("runnable", "start setting activity");
//                    Intent i = new Intent(MainActivity.this, AboutActivity.class);
//                    startActivity(i);
//                }
//            };
//        });

        registerLockscreenReceiver();

        TextView textViewCoinNum = (TextView)findViewById(R.id.textView_numCoin);
        Typeface f = Typeface.createFromAsset(getAssets(), "TodoMainCurly.ttf");
        textViewCoinNum.setTypeface(f);

        ftpclient = new FtpClient();

        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        registerReceiver(_batteryinfo, ifilter);

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(_wifiInfo, intentFilter);

    }
    /** code to post/handler request for permission */
    public static final int OVERLAY_PERMISSION_REQ_CODE = 4545;

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
        logger.tagScreen("MainActivity");

        User currentUser = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();

        ImageButton libraryButton = (ImageButton) findViewById(R.id.button_library);
        ImageButton toolsButton = (ImageButton) findViewById(R.id.button_tool);

        LinearLayout libraryOverlay = (LinearLayout) findViewById(R.id.overlay_library);
        LinearLayout toolsOverlay = (LinearLayout) findViewById(R.id.overlay_tools);

        if(currentUser.isFinishTutorial()) {
            libraryButton.setEnabled(true);
            libraryOverlay.setBackgroundColor(ContextCompat.getColor(this, android.R.color.transparent));
            toolsButton.setEnabled(true);
            toolsOverlay.setBackgroundColor(ContextCompat.getColor(this, android.R.color.transparent));

        }
        else {
            libraryButton.setEnabled(false);
            libraryOverlay.setBackgroundColor(Color.parseColor("#cc000000"));
            toolsButton.setEnabled(false);
            toolsOverlay.setBackgroundColor(Color.parseColor("#cc000000"));

        }
        TextView textViewCoinNum = (TextView)findViewById(R.id.textView_numCoin);
        textViewCoinNum.setText(String.format("%d",currentUser.getNumStars()));

    }


    @Override
    protected void onDestroy() {

        unregisterReceiver(receiver);
        unregisterReceiver(_batteryinfo);
        unregisterReceiver(_wifiInfo);

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
            if(Build.VERSION.SDK_INT >= 23) {
                if (!Settings.canDrawOverlays(this)) {
                    Toast.makeText(this, "User can access system settings without this permission!", Toast.LENGTH_SHORT).show();
                } else {
                    disableStatusBar();
                }
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
        localLayoutParams.height = (int) (24 * getResources().getDisplayMetrics().scaledDensity);
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

        try{
            key.disableKeyguard();
        }
        catch (SecurityException e)
        {
            //kindle code goes here
        }

        //Start listening for the Screen On, Screen Off, and Boot completed actions
        IntentFilter filter = new IntentFilter(Intent.ACTION_SCREEN_ON);
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        filter.addAction(Intent.ACTION_BOOT_COMPLETED);

        //Set up a receiver to listen for the Intents in this Service
        receiver = new LockScreenReceiver();
        registerReceiver(receiver, filter);
    }


    public boolean connectToWifi(){
        try{
            WifiManager wifiManager = (WifiManager) super.getApplicationContext().getSystemService(android.content.Context.WIFI_SERVICE);
            WifiConfiguration wc = new WifiConfiguration();
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            wc.SSID = "\"XPRIZE\"";
            wc.status = WifiConfiguration.Status.ENABLED;
            wc.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
            wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
            wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
            wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
            wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
            wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            wifiManager.setWifiEnabled(true);
            int netId = wifiManager.addNetwork(wc);
            Log.d("wifitest", "netid : " + netId);
            if (netId == -1) {
                netId = getExistingNetworkId(wc.SSID);
                Log.d("wifitest", "netid2 : " + netId);
            }
            wifiManager.disconnect();
            wifiManager.enableNetwork(netId, true);
            return wifiManager.reconnect();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private int getExistingNetworkId(String SSID) {
        WifiManager wifiManager = (WifiManager) super.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        List<WifiConfiguration> configuredNetworks = wifiManager.getConfiguredNetworks();
        if (configuredNetworks != null) {
            for (WifiConfiguration existingConfig : configuredNetworks) {
                if (existingConfig.SSID.equals(SSID)) {
                    return existingConfig.networkId;
                }
            }
        }
        return -1;
    }

    private Handler handler = new Handler() {

        public void handleMessage(android.os.Message msg) {

            if (msg.what == 0) {
                // try upload
                uploadLogs();
            } else if (msg.what == 1) {
            } else if (msg.what == 2) {
                //Toast.makeText(MainActivity.this, "Uploaded Successfully!",
                //        Toast.LENGTH_LONG).show();
                ftpclient.ftpDisconnect();
            } else if (msg.what == 3) {
                //Toast.makeText(MainActivity.this, "Disconnected Successfully!",
                //        Toast.LENGTH_LONG).show();
            } else {
                //Toast.makeText(MainActivity.this, "Unable to Perform Action! : " + msg.what,
                //        Toast.LENGTH_LONG).show();
            }

        }

    };


    private boolean isOnline(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context
                .getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        if (netInfo != null && netInfo.isConnected()) {
            return true;
        }
        return false;
    }

    private void connectToFTPAddress() {
        if (System.currentTimeMillis() < lastLogSentTimeInMillis + logCooldownTimeInMillis) {
            //  Cooldown
            handler.sendEmptyMessage(-1);
            return;
        }

        if (ftpConnector != null) {
            //  Already connecting
            handler.sendEmptyMessage(-1);
            return;
        }

        // [v] NB(xenosoz, 2017): We don't edit ftp address preferences here. (cf. SettingActivity.connectTOFTPAddress)
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        final String host = prefs.getString("host", "192.168.0.1");
        final String username = prefs.getString("username", "anonymous");
        final String password = prefs.getString("password", "");
        final int port = Integer.parseInt(prefs.getString("port", "21"));
        // [^]

        Log.d(TAG,"connecting to ftp. "+username+"@"+host+":"+port + " password : "+password);
        ftpConnector = new Thread(new Runnable() {
            public void run() {
                try {
                    boolean status = false;
                    int retryCount = 5;
                    while (!status && retryCount > 0) {
                        retryCount -= 1;
                        status = ftpclient.ftpConnect(host, username, password, port);
                        if (!status) {
                            try {
                                Thread.sleep(5000);
                            } catch (InterruptedException e) {
                                // Pass
                            }
                        }
                    }
                    if (status == true) {
                        Log.d(TAG, "Connection Success");
                        handler.sendEmptyMessage(0);
                    } else {
                        Log.d(TAG, "Connection failed");
                        handler.sendEmptyMessage(-1);
                    }
                }
                finally {
                    ftpConnector = null;
                }
            }
        });
        ftpConnector.start();
    }

    private void uploadLogs() {
        if (logUploader != null) return;
        logUploader = new Thread(new Runnable() {
            public void run() {
                try {
                    boolean status = false;
                    String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
                    String logPath = documentsPath + "/logs";

                    File[] txts = new File(logPath).listFiles((FileFilter)new WildcardFileFilter("*.txt"));
                    for (File txt : txts) {
                        Log.d("TXTTest", txt.toString());
                        status = ftpclient.ftpUpload(
                                txt.getPath(),
                                txt.getName(), "remote/", cntx);

                    }

                    if (status == true) {
                        Log.d(TAG, "Upload success");
                        lastLogSentTimeInMillis = System.currentTimeMillis();
                        handler.sendEmptyMessage(2);
                    } else {
                        Log.d(TAG, "Upload failed");
                        handler.sendEmptyMessage(-1);
                    }
                }
                finally {
                    logUploader = null;
                }
            }
        });
        logUploader.start();
    }

}
