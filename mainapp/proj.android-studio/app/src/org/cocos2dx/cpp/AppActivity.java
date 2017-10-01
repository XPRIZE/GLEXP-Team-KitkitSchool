/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.util.Locale;
import java.util.UUID;
import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.lang.Exception;

import android.Manifest;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Process;
import android.preference.Preference;
import android.preference.PreferenceManager;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.os.Build.*;
import android.app.ActivityManager;
import android.content.Intent;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitkitContextWrapper;

import org.cocos2dx.lib.*;
import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    public static AppActivity _activity;
    public static String _launchString;
    private Cocos2dxGLSurfaceView glSurfaceView;

    private static String TAG = "KitkitschoolActivity";

    protected String appLanguage;

    protected static String currentUsername;
    protected static User currentUser;

    public  boolean isStoragePermissionGranted() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG,"Permission is granted");
                return true;
            } else {

                Log.v(TAG,"Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
                return false;
            }
        }
        else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG,"Permission is granted");
            return true;
        }
    }
    public static String getLaunchString() {
        return _launchString;
    }

    public static void clearLaunchString() {
        _launchString = "";
    }

    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.d(TAG,"onCreate");
        isStoragePermissionGranted();
        _activity = this;

        Bundle extras = getIntent().getExtras();
        if(extras != null) {
            if(extras.getBoolean("clearAppData", false)) {
                clearAppData();
            }
            if (getIntent().hasExtra("test")) {
                _launchString = extras.getString("test");
                Log.d(TAG,"onCreate launch string " + _launchString);

            }
        }

        try {
            Context launcherContext = createPackageContext("todoschoollauncher.enuma.com.todoschoollauncher",0);
            SharedPreferences pref = launcherContext.getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
            appLanguage = pref.getString("appLanguage", getString(com.enuma.kitkitlogger.R.string.defaultLanguage));
            Cocos2dxHelper.setStringForKey("appLanguage", appLanguage);
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        try {
            currentUser = ((KitkitSchoolApplication)getApplication()).getDbHandler().getCurrentUser();
            currentUsername = currentUser.getUserName();
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }


    }

    public Cocos2dxGLSurfaceView onCreateView()
    {
        glSurfaceView = new Cocos2dxGLSurfaceView(this);

        this.hideSystemUI();

        // create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    public static void sendToBack() {
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                _activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        _activity.moveTaskToBack(true);
                    }
                });
            }
        });
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG,"onResume");

        Bundle extras = getIntent().getExtras();
        if(extras != null) {
            Log.d(TAG,"onResume extra is not null");
            if(extras.getBoolean("clearAppData", false)) {
                Log.d(TAG,"onResume clearAppData is true");
                clearAppData();
            }
            if (getIntent().hasExtra("test")) {
                _launchString = extras.getString("test");
                Log.d(TAG,"onResume launch string " + _launchString);
            }
        }

        try {
            Context context = createPackageContext("todoschoollauncher.enuma.com.todoschoollauncher",0);
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            String sharedLang = pref.getString("appLanguage", getString(com.enuma.kitkitlogger.R.string.defaultLanguage));
            if (!appLanguage.equals(sharedLang)) {
                Cocos2dxHelper.setStringForKey("appLanguage", appLanguage);

                restartApp();
            }
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        try {
            currentUser = ((KitkitSchoolApplication)getApplication()).getDbHandler().getCurrentUser();

            if (!currentUsername.equals(currentUser.getUserName())) {
                restartApp();
            }
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }

    }



    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
        {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI()
    {
        // Set the IMMERSIVE flag.
        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
        glSurfaceView.setSystemUiVisibility(
                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    public void hideSystemUIOnUIThread()
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                hideSystemUI();
            }
        });
    }


    public static void moveToBackground() {
        _activity.moveTaskToBack(true);
        //moveTaskToBack(true);
    }

    static
    {
        System.loadLibrary("cocos2dcpp");
    }

    public static void staticSetFullScreen() {
        _activity.hideSystemUIOnUIThread();

    }

    public static void logEvent(String eventString) {
        Log.d(TAG, "logEvent");
        KitKitLogger logger = ((KitkitSchoolApplication)_activity.getApplication()).getLogger();
        logger.logEvent(eventString);

//        boolean isNew = false;
//        File path = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/todoschool_" + installID);
//        if(!path.exists()){
//            path.mkdirs();
//        }
//
//        File[] files = path.listFiles();
//
//        String header = "todoschool_" + installID + "_log_";
//        String tail = ".txt";
//        String lastFilename = null;
//        File lastFile = null;
//        int lastNum = 1;
//        for (int i=0; i<files.length; i++){
//            String filename = files[i].getName();
//
//            if(lastFilename == null){
//                lastFilename = filename;
//                lastFile = files[i];
//            }
//
//            if(filename.contains(header)){
//                int num = Integer.parseInt(filename.split("_")[3].split("\\.")[0]);
//                if(num > lastNum){
//                    lastNum = num;
//                    lastFilename = filename;
//                    lastFile = files[i];
//                }
//            }
//        }
//
//
//        if(lastFilename == null){
//            lastFilename = path + "/" + header + "1" + tail;
//            lastFile = new File(lastFilename);
//            if(!lastFile.exists()){
//                try{ lastFile.createNewFile(); }catch(Exception e){}
//                isNew = true;
//            }
//        }
//
//        long size = lastFile.length();
//
//        if(size > 100*1024){
//            lastNum++;
//            lastFilename = path + "/" + header + Integer.toString(lastNum) + tail;
//            lastFile = new File(lastFilename);
//
//            if(!lastFile.exists()){
//                try{ lastFile.createNewFile(); }catch(Exception e){}
//                isNew = true;
//            }
//        }
//
//        try{
//            FileWriter fw = new FileWriter(lastFile.getAbsoluteFile(), true);
//            BufferedWriter bw = new BufferedWriter(fw);
//            bw.write((isNew ? "" : "\r\n") + eventString);
//            bw.close();
//        }catch(Exception e){
//
//        }
    }

    public static String getRandomUUID() {
        return UUID.randomUUID().toString();
    }

    public static String getAppVersion() {
        try {
            String versionName = getContext().getPackageManager()
                    .getPackageInfo(getContext().getPackageName(), 0).versionName;
            return versionName;
        } catch (Exception e) {

        }

        return "";
    }
    private void clearAppData() {
        if (VERSION_CODES.KITKAT <= VERSION.SDK_INT) {
            ((ActivityManager)getContext().getSystemService(ACTIVITY_SERVICE))
                    .clearApplicationUserData();
        } else {
            try {
                String packageName = getApplicationContext().getPackageName();
                Runtime runtime = Runtime.getRuntime();
                runtime.exec("pm clear "+packageName);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED){
            Log.v(TAG,"Permission: "+permissions[0]+ "was "+grantResults[0]);
        }
    }

//    @Override
//    protected void attachBaseContext(Context newBase) {
//
//        final String defaultLanguage = newBase.getString(com.enuma.kitkitlogger.R.string.defaultLanguage);
//        SharedPreferences preferences = newBase.getSharedPreferences("Cocos2dxPrefsFile",0);
//        String lang = preferences.getString("appLanguage","");
//        if (lang.isEmpty()) {
//            lang = defaultLanguage;
//            SharedPreferences.Editor editor = preferences.edit();
//            editor.putString("appLanguage", defaultLanguage);
//            editor.commit();
//        }
//        //String lang = Cocos2dxHelper.getStringForKey("appLanguage", defaultLanguage);
//
//        Locale newLocale = new Locale(lang);
//
//        Context context = KitkitContextWrapper.wrap(newBase, newLocale);
//        super.attachBaseContext(context);
//    }
    protected void restartApp() {
        Process.killProcess(Process.myPid());
        AlarmManager alm = (AlarmManager) this.getSystemService(Context.ALARM_SERVICE);
        alm.set(AlarmManager.RTC, System.currentTimeMillis() + 1000, PendingIntent.getActivity(this, 0, new Intent(this, this.getClass()), 0));
    }

    public static void updateStars(int numStars) {

        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication)_activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();

            user.setNumStars(numStars);
            dbHandler.updateUser(user);

        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }


    }

    public static int getStars() {
        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication)_activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();

            return user.getNumStars();
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
            return 0;
        }


    }

    public static String getCurrentUsername() {
        try {
            return currentUsername;
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
            return "";
        }

    }

    public static void finishTutorial() {

        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication)_activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();
            user.setFinishTutorial(true);
            dbHandler.updateUser(user);

        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }

    }

    public static String getResourceUri(String filename) {

        try {
            String packageName = "library.todoschool.enuma.com.todoschoollibrary";

            Context libraryContext = _activity.createPackageContext(packageName,0);
            int rId = libraryContext.getResources().getIdentifier(filename, "raw", libraryContext.getPackageName());

            if (rId > 0) {
              String uri = "android.resource://" + packageName + "/raw/" + filename;

              return uri;
            }
         }
         catch (Exception e) {
             Log.e(TAG, e.toString());
         }

         return "";
    }
}
