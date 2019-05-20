package com.maq.xprize.kitkitlauncher.hindi;

import android.app.DialogFragment;
import android.app.KeyguardManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.support.annotation.RequiresApi;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.maq.kitkitlogger.KitKitLoggerActivity;

import org.apache.commons.io.filefilter.WildcardFileFilter;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;


public class MainActivity extends KitKitLoggerActivity implements PasswordDialogFragment.PasswordDialogListener {

    public static String TAG = "MainActivity";
    private FtpClient ftpclient = null;

    private long lastLogSentTimeInMillis = 0;
    private static final long logCooldownTimeInMillis = 5 * 60 * 1000; // 5 minutes

    private long nextTimeUpdateInMillis = 0;
    private static final long timeUpdateRetryTimeInMillis = 1 * 60 * 1000; // 1 minute
    private static final long timeUpdateCooldownTimeInMillis = 60 * 60 * 1000; // 60 minutes

    private static Thread ftpConnector = null;
    private static Thread logUploader = null;
    private static Thread imageUploader = null;

    private Context cntx = null;
    private Button mTitle;
    private TextView mTvUserName;

    /*private _batteryinfo = new PowerConnectionReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            int chargePlug = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, -1);
            boolean usbCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_USB;
            boolean acCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_AC;

            //            Log.d("PowerConnectionReceiver", usbCharge ? "usb charging true" : "usb charging not true");
            //            Log.d("PowerConnectionReceiver", acCharge ? "ac charging true" : "ac charging not true");
            if (usbCharge || acCharge) {
                checkNetworkAndProcess(context);
                //                connectToWifi();
            }
        }
    };*/

