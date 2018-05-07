package com.enuma.voice_engine_a;

import android.os.Bundle;
import android.preference.PreferenceFragment;

/**
 * NB(xenosoz, 2018): Fragment for Preference Activity
 */

public class GeneralSettingsFragment extends PreferenceFragment {
    static final String SHARED_PREFS_NAME = "VoiceEngineSettings";
    static final String WHISPER_KEY = "robot_speak_whisper";  // XXX

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getPreferenceManager().setSharedPreferencesName(SHARED_PREFS_NAME);
        addPreferencesFromResource(R.xml.general_settings);
    }
}
