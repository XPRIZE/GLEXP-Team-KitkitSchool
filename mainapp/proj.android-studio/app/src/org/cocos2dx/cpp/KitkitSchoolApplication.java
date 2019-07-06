package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.maq.xprize.kitkitschool.hindi.R;

import java.util.UUID;

/**
 * Created by ingtellect on 7/24/17.
 */

public class KitkitSchoolApplication extends Application {
    private Thread.UncaughtExceptionHandler defaultExceptionHandler;
    private KitKitLogger logger;

    private KitkitDBHandler dbHandler;
    private User currentUser;
    private String currentUsername;


    @Override
    public void onCreate() {
        super.onCreate();
        logger = new KitKitLogger(getPackageName(), getApplicationContext());

        dbHandler = new KitkitDBHandler(getApplicationContext());
        SharedPreferences preferences = getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
        String lang = getString(R.string.defaultLanguage);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("appLanguage", lang);
        editor.apply();
//        From application activity of Launcher
        if (dbHandler.numUser() == 0) {
            //make users in DB
            for (int i = 0; i < 100; i++) {
                User user = new User("user" + i, 0);
                dbHandler.addUser(user);

                if (i == 0) {
                    dbHandler.setCurrentUser(user);
                    currentUser = user;
                    currentUsername = user.getUserName();
                }
            }

        }
        if (currentUser == null) {
            currentUser = dbHandler.getCurrentUser();
            currentUsername = currentUser.getUserName();
        }
        defaultExceptionHandler = Thread.getDefaultUncaughtExceptionHandler();
        // Setup handler for uncaught exceptions.
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable e) {
                handleUncaughtException(thread, e);
            }
        });
    }

    public void handleUncaughtException(Thread thread, Throwable e) {
        e.printStackTrace(); // not all Android versions will print the stack trace automatically

        logger.extractLogToFile();

        defaultExceptionHandler.uncaughtException(thread, e);
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
