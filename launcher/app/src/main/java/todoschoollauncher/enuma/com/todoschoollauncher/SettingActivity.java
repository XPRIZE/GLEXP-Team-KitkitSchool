package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.app.LoaderManager;
import android.content.Context;
import android.content.CursorLoader;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.Loader;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.support.v4.widget.SimpleCursorAdapter;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.KitkitProvider;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

import org.apache.commons.io.filefilter.WildcardFileFilter;

import java.io.File;
import java.io.FileFilter;
import java.util.List;
import java.util.Locale;

/**
 * Created by ingtellect on 8/7/17.
 */

public class SettingActivity extends KitKitLoggerActivity {

    private static final String TAG = "SettingActivity";
    private FtpClient ftpclient = null;
    private Context cntx = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        hideStatusbar();
        setContentView(R.layout.activity_setting);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });

        cntx = this.getBaseContext();
        ftpclient = new FtpClient();
        //listLogFiles();


        EditText ftpAddressEditText = (EditText)findViewById(R.id.ftp_address);
        EditText ftpUsernameEditText = (EditText)findViewById(R.id.ftp_username);
        EditText ftpPasswordEditText = (EditText)findViewById(R.id.ftp_password);
        EditText ftpPortEditText = (EditText)findViewById(R.id.ftp_port);

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        ftpAddressEditText.setText(prefs.getString("host",""));
        ftpUsernameEditText.setText(prefs.getString("username",""));
        ftpPasswordEditText.setText(prefs.getString("password",""));
        ftpPortEditText.setText(prefs.getString("port",""));


        SharedPreferences preferences = getSharedPreferences("sharedPref",Context.MODE_PRIVATE);


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

    public void onClickClearAppdata(View v) {
        Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.xprize");
        i.putExtra("clearAppData", true);
        startActivity(i);
    }

    public void onClickSetting(View v) {
        Intent i = getPackageManager().getLaunchIntentForPackage("com.android.settings");
        startActivity(i);
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
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
        logger.tagScreen("SettingActivity");

        connectToWifi();
        refresh();

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
                ftpclient.ftpDisconnect();
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
        new Thread(new Runnable() {
            public void run() {
                boolean status = false;
                String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
                String logPath = documentsPath + "/logs";

                File[] txts = new File(logPath).listFiles((FileFilter)new WildcardFileFilter("*.txt"));
                for (File txt : txts) {
                    Log.d("TXTTest", txt.toString());
                    status = ftpclient.ftpUpload(
                            txt.getPath(),
                            txt.getName(), "/", cntx);

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

//    private void listLogFiles() {
//        Log.d("TXTTest", "list log files");
//
//        String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
//        List<String> dirsList = new ArrayList<String>();
//        File[] dirs = new File(documentsPath).listFiles((FileFilter)new WildcardFileFilter("kitkitschool*"));
//        for (File dir : dirs) {
//            //Log.d("TXTTest", dir.toString());
//
//            File[] txts = new File(dir.toString()).listFiles((FileFilter)new WildcardFileFilter("*.txt"));
//            for (File txt : txts) {
//                Log.d("TXTTest", txt.toString());
//            }
//        }
//    }

    public void onClickPretest(View v) {
        Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.kitkittest");
        i.putExtra("test", "Pre-Test");
        startActivity(i);
    }

    public void onClickPosttest(View v) {
        Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.kitkittest");
        i.putExtra("test", "Post-Test");
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
                                    lang = "en";
                                    break;
                                case 1:
                                    lang = "sw";
                                    break;
                                default:
                                    lang = "sw";
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

    public static class SelectUserDialogFragment extends DialogFragment  implements LoaderManager.LoaderCallbacks<Cursor> {

        ArrayAdapter<String> arrayAdapter;
        SimpleCursorAdapter mAdapter;


        @Override
        public void onActivityCreated (Bundle savedInstanceState) {
            super.onActivityCreated(savedInstanceState);

            getLoaderManager().initLoader(0, null, this);


        }

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {

            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

            mAdapter = new SimpleCursorAdapter(getActivity(),
                    android.R.layout.simple_list_item_2, null,
                    new String[] { KitkitDBHandler.COLUMN_USERNAME},
                    new int[] { android.R.id.text1}, 0);
            builder.setTitle(R.string.select_user)
                    .setAdapter(mAdapter, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            Cursor cursor = (Cursor)mAdapter.getItem(which);
                            String userName = cursor.getString(cursor.getColumnIndex(KitkitDBHandler.COLUMN_USERNAME));
                            ((LauncherApplication)getActivity().getApplication()).getDbHandler().setCurrentUser(userName);
                            ((SettingActivity)getActivity()).refresh();
                        }
                    });

//            builder.setTitle(R.string.select_user)
//                    .setAdapter(arrayAdapter, new DialogInterface.OnClickListener() {
//                        @Override
//                        public void onClick(DialogInterface dialog, int which) {
//                            String userName = arrayAdapter.getItem(which);
//                            ((LauncherApplication)getActivity().getApplication()).getDbHandler().setCurrentUser(userName);
//                        }
//                    });
            return builder.create();
        }

        @Override
        public Loader<Cursor> onCreateLoader(int id, Bundle args) {
            return new CursorLoader(getActivity(),
                    KitkitProvider.CONTENT_URI
                    , new String[]{KitkitDBHandler.COLUMN_ID, KitkitDBHandler.COLUMN_USERNAME}, null, null, null);

        }

        @Override
        public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
/*            arrayAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.select_dialog_singlechoice);
            data.moveToFirst();
            while (!data.isAfterLast()) {
                arrayAdapter.add(data.getString(0));
                data.moveToNext();
            }*/
            mAdapter.swapCursor(data);
        }

        @Override
        public void onLoaderReset(Loader<Cursor> loader) {
            mAdapter.swapCursor(null);
        }
    }


    public void onClickChangeLanguage(View v) {

        DialogFragment dialog = new LanguageDialogFragment();
        dialog.show(getFragmentManager() ,"LanguageDialogFragment");
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

        DialogFragment dialog = new SelectUserDialogFragment();
        dialog.show(getFragmentManager() ,"SelectUserDialogFragment");
    }

    public void refresh() {
        User user = ((LauncherApplication)getApplication()).getDbHandler().getCurrentUser();
        String currentUsername = user.getUserName();
        TextView textViewUsername = (TextView)findViewById(R.id.textView_currentUsername);
        textViewUsername.setText(currentUsername);

        TextView textViewNumcoin = (TextView)findViewById(R.id.textView_numCoinSetting);
        textViewNumcoin.setText(String.format("%d",user.getNumStars()));
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
        refresh();
    }

    public void onClickAddThousandStars(View v) {
        KitkitDBHandler dbHandler = ((LauncherApplication)getApplication()).getDbHandler();
        User user = dbHandler.getCurrentUser();
        user.setNumStars(user.getNumStars()+1000);
        dbHandler.updateUser(user);
        refresh();
    }

//    public void changeLanguageOthers(String lang) {
//        Log.d(TAG, "change language all");
//        String apps[] = {"com.enuma.xprize", "library.todoschool.enuma.com.todoschoollibrary", "com.enuma.todoschoollockscreen" };
//        for(String app : apps) {
//            Intent i = getPackageManager()
//                    .getLaunchIntentForPackage( app );
//            i.putExtra("changeAppLanguage", lang);
//            startActivity(i);
//        }
//
//    }
}
