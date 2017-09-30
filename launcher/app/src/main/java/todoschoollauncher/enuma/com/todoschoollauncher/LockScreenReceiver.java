package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Created by ingtellect on 1/9/17.
 */

public class LockScreenReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {


        Log.d("LockScreenReceiver","broadcast received");

        String action = intent.getAction();

        //If the screen was just turned on or it just booted up, start your Lock Activity
        if(action.equals(Intent.ACTION_SCREEN_OFF) || action.equals(Intent.ACTION_BOOT_COMPLETED))
        {
            Log.d("LockScreenReceiver","Start intent");
//            Intent i = new Intent(context, LockScreenActivity.class);
//            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//            context.startActivity(i);
            try {
                Intent i =  context.getPackageManager().getLaunchIntentForPackage("com.enuma.todoschoollockscreen");
                context.startActivity(i);

            }
            catch (Exception e)
            {

            }
        }
    }
}
