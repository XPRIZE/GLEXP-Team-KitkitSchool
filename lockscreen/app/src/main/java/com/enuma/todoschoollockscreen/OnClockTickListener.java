package com.enuma.todoschoollockscreen;

/**
 * Created by ingtellect on 1/9/17.
 */

import android.text.format.Time;

public interface OnClockTickListener {
    public void OnSecondTick(Time currentTime);
    public void OnMinuteTick(Time currentTime);
}

