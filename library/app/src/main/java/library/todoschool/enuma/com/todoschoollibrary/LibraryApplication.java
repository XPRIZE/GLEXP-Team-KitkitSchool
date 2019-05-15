package library.todoschool.enuma.com.todoschoollibrary;

import android.app.Application;
import android.content.Context;

import com.enuma.kitkitlogger.KitKitLogger;
import com.nostra13.universalimageloader.cache.disc.naming.Md5FileNameGenerator;
import com.nostra13.universalimageloader.core.ImageLoader;
import com.nostra13.universalimageloader.core.ImageLoaderConfiguration;
import com.nostra13.universalimageloader.core.assist.QueueProcessingType;

/**
 * Created by ingtellect on 7/21/17.
 */

public class LibraryApplication extends Application {
    private Thread.UncaughtExceptionHandler defaultExceptionHandler;
    private KitKitLogger logger;

    @Override
    public void onCreate ()
    {
        super.onCreate();
        logger = new KitKitLogger(getPackageName(), getApplicationContext());

        defaultExceptionHandler = Thread.getDefaultUncaughtExceptionHandler();

        initImageLoader(this);

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

    public static void initImageLoader(Context context) {
        // This configuration tuning is custom. You can tune every option, you may tune some of them,
        // or you can create default configuration by
        //  ImageLoaderConfiguration.createDefault(this);
        // method.
        ImageLoaderConfiguration.Builder config = new ImageLoaderConfiguration.Builder(context);
        config.threadPriority(Thread.NORM_PRIORITY - 2);
        config.denyCacheImageMultipleSizesInMemory();
        config.diskCacheFileNameGenerator(new Md5FileNameGenerator());
        config.diskCacheSize(100 * 1024 * 1024); // 100 MiB
        config.tasksProcessingOrder(QueueProcessingType.LIFO);

        if (BuildConfig.DEBUG == true) {
            config.writeDebugLogs(); // Remove for release app
        }

        // Initialize ImageLoader with configuration.
        ImageLoader.getInstance().init(config.build());
    }
}
