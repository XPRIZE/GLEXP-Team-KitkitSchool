/****************************************************************************
Copyright (c) 2015-2017 Chukong Technologies Inc.
 
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


import android.Manifest;
import android.app.ActivityManager;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Build.VERSION_CODES;
import android.os.Bundle;
import android.os.Environment;
import android.os.Process;
import android.support.v4.app.ActivityCompat;
import android.util.Log;

import com.maq.kitkitProvider.Fish;
import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.google.android.vending.expansion.downloader.Helpers;

import org.cocos2dx.cpp.ReadingBird.PlayAudio;
import org.cocos2dx.cpp.ReadingBird.SpeechRecognition;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxVideoHelper;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.UUID;
import java.util.zip.ZipFile;

import kitkitschool.DownloadExpansionFile;
import utils.Zip;


public class AppActivity extends Cocos2dxActivity {
    public static AppActivity _activity;
    public static String _launchString;
    private Cocos2dxGLSurfaceView glSurfaceView;
    public static KitkitDBHandler _dbHandler;

    private static String TAG = "KitkitSchoolActivity";
    private static final String EXPANSION_FILE_VERSION_KEY_NAME = "0";

    protected String appLanguage;
    protected static String currentUsername;
    protected static User currentUser;
    protected boolean signModeOn;
    private static int _videoPlayerIndex = 0;
    public static AppActivity instance() { return _activity; }

    public boolean isPermissionGranted() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED &&
                    checkSelfPermission(Manifest.permission.RECORD_AUDIO)
                            == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG,"Permission is granted");
                return true;
            } else {

                Log.v(TAG,"Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1);
                return false;
            }
        } else { //permission is automatically granted on sdk<23 upon installation
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        String packageNameDirPath = Environment.getExternalStorageDirectory() + "/Android/data/" + getApplicationContext().getPackageName();
        File packageNameDir = new File(packageNameDirPath);
        if (!packageNameDir.exists()) {
            packageNameDir.mkdir();
        }
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        Log.d(TAG,"onCreate");
        isPermissionGranted();
        _activity = this;
        _dbHandler = new KitkitDBHandler(_activity);

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

        // init sign-language value
        try {
            Context launcherContext = createPackageContext("com.maq.xprize.kitkitlauncher.hindi",0);
            SharedPreferences pref = launcherContext.getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
            signModeOn = pref.getBoolean("sign_language_mode_on", false);
            Cocos2dxHelper.setBoolForKey("sign_language_mode_on", signModeOn);
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        try {
            Context launcherContext = createPackageContext("com.maq.xprize.kitkitlauncher.hindi",0);
            SharedPreferences pref = launcherContext.getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
            appLanguage = pref.getString("appLanguage", getString(com.maq.kitkitlogger.R.string.defaultLanguage));
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

        /*SharedPreferences prefs = getSharedPreferences("किटकिट स्कूल", Context.MODE_PRIVATE);
        if (isRequiredUnzipExpansionFile(prefs)) {
            unzipExpansionFile(getExpansionFilePath());
            prefs.edit().putInt(EXPANSION_FILE_VERSION_KEY_NAME, DownloadExpansionFile.xAPK.mFileVersion).apply();
        }*/
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
                        try {
                            _activity.moveTaskToBack(true);
                        } catch (Exception e) {
                            Log.e(TAG, "", e);
                            Process.killProcess(Process.myPid());
                        }
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
        {
            try {
                Context context = createPackageContext("com.maq.xprize.kitkitlauncher.hindi",0);
                SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
                boolean isReviewModeOn = pref.getBoolean("review_mode_on", false);
                Cocos2dxHelper.setBoolForKey("review_mode_on", isReviewModeOn);

            } catch (Exception e) {
                Log.e(TAG, "error when getting review mode setting. please check launcher is installed.");
            }
        }

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

        // sign-language
        try {
            Context context = createPackageContext("com.maq.xprize.kitkitlauncher.hindi",0);
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            boolean sharedSignModeOn = pref.getBoolean("sign_language_mode_on", false);

            if (signModeOn != sharedSignModeOn) {
                signModeOn = sharedSignModeOn;
                Cocos2dxHelper.setBoolForKey("sign_language_mode_on", signModeOn);
                restartApp();
            }
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        // language
        try {
            Context context = createPackageContext("com.maq.xprize.kitkitlauncher.hindi",0);
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            String sharedLang = pref.getString("appLanguage", getString(com.maq.kitkitlogger.R.string.defaultLanguage));

            if (!appLanguage.equals(sharedLang)) {
                Cocos2dxHelper.setStringForKey("appLanguage", appLanguage);

                restartApp();
            }
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        // user
        try {
            currentUser = ((KitkitSchoolApplication)getApplication()).getDbHandler().getCurrentUser();

            if (!currentUsername.equals(currentUser.getUserName())) {
                restartApp();
            }
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }

        resumeAudio();

        Cocos2dxVideoHelper.resumeVideo(_videoPlayerIndex);
        Cocos2dxVideoHelper.startVideo(_videoPlayerIndex);
    }

    private boolean isRequiredUnzipExpansionFile(SharedPreferences prefs) {
        return prefs.getInt(EXPANSION_FILE_VERSION_KEY_NAME, 0) != DownloadExpansionFile.xAPK.mFileVersion;
    }

    private void unzipExpansionFile(String zipFilePath) {
        Log.i(getClass().getName(), "going to unzip the file for " + zipFilePath);
        try {
            File file = new File(zipFilePath);
            ZipFile zipFile = new ZipFile(file);
            Zip _zip = new Zip(zipFile, this);
            _zip.unzip(getUnzippedExpansionFilePath());
            _zip.close();
            Log.i(getClass().getName(), "the file " + zipFilePath + " unzipped successfully");
            //file.delete();
        } catch (IOException ie) {
            Log.e(getClass().getName(), "fail to unzip the file " + zipFilePath, ie);
        }
    }

    private String getUnzippedExpansionFilePath() {
        return getContext().getExternalFilesDir(null).toString() + File.separator;
    }

    /**
     * get the path which the expansionFile which downloaded from play store will be stored
     *
     * @return
     */
    private String getExpansionFilePath() {

        //return Environment.getExternalStorageDirectory().toString() + File.separator + "autocognita.zip";
        return Environment.getExternalStorageDirectory().toString() + "/Android/obb/"/*APKExpansionSupport.EXP_PATH*/ + Helpers.getPackageName(this) + File.separator +
                Helpers.getExpansionAPKFileName(this, DownloadExpansionFile.xAPK.mIsMain, DownloadExpansionFile.xAPK.mFileVersion);
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

    public static void deleteAllUserDefault() {
        if (_activity != null) {
            _activity.getSharedPreferences("Cocos2dxPrefsFile", 0).edit().clear().apply();
        }
    }

    public static void moveToBackground() {
        _activity.moveTaskToBack(true);
        //moveTaskToBack(true);
    }

    static
    {
        System.loadLibrary("MyGame");
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
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED && grantResults[1]== PackageManager.PERMISSION_GRANTED){
            Log.v(TAG, "Permission: "+permissions[0]+ " was "+grantResults[0]);
            Log.v(TAG, "Permission: "+permissions[1]+ " was "+grantResults[1]);
        }
    }

    //    @Override
//    protected void attachBaseContext(Context newBase) {
//
//        final String defaultLanguage = newBase.getString(com.maq.kitkitlogger.R.string.defaultLanguage);
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

    public static void setUnlockFishBowl(boolean isUnlock) {
        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication)_activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();
            user.setUnlockFishBowl(isUnlock);
            dbHandler.updateUser(user);
        }
        catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }
    }

    public static String getResourceUri(String filename) {
        File fileCheck = new File(Environment.getExternalStorageDirectory() + File.separator + "Library" + File.separator + "cache.txt");
        if (fileCheck.exists()) {
            String appLanguage = Cocos2dxHelper.getStringForKey("appLanguage", "sw-tz").toLowerCase();
            String pathExternalRaw = Environment.getExternalStorageDirectory() + File.separator + "Library" + File.separator + appLanguage + File.separator + "res" + File.separator + "raw";

            File resourceFile = new File(pathExternalRaw + File.separator + filename + ".mp4");
            Log.i(TAG, "resourcePath : " + resourceFile.getAbsolutePath() + ", exists : " + resourceFile.exists());
            if (resourceFile.exists()) {
                return resourceFile.getAbsolutePath();
            }
        } else {
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
        }

        return "";
    }

    @Override
    protected void onPause() {
        super.onPause();
        onStopListeningAndRecognition();
        pauseAudio();
        Cocos2dxVideoHelper.pauseVideo(_videoPlayerIndex);
    }

    private static SpeechRecognition mSpeechRecognition;
    private static PlayAudio mPlayAudio;
    public static void onSetupSpeechRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i( "onSetupSpeechRecognition");
        mSpeechRecognition = new SpeechRecognition();
        mSpeechRecognition.setup(_activity);

