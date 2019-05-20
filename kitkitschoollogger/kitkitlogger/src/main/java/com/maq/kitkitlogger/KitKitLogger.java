package com.maq.kitkitlogger;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.util.Log;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.SntpResult;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.NetworkInterface;
import java.util.Calendar;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/**
 * Created by ingtellect on 7/14/17.
 */

public class KitKitLogger {
    private static final String TAG = "KitkitLogger";
    private File basePath;
    private String appName;
    private String shortAppName;
    private Context _ctnx;
    //private String installId;
    //private String androidId;
    private String serialNumber;
    private String macAddress;


    // NB(xenosoz, 2018): SNTP cache.
    Map<String, SntpResult> _sntpCache = new HashMap<>();

    private KitkitDBHandler dbHandler;

    public static String getMacAddr() {
        try {
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface nif : all) {
                if (!nif.getName().equalsIgnoreCase("wlan0")) continue;

                byte[] macBytes = nif.getHardwareAddress();
                if (macBytes == null) {
                    return "";
                }

                StringBuilder res1 = new StringBuilder();
                for (byte b : macBytes) {
                    // res1.append(Integer.toHexString(b & 0xFF) + ":");
                    res1.append(String.format("%02X:",b));
                }

                if (res1.length() > 0) {
                    res1.deleteCharAt(res1.length() - 1);
                }
                return res1.toString();
            }
        } catch (Exception ex) {
            //handle exception
        }
        return "";
    }

    public KitKitLogger(String appName, Context ctnx) {
        this.appName = appName;
        int pos;
        if ( (pos = appName.lastIndexOf('.')) != -1) {
            this.shortAppName = appName.substring(pos+1);
        }
        else
            this.shortAppName = appName;


//        try {
//            Context launcherContext = ctnx.createPackageContext("com.maq.xprize.kitkitlauncher.hindi", Context.MODE_PRIVATE);
//            SharedPreferences prefs = launcherContext.getSharedPreferences("sharedpreference",Context.MODE_PRIVATE);
//            installId = prefs.getString("installId","");
//        } catch (PackageManager.NameNotFoundException e) {
//            e.printStackTrace();
//        }

        //basePath = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/" + installId);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            basePath = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/" +"logs");
        } else {
            basePath = new File(Environment.getExternalStorageDirectory() + "/Documents" + "/" +"logs");
        }

        if(!basePath.exists()){
            basePath.mkdirs();
        }
        _ctnx = ctnx;
        //androidId = Settings.Secure.getString(_ctnx.getContentResolver(), Settings.Secure.ANDROID_ID);

        serialNumber = Build.SERIAL;

        // add mac address to serial number, for devices with the same serial numbers (e.g., MPGIO, Joy, etc...)
        macAddress = getMacAddr().replace(":", "");

        serialNumber = serialNumber + "_" + macAddress;



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

    public static String intToLei(int n) {
        // NB(xenosoz, 2018): Length encoded integer
        if (n == 0) { return "A"; }
        return (n < 0 ? "-" : "") + (char) ('B' + (int) Math.log10(Math.abs(n))) + n;
    }

    public static int leiToInt(String lei) {
        // NB(xenosoz, 2018): Length encoded integer
        int mul = 1;
        int p = 0;

        if (lei.startsWith("-")) { mul = -Math.abs(mul); p = 1; }
        if ("A".equals(lei.substring(p))) { return mul * 0; }

        p += 1;
        return Integer.valueOf(lei.substring(p)).intValue();
    }

    public String normAppName() {
        // NB(xenosoz, 2018): Prepare to split fields with a dot.
        return appName.replace('.', '_');
    }

    public String lastLogPathName() {
        // NB(xenosoz, 2018): com_maq_xprize.HGAF1GZW.lastlog.txt
        return normAppName() + "." + serialNumber + "." + "lastlog" + ".txt";
    }

    private byte[] readAllBytes(File f) {
        byte[] b = new byte[(int)f.length()];
        try {
            FileInputStream fis = new FileInputStream(f);
            fis.read(b);
        }
        catch (Exception e) {
            Log.e(TAG,e.getMessage());
        }
        return b;
    }

    public void putSntpResult(String serverSpec, long now, String snow) {
        SntpResult sr = new SntpResult(serverSpec, now, snow);
        dbHandler.uniqueInsertSntpResult(sr);

        // NB(xenosoz, 2018): Heat cache to prevent passive time event.
        _sntpCache.put(sr.serverSpec, sr);

        try {
            JSONObject eventJson = new JSONObject();
            eventJson.put("0.name", "activeSntpUpdate");
            eventJson.put("1.serverSpec", sr.serverSpec);
            eventJson.put("2.now", sr.now);
            eventJson.put("3.snow", sr.snow);

            logEvent(eventJson.toString());
        }
        catch (JSONException e) {
            Log.e(TAG, e.getMessage());
        }

        logEvent("putSntpResult");
    }

    public int logSntpUpdateTo(BufferedWriter bw) {
        int resultId = -1;
        if (bw == null) { return resultId; }

        for (SntpResult sr : dbHandler.getSntpResults()) {
            if (resultId < sr.id) {
                resultId = sr.id;
            }

            SntpResult cache = _sntpCache.get(sr.serverSpec);

            if (cache != null && !cache.serverSpec.equals(sr.serverSpec)) { cache = null; }
            if (cache != null && cache.now != sr.now) { cache = null; }
            if (cache != null && !cache.snow.equals(sr.snow)) { cache = null; }
            if (cache != null) {
                // NB(xenosoz, 2018): Cache value exists, no update. -> quit.
                continue;
            }

            JSONObject logJson = new JSONObject();
            try {
                JSONObject eventJson = new JSONObject();
                eventJson.put("0.name", "passiveSntpUpdate");
                eventJson.put("1.serverSpec", sr.serverSpec);
                eventJson.put("2.now", sr.now);
                eventJson.put("3.snow", sr.snow);
                //eventJson.put("4.id", sr.id);

                logJson.put("appName", appName);
                logJson.put("timeStamp", (double) (Calendar.getInstance().getTimeInMillis() / 1000));
                logJson.put("event", eventJson);
                logJson.put("id", sr.id);
                //logJson.put("user", dbHandler.getCurrentUsername());

                bw.write(logJson.toString() + "\n");
            }
            catch (JSONException e) {
                Log.e(TAG, e.getMessage());
            }
            catch (IOException e) {
                Log.e(TAG, e.getMessage());
            }

            // NB(xenosoz, 2018): Heat the cache.
            _sntpCache.put(sr.serverSpec, sr);
        }

        return resultId;
    }

    public void logEvent(String eventString) {

        // NB(xenosoz, 2018): Try to make the last log file if not exist.
        File logPath = new File(basePath + "/" + lastLogPathName());
        if (!logPath.exists()) {
            try {
                logPath.createNewFile();
            }
            catch (Exception e) {
            }
        }

        // NB(xenosoz, 2018): Write log contents to the last log file.
        try {
            FileWriter fw = new FileWriter(logPath.getAbsoluteFile(), true);
            BufferedWriter bw = new BufferedWriter(fw);

            int id = logSntpUpdateTo(bw);

            JSONObject eventJson = new JSONObject(eventString);
            JSONObject logJson = new JSONObject();

            logJson.put("appName", shortAppName);
            logJson.put("timeStamp", (double)(Calendar.getInstance().getTimeInMillis() / 1000));
            logJson.put("event", eventJson);
            logJson.put("sntp", id);

            String user = dbHandler.getCurrentUsername();
            String tabletNumber = dbHandler.getTabletNumber();
            if (!tabletNumber.isEmpty()) {
                user = "t" + tabletNumber + "-" + user;
            }
            logJson.put("user", user);
            bw.write(logJson.toString() + "\n");
            bw.close();
        }
        catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }

        // NB(xenosoz, 2018): If the log size is small enough (under 400kb), we're done here.
        long logSize = logPath.length();
        if (logSize < 400 * 1024) {
            return;
        }

        // NB(xenosoz, 2018): Find candidate name for making zip archive.
        File[] files = basePath.listFiles();

        String header = normAppName() + "." + serialNumber + ".";
        String zipFooter = ".log.zip";
        String txtFooter = ".log.txt";
        int lastNum = -1;

        if (files != null) {
            for (int i = 0; i < files.length; i++) {
                String filename = files[i].getName();
                if (!filename.startsWith(header)) { continue; }
                if (!filename.endsWith(zipFooter)) { continue; }

                int num = leiToInt(filename.split("\\.")[2]);
                if (num > lastNum) {
                    lastNum = num;
                }
            }
        }

        // NB(xenosoz, 2018): Create archive file.
        try {
            int zipNum = lastNum + 1;
            File zipPath = new File(basePath + "/" + (header + intToLei(zipNum) + zipFooter));
            ZipOutputStream zipOS = new ZipOutputStream(new FileOutputStream(zipPath));
            {
                ZipEntry e = new ZipEntry(header + intToLei(zipNum) + txtFooter);
                zipOS.putNextEntry(e);
                byte[] bytes = readAllBytes(logPath);
                zipOS.write(bytes, 0, bytes.length);
                zipOS.closeEntry();
            }
            zipOS.close();
        }
        catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }

        // NB(xenosoz, 2018): Remove the log file.
        logPath.delete();
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

        String fullName = documentsPath + "/crashlog." + normAppName() + ".txt";

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
            writer = new FileWriter(file);
            writer.write("Android version: " +  Build.VERSION.SDK_INT + "\n");
            writer.write("Device: " + model + "\n");
            writer.write("App name: " + appName);
            writer.write("App version: " + (info == null ? "(null)" : info.versionCode) + "\n");

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
