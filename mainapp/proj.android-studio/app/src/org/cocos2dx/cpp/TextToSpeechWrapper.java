/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// NB(xenosoz, 2018): from https://android.googlesource.com/platform/cts/+/70ebda42367ada13ff1500bf7d985714055c4cdc/tests/tests/speech/src/android/speech/tts/cts/TextToSpeechWrapper.java
// NB(xenosoz, 2018): Modified to always pick a license-free TTS engine.

package org.cocos2dx.cpp;

import android.content.Context;
import android.media.MediaPlayer;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.speech.tts.TextToSpeech.OnUtteranceCompletedListener;
import android.util.Log;

import java.util.HashSet;
import java.util.Locale;
import java.util.StringTokenizer;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Wrapper for {@link TextToSpeech} with some handy test functionality.
 */
public class TextToSpeechWrapper {
    public static final String MOCK_TTS_ENGINE = "com.android.cts.stub";
    //public static final String FREE_TO_USE_TTS_ENGINE = "com.svox.pico";
    //public static final String FREE_TO_USE_TTS_ENGINE = "com.example.android.ttsengine";
    public static final String FREE_TO_USE_TTS_ENGINE = "com.enuma.voice_engine_a";
    private static final String LOG_TAG = "TextToSpeechServiceTest";
    /**
     * maximum time to wait for tts to be initialized
     */
    private static final int TTS_INIT_MAX_WAIT_TIME = 30 * 1000;
    /**
     * maximum time to wait for speech call to be complete
     */
    private static final int TTS_SPEECH_MAX_WAIT_TIME = 5 * 1000;
    private final Context mContext;
    private final String mLocale;
    private final InitWaitListener mInitListener;
    private final UtteranceWaitListener mUtteranceListener;
    private boolean _goodEngine = false;
    private boolean _energetic = false;
    private TextToSpeech mTts;

    private TextToSpeechWrapper(Context context, String locale) {
        mContext = context;
        mLocale = locale;
        mInitListener = new InitWaitListener();
        mUtteranceListener = new UtteranceWaitListener();
    }

    public static TextToSpeechWrapper createTextToSpeechWrapper(Context context, String locale)
            throws InterruptedException {
        TextToSpeechWrapper wrapper = new TextToSpeechWrapper(context, locale);
        if (wrapper.initTts(FREE_TO_USE_TTS_ENGINE)) {
            return wrapper;
        } else {
            return null;
        }
    }

    public static TextToSpeechWrapper createTextToSpeechMockWrapper(Context context, String locale)
            throws InterruptedException {
        TextToSpeechWrapper wrapper = new TextToSpeechWrapper(context, locale);
        if (wrapper.initTts(MOCK_TTS_ENGINE)) {
            return wrapper;
        } else {
            return null;
        }
    }

    /**
     * Determines if given file path is a valid, playable music file.
     */
    public static boolean isSoundFile(String filePath) {
        // use media player to play the file. If it succeeds with no exceptions, assume file is
        //valid
        MediaPlayer mp = null;
        try {
            mp = new MediaPlayer();
            mp.setDataSource(filePath);
            mp.prepare();
            mp.start();
            mp.stop();
            return true;
        } catch (Exception e) {
            Log.e(LOG_TAG, "Exception while attempting to play music file", e);
            return false;
        } finally {
            if (mp != null) {
                mp.release();
            }
        }
    }

    private boolean initTts() throws InterruptedException {
        return initTts(new TextToSpeech(mContext, mInitListener));
    }

    private boolean initTts(String engine) throws InterruptedException {
        return initTts(new TextToSpeech(mContext, mInitListener, engine));
    }

    private boolean initTts(TextToSpeech tts) throws InterruptedException {
        _goodEngine = false;
        mTts = tts;
        if (!mInitListener.waitForInit()) {
            return false;
        }
        mTts.setOnUtteranceCompletedListener(mUtteranceListener);
        updateEngineStatus();
        updateVitalStatus();

        return true;
    }

    private void updateEngineStatus() {
        if (mTts != null) {
            for (TextToSpeech.EngineInfo info : mTts.getEngines()) {
                Log.d("Info!", info.name);
                if (FREE_TO_USE_TTS_ENGINE.equals(info.name)) {
                    _goodEngine = true;
                    return;
                }
            }
        }

        _goodEngine = false;
    }

    private void updateVitalStatus() {
        if (mTts != null) {
            // NB(xenosoz, 2018): I don't have Locale.forLanguageTag here.

            String lang = "";
            String country = "";

            StringTokenizer st = new StringTokenizer(mLocale, "-");
            if (st.hasMoreTokens()) {
                lang = st.nextElement().toString();
            }
            if (st.hasMoreTokens()) {
                country = st.nextElement().toString();
            }

            Locale loc = new Locale(lang, country);
            int status = mTts.setLanguage(loc);
            if (status >= TextToSpeech.LANG_AVAILABLE) {
                _energetic = true;
            }
        }
    }

    public boolean waitForComplete(String utteranceId) throws InterruptedException {
        return mUtteranceListener.waitForComplete(utteranceId);
    }

    public TextToSpeech getTts() {
        return mTts;
    }

    public boolean isGood() {
        return _goodEngine;
    }

    public boolean isEnergetic() {
        return _energetic;
    }

    public void shutdown() {
        mTts.shutdown();
    }

    /**
     * Listener for waiting for TTS engine initialization completion.
     */
    private static class InitWaitListener implements OnInitListener {
        private final Lock mLock = new ReentrantLock();
        private final Condition mDone = mLock.newCondition();
        private Integer mStatus = null;

        public void onInit(int status) {
            mLock.lock();
            try {
                mStatus = new Integer(status);
                mDone.signal();
            } finally {
                mLock.unlock();
            }
        }

        public boolean waitForInit() throws InterruptedException {
            long timeOutNanos = TimeUnit.MILLISECONDS.toNanos(TTS_INIT_MAX_WAIT_TIME);
            mLock.lock();
            try {
                while (mStatus == null) {
                    if (timeOutNanos <= 0) {
                        return false;
                    }
                    timeOutNanos = mDone.awaitNanos(timeOutNanos);
                }
                return mStatus == TextToSpeech.SUCCESS;
            } finally {
                mLock.unlock();
            }
        }
    }

    /**
     * Listener for waiting for utterance completion.
     */
    private static class UtteranceWaitListener implements OnUtteranceCompletedListener {
        private final Lock mLock = new ReentrantLock();
        private final Condition mDone = mLock.newCondition();
        private final HashSet<String> mCompletedUtterances = new HashSet<String>();

        public void onUtteranceCompleted(String utteranceId) {
            mLock.lock();
            try {
                mCompletedUtterances.add(utteranceId);
                mDone.signal();
            } finally {
                mLock.unlock();
            }
        }

        public boolean waitForComplete(String utteranceId)
                throws InterruptedException {
            long timeOutNanos = TimeUnit.MILLISECONDS.toNanos(TTS_INIT_MAX_WAIT_TIME);
            mLock.lock();
            try {
                while (!mCompletedUtterances.remove(utteranceId)) {
                    if (timeOutNanos <= 0) {
                        return false;
                    }
                    timeOutNanos = mDone.awaitNanos(timeOutNanos);
                }
                return true;
            } finally {
                mLock.unlock();
            }
        }
    }
}