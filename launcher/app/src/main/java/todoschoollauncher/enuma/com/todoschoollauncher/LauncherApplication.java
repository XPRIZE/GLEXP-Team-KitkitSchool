package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;

import java.util.UUID;


/**
 * Created by ingtellect on 7/18/17.
 */

public class LauncherApplication extends Application {

    private Thread.UncaughtExceptionHandler defaultExceptionHandler;
    private KitKitLogger logger;
    private KitkitDBHandler dbHandler;
    private User currentUser;
    private String currentUsername;

    @Override
    public void onCreate ()
    {
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

        SharedPreferences preferences = getSharedPreferences("sharedPref",Context.MODE_PRIVATE);
        String sharedLang = preferences.getString("appLanguage", "");
        if (sharedLang.isEmpty()) {
            String lang = getString(com.enuma.kitkitlogger.R.string.defaultLanguage);
            SharedPreferences.Editor editor = preferences.edit();
            editor.putString("appLanguage",lang);
            editor.apply();
        }


        logger = new KitKitLogger(getPackageName(), getApplicationContext());
        dbHandler = new KitkitDBHandler(getApplicationContext());

        if (dbHandler.numUser() == 0) {
            //make users in DB
            for (int i = 0; i < 30; i++) {
                User user = new User("user"+i,0);
//                user.setNumStars(i*50);
//                user.setFinishTutorial(true);
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
        Thread.setDefaultUncaughtExceptionHandler (new Thread.UncaughtExceptionHandler()
        {
            @Override
            public void uncaughtException (Thread thread, Throwable e)
            {
                handleUncaughtException (thread, e);
            }
        });
    }

    public void handleUncaughtException (Thread thread, Throwable e)
    {
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

    public User getCurrentUser() { return currentUser; }
    public String getCurrentUsername() { return currentUsername; }

    public KitkitDBHandler getDbHandler() { return dbHandler; }

}
