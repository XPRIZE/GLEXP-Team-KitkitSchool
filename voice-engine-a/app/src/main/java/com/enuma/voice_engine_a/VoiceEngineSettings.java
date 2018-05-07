package com.enuma.voice_engine_a;

import android.os.Bundle;
import android.preference.PreferenceActivity;

import java.util.List;

/**
 *  NB(xenosoz, 2018): Preference Activity
 */

public class VoiceEngineSettings extends PreferenceActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onBuildHeaders(List<Header> target) {
        loadHeadersFromResource(R.xml.preferences_headers, target);
    }
}
