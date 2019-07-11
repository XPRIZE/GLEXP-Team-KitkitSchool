package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.os.AsyncTask;
import android.os.SystemClock;
import android.util.Log;

import com.maq.kitkitlogger.KitKitLogger;

import java.text.SimpleDateFormat;

import static org.cocos2dx.cpp.kitkitlauncher.hindi.MainActivity.TAG;

/**
 * Created by xenosoz on Apr 03, 2018.
 */

public class SntpUpdater extends AsyncTask<String, Void, Boolean> {

    public int timeout = 10000;
    public long now = 0;
    public String snow = "";
    KitKitLogger logger = null;

    SntpUpdater(KitKitLogger logger) {
        this.logger = logger;
    }

    protected Boolean doInBackground(String... servers) {
        Boolean ret = false;
        for (String server : servers) {
            ret = ret || processServer(server);
        }

        return ret;
    }

    protected void onPostExecute(Boolean result) {
    }

    private boolean processServer(String server) {
        SntpClient client = new SntpClient();
        if (client.requestTime(server, timeout)) {
            long time = client.getNtpTime();
            log("NTP Time (" + server + "): " + String.valueOf(time), now);

            long now = client.getNtpTime() + SystemClock.elapsedRealtime() - client.getNtpTimeReference();
            this.now = now;

            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            String snow = sdf.format(now);
            this.snow = snow;

            log("NTP Converted (" + server + "): " + snow, now);
            return true;
        }

        log("NTP Time (" + server + "): Error", 0);
        return false;
    }

    private void log(String msg, long value) {
        Log.d(TAG, msg);
        if (logger == null) {
            return;
        }
        logger.logEvent("MainActivity", "SntpUpdater", msg, value);
    }
}