    private WifiReceiver _wifiInfo = new WifiReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            /*if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
//                checkNetworkAndProcess(context);
//                ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
//                NetworkInfo networkInfo = cm.getActiveNetworkInfo();
//
//                if (networkInfo != null && networkInfo.getType() == ConnectivityManager.TYPE_WIFI &&
//                        networkInfo.isConnected()) {
//                    // Wifi is connected
//                    WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
//                    WifiInfo wifiInfo = wifiManager.getConnectionInfo();
//                    String ssid = wifiInfo.getSSID();
//
//                    Log.d(TAG, " -- Wifi connected --- " + " SSID " + ssid );
//                    connectToFTPAddress();
//
//                }
            } else if (intent.getAction().equalsIgnoreCase(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, WifiManager.WIFI_STATE_UNKNOWN);
                if (wifiState == WifiManager.WIFI_STATE_DISABLED) {
                    Log.e(TAG, "----- Wifi  Disconnected -----");
                }

            }*/
        }
    };

    private void checkNetworkAndProcess(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = cm.getActiveNetworkInfo();

        if (networkInfo != null && networkInfo.getType() == ConnectivityManager.TYPE_WIFI &&
                networkInfo.isConnected()) {
// Wifi is connected
            WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            String ssid = wifiInfo.getSSID();

            Log.d(TAG, "-- Wifi connected ---" + " SSID " + ssid);
            if (ssid.equals("\"XPRIZE\"")) {
                connectToFTPAddress();
                connectToSNTPServers();
            } else {
                connectToWifi();
            }
        } else {
            connectToWifi();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Util.hideSystemUI(this);
        cntx = getBaseContext();
        Util.setScale(this, findViewById(R.id.main_content));

        /*if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {

            if (!Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Please give my app this permission!", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, OVERLAY_PERMISSION_REQ_CODE);
            } else {
                *//*Util.disableStatusBar(this);*//*
            }
        } else {
            *//*Util.disableStatusBar(this);*//*
        }*/

        loadApps();

        String mainAppPackageName = "com.maq.xprize.kitkitschool.hindi";
        final File mainAppFlagFile = new File("/storage/emulated/0/Android/data/" + mainAppPackageName + "/files/.success.txt");
        ImageButton todoSchoolButton = findViewById(R.id.button_todoschool);

        todoSchoolButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (gotoVideoPlayer() == false) {
                    try {
                        Intent i = new Intent(Intent.ACTION_MAIN);
                        SharedPreferences prefs = getSharedPreferences("किटकिट स्कूल - Kitkit School", Context.MODE_PRIVATE);
                        if (!mainAppFlagFile.exists()) {
                            i.setComponent(new ComponentName("com.maq.xprize.kitkitschool.hindi", "org.cocos2dx.cpp.SplashScreenActivity"));
                            startActivity(i);
                        } else {
                            i.setComponent(new ComponentName("com.maq.xprize.kitkitschool.hindi", "org.cocos2dx.cpp.AppActivity"));
                            startActivity(i);
                        }
                    } catch (Exception e) {
                        Intent i = new Intent(MainActivity.this, VideoPlayerActivity.class);
                        i.putExtra("video", "main_app_demo_video");
                        startActivity(i);
                        Toast.makeText(MainActivity.this, "Click the install button to download this learning module", Toast.LENGTH_LONG).show();
                        overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
                    }
                }
            }
        });

        String libraryAppPackageName = "com.maq.xprize.kitkitlibrary.english";
        final File libraryAppFlagFile = new File("/storage/emulated/0/Android/data/" + libraryAppPackageName + "/files/.success.txt");
        ImageButton libraryButton = findViewById(R.id.button_library);
        libraryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (gotoVideoPlayer() == false) {
                    if (view.isEnabled()) {
//                    Intent i = manager.getLaunchIntentForPackage("com.maq.xprize.kitkitlibrary.english");
                        try {
                            Intent i = new Intent(Intent.ACTION_MAIN);
                            if (!libraryAppFlagFile.exists()) {
                                i.setComponent(new ComponentName("com.maq.xprize.kitkitlibrary.english", "com.maq.xprize.kitkitlibrary.english.SplashScreenActivity"));
                            } else {
                                i.setComponent(new ComponentName("com.maq.xprize.kitkitlibrary.english", "com.maq.xprize.kitkitlibrary.english.SelectActivity"));
                            }
                            startActivity(i);
                        } catch (Exception e) {
                            Intent i = new Intent(MainActivity.this, VideoPlayerActivity.class);
                            i.putExtra("video", "library_app_demo");
                            startActivity(i);
                            Toast.makeText(MainActivity.this, "Click the install button to download this Library app", Toast.LENGTH_LONG).show();
                            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
                        }
                    }
                }
            }
        });

        ImageButton toolsButton = findViewById(R.id.button_tool);
        toolsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (gotoVideoPlayer() == false) {
                    if (view.isEnabled()) {
                        Intent i = new Intent(MainActivity.this, ToolsActivity.class);
                        startActivity(i);
                    }
                }
            }
        });

        mTitle = findViewById(R.id.launcher_title_button);
        Typeface face = Typeface.createFromAsset(getAssets(),
                "TodoMainCurly.ttf");
        mTitle.setTypeface(face);
        mTitle.setOnTouchListener(mLongTouchListener);

        mTvUserName = findViewById(R.id.textView_currentUserId);
        mTvUserName.setOnTouchListener(mLongTouchListener);

        registerLockscreenReceiver();

        TextView textViewCoinNum = findViewById(R.id.textView_numCoin);
        Typeface f = Typeface.createFromAsset(getAssets(), "TodoMainCurly.ttf");
        textViewCoinNum.setTypeface(f);

        displayCurrentUser();

        ftpclient = new FtpClient();

        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
