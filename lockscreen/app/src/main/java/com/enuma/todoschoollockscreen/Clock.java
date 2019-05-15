package com.enuma.todoschoollockscreen;

/**
 * Created by ingtellect on 1/9/17.
 */


import java.util.*;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.SystemClock;
import android.text.format.Time;

public class Clock
{
    public static final int TICKPERSECOND=0;
    public static final int TICKPERMINUTE=1;

    private Time Time;
    private TimeZone TimeZone;
    private Handler Handler;
    private List<OnClockTickListener> OnClockTickListenerList = new ArrayList<OnClockTickListener>();

    private Runnable Ticker;

    private  BroadcastReceiver IntentReceiver;
    private IntentFilter IntentFilter;

    private int TickMethod=1;
    Context Context;

    public Clock(Context context)
    {
        this(context, Clock.TICKPERMINUTE);
    }
    public Clock(Context context,int tickMethod)
    {
        this.Context=context;
        this.TickMethod=tickMethod;
        this.Time=new Time();
        this.Time.setToNow();

        switch (TickMethod)
        {
            case 0:
                this.StartTickPerSecond();
                break;
            case 1:
                this.StartTickPerMinute();
                break;

            default:
                break;
        }
    }
    private void Tick(long tickInMillis)
    {
        Clock.this.Time.set(Clock.this.Time.toMillis(true)+tickInMillis);
        this.NotifyOnTickListners();
    }
    private void NotifyOnTickListners()
    {
        switch (TickMethod)
        {
            case 0:
                for(OnClockTickListener listner:OnClockTickListenerList)
                {
                    listner.OnSecondTick(Time);
                }
                break;
            case 1:
                for(OnClockTickListener listner:OnClockTickListenerList)
                {
                    listner.OnMinuteTick(Time);
                }
                break;
        }

    }
    private void StartTickPerSecond()
    {
        this.Handler=new Handler();
        this.Ticker = new Runnable()
        {
            public void run()
            {
                Tick(1000);
                long now = SystemClock.uptimeMillis();
                long next = now + (1000 - now % 1000);
                Handler.postAtTime(Ticker, next);
            }
        };
        this.Ticker.run();

    }
    private void StartTickPerMinute()
    {
        this.IntentReceiver= new BroadcastReceiver()
        {
            @Override
            public void onReceive(Context context, Intent intent)
            {
                Tick(60000);

            }
        };
        this.IntentFilter = new IntentFilter();
        this.IntentFilter.addAction(Intent.ACTION_TIME_TICK);
        this.Context.registerReceiver(this.IntentReceiver, this.IntentFilter, null, this.Handler);

    }
    public void StopTick()
    {
        if(this.IntentReceiver!=null)
        {
            this.Context.unregisterReceiver(this.IntentReceiver);
        }
        if(this.Handler!=null)
        {
            this.Handler.removeCallbacks(this.Ticker);
        }
    }
    public Time GetCurrentTime()
    {
        return this.Time;
    }
    public void AddClockTickListner(OnClockTickListener listner)
    {
        this.OnClockTickListenerList.add(listner);

    }

}