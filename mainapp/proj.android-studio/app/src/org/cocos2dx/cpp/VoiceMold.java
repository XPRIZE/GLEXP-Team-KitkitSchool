/**
 * VoiceMold.java -- Wrap[TextToSpeechWrapper] (=tts-wrapper-wrapper).
 *
 * NB(xenosoz, 2018): Date created: 26 Feb, 2018
 */

package org.cocos2dx.cpp;

import java.io.File;
import java.lang.String;
import java.util.HashMap;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.media.MediaMetadataRetriever;
import android.speech.tts.TextToSpeech;
import android.util.Log;


public class VoiceMold {
    private static final String SPEAK_UTTERANCE_ID = "com.enuma.voice_engine_a.speak";
    private static final String SYNTH_UTTERANCE_ID = "com.enuma.voice_engine_a.synth";

    public TextToSpeechWrapper wrapper;
    public String locale;


    VoiceMold(String locale) {
        Context context = AppActivity.instance();
        try {
            this.locale = locale;
            wrapper = TextToSpeechWrapper.createTextToSpeechWrapper(context, locale);

            Intent ttsIntent = new Intent();
            ttsIntent.setAction(TextToSpeech.Engine.ACTION_CHECK_TTS_DATA);

            PackageManager pm = context.getPackageManager();
            List<ResolveInfo> list = pm.queryIntentActivities(ttsIntent,
                    PackageManager.GET_META_DATA);

            for (ResolveInfo info : list) {
                Log.d("Info", info.toString());
            }
        }
        catch (InterruptedException e) {
            Log.d("Error", e.toString());
        }
    }

    public boolean isEnergetic() {
        return wrapper != null && wrapper.isEnergetic();
    }

    public void speak(String text) {
        if (wrapper == null) {
            Log.d("Warn", "SpeechWrapper wrapper is null in VoiceMold::speak().");
            return;
        }

        if (!wrapper.isGood()) {
            String header = "Yo! ";
            text = header + text;
        }

        wrapper.getTts().speak(text, TextToSpeech.QUEUE_FLUSH, createParamsForSpeak());
    }

    public void warmup() {
        if (wrapper == null) {
            Log.d("Warn", "SpeechWrapper wrapper is null in VoiceMold::speak().");
            return;
        }

        String text = "";
        wrapper.getTts().speak(text, TextToSpeech.QUEUE_FLUSH, createParamsForSpeak());
    }

    public float guessSpeakDuration(String text) {
        if (wrapper == null) {
            Log.d("Warn", "SpeechWrapper wrapper is null in VoiceMold::speak().");
            return Float.NaN;
        }

        if (!wrapper.isGood()) {
            String header = "Yo! ";
            text = header + text;
        }

        try {
            // NB(xenosoz, 2018): I'm sad we need android-21 to get it done this in-memory.
            String filename;

            Context context = AppActivity.instance();
            File dummyFile = File.createTempFile("voice-engine-a.", "", context.getExternalCacheDir());

            filename = dummyFile.getAbsolutePath() + ".mp3";
            dummyFile.delete();


            if (0 == wrapper.getTts().synthesizeToFile(text, createParamsForSynth(), filename)) {
                wrapper.waitForComplete(SYNTH_UTTERANCE_ID);

                Long ms;
                MediaMetadataRetriever mm = new MediaMetadataRetriever();
                mm.setDataSource(filename);
                ms = Long.parseLong(mm.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION));
                (new File(filename)).delete();

                return ms.floatValue() / 1000.f;
            }
            else {
                Log.e("voice-engine-a", "synthesizeToFile failed");
            }
        }
        catch (Exception e) {  // XXX
            Log.e("voice-engine-a", e.toString());
        }

        return Float.NaN;
    }

    private HashMap<String, String> createParamsForSpeak() {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, SPEAK_UTTERANCE_ID);
        return params;
    }

    private HashMap<String, String> createParamsForSynth() {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, SYNTH_UTTERANCE_ID);
        return params;
    }
}
