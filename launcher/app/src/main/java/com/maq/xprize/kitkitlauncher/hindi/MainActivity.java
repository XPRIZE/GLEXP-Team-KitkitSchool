package com.maq.xprize.kitkitlauncher.hindi;

import android.app.DialogFragment;
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
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
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
import java.util.Locale;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;


public class MainActivity extends KitKitLoggerActivity implements PasswordDialogFragment.PasswordDialogListener {

    /**
     * code to post/handler request for permission
     */
    public static final int OVERLAY_PERMISSION_REQ_CODE = 4545;
    public static final String TEMP_ZIP_FOLDER_NAME = "TEMP";
    public static final String UPLOAD_TIME_RECORD_FILE = "upload_time.txt";
    public static final String PATH_IMAGE_LOG_WRITING_BOARD = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "writingboard" + File.separator + "log_image" + File.separator;
    public static final String PATH_IMAGE_LOG_SEA_WORLD = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "sea_world" + File.separator + "images" + File.separator;
    public static String TAG = "MainActivity";
    public static String PLAYSTORE_URL = "https://play.google.com/store/apps/details?id=";
    private static Thread logUploader = null;
    private static Thread imageUploader = null;
    private final int MSG_UPLOAD_IMAGE = 100;
    Rect mTempRect = new Rect();
    BroadcastReceiver receiver;
    private FtpClient ftpclient = null;
    private Context cntx = null;
    private Button mTitle;
    private TextView mTvUserName;
    private WifiReceiver _wifiInfo = new WifiReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {

        }
    };
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
    private Handler handler = new Handler() {

        public void handleMessage(android.os.Message msg) {

            int MSG_SUCCESS = 2;
            int MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD = 101;
            int MSG_UPLOAD_LOG_IMAGE_SEA_WORLD = 102;
            if (msg.what == 0) {
                // try upload
                uploadLogs();
            } else if (msg.what == MSG_SUCCESS) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        boolean result = ftpclient.ftpDisconnect();
                        Log.i(TAG, "ftp disconnect : " + result);
                    }
                }).start();
            } else if (msg.what == MSG_UPLOAD_IMAGE) {
                uploadImages(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath(), "drawing", 20, MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD);
            } else if (msg.what == MSG_UPLOAD_LOG_IMAGE_WRITING_BOARD) {
                uploadImages(PATH_IMAGE_LOG_WRITING_BOARD, "writingboard", 0, MSG_UPLOAD_LOG_IMAGE_SEA_WORLD);
            } else if (msg.what == MSG_UPLOAD_LOG_IMAGE_SEA_WORLD) {
                uploadImages(PATH_IMAGE_LOG_SEA_WORLD, "seaworld", 100, MSG_SUCCESS);
            }
        }

    };
    private Runnable mRunnableTitle = new Runnable() {
        @Override
        public void run() {
            DialogFragment dialog = new PasswordDialogFragment();
            Bundle bundle = new Bundle();
            bundle.putSerializable("userobject", "ABOUT");
            dialog.setArguments(bundle);
            dialog.show(getFragmentManager(), "PasswordDialogFragment");
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

    public static File[] getImageFolderList(File folder) {
        File[] result = folder.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                if (!pathname.isDirectory()) {
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
                if (pathname.isDirectory()) {
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
            if (!folder.exists()) {
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
        if (!parent.exists()) {
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
        return dayTime.format(new Date(System.currentTimeMillis()));
    }

    public static void deleteImageZipFiles() {
        deleteImageZipFiles(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath());
    }

    public static void deleteImageZipFiles(String imagePath) {
        File[] files = new File(imagePath + File.separator + TEMP_ZIP_FOLDER_NAME).listFiles();
        if (files != null && files.length > 0) {
            for (File file : files) {
                if (file.exists()) {
                    file.delete();
                }
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Util.hideSystemUI(this);
        cntx = getBaseContext();
        Util.setScale(this, findViewById(R.id.main_content));

        loadApps();

        final String mainAppPackageName = "com.maq.xprize.kitkitschool.hindi";
        ImageButton todoSchoolButton = findViewById(R.id.button_todoschool);

        todoSchoolButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    Intent i = new Intent(Intent.ACTION_MAIN);
                    i.setComponent(new ComponentName(mainAppPackageName, "org.cocos2dx.cpp.AppActivity"));
                    startActivity(i);
                } catch (Exception e) {
                    Intent i = new Intent(MainActivity.this, VideoPlayerActivity.class);
                    i.putExtra("video", "main_app_demo_video");
                    startActivity(i);
                    Toast.makeText(MainActivity.this, "Click the install button to download this learning module", Toast.LENGTH_LONG).show();
                    overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
                }
            }
        });

        final String libraryAppPackageName = "com.maq.xprize.kitkitlibrary.english";
        ImageButton libraryButton = findViewById(R.id.button_library);

        libraryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (view.isEnabled()) {
                    try {
                        Intent i = new Intent(Intent.ACTION_MAIN);
                        i.setComponent(new ComponentName(libraryAppPackageName, "com.maq.xprize.kitkitlibrary.english.SelectActivity"));
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
        });

        ImageButton toolsButton = findViewById(R.id.button_tool);
        toolsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (view.isEnabled()) {
                    Intent i = new Intent(MainActivity.this, ToolsActivity.class);
                    startActivity(i);
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

        TextView textViewCoinNum = findViewById(R.id.textView_numCoin);
        Typeface f = Typeface.createFromAsset(getAssets(), "TodoMainCurly.ttf");
        textViewCoinNum.setTypeface(f);

        displayCurrentUser();

        ftpclient = new FtpClient();

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(_wifiInfo, intentFilter);

        setDefaultPreference();
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
        logger.tagScreen("MainActivity");

        refreshUI();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    @Override
    protected void onDestroy() {

        unregisterReceiver(receiver);
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
        textViewCoinNum.setText(String.format(Locale.US, "%d", currentUser.getNumStars()));

        displayCurrentUser();
    }

    private void loadApps() {
        PackageManager manager = getPackageManager();
        List<AppDetail> apps = new ArrayList<AppDetail>();

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

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return false;
    }

    private void uploadLogs() {

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
                        if (status) {
                            try {
                                status = ftpclient.mFTPClient.changeToParentDirectory();
                                if (!status) {
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

                    if (status) {
                        Log.d(TAG, "Upload success");
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

    private void uploadImages(final String imagePath, final String type, final int maxUploadImageCount, final int nextMsgSuccess) {
        if (imageUploader != null) return;
        imageUploader = new Thread(new Runnable() {
            public void run() {
                try {
                    deleteImageZipFiles(imagePath);

                    boolean status = true;
                    String tempZipFolder = imagePath + File.separator + TEMP_ZIP_FOLDER_NAME;
                    File uploadTimeFile = null;

                    if (writeFile(getCurrentDisplayTime(), tempZipFolder + File.separator + UPLOAD_TIME_RECORD_FILE)) {
                        uploadTimeFile = new File(tempZipFolder + File.separator + UPLOAD_TIME_RECORD_FILE);
                        if (!uploadTimeFile.exists()) {
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
                                if (compressZip(tempZipFolder, zipFileName, images)) {
                                    status = uploadFtpImageFile(new File(tempZipFolder + File.separator + zipFileName));
                                    if (!status) {
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (status) {
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

        if (result) {
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

    private void setDefaultPreference() {
        SharedPreferences preference = getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS).edit();
        editor.putBoolean("review_mode_on", preference.getBoolean("review_mode_on", false));
        editor.putBoolean("sign_language_mode_on", preference.getBoolean("sign_language_mode_on", false));
        editor.commit();
    }

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

    public static class AppDetail {
        CharSequence label;
        CharSequence name;
        Drawable icon;
    }
}