package com.maq.xprize.kitkitlauncher.hindi;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;

import java.util.UUID;


/**
 * Created by ingtellect on 7/18/17.
 */

public class LauncherApplication extends Application {

    private KitKitLogger logger;
    private KitkitDBHandler dbHandler;
    private User currentUser;
    private String currentUsername;

    @Override
    public void onCreate() {
        super.onCreate();

//        SharedPreferences preferences = getSharedPreferences("sharedpreference", Context.CONTEXT_IGNORE_SECURITY);
//
//        String installId = preferences.getString("installId","");
//        if (installId.isEmpty()) {
//            installId = getRandomUUID();
//            SharedPreferences.Editor editor = preferences.edit();
//            editor.putString("installId", installId);
//            editor.commit();
//        }


//        SharedPreferences preferences = getSharedPreferences("sharedPref",Context.MODE_PRIVATE);
//
//        int versionCode = BuildConfig.VERSION_CODE;
//        int lastVersion = preferences.getInt("launcherVersion", 0);
//        String sharedLang = preferences.getString("appLanguage", "");
//
//        if ((lastVersion < versionCode) || (sharedLang.isEmpty())) {
//            String lang = getString(R.string.defaultLanguage);
//            SharedPreferences.Editor editor = preferences.edit();
//            editor.putString("appLanguage",lang);
//            editor.putInt("launcherVersion", versionCode);
//            editor.apply();
//        }

        SharedPreferences preferences = getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
        String lang = getString(R.string.defaultLanguage);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("appLanguage", lang);
        editor.apply();

        logger = new KitKitLogger(getPackageName(), getApplicationContext());
        dbHandler = new KitkitDBHandler(getApplicationContext());
    }

    public KitKitLogger getLogger() {
        return logger;
    }

    public String getRandomUUID() {
        return UUID.randomUUID().toString();
    }

    public User getCurrentUser() {
        return currentUser;
    }

    public String getCurrentUsername() {
        return currentUsername;
    }

    public KitkitDBHandler getDbHandler() {
        return dbHandler;
    }
}