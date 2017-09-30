package com.enuma.kitkitlogger;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.preference.Preference;
import android.provider.Settings;
import android.util.Log;

import com.enuma.kitkitProvider.KitkitDBHandler;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Calendar;

/**
 * Created by ingtellect on 7/14/17.
 */

public class KitKitLogger {
    private static final String TAG = "KitkitLogger";
    private File path;
    private String appName;
    private Context _ctnx;
    //private String installId;
    //private String androidId;
    private String serialNumber;

    private KitkitDBHandler dbHandler;

    public KitKitLogger(String appName, Context ctnx) {
        this.appName = appName;

//        try {
//            Context launcherContext = ctnx.createPackageContext("todoschoollauncher.enuma.com.todoschoollauncher", Context.MODE_PRIVATE);
//            SharedPreferences prefs = launcherContext.getSharedPreferences("sharedpreference",Context.MODE_PRIVATE);
//            installId = prefs.getString("installId","");
//        } catch (PackageManager.NameNotFoundException e) {
//            e.printStackTrace();
//        }

        //path = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/" + installId);
        path = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/" +"logs");
        if(!path.exists()){
            path.mkdirs();
        }
        _ctnx = ctnx;
        //androidId = Settings.Secure.getString(_ctnx.getContentResolver(), Settings.Secure.ANDROID_ID);

        serialNumber = Build.SERIAL;

        //Log.d(TAG,appName+ " installId : " + installId);
        dbHandler = new KitkitDBHandler(_ctnx);
    }

    public void tagScreen(String screenName) {
        try {
            JSONObject eventValue = new JSONObject();
            eventValue.put("category", screenName);
//            eventValue.put("timestamp", (double)(Calendar.getInstance().getTimeInMillis() / 1000));
            eventValue.put("action","tagScreen");
            logEvent(eventValue.toString());
        }
        catch (JSONException ex) {
            ex.printStackTrace();
            Log.e(TAG,ex.getMessage());
        }
    }

    public void logEvent(String category, String action, String label, double value) {
        try {
            JSONObject eventValue = new JSONObject();
//            eventValue.put("timestamp", (double)(Calendar.getInstance().getTimeInMillis() / 1000));
            eventValue.put("category",category);
            eventValue.put("action",action);
            eventValue.put("label",label);
            eventValue.put("value",value);
            logEvent(eventValue.toString());
        }
        catch (JSONException ex) {
            ex.printStackTrace();
            Log.e(TAG,ex.getMessage());
        }
    }

    public void logEvent(String eventString) {
        boolean isNew = false;

        File[] files = path.listFiles();

        String header = serialNumber + "_log_";
        String tail = ".txt";
        String lastFilename = null;
        File lastFile = null;
        int lastNum = 1;

        if (files != null) {
            for (int i=0; i<files.length; i++){
                String filename = files[i].getName();

                if(lastFilename == null){
                    lastFilename = filename;
                    lastFile = files[i];
                }

                if(filename.contains(header)){
                    int num = Integer.parseInt(filename.split("_")[2].split("\\.")[0]);
                    if(num > lastNum){
                        lastNum = num;
                        lastFilename = filename;
                        lastFile = files[i];
                    }
                }
            }
        }

        if(lastFilename == null){
            lastFilename = path + "/" + header + "1" + tail;
            lastFile = new File(lastFilename);
            if(!lastFile.exists()){
                try{ lastFile.createNewFile(); }catch(Exception e){}
                isNew = true;
            }
        }

        long size = lastFile.length();

        if(size > 100*1024){
            lastNum++;
            lastFilename = path + "/" + header + Integer.toString(lastNum) + tail;
            lastFile = new File(lastFilename);

            if(!lastFile.exists()){
                try{ lastFile.createNewFile(); }catch(Exception e){}
                isNew = true;
            }
        }

        try{
            JSONObject logJson = new JSONObject();
            logJson.put("appName",appName);
            logJson.put("timeStamp", (double)(Calendar.getInstance().getTimeInMillis() / 1000));
            JSONObject eventJson = new JSONObject(eventString);
            logJson.put("event",eventJson);
            logJson.put("user",dbHandler.getCurrentUsername());

            FileWriter fw = new FileWriter(lastFile.getAbsoluteFile(), true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write((isNew ? "" : "\r\n") + logJson.toString());
            bw.close();
        }catch(Exception e){
            Log.e(TAG,e.getMessage());
        }

    }

    public String extractLogToFile() {
        Log.d(TAG, "kitkit logger caught unhandled exception");
        PackageManager manager = _ctnx.getPackageManager();
        PackageInfo info = null;
        try {
            info = manager.getPackageInfo (_ctnx.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e2) {
            Log.e(TAG,e2.getMessage());
        }
        String model = Build.MODEL;
        if (!model.startsWith(Build.MANUFACTURER))
            model = Build.MANUFACTURER + " " + model;

        // Make file name - file must be saved to external storage or it wont be readable by
        // the email app.
        String documentsPath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();

        String fullName = documentsPath + "/crashlog_"+appName+".txt";

        // Extract to file.
        File file = new File (fullName);
        if (!file.exists()) {
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG,e.getMessage());
            }
        }
        InputStreamReader reader = null;
        FileWriter writer = null;
        try
        {
            // For Android 4.0 and earlier, you will get all app's log output, so filter it to
            // mostly limit it to your app's output.  In later versions, the filtering isn't needed.
            String cmd = (Build.VERSION.SDK_INT <= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1) ?
                    "logcat -d -v time "+appName+":v dalvikvm:v System.err:v *:s" :
                    "logcat -d -v time";

            // get input stream
            Process process = Runtime.getRuntime().exec(cmd);
            reader = new InputStreamReader(process.getInputStream());

            // write output stream
            writer = new FileWriter (file);
            writer.write ("Android version: " +  Build.VERSION.SDK_INT + "\n");
            writer.write ("Device: " + model + "\n");
            writer.write ("App name: " + appName);
            writer.write ("App version: " + (info == null ? "(null)" : info.versionCode) + "\n");

            char[] buffer = new char[10000];
            do
            {
                int n = reader.read (buffer, 0, buffer.length);
                if (n == -1)
                    break;
                writer.write (buffer, 0, n);
            } while (true);

            reader.close();
            writer.close();
        }
        catch (IOException e)
        {
            if (writer != null)
                try {
                    writer.close();
                } catch (IOException e1) {
                    Log.e(TAG, e1.getMessage());
                }
            if (reader != null)
                try {
                    reader.close();
                } catch (IOException e1) {
                    Log.e(TAG, e1.getMessage());
                }

            // You might want to write a failure message to the log here.
            Log.e(TAG, "Kitkit logger failed to write crash log + \n"+e.getMessage());
            return null;
        }

        return fullName;
    }
}
