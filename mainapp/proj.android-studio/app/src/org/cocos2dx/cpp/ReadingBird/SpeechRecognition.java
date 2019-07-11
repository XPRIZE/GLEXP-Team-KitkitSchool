package org.cocos2dx.cpp.ReadingBird;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;

import edu.cmu.pocketsphinx.Assets;
import edu.cmu.pocketsphinx.Decoder;
import edu.cmu.pocketsphinx.Hypothesis;

public class SpeechRecognition {
    public static final String PATH_SDCARD = Environment.getExternalStorageDirectory().getAbsolutePath();
    public static final String PATH_ROOT = PATH_SDCARD + File.separator + "Speech";
    private static final String RECORD_FILE_NAME = "speech.pcm";

    private static final String TAG = "SpeechRecognition";
    private static final String PHONE_SEARCH = "phones";

    private File mAssetDir = null;
    private Recognizer mSpeechRecognizer;
    private Decoder mDecoder;
    private String mOriginalPhone;
    private RecognizerListener mRecognitionListener = new RecognizerListener() {
        public void onBeginningOfSpeech() {
            Log.i(TAG, "onBeginningOfSpeech");
        }

        public void onEndOfSpeech() {
            Log.i(TAG, "onEndOfSpeech");
        }

        public void onVolume(int volume) {
            Log.i(TAG, "volume : " + volume);
            onRecordVolume(volume);
        }

        public void onResult(Hypothesis var1) {
            Log.i(TAG, "onResult : " + mOriginalPhone);

            if (var1 == null) {
                onRecordScore(0);
                return;
            }

            if (mOriginalPhone.isEmpty()) {
                onRecordScore(100);
                return;
            }

            String speech = var1.getHypstr();
            int score = (int) (PhonemesSimilarity.diceCoefficient(mOriginalPhone, speech) * 100);
            Log.i(TAG, "estimation : " + PhonemesSimilarity.getDebugMessage() + "\nscore : " + score);
            onRecordScore(score);
        }

        public void onError(Exception var1) {
            Log.i(TAG, "onError");
        }

        public void onTimeout() {
            Log.i(TAG, "onTimeout");
            mSpeechRecognizer.stop();
        }
    };

    public static native void onRecordVolume(int volume);

    public static native void onRecordScore(int score);

    public void setup(Activity activity) {
        makeRootFolder();
        new SetupTask(activity, this).execute();
    }

    public void cleanUp() {
        if (mSpeechRecognizer != null) {
            mSpeechRecognizer.cancel();
            mSpeechRecognizer.shutdown();
        }
    }

    public void startListening(int triggerVolume, int silentVolume, String phone) {
        if (mSpeechRecognizer != null) {
            final String recordFilePath = PATH_ROOT + File.separator + RECORD_FILE_NAME;
            mOriginalPhone = phone;
            mSpeechRecognizer.startListening(PHONE_SEARCH, recordFilePath, triggerVolume, silentVolume);
        }
    }

    public void stopListeningAndRecognition() {
        if (mSpeechRecognizer != null) {
            mSpeechRecognizer.stop();
        }
    }

    public void pauseListeningAndRecognition() {
        if (mSpeechRecognizer != null) {
            mSpeechRecognizer.onPause();
        }
    }

    public void resumeListeningAndRecognition() {
        if (mSpeechRecognizer != null) {
            mSpeechRecognizer.onResume();
        }
    }

    public String getSpeechRecordFilePath() {
        return PATH_ROOT + File.separator + RECORD_FILE_NAME;
    }

    private void makeRootFolder() {
        File rootFolder = new File(PATH_ROOT);
        if (rootFolder.exists() == false) {
            rootFolder.mkdirs();
        }
    }

    private void setupDecoder() {
        try {
            mSpeechRecognizer = RecognizerSetup.defaultSetup()
                    .setSampleRate(16000)
                    .setAcousticModel(new File(mAssetDir, "en-us-ptm"))
                    .getRecognizer();

            File phoneticModel = new File(mAssetDir, "en-phone.dmp");
            mSpeechRecognizer.addAllphoneSearch(PHONE_SEARCH, phoneticModel);
            mSpeechRecognizer.addListener(mRecognitionListener);

            mDecoder = mSpeechRecognizer.getDecoder();
            mDecoder.setSearch(PHONE_SEARCH);

        } catch (Exception e) {
            Log.e(TAG, "" + e);
        }
    }

    private static class SetupTask extends AsyncTask<Void, Void, Exception> {
        WeakReference<Activity> mActivity;
        WeakReference<SpeechRecognition> mSpeechRecognition;

        SetupTask(Activity activity, SpeechRecognition speechRecognition) {
            mActivity = new WeakReference<>(activity);
            mSpeechRecognition = new WeakReference<>(speechRecognition);
        }

        @Override
        protected Exception doInBackground(Void... params) {
            try {
                Assets assets = new Assets(mActivity.get());
                mSpeechRecognition.get().mAssetDir = assets.syncAssets();
                mSpeechRecognition.get().setupDecoder();
            } catch (IOException e) {
                return e;
            }
            return null;
        }

        @Override
        protected void onPostExecute(Exception result) {
            if (result != null) {
                Log.d(TAG, "result : " + result.getMessage());

            } else {
                Log.d(TAG, "success");
            }
        }
    }
}
