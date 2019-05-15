package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Rect;
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
import android.text.SpannableString;
import android.text.style.UnderlineSpan;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

import org.apache.commons.io.filefilter.WildcardFileFilter;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by ingtellect on 8/7/17.
 */

public class SettingActivity extends KitKitLoggerActivity {
    private static final String TAG = "SettingActivity";
    private View mVBack;
    private FtpClient ftpclient = null;
    private Context cntx = null;
    private TextView mTvTabletNumber;
    private Switch mLibrarySwitch;
    private Switch mToolsSwitch;
    private boolean mbGotoHome;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);
        setContentView(R.layout.activity_setting);
        Util.setScale(this, findViewById(R.id.root_container));

        mVBack = findViewById(R.id.v_back);
        mVBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        cntx = this.getBaseContext();
        ftpclient = new FtpClient();

        EditText ftpAddressEditText = (EditText)findViewById(R.id.ftp_address);
        EditText ftpUsernameEditText = (EditText)findViewById(R.id.ftp_username);
        EditText ftpPasswordEditText = (EditText)findViewById(R.id.ftp_password);
        EditText ftpPortEditText = (EditText)findViewById(R.id.ftp_port);

        final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        ftpAddressEditText.setText(prefs.getString("host",""));
        ftpUsernameEditText.setText(prefs.getString("username",""));
        ftpPasswordEditText.setText(prefs.getString("password",""));
        ftpPortEditText.setText(prefs.getString("port",""));

        mLibrarySwitch = (Switch)findViewById(R.id.librarySwitch);
        mToolsSwitch = (Switch)findViewById(R.id.toolsSwitch);

        mTvTabletNumber = (TextView)findViewById(R.id.tv_tablet_number);

        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.KITKAT) {
            mLibrarySwitch.setSwitchTextAppearance(this, R.style.SwitchTextAppearance);
            mToolsSwitch.setSwitchTextAppearance(this, R.style.SwitchTextAppearance);
        }

        mbGotoHome = false;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            Util.hideSystemUI(this);
        }
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


    public void onClickUpload(View v) {
        //throw new RuntimeException("This is a crash");
        if (isOnline(cntx)) {
            connectToFTPAddress();
        }

    }

    public void onClickLibrary(View v) {
        v.findViewById(R.id.librarySwitch).performClick();
    }

    public void onClickTools(View v) {
        v.findViewById(R.id.toolsSwitch).performClick();
    }

    public void onClickReviewMode(View v) {
        v.findViewById(R.id.reviewModeSwitch).performClick();
    }

    public void onClickClearAppdata(View v) {
        try {
            Intent i = new Intent(Intent.ACTION_MAIN);
            i.setComponent(new ComponentName("com.enuma.xprize", "org.cocos2dx.cpp.AppActivity"));
            i.putExtra("clearAppData", true);
            startActivity(i);

        } catch (Exception e) {
            Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
        }
    }

    public void onClickSetting(View v) {
        Intent i = getPackageManager().getLaunchIntentForPackage("com.android.settings");
        startActivity(i);
    }

    public void onClickPretest(View v) {
        processSelectUser("Pre-Test");
    }

    public void onClickPosttest(View v) {
        processSelectUser("Post-Test");
    }

    private void gotoTest(String mode) {
        try {
            Intent intent = new Intent(Intent.ACTION_MAIN);
            intent.setComponent(new ComponentName("com.enuma.kitkittest", "org.cocos2dx.cpp.AppActivity"));
            intent.putExtra("test", mode);
            startActivity(intent);
            mbGotoHome = true;
        } catch (Exception e) {
            Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
            mbGotoHome = false;
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
        logger.tagScreen("SettingActivity");

        connectToWifi();
        refreshUI();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }

        if (mbGotoHome) {
            Intent intent = new Intent(this, MainActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            startActivity(intent);
        }
    }

    private Handler handler = new Handler() {

        public void handleMessage(android.os.Message msg) {

            if (msg.what == 0) {
                // try upload
                uploadLogs();
            } else if (msg.what == 1) {
            } else if (msg.what == 2) {
                Toast.makeText(SettingActivity.this, "Uploaded Successfully!",
                        Toast.LENGTH_LONG).show();

                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        boolean result = ftpclient.ftpDisconnect();
                        Log.i(TAG, "ftp disconnect : " + result);
                    }
                }).start();

            } else if (msg.what == 3) {
                Toast.makeText(SettingActivity.this, "Disconnected Successfully!",
                        Toast.LENGTH_LONG).show();

            } else {
                Toast.makeText(SettingActivity.this, "Unable to Perform Action! : " + msg.what,
                        Toast.LENGTH_LONG).show();
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

        EditText ftpAddressEditText = (EditText)findViewById(R.id.ftp_address);
        EditText ftpUsernameEditText = (EditText)findViewById(R.id.ftp_username);
        EditText ftpPasswordEditText = (EditText)findViewById(R.id.ftp_password);
        EditText ftpPortEditText = (EditText)findViewById(R.id.ftp_port);

        final String host = ftpAddressEditText.getText().toString().isEmpty() ?  "192.168.0.1".trim() : ftpAddressEditText.getText().toString().trim();
        final String username = ftpUsernameEditText.getText().toString().isEmpty() ?  "anonymous" : ftpUsernameEditText.getText().toString().trim();
        final String password = ftpPasswordEditText.getText().toString().isEmpty() ?  "" : ftpPasswordEditText.getText().toString().trim();
        final int port = ftpPortEditText.getText().toString().isEmpty() ? 21 : Integer.parseInt(ftpPortEditText.getText().toString());

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString("host", host);
        editor.putString("username", username);
        editor.putString("password", password);
        editor.putString("port", String.valueOf(port));
        editor.commit();

        new Thread(new Runnable() {
            public void run() {
                boolean status = false;
                status = ftpclient.ftpConnect(host, username, password, port);
                if (status == true) {
                    Log.d(TAG, "Connection Success");
                    handler.sendEmptyMessage(0);
                } else {
                    Log.d(TAG, "Connection failed");
                    handler.sendEmptyMessage(-1);
                }
            }
        }).start();
    }

    private void uploadLogs() {
        // NB(xenosoz, 2018): Duplication alert! I saw the code below in both MainActivity and SettingActivity. [2/2]

        new Thread(new Runnable() {
            public void run() {
                boolean status = false;
                String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
                String logPath = documentsPath + "/logs";

                // 2017.12.20 yongsoo : It does not matter if it fails
                ftpclient.ftpChangeDirectory("sda1/");

                File[] txts = new File(logPath).listFiles((FileFilter)new WildcardFileFilter(Arrays.asList("*.txt", "*.zip")));
                for (File txt : txts) {
                    Log.d("TXTTest", txt.toString());
                    status = ftpclient.ftpUpload(
                            txt.getPath(),
                            txt.getName(), "remote/", cntx);

                    if (status == true) {
                        try {
                            ftpclient.mFTPClient.changeToParentDirectory();
                        } catch (Exception e) {
                            Log.e(TAG, "", e);
                        }
                    }
                }

                if (status == true) {
                    status = uploadImages();
                }

                if (status == true) {
                    Log.d(TAG, "Upload success");
                    handler.sendEmptyMessage(2);
                } else {
                    Log.d(TAG, "Upload failed");
                    handler.sendEmptyMessage(-1);
                }
            }
        }).start();

    }

    private boolean uploadImages() {
        boolean status = true;
        try {
            MainActivity.deleteImageZipFiles();

            File dcimFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);
            String tempZipFolder = dcimFolder.getAbsolutePath() + File.separator + MainActivity.TEMP_ZIP_FOLDER_NAME;
            File uploadTimeFile = null;

            if (MainActivity.writeFile(MainActivity.getCurrentDisplayTime(), tempZipFolder + File.separator + MainActivity.UPLOAD_TIME_RECORD_FILE) == true) {
                uploadTimeFile = new File(tempZipFolder + File.separator + MainActivity.UPLOAD_TIME_RECORD_FILE);
                if (uploadTimeFile.exists() == false) {
                    uploadTimeFile = null;
                }
            }

            ArrayList<File> commonImages = MainActivity.getImageFileList(dcimFolder);
            if (commonImages.size() > 0) {
                if (uploadTimeFile != null) {
                    commonImages.add(uploadTimeFile);
                }

                String zipFileName = Build.SERIAL + "_" + "user_common" + ".zip";
                if (MainActivity.compressZip(tempZipFolder, zipFileName, commonImages) == true) {
                    status = uploadFtpImageFile(new File(tempZipFolder + File.separator + zipFileName));
                }
            }

            File[] userFolders = MainActivity.getImageFolderList(dcimFolder);
            if (userFolders != null && userFolders.length > 0) {
                for (File userFolder : userFolders) {
                    ArrayList<File> images = MainActivity.getImageFileList(userFolder);
                    if (images.size() > 0) {
                        if (uploadTimeFile != null) {
                            images.add(uploadTimeFile);
                        }

                        String zipFileName = Build.SERIAL + "_" + userFolder.getName() + ".zip";
                        if (MainActivity.compressZip(tempZipFolder, zipFileName, images) == true) {
                            status = uploadFtpImageFile(new File(tempZipFolder + File.separator + zipFileName));
                        }
                    }
                }
            }

        } finally {
            MainActivity.deleteImageZipFiles();
        }

        return status;
    }

    private boolean uploadFtpImageFile(File file) {
        boolean result = ftpclient.ftpUpload(
                file.getPath(),
                file.getName(), "remote/", cntx);

        if (result == true) {
            try {
                ftpclient.mFTPClient.changeToParentDirectory();
            } catch (Exception e) {
                Log.e(TAG, "", e);
            }
        }
        return result;
    }

    public void onClickTabletNumber(View v) {
        final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        SelectNumberDialog dialog = new SelectNumberDialog(this, SelectNumberDialog.MODE.TABLET_NUM, new SelectNumberDialog.Callback() {
            @Override
            public void onSelectedNumber(int number) {
                SharedPreferences.Editor editor = prefs.edit();
                editor.putString("TABLET_NUMBER", String.valueOf(number));
                editor.commit();
                refreshUI();
            }
        });
        dialog.show();
    }

    public void onClickUserName(View v) {
        Intent i = new Intent(this, UserNameActivity.class);
        startActivity(i);
    }

    public static class LanguageDialogFragment extends DialogFragment {
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle(R.string.select_language)
                    .setItems(R.array.languages, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            String lang = "";
                            switch (which) {
                                case 0:
                                    lang = "en-US";
                                    break;
                                case 1:
                                    lang = "en-KE";
                                    break;
                                case 2:
                                    lang = "sw-TZ";
                                    break;
                                default:
                                    lang = "sw-TZ";
                                    break;
                            }

//                            ((SettingActivity)getActivity()).changeLanguageOthers(lang);
                            ((SettingActivity)getActivity()).changeLanguage(lang);
                            Intent i = getActivity().getBaseContext().getPackageManager()
                                    .getLaunchIntentForPackage( getActivity().getBaseContext().getPackageName() );
                            i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            startActivity(i);


                        }
                    });
            return builder.create();
        }
    }


    public void onClickChangeLanguage(View v) {
//        DialogFragment dialog = new LanguageDialogFragment();
//        dialog.show(getFragmentManager() ,"LanguageDialogFragment");
    }

    public void changeLanguage(String lang) {
        SharedPreferences preferences = getSharedPreferences("sharedPref",Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("appLanguage",lang);
        editor.apply();
        Log.d(TAG, "change language to " + lang);
        finish();
        //android.os.Process.killProcess(android.os.Process.myPid());
        //System.exit(1);
    }

    public void onClickSelectUser(View v) {
        processSelectUser(null);
    }

    private void processSelectUser(final String testMode) {
        SelectNumberDialog selectNumberDialog = new SelectNumberDialog(this, SelectNumberDialog.MODE.USER_NO, new SelectNumberDialog.Callback() {
            @Override
            public void onSelectedNumber(int number) {
                KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                User user = dbHandler.findUser("user" + number);
                if (user != null) {
                    dbHandler.setCurrentUser(user);
                    refreshUI();

                    if (testMode != null) {
                        gotoTest(testMode);
                    }
                }
            }
        });

        selectNumberDialog.show();
    }

    public void refreshUI() {
        Util.displayUserName(this, (TextView)findViewById(R.id.textView_currentUsername));

        final User user = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();
        TextView textViewNumcoin = (TextView)findViewById(R.id.textView_numCoin);
        textViewNumcoin.setText(String.format("%d",user.getNumStars()));

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        String tabletNumber = prefs.getString("TABLET_NUMBER", "");

        if (!tabletNumber.isEmpty()) {
            SpannableString content = new SpannableString(tabletNumber);
            content.setSpan(new UnderlineSpan(), 0, content.length(), 0);
            mTvTabletNumber.setText(content);

        } else {
            mTvTabletNumber.setText(tabletNumber);

        }

        mLibrarySwitch.setChecked(user.isOpenLibrary());
        mLibrarySwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                user.setOpenLibrary(isChecked);
                ((LauncherApplication) getApplication()).getDbHandler().updateUser(user);
            }
        });

        mToolsSwitch.setChecked(user.isOpenTools());
        mToolsSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                user.setOpenTools(isChecked);
                ((LauncherApplication) getApplication()).getDbHandler().updateUser(user);
            }
        });
    }

    public void onClickUnlockAll(View v) {
        KitkitDBHandler dbHandler = ((LauncherApplication)getApplication()).getDbHandler();
        User user = dbHandler.getCurrentUser();
        user.setFinishTutorial(true);
//        user.setUnlockDrum(true);
//        user.setUnlockMarimba(true);
//        user.setUnlockDrawing(true);
//        user.setUnlockColoring(true);

        dbHandler.updateUser(user);

    }

    public void onClickAddHundredStars(View v) {
        KitkitDBHandler dbHandler = ((LauncherApplication)getApplication()).getDbHandler();
        User user = dbHandler.getCurrentUser();
        user.setNumStars(user.getNumStars()+100);
        dbHandler.updateUser(user);
        refreshUI();
    }

    public void onClickAddThousandStars(View v) {
        KitkitDBHandler dbHandler = ((LauncherApplication)getApplication()).getDbHandler();
        User user = dbHandler.getCurrentUser();
        user.setNumStars(user.getNumStars()+1000);
        dbHandler.updateUser(user);
        refreshUI();
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
