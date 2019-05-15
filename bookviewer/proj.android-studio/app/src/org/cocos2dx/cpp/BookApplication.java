package org.cocos2dx.cpp;

import android.app.Application;

import com.enuma.kitkitlogger.KitKitLogger;

/**
 * Created by ingtellect on 7/25/17.
 */

public class BookApplication extends Application {
    private Thread.UncaughtExceptionHandler defaultExceptionHandler;
    private KitKitLogger logger;

    @Override
    public void onCreate ()
    {
        super.onCreate();
        logger = new KitKitLogger(getPackageName(), getApplicationContext());

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
}
