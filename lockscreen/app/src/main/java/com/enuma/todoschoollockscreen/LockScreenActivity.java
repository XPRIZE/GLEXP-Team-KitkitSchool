package com.enuma.todoschoollockscreen;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.icu.text.SimpleDateFormat;
import android.icu.util.TimeZone;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.text.format.Time;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Date;


public class LockScreenActivity extends Activity {

    TextView clockTextView;
    ImageView chargingIcon;

    private  PowerConnectionReceiver _batteryinfo = new PowerConnectionReceiver() {
        @Override
        public  void onReceive(Context context, Intent intent) {
            int chargePlug = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, -1);
            boolean usbCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_USB;
            boolean acCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_AC;

//            Log.d("PowerConnectionReceiver", usbCharge ? "usb charging true" : "usb charging not true");
//            Log.d("PowerConnectionReceiver", acCharge ? "ac charging true" : "ac charging not true");

            updateChargingStatus(usbCharge || acCharge);

        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //Set up our Lockscreen
        makeFullScreen();
//        startService(new Intent(this,LockScreenService.class));

        setContentView(R.layout.activity_lockscreen);

        clockTextView = (TextView)findViewById(R.id.clock_textView);

        Clock c=new Clock(this);

        clockTextView.setText(DateFormat.format("h:mm aa", c.GetCurrentTime().toMillis(true)).toString());

        c.AddClockTickListner(new OnClockTickListener() {

            @Override
            public void OnSecondTick(Time currentTime) {
                Log.d("Tick Test per Second", DateFormat.format("h:mm:ss aa ", currentTime.toMillis(true)).toString());

            }

            @Override
            public void OnMinuteTick(Time currentTime) {
                Log.d("Tick Test per Minute",DateFormat.format("h:mm aa", currentTime.toMillis(true)).toString());
                clockTextView.setText(DateFormat.format("h:mm aa", currentTime.toMillis(true)).toString());

                // new day. update the date.
                if (currentTime.hour == 0 && currentTime.minute == 0) {
                    setDate();
                }
            }
        });

        setDate();

//        registerReceiver(_batteryinfo, new IntentFilter(Intent.ACTION_POWER_CONNECTED));
//        registerReceiver(_batteryinfo, new IntentFilter(Intent.ACTION_POWER_DISCONNECTED));

        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = registerReceiver(_batteryinfo, ifilter);

        int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
        boolean isCharging = status == BatteryManager.BATTERY_STATUS_CHARGING ||
                status == BatteryManager.BATTERY_STATUS_FULL;

        int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);

        int batteryPct = java.lang.Math.round(level / (float)scale * 100);

//        Log.d("LockScreen","battery status : "+status + " battery level : " + batteryPct);

        View lowBattery = findViewById(R.id.battery_low_image);
        View halfBattery = findViewById(R.id.battery_half_image);
        View fullBattery = findViewById(R.id.battery_full_image);

        if (batteryPct <= 10) {
            lowBattery.setVisibility(View.VISIBLE);
            halfBattery.setVisibility(View.GONE);
            fullBattery.setVisibility(View.GONE);
        }
        else if(batteryPct < 100) {
            lowBattery.setVisibility(View.GONE);
            halfBattery.setVisibility(View.VISIBLE);
            fullBattery.setVisibility(View.GONE);

        }
        else {
            lowBattery.setVisibility(View.GONE);
            halfBattery.setVisibility(View.GONE);
            fullBattery.setVisibility(View.VISIBLE);

        }

        chargingIcon = (ImageView) findViewById(R.id.battery_charging_image);

        int chargePlug = batteryStatus.getIntExtra(BatteryManager.EXTRA_PLUGGED, -1);
        boolean usbCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_USB;
        boolean acCharge = chargePlug == BatteryManager.BATTERY_PLUGGED_AC;

        updateChargingStatus(usbCharge || acCharge);

    }

    private void setDate() {
        SimpleDateFormat sdf = new SimpleDateFormat("EEEE, MMMM d");
        sdf.setTimeZone(TimeZone.getDefault());
        String currentDateandTime = sdf.format(new Date());

        TextView dateTextView = (TextView)findViewById(R.id.date_textView);
        dateTextView.setText(currentDateandTime);
    }

    private void updateChargingStatus(boolean isCharging) {
//        Log.d("updateChargingStatus ", isCharging ? "charging" : "not charging");
        if (isCharging)
        {
            chargingIcon.setVisibility(View.VISIBLE);
        }
        else {
            chargingIcon.setVisibility(View.GONE);
        }
    }

    /**
     * A simple method that sets the screen to fullscreen.  It removes the Notifications bar,
     *   the Actionbar and the virtual keys (if they are on the phone)
     */
    public void makeFullScreen() {
//        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN |
                        WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD |
                        WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED);
        if(Build.VERSION.SDK_INT < 19) { //View.SYSTEM_UI_FLAG_IMMERSIVE is only on API 19+
            this.getWindow().getDecorView()
                    .setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        } else {
            this.getWindow().getDecorView()
                    .setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE);
        }
    }

    @Override
    public void onBackPressed() {
        return; //Do nothing!
    }

    public void unlockScreen(View view) {
        //Instead of using finish(), this totally destroys the process
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}