//        String externalFolderPath = Environment.getExternalStorageDirectory().getAbsolutePath() +"/KitkitSchool/";
        String externalFolderPath = getContext().getExternalFilesDir(null).toString() + File.separator; // Android/data/<package_name>/files/
//        File externalFile = new File(externalFolderPath + "cache.txt");
//        mPlayAudio = new PlayAudio(_activity, externalFile.exists(), externalFolderPath);
        mPlayAudio = new PlayAudio(_activity, true, externalFolderPath);
    }

    public static void onCleanUpSpeechRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i( "onCleanUpSpeechRecognition");
        if (mSpeechRecognition != null) {
            mSpeechRecognition.cleanUp();
            mSpeechRecognition = null;
        }

        if (mPlayAudio != null) {
            mPlayAudio.cleanUp();
            mPlayAudio = null;
        }
    }

    public static void onStartListening(int triggerVolume, int silentVolume, String phone) {
        org.cocos2dx.cpp.ReadingBird.Log.i( "onStartListening : " + triggerVolume + ", phone : " + phone);
        if (mSpeechRecognition != null) {
            mSpeechRecognition.startListening(triggerVolume, silentVolume, phone);
        }
    }

    public static void onStopListeningAndRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i( "onStopListeningAndRecognition");
        if (mSpeechRecognition != null) {
            mSpeechRecognition.stopListeningAndRecognition();
        }
    }

    public static void onPauseListeningAndRecognition() {
        if (mSpeechRecognition != null) {
            mSpeechRecognition.pauseListeningAndRecognition();
        }
    }

    public static void onResumeListeningAndRecognition() {
        if (mSpeechRecognition != null) {
            mSpeechRecognition.resumeListeningAndRecognition();
        }
    }

    public static void playPCMAudio() {
        if (mPlayAudio != null && mSpeechRecognition != null) {
            mPlayAudio.playPCM(mSpeechRecognition.getSpeechRecordFilePath());
        }
    }

    public static void playAudio(String filePath) {
        if (mPlayAudio != null) {
            mPlayAudio.play(filePath.toLowerCase().replace(" ", "_"));
        }
    }

    public static void stopAudio() {
        if (mPlayAudio != null) {
            mPlayAudio.stop();
        }
    }

    public static void pauseAudio() {
        if (mPlayAudio != null) {
            mPlayAudio.pause();
        }
    }

    public static void resumeAudio() {
        if (mPlayAudio != null) {
            mPlayAudio.resume();
        }
    }

    public static void playVideoPlayer(int videoPlayerIndex) {
        _videoPlayerIndex = videoPlayerIndex;
        Log.i("myLog", "playVideoPlayer : " + videoPlayerIndex);
    }

    public static String getExternalStorageDirectory() {
        return Environment.getExternalStorageDirectory().getAbsolutePath();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // for fishbowl
    public static void addFish(String fishID, int skinNo, String fishName, String position) {
        if (_dbHandler != null) {
            _dbHandler.addFish(fishID, skinNo, fishName, position);
        }
    }

    public static String getFishes() {
        String result = "[]";
        if (_dbHandler != null) {
            ArrayList<Fish> fishes = _dbHandler.getFishes();
            result = fishes.toString();
        }
        return result;
    }

    public static boolean deleteFish(int id) {
        if (_dbHandler != null) {
            return _dbHandler.deleteFish(id);
        }
        return false;
    }
}