//        registerReceiver(_batteryinfo, ifilter);

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(_wifiInfo, intentFilter);

        setDefaultPreference();
    }

    /**
     * code to post/handler request for permission
     */
    public static final int OVERLAY_PERMISSION_REQ_CODE = 4545;

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
        logger.tagScreen("MainActivity");

        refreshUI();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    @Override
    protected void onDestroy() {

        unregisterReceiver(receiver);
//        unregisterReceiver(_batteryinfo);
        unregisterReceiver(_wifiInfo);

        super.onDestroy();

        Util.recycle(getWindow().getDecorView());
        if (Util.mBlockingView != null) {
            WindowManager manager = ((WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE));
            manager.removeView(Util.mBlockingView);
            Util.mBlockingView = null;
            Log.v("MainActivity", "mBlockingView destroyed");
        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == OVERLAY_PERMISSION_REQ_CODE) {
            if (Build.VERSION.SDK_INT >= 23) {
                if (!Settings.canDrawOverlays(this)) {
                    Toast.makeText(this, "User can access system settings without this permission!", Toast.LENGTH_SHORT).show();
                } else {
                    Util.disableStatusBar(this);
                }
            }
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    private void refreshUI() {
        User currentUser = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();

        ImageButton libraryButton = findViewById(R.id.button_library);
        ImageButton toolsButton = findViewById(R.id.button_tool);

        View libraryOverlay = findViewById(R.id.overlay_library);
        View toolsOverlay = findViewById(R.id.overlay_tools);

        if (currentUser.isOpenLibrary()) {
            libraryButton.setEnabled(true);
            libraryOverlay.setVisibility(View.GONE);

        } else {
            libraryButton.setEnabled(false);
            libraryOverlay.setVisibility(View.VISIBLE);

        }


        if (currentUser.isOpenTools()) {
            toolsButton.setEnabled(true);
            toolsOverlay.setVisibility(View.GONE);

        } else {
            toolsButton.setEnabled(false);
            toolsOverlay.setVisibility(View.VISIBLE);

        }

        TextView textViewCoinNum = findViewById(R.id.textView_numCoin);
        textViewCoinNum.setText(String.format("%d", currentUser.getNumStars()));

        displayCurrentUser();
    }

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

    Rect mTempRect = new Rect();
    private View.OnTouchListener mBlockViewTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                mTitle.getGlobalVisibleRect(mTempRect);
                if (mTempRect.contains((int) event.getX(), (int) event.getY())) {
                    mTitle.dispatchTouchEvent(event);
                }
            } else {
                mTitle.dispatchTouchEvent(event);
            }

            return true;
        }
    };

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return false;
    }

    BroadcastReceiver receiver;


    private void registerLockscreenReceiver() {
        KeyguardManager.KeyguardLock key;
        KeyguardManager km = (KeyguardManager) getSystemService(KEYGUARD_SERVICE);

//This is deprecated, but it is a simple way to disable the lockscreen in code
        key = km.newKeyguardLock("IN");

        try {
            key.disableKeyguard();
        } catch (SecurityException e) {
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


    public boolean connectToWifi() {
        try {
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

    private final int MSG_SUCCESS = 2;
    private final int MSG_UPLOAD_IMAGE = 100;
    private final int MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD = 101;
    private final int MSG_UPLOAD_LOG_IMAGE_SEA_WORLD = 102;

    private Handler handler = new Handler() {

        public void handleMessage(android.os.Message msg) {

            if (msg.what == 0) {
// try upload
                uploadLogs();
            } else if (msg.what == 1) {
            } else if (msg.what == MSG_SUCCESS) {
//Toast.makeText(MainActivity.this, "Uploaded Successfully!",
//        Toast.LENGTH_LONG).show();
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        boolean result = ftpclient.ftpDisconnect();
                        Log.i(TAG, "ftp disconnect : " + result);
                    }
                }).start();

            } else if (msg.what == 3) {
//Toast.makeText(MainActivity.this, "Disconnected Successfully!",
//        Toast.LENGTH_LONG).show();
            } else if (msg.what == MSG_UPLOAD_IMAGE) {
                uploadImages(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath(), "drawing", 20, MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD);

            } else if (msg.what == MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD) {
                uploadImages(PATH_IMAGE_LOG_WRITING_BOARD, "writingboard", 0, MSG_UPLOAD_LOG_IMAGE_SEA_WORLD);

            } else if (msg.what == MSG_UPLOAD_LOG_IMAGE_SEA_WORLD) {
                uploadImages(PATH_IMAGE_LOG_SEA_WORLD, "seaworld", 100, MSG_SUCCESS);

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
        return netInfo != null && netInfo.isConnected();
    }

    private void connectToSNTPServer(final String server, final boolean updateCooldown) {
        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();

        new SntpUpdater(logger) {
            protected void onPostExecute(Boolean result) {
                if (!result) {
                    return;
                }
                if (updateCooldown) {
                    nextTimeUpdateInMillis = System.currentTimeMillis() + timeUpdateCooldownTimeInMillis;
                }
                logger.putSntpResult(server, this.now, this.snow);
            }
        }.execute(server);
    }

    private void connectToSNTPServers() {
        if (System.currentTimeMillis() < nextTimeUpdateInMillis) {
// Cooldown
            handler.sendEmptyMessage(-1);
            return;
        }

        Log.d(TAG, "-- Try to connect to SNTP server --");
        nextTimeUpdateInMillis = System.currentTimeMillis() + timeUpdateRetryTimeInMillis;

        connectToSNTPServer("time.google.com", false);
        connectToSNTPServer("192.168.0.1", true);
        connectToSNTPServer("0.us.pool.ntp.org", false);
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

        Log.d(TAG, "connecting to ftp. " + username + "@" + host + ":" + port + " password : " + password);
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
                } finally {
                    ftpConnector = null;
                }
            }
        });
        ftpConnector.start();
    }

    private void uploadLogs() {
// NB(xenosoz, 2018): Duplication alert! I saw the code below in both MainActivity and SettingActivity. [1/2]

        if (logUploader != null) return;

        logUploader = new Thread(new Runnable() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            public void run() {
                try {
                    boolean status = false;
                    String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
                    String logPath = documentsPath + "/logs";

// 2017.12.20 yongsoo : It does not matter if it fails
                    ftpclient.ftpChangeDirectory("sda1/");

                    File[] logs = new File(logPath).listFiles((FileFilter) new WildcardFileFilter(Arrays.asList("*.txt", "*.zip")));
                    for (File log : logs) {
                        Log.d("LOGTest", log.toString());
                        status = ftpclient.ftpUpload(
                                log.getPath(),
                                log.getName(), "remote/", cntx);
                        if (status == true) {
                            try {
                                status = ftpclient.mFTPClient.changeToParentDirectory();
                                if (status == false) {
                                    break;
                                }
                            } catch (Exception e) {
                                Log.e(TAG, "", e);
                                status = false;
                                break;
                            }
                        } else {
                            break;
                        }
                    }

                    if (status == true) {
                        Log.d(TAG, "Upload success");
                        lastLogSentTimeInMillis = System.currentTimeMillis();
                        handler.sendEmptyMessage(MSG_UPLOAD_IMAGE);
                    } else {
                        Log.d(TAG, "Upload failed");
                        handler.sendEmptyMessage(-1);
                    }
                } finally {
                    logUploader = null;
                }
            }
        });
        logUploader.start();
    }

    public static final String TEMP_ZIP_FOLDER_NAME = "TEMP";
    public static final String UPLOAD_TIME_RECORD_FILE = "upload_time.txt";
    public static final String PATH_IMAGE_LOG_WRITING_BOARD = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "writingboard" + File.separator + "log_image" + File.separator;
    public static final String PATH_IMAGE_LOG_SEA_WORLD = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "sea_world" + File.separator + "images" + File.separator;

    private void uploadImages(final String imagePath, final String type, final int maxUploadImageCount, final int nextMsgSuccess) {
        if (imageUploader != null) return;
        imageUploader = new Thread(new Runnable() {
            public void run() {
                try {
                    deleteImageZipFiles(imagePath);

                    boolean status = true;
                    String tempZipFolder = imagePath + File.separator + TEMP_ZIP_FOLDER_NAME;
                    File uploadTimeFile = null;

                    if (writeFile(getCurrentDisplayTime(), tempZipFolder + File.separator + UPLOAD_TIME_RECORD_FILE) == true) {
                        uploadTimeFile = new File(tempZipFolder + File.separator + UPLOAD_TIME_RECORD_FILE);
                        if (uploadTimeFile.exists() == false) {
                            uploadTimeFile = null;
                        }
                    }

                    File[] userFolders = getImageFolderList(new File(imagePath));
                    if (userFolders != null && userFolders.length > 0) {
                        for (File userFolder : userFolders) {
                            ArrayList<File> images = getImageFileList(userFolder, maxUploadImageCount);
                            if (images.size() > 0) {
                                if (uploadTimeFile != null) {
                                    images.add(uploadTimeFile);
                                }

                                String zipFileName = Build.SERIAL + "_" + userFolder.getName() + "_" + type + ".zip";
                                if (compressZip(tempZipFolder, zipFileName, images) == true) {
                                    status = uploadFtpImageFile(new File(tempZipFolder + File.separator + zipFileName));
                                    if (status == false) {
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (status == true) {
                        Log.d(TAG, "uploadImages (" + type + ") - Upload success");
                        imageUploader = null;
                        handler.sendEmptyMessage(nextMsgSuccess);
                    } else {
                        Log.d(TAG, "uploadImages (" + type + ") - Upload failed");
                        handler.sendEmptyMessage(-1);
                    }
                } finally {
                    imageUploader = null;
                    deleteImageZipFiles(imagePath);
                }
            }
        });
        imageUploader.start();
    }

    private boolean uploadFtpImageFile(File file) {
        boolean result = ftpclient.ftpUpload(
                file.getPath(),
                file.getName(), "remote/", cntx);

        if (result == true) {
            try {
                result = ftpclient.mFTPClient.changeToParentDirectory();
            } catch (Exception e) {
                Log.e(TAG, "", e);
                result = false;
            }
        }
        return result;
    }

    private void displayCurrentUser() {
        Util.displayUserName(this, mTvUserName);
    }

    public static File[] getImageFolderList(File folder) {
        File[] result = folder.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                if (pathname.isDirectory() == false) {
                    return false;
                }

                return pathname.getName().startsWith("user");
            }
        });

        return result;
    }

    public static ArrayList<File> getImageFileList(File folder) {
        return getImageFileList(folder, 0);
    }

    public static ArrayList<File> getImageFileList(File folder, int maxUploadImageCount) {
        ArrayList<File> result = new ArrayList<>();
        File[] files = folder.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                if (pathname.isDirectory() == true) {
                    return false;
                }

                return !pathname.getName().equalsIgnoreCase(".thumbnails");
            }
        });

        if (files != null && files.length > 0) {
            result = new ArrayList<>(Arrays.asList(files));
            Collections.sort(result, new Comparator<File>() {
                @RequiresApi(api = Build.VERSION_CODES.KITKAT)
                @Override
                public int compare(File o1, File o2) {
                    return Long.compare(o2.lastModified(), o1.lastModified());
                }
            });

            if (maxUploadImageCount > 0) {
                if (result.size() > maxUploadImageCount) {
                    result = new ArrayList<>(result.subList(0, maxUploadImageCount));
                }
            }

            for (File file : result) {
                Log.i(TAG, "file : " + file.getName());
            }
        }
        return result;
    }

    public static boolean compressZip(String destFolderPath, String destFileName, ArrayList<File> files) {
        long startTime = System.currentTimeMillis();
        byte[] buf = new byte[4096];

        try {
            File folder = new File(destFolderPath);
            if (folder.exists() == false) {
                folder.mkdirs();
            }

            ZipOutputStream out = new ZipOutputStream(new FileOutputStream(destFolderPath + File.separator + destFileName));

            for (int i = 0; i < files.size(); i++) {
                FileInputStream in = new FileInputStream(files.get(i));
                ZipEntry ze = new ZipEntry(files.get(i).getName());
                out.putNextEntry(ze);

                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }

                out.closeEntry();
                in.close();

            }

            out.close();

        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        Log.d(TAG, "compress time : " + (System.currentTimeMillis() - startTime) + "ms");
        return true;
    }

    public static boolean writeFile(String string, String path) {
        if (path.equals("")) {
            return false;
        }
        if (string == null) {
            return false;
        }

        File file = new File(path);
        if (file.getParent() == null) {
            return false;
        }
        File parent = new File(file.getParent());
        if (parent.exists() == false) {
            parent.mkdirs();
        }

        try {
            file.createNewFile();
            if (file.canWrite()) {
                FileOutputStream fo = new FileOutputStream(file);
                fo.write(string.getBytes(), 0, string.getBytes().length);
                fo.flush();
                fo.close();
            }
        } catch (IOException e) {
            Log.e(TAG, "", e);
            return false;
        }

        return true;
    }

    public static String getCurrentDisplayTime() {
        SimpleDateFormat dayTime = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
        String result = dayTime.format(new Date(System.currentTimeMillis()));
        return result;
    }

    public static void deleteImageZipFiles() {
        deleteImageZipFiles(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath());
    }

    public static void deleteImageZipFiles(String imagePath) {
        File[] files = new File(imagePath + File.separator + TEMP_ZIP_FOLDER_NAME).listFiles();
        if (files != null && files.length > 0) {
            for (File file : files) {
                if (file.exists() == true) {
                    file.delete();
                }
            }
        }
    }

    private boolean gotoVideoPlayer() {
//        User user = ((LauncherApplication)getApplication()).getDbHandler().getCurrentUser();
//
//        if (user.isFinishLauncherTutorial() == false) {
//            user.setFinishLauncherTutorial(true);
//            ((LauncherApplication)getApplication()).getDbHandler().updateUser(user);
//
//            Intent i = new Intent(MainActivity.this, VideoPlayerActivity.class);
//            startActivity(i);
//            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
//            return true;
//        }

        return false;
    }

    private void setDefaultPreference() {
        SharedPreferences preference = getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS).edit();
        editor.putBoolean("review_mode_on", preference.getBoolean("review_mode_on", false));
        editor.putBoolean("sign_language_mode_on", preference.getBoolean("sign_language_mode_on", false));
        editor.commit();
    }

    private Runnable mRunnableTitle = new Runnable() {
        @Override
        public void run() {
            DialogFragment dialog = new PasswordDialogFragment();
            Bundle bundle = new Bundle();
            bundle.putSerializable("userobject", "ABOUT");
            dialog.setArguments(bundle);
            dialog.show(getFragmentManager(), "PasswordDialogFragment");

// for test
//            Util.copyDBFileToSDCard(MainActivity.this);
        }
    };

    private Runnable mRunnableUserName = new Runnable() {
        @Override
        public void run() {
            DialogFragment dialog = new PasswordDialogFragment();
            Bundle bundle = new Bundle();
            bundle.putSerializable("userobject", "SELECT USER");
            dialog.setArguments(bundle);
            dialog.show(getFragmentManager(), "PasswordDialogFragment");
        }
    };

    private View.OnTouchListener mLongTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View view, MotionEvent motionEvent) {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_BUTTON_PRESS:
                case MotionEvent.ACTION_DOWN:
                    if (view == mTitle) {
                        handler.postDelayed(mRunnableTitle, 2 * 1000);

                    } else if (view == mTvUserName) {
                        handler.postDelayed(mRunnableUserName, 2 * 1000);

                    }

                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                case MotionEvent.ACTION_BUTTON_RELEASE:
                    if (view == mTitle) {
                        handler.removeCallbacks(mRunnableTitle);

                    } else if (view == mTvUserName) {
                        handler.removeCallbacks(mRunnableUserName);

                    }
                    break;

                default:
                    break;

            }
            return true;
        }
    };

    @Override
    public void onDialogPositiveClick(DialogFragment dialog, String redirectTo) {
        dialog.dismiss();
        if (redirectTo.equals("ABOUT")) {
            Intent i = new Intent(MainActivity.this, AboutActivity.class);
            startActivity(i);

        } else if (redirectTo.equals("SELECT USER")) {
            SelectNumberDialog selectNumberDialog = new SelectNumberDialog(this, SelectNumberDialog.MODE.USER_NO, new SelectNumberDialog.Callback() {
                @Override
                public void onSelectedNumber(int number) {
                    KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                    User user = dbHandler.findUser("user" + number);
                    if (user != null) {
                        dbHandler.setCurrentUser(user);
                        refreshUI();
                    }
                }
            });

            selectNumberDialog.show();
        }
    }

    @Override
    public void onDialogNegativeClick(DialogFragment dialog) {
        dialog.dismiss();
    }
}