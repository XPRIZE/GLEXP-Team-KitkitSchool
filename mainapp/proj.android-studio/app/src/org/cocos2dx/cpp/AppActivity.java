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
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;

import com.google.firebase.analytics.FirebaseAnalytics;
import com.maq.kitkitProvider.Fish;
import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.ReadingBird.PlayAudio;
import org.cocos2dx.cpp.ReadingBird.SpeechRecognition;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxVideoHelper;

import java.io.File;
import java.util.ArrayList;
import java.util.Objects;
import java.util.UUID;

import kitkitschool.DownloadExpansionFile;

import static kitkitschool.DownloadExpansionFile.xAPKS;

public class AppActivity extends Cocos2dxActivity {
    public static AppActivity _activity;
    public static String _launchString;
    public static KitkitDBHandler _dbHandler;
    private static FirebaseAnalytics mFirebaseAnalytics;
    private static String TAG = "KitkitSchoolActivity";
    private static final String EXPANSION_FILE_VERSION_KEY_NAME = "0";

    protected String appLanguage;
    protected static String currentUsername;
    protected static User currentUser;
    private static int _videoPlayerIndex = 0;
    private static SpeechRecognition mSpeechRecognition;
    private static PlayAudio mPlayAudio;

    static {
        System.loadLibrary("MyGame");
    }

    protected boolean signModeOn;
    private Cocos2dxGLSurfaceView glSurfaceView;

    public static AppActivity instance() {
        return _activity;
    }

    public static String getLaunchString() {
        return _launchString;
    }

    public static void clearLaunchString() {
        _launchString = "";
    }

    public Cocos2dxGLSurfaceView onCreateView() {
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
//                            Calling Kitkit Launcher MainActivity
                            _activity.moveTaskToBack(true);
                            Intent startLauncher = new Intent(_activity, org.cocos2dx.cpp.kitkitlauncher.hindi.MainActivity.class);
                            _activity.startActivity(startLauncher);
                        } catch (Exception e) {
                            Process.killProcess(Process.myPid());
                        }
                    }
                });
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
    }

    public static void staticSetFullScreen() {
        _activity.hideSystemUIOnUIThread();

    }

    public static void logEvent(String eventString) {
        Log.d(TAG, "logEvent");
        KitKitLogger logger = ((KitkitSchoolApplication) _activity.getApplication()).getLogger();
        logger.logEvent(eventString);
    }

    public static String getRandomUUID() {
        return UUID.randomUUID().toString();
    }

    public static String getAppVersion() {
        try {
            return getContext().getPackageManager()
                    .getPackageInfo(getContext().getPackageName(), 0).versionName;
        } catch (Exception ignored) {
        }

        return "";
    }

    // Set the parameter "current_screen" in the logged events
    public void firebase_setCurrentScreen(String screenName, String screenClass) {
        if (screenName.equals("")) {
            screenName = null;
        }
        if (screenClass.equals("")) {
            screenClass = null;
        }
        mFirebaseAnalytics.setCurrentScreen(this, screenName, screenClass);
    }

    // Log the event "playGame" after a game has been played
    public static void logFirebaseEvent_playGame(String game, int level, double duration, boolean freechoice, boolean completed) {
        Bundle params = new Bundle();
        params.putString("game", game);
        params.putInt("level", level);
        params.putDouble("duration", duration);
        params.putBoolean("freechoice", freechoice);
        params.putBoolean("completed", completed);
        mFirebaseAnalytics.logEvent("playGame", params);
    }

    public static void updateStars(int numStars) {

        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication) _activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();

            user.setNumStars(numStars);
            dbHandler.updateUser(user);

        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }


    }

    public static int getStars() {
        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication) _activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();

            return user.getNumStars();
        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
            return 0;
        }

    }

    public static String getCurrentUsername() {
        try {
            return currentUsername;
        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
            return "";
        }

    }

    public static void finishTutorial() {

        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication) _activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();
            user.setFinishTutorial(true);
            dbHandler.updateUser(user);

        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }

    }

    public static void setUnlockFishBowl(boolean isUnlock) {
        try {
            KitkitDBHandler dbHandler = ((KitkitSchoolApplication) _activity.getApplication()).getDbHandler();
            User user = dbHandler.getCurrentUser();
            user.setUnlockFishBowl(isUnlock);
            dbHandler.updateUser(user);
        } catch (Exception e) {
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

                Context libraryContext = _activity.createPackageContext(packageName, 0);
                int rId = libraryContext.getResources().getIdentifier(filename, "raw", libraryContext.getPackageName());
                if (rId > 0) {

                    return "android.resource://" + packageName + "/raw/" + filename;
                }
            } catch (Exception e) {
                Log.e(TAG, e.toString());
            }
        }

        return "";
    }

    public static void onSetupSpeechRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i("onSetupSpeechRecognition");
        mSpeechRecognition = new SpeechRecognition();
        mSpeechRecognition.setup(_activity);

        String externalFolderPath = Objects.requireNonNull(getContext().getExternalFilesDir(null)).toString() + File.separator; // Android/data/<package_name>/files/
        mPlayAudio = new PlayAudio(_activity, true, externalFolderPath);
    }

    public static void onCleanUpSpeechRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i("onCleanUpSpeechRecognition");
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
        org.cocos2dx.cpp.ReadingBird.Log.i("onStartListening : " + triggerVolume + ", phone : " + phone);
        if (mSpeechRecognition != null) {
            mSpeechRecognition.startListening(triggerVolume, silentVolume, phone);
        }
    }

    public static void onStopListeningAndRecognition() {
        org.cocos2dx.cpp.ReadingBird.Log.i("onStopListeningAndRecognition");
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

    public void isPermissionGranted() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED &&
                    checkSelfPermission(Manifest.permission.RECORD_AUDIO)
                            == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG, "Permission is granted");
            } else {

                Log.v(TAG, "Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1);
            }
        } else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG, "Permission is granted");
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        SharedPreferences sharedPref = getSharedPreferences("ExpansionFile", MODE_PRIVATE);
        int defaultFileVersion = 0;

        // Retrieve the stored values of main and patch file version
        int storedMainFileVersion = sharedPref.getInt(getString(R.string.mainFileVersion), defaultFileVersion);
        int storedPatchFileVersion = sharedPref.getInt(getString(R.string.patchFileVersion), defaultFileVersion);
        boolean isExtractionRequired = isExpansionExtractionRequired(storedMainFileVersion, storedPatchFileVersion);

        if (storedMainFileVersion == 0 && storedPatchFileVersion == 0) {
            // Set main and patch file version to 0, if the extractions takes place for the first time
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.putInt(getString(R.string.mainFileVersion), 0);
            editor.putInt(getString(R.string.patchFileVersion), 0);
            editor.commit();
            startSplashScreenActivity();
        } else if (isExtractionRequired) {
            // If main or patch file is updated, the extraction process needs to be performed again
            startSplashScreenActivity();
        }

        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);

        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        Log.d(TAG, "onCreate");
        isPermissionGranted();
        _activity = this;
        _dbHandler = new KitkitDBHandler(_activity);
        mFirebaseAnalytics = FirebaseAnalytics.getInstance(this);

        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            if (extras.getBoolean("clearAppData", false)) {
                clearAppData();
            }
            if (getIntent().hasExtra("test")) {
                _launchString = extras.getString("test");
                Log.d(TAG, "onCreate launch string " + _launchString);

            }
        }

        // init sign-language value
        try {
            Context launcherContext = createPackageContext(getPackageName(), 0);
            SharedPreferences pref = launcherContext.getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
            signModeOn = pref.getBoolean("sign_language_mode_on", false);
            Cocos2dxHelper.setBoolForKey("sign_language_mode_on", signModeOn);
        } catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        try {
            Context launcherContext = createPackageContext(getPackageName(), 0);
            SharedPreferences pref = launcherContext.getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
            appLanguage = pref.getString("appLanguage", getString(com.maq.kitkitlogger.R.string.defaultLanguage));
            Cocos2dxHelper.setStringForKey("appLanguage", appLanguage);
        } catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        try {
            currentUser = ((KitkitSchoolApplication) getApplication()).getDbHandler().getCurrentUser();
            currentUsername = currentUser.getUserName();
        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }
    }

    private void startSplashScreenActivity() {
        Intent intent = new Intent(AppActivity.this, SplashScreenActivity.class);
        startActivity(intent);
        finish();
    }

    private boolean isExpansionExtractionRequired(int storedMainFileVersion, int storedPatchFileVersion) {
        for (DownloadExpansionFile.XAPKFile xf : xAPKS) {
            // If main or patch file is updated set isExtractionRequired to true
            if (xf.mIsMain && xf.mFileVersion != storedMainFileVersion || !xf.mIsMain && xf.mFileVersion != storedPatchFileVersion) {
                return true;
            }
        }
        return false;
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
                Context context = createPackageContext(getPackageName(), 0);
                SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
                boolean isReviewModeOn = pref.getBoolean("review_mode_on", false);
                Cocos2dxHelper.setBoolForKey("review_mode_on", isReviewModeOn);

            } catch (Exception e) {
                Log.e(TAG, "error when getting review mode setting. please check launcher is installed.");
            }
        }

        super.onResume();

        Log.d(TAG, "onResume");

        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            Log.d(TAG, "onResume extra is not null");
            if (extras.getBoolean("clearAppData", false)) {
                Log.d(TAG, "onResume clearAppData is true");
                clearAppData();
            }
            if (getIntent().hasExtra("test")) {
                _launchString = extras.getString("test");
                Log.d(TAG, "onResume launch string " + _launchString);
            }

        }

        // sign-language
        try {
            Context context = createPackageContext(getPackageName(), 0);
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            boolean sharedSignModeOn = pref.getBoolean("sign_language_mode_on", false);

            if (signModeOn != sharedSignModeOn) {
                signModeOn = sharedSignModeOn;
                Cocos2dxHelper.setBoolForKey("sign_language_mode_on", signModeOn);
                restartApp();
            }
        } catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        // language
        try {
            Context context = createPackageContext(getPackageName(), 0);
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            String sharedLang = pref.getString("appLanguage", getString(com.maq.kitkitlogger.R.string.defaultLanguage));

            if (!appLanguage.equals(sharedLang)) {
                Cocos2dxHelper.setStringForKey("appLanguage", appLanguage);

                restartApp();
            }
        } catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        // user
        try {
            currentUser = ((KitkitSchoolApplication) getApplication()).getDbHandler().getCurrentUser();

            if (!currentUsername.equals(currentUser.getUserName())) {
                restartApp();
            }
        } catch (Exception e) {
            Log.e(TAG, "error when getting current user. please check launcher is installed.");
        }

        resumeAudio();

        Cocos2dxVideoHelper.resumeVideo(_videoPlayerIndex);
        Cocos2dxVideoHelper.startVideo(_videoPlayerIndex);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI() {
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

    public void hideSystemUIOnUIThread() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                hideSystemUI();
            }
        });
    }

    private void clearAppData() {
        if (VERSION_CODES.KITKAT <= VERSION.SDK_INT) {
            ((ActivityManager) Objects.requireNonNull(getContext().getSystemService(ACTIVITY_SERVICE)))
                    .clearApplicationUserData();
        } else {
            try {
                String packageName = getApplicationContext().getPackageName();
                Runtime runtime = Runtime.getRuntime();
                runtime.exec("pm clear " + packageName);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults[0] == PackageManager.PERMISSION_GRANTED && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
            Log.v(TAG, "Permission: " + permissions[0] + " was " + grantResults[0]);
            Log.v(TAG, "Permission: " + permissions[1] + " was " + grantResults[1]);
        }
    }

    protected void restartApp() {
        Process.killProcess(Process.myPid());
        AlarmManager alm = (AlarmManager) this.getSystemService(Context.ALARM_SERVICE);
        if (alm != null) {
            alm.set(AlarmManager.RTC, System.currentTimeMillis() + 1000, PendingIntent.getActivity(this, 0, new Intent(this, this.getClass()), 0));
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        onStopListeningAndRecognition();
        pauseAudio();
        Cocos2dxVideoHelper.pauseVideo(_videoPlayerIndex);
    }
}