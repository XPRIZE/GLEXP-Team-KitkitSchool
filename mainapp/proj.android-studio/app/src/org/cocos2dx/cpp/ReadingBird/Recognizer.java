package org.cocos2dx.cpp.ReadingBird;

import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.Looper;

import org.cocos2dx.cpp.ReadingBird.fftpack.RealDoubleFFT;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;

import edu.cmu.pocketsphinx.Config;
import edu.cmu.pocketsphinx.Decoder;
import edu.cmu.pocketsphinx.FsgModel;
import edu.cmu.pocketsphinx.Hypothesis;

public class Recognizer {
    private static final float BUFFER_SIZE_SECONDS = 0.4F;
    private static int mTriggerVolume;
    private static int mSilentVolume;
    private final Decoder decoder;
    private final int SAMPLE_RATE = 16000;
    private final AudioRecord recorder;
    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private final Collection<RecognizerListener> listeners = new HashSet();
    private int bufferSize;
    private Thread recognizerThread;
    private boolean mbCheckVolume;
    private String mPCMFilePath;
    private boolean mbStop = false;
    private boolean mbPause = false;
    private RealDoubleFFT mTransformer;

    protected Recognizer(Config config) throws IOException {
        this.decoder = new Decoder(config);
        this.bufferSize = Math.round((float) SAMPLE_RATE * 0.4F);
        mTransformer = new RealDoubleFFT(bufferSize);
        this.recorder = new AudioRecord(MediaRecorder.AudioSource.VOICE_RECOGNITION, SAMPLE_RATE, 16, 2, this.bufferSize * 2);
        this.mbPause = false;

//        if (NoiseSuppressor.isAvailable() == true) {
//            NoiseSuppressor noiseSuppressor = NoiseSuppressor.create(this.recorder.getAudioSessionId());
//            if (noiseSuppressor != null) {
//                noiseSuppressor.setEnabled(true);
//                Log.i("NoiseSuppressor.setEnabled(true)");
//            } else {
//                Log.i("NoiseSuppressor.isAvailable() : false");
//            }
//        }
//
//        if (AcousticEchoCanceler.isAvailable() == true) {
//            AcousticEchoCanceler auAcousticEchoCanceler = AcousticEchoCanceler.create(this.recorder.getAudioSessionId());
//            if (auAcousticEchoCanceler != null) {
//                auAcousticEchoCanceler.setEnabled(true);
//                Log.i("AcousticEchoCanceler.setEnabled(true)");
//            } else {
//                Log.i("AcousticEchoCanceler.isAvailable() : false");
//            }
//        }

        if (this.recorder.getState() == 0) {
            this.recorder.release();
            throw new IOException("Failed to initialize recorder. Microphone might be already in use.");
        }
    }

    public static int getVolume(short[] buffer, int bufferSize) {
        int max = 0;
        for (int i = 0; i < bufferSize; ++i) {
            if (max < Math.abs(buffer[i])) {
                max = Math.abs(buffer[i]);
            }
        }

        return (int) Math.sqrt(max);
    }

    public static int getAmplificationVolume(double maxVolume, int curVolume) {
        double val = 1.0 - ((maxVolume - curVolume) / maxVolume);
        double amplification = val * val;
        return (int) (amplification * maxVolume);
    }

    public static int getVolume(byte[] buffer, int bufferSize) {
        short[] shortBuffer = new short[bufferSize / 2];
        for (int i = 0; i < shortBuffer.length; ++i) {
            shortBuffer[i] = (short) (buffer[i * 2] | (buffer[i * 2 + 1] << 8));
        }

        return getVolume(shortBuffer, shortBuffer.length);
    }

    public void addListener(RecognizerListener listener) {
        Collection var2 = this.listeners;
        synchronized (this.listeners) {
            this.listeners.add(listener);
        }
    }

    public void removeListener(RecognizerListener listener) {
        Collection var2 = this.listeners;
        synchronized (this.listeners) {
            this.listeners.remove(listener);
        }
    }

    public boolean startListening(String searchName, String wavFilePath, int triggerVolume, int silentVolume) {
        boolean result = false;
        if (null != this.recognizerThread) {
            return result;
        } else {
            try {
                Log.i(String.format("Start recognition \"%s\"", new Object[]{searchName}));
                this.decoder.setSearch(searchName);
                this.recognizerThread = new RecordThread();
                this.recognizerThread.start();
                this.mPCMFilePath = wavFilePath;
                this.mTriggerVolume = triggerVolume;
                this.mSilentVolume = silentVolume;
                this.mbCheckVolume = true;
                this.mbStop = false;
                this.mbPause = false;
                File file = new File(this.mPCMFilePath);
                if (file.exists() == true) {
                    file.delete();
                }

                result = true;
            } catch (Exception e) {
                result = false;
            }
        }

        return result;
    }

    public boolean startRecognize() {
        if (this.recognizerThread == null) {
            return false;
        }

        return true;
    }

    private boolean stopRecognizerThread() {
        if (null == this.recognizerThread) {
            return false;
        } else {
            mbStop = true;
            this.recognizerThread = null;
            return true;
        }
    }

    public boolean stop() {
        boolean result = this.stopRecognizerThread();
        return result;
    }

    public boolean cancel() {
        boolean result = this.stopRecognizerThread();
        if (result) {
            Log.i("Cancel recognition");
        }

        return result;
    }

    public Decoder getDecoder() {
        return this.decoder;
    }

    public void shutdown() {
        this.recorder.release();
    }

    public String getSearchName() {
        return this.decoder.getSearch();
    }

    public void addFsgSearch(String searchName, FsgModel fsgModel) {
        this.decoder.setFsg(searchName, fsgModel);
    }

    public void addGrammarSearch(String name, File file) {
        Log.i(String.format("Load JSGF %s", new Object[]{file}));
        this.decoder.setJsgfFile(name, file.getPath());
    }

    public void addGrammarSearch(String name, String jsgfString) {
        this.decoder.setJsgfString(name, jsgfString);
    }

    public void addNgramSearch(String name, File file) {
        Log.i(String.format("Load N-gram model %s", new Object[]{file}));
        this.decoder.setLmFile(name, file.getPath());
    }

    public void addKeyphraseSearch(String name, String phrase) {
        this.decoder.setKeyphrase(name, phrase);
    }

    public void addKeywordSearch(String name, File file) {
        this.decoder.setKws(name, file.getPath());
    }

    public void addAllphoneSearch(String name, File file) {
        this.decoder.setAllphoneFile(name, file.getPath());
    }

    public void onPause() {
        mbPause = true;
    }

    public void onResume() {
        Log.i("");
        mbPause = false;
    }

    private class TimeoutEvent extends Recognizer.RecognitionEvent {
        private TimeoutEvent() {
            super();
        }

        protected void execute(RecognizerListener listener) {
            listener.onTimeout();
        }
    }

    private class OnErrorEvent extends Recognizer.RecognitionEvent {
        private final Exception exception;

        OnErrorEvent(Exception exception) {
            super();
            this.exception = exception;
        }

        protected void execute(RecognizerListener listener) {
            listener.onError(this.exception);
        }
    }

    private class ResultEvent extends Recognizer.RecognitionEvent {
        protected final Hypothesis hypothesis;
        private final boolean finalResult;
        private final int volume;

        ResultEvent(Hypothesis hypothesis, boolean finalResult, int volume) {
            super();
            this.hypothesis = hypothesis;
            this.finalResult = finalResult;
            this.volume = volume;
        }

        protected void execute(RecognizerListener listener) {
            if (this.finalResult) {
                listener.onResult(this.hypothesis);
            } else {
                listener.onVolume(volume);
//                listener.onPartialResult(this.hypothesis, volume);
            }

        }
    }

    private class InSpeechChangeEvent extends Recognizer.RecognitionEvent {
        private final boolean state;

        InSpeechChangeEvent(boolean state) {
            super();
            this.state = state;
        }

        protected void execute(RecognizerListener listener) {
            if (this.state) {
                listener.onBeginningOfSpeech();
            } else {
                listener.onEndOfSpeech();
            }

        }
    }

    private abstract class RecognitionEvent implements Runnable {
        private RecognitionEvent() {
        }

        public void run() {
            RecognizerListener[] emptyArray = new RecognizerListener[0];
            RecognizerListener[] var2 = Recognizer.this.listeners.toArray(emptyArray);
            int var3 = var2.length;

            for (int var4 = 0; var4 < var3; ++var4) {
                RecognizerListener listener = var2[var4];
                this.execute(listener);
            }

        }

        protected abstract void execute(RecognizerListener var1);
    }

    private final class RecognitionThread extends Thread {
        private boolean mbStop = false;
        private boolean mbRecognition = false;
        private ArrayList<short[]> mBuffers = new ArrayList<>();
        private ArrayList<Integer> mVolumes = new ArrayList<>();

        public void run() {

            try {
                Log.d("Starting decoding");
                Recognizer.this.decoder.startUtt();
                int processCount = 0;

                while (!mbStop) {
                    if (mBuffers.size() > 0) {
                        short[] buffer = popBuffer();
                        int volume = popVolume();

                        if (volume > mSilentVolume) {
                            mbRecognition = true;
                            Recognizer.this.decoder.processRaw(buffer, (long) buffer.length, false, false);
                        }

                        ++processCount;

                    } else {
                        Thread.sleep(10);
                    }
                }
                Recognizer.this.decoder.endUtt();
                Log.i("process count : " + processCount + ", remain process : " + mBuffers.size());

                if (mbRecognition && !mbCheckVolume) {
                    Hypothesis hypothesis = Recognizer.this.decoder.hyp();
                    Recognizer.this.mainHandler.post(new Recognizer.ResultEvent(hypothesis, true, 0));

                } else {
                    Recognizer.this.mainHandler.post(new Recognizer.ResultEvent(null, true, 0));
                }

            } catch (Exception e) {
                Log.e("" + e);
                Recognizer.this.mainHandler.post(Recognizer.this.new OnErrorEvent(e));
            }
        }

        synchronized public void addData(short[] buffer, int volume) {
            mBuffers.add(buffer);
            mVolumes.add(volume);
        }

        synchronized private short[] popBuffer() {
            if (mBuffers.size() > 0) {
                return mBuffers.remove(0);
            }

            return null;
        }

        synchronized private int popVolume() {
            if (mVolumes.size() > 0) {
                return mVolumes.remove(0);
            }

            return 0;
        }

        synchronized private void setStop() {
            mbStop = true;
        }
    }

    private final class RecordThread extends Thread {
        private RecognitionThread mRecognitionThread;

        public void run() {
            Recognizer.this.recorder.startRecording();
            if (Recognizer.this.recorder.getRecordingState() == 1) {
                Recognizer.this.recorder.stop();
                IOException ioe = new IOException("Failed to start recording. Microphone might be already in use.");
                Recognizer.this.mainHandler.post(Recognizer.this.new OnErrorEvent(ioe));
            } else {
                mRecognitionThread = new RecognitionThread();
                mRecognitionThread.start();

                short[] buffer = new short[Recognizer.this.bufferSize];
                double[] toTransform = new double[Recognizer.this.bufferSize];

                ArrayList<short[]> arrBuffers = new ArrayList<>();
                ArrayList<Integer> arrVolume = new ArrayList<>();

                try {
                    Log.i("start");
                    do {
                        int nread = Recognizer.this.recorder.read(buffer, 0, buffer.length);

                        if (-1 == nread) {
                            throw new RuntimeException("error reading audio buffer");
                        }


                        if (nread > 0) {
                            if (mbPause) {
                                Log.i("Recognizer mbPause");
                                continue;
                            }

                            for (int i = 0; i < Recognizer.this.bufferSize && i < nread; i++) {
                                toTransform[i] = (double) buffer[i] / 32768.0; // signed
                            }

                            mTransformer.ft(toTransform);
                            int volume = getVolume(buffer, nread);

                            Log.i("volume : " + volume);
                            double maxValue = 0;
                            int maxIndex = -1;
                            for (int i = 0; i < toTransform.length; ++i) {
                                if (Math.abs(toTransform[i]) > maxValue) {
                                    maxValue = Math.abs(toTransform[i]);
                                    maxIndex = i;
                                }
                            }

                            if (mbCheckVolume) {
                                Log.i("Recognizer record " + maxIndex + ", value : " + maxValue + ", " + ((SAMPLE_RATE / 2.0f / Recognizer.this.bufferSize) * maxIndex) + "Hz");
                                Log.i("-> Recognizer record " + 0 + ", value : " + Math.abs(toTransform[0]) + ", " + ((SAMPLE_RATE / 2.0f / Recognizer.this.bufferSize) * 0) + "Hz");
                                if (mTriggerVolume <= volume && Math.abs(toTransform[0]) < 10) {
                                    mbCheckVolume = false;
                                }

                            }

                            if (!mbCheckVolume) {
                                Log.i("~~~ Recognizer record " + maxIndex + ", value : " + maxValue + ", " + ((SAMPLE_RATE / 2.0f / Recognizer.this.bufferSize) * maxIndex) + "Hz");
                                Recognizer.this.mainHandler.post(Recognizer.this.new ResultEvent(null, false, volume));
                            }

                            if (Recognizer.this.mPCMFilePath != null && !mbCheckVolume) {
                                arrBuffers.add(buffer.clone());
                                arrVolume.add(volume);
                                mRecognitionThread.addData(buffer.clone(), volume);
                            }
                        }
                    }
                    while (!Recognizer.this.mbStop);

                    Recognizer.this.recorder.stop();
                    Recognizer.this.mainHandler.removeCallbacksAndMessages(null);
                    mRecognitionThread.setStop();

                    if (Recognizer.this.mPCMFilePath != null && !mbCheckVolume && arrBuffers.size() > 0) {
                        ArrayList<short[]> writeBuffers = new ArrayList<>();
                        for (int i = 0; i < arrBuffers.size(); ++i) {
                            writeBuffers.add((arrBuffers.get(i)));
                        }

                        int maxVolume = 0;
                        for (int volume : arrVolume) {
                            if (volume > maxVolume) {
                                maxVolume = volume;
                            }
                        }

                        int removeCount = 0;
                        for (int i = writeBuffers.size() - 1; i >= 0; --i) {
                            if (getAmplificationVolume(maxVolume, arrVolume.get(i)) <= mSilentVolume) {
                                Log.i(i + " mSilentVolume : " + mSilentVolume);
                                Log.i(i + " maxVolume : " + maxVolume);
                                Log.i(i + " getVolume : " + getVolume(writeBuffers.get(i), arrVolume.get(i)));
                                Log.i(i + " getAmplificationVolume : " + getAmplificationVolume(maxVolume, arrVolume.get(i)));
//                                writeBuffers.remove(i);
                                ++removeCount;

                            } else {
                                break;
                            }
                        }

                        for (int i = 0; i <= removeCount / 2; ++i) {
                            Log.i("remove");
                            writeBuffers.remove(writeBuffers.size() - 1);
                        }

                        writePCMFile(writeBuffers, Recognizer.this.mPCMFilePath);
                    }
                } catch (Exception e) {
                    if (Recognizer.this.mPCMFilePath != null) {
                        File file = new File(Recognizer.this.mPCMFilePath);
                        if (file.exists()) {
                            file.delete();
                        }
                    }

                    Log.e("" + e);
                }
            }
        }

        private boolean writePCMFile(ArrayList<short[]> buffers, String fileName) {
            if (buffers == null || buffers.size() == 0) {
                return false;
            }

            if (fileName == null || fileName.isEmpty()) {
                return false;
            }

            Log.i("writePCMFile");

            try {
                DataOutputStream dos = new DataOutputStream(
                        new BufferedOutputStream(new FileOutputStream(
                                fileName)));

                for (int i = 0; i < buffers.size(); i++) {
                    for (int j = 0; j < buffers.get(i).length; ++j) {
                        dos.writeShort(buffers.get(i)[j]);
                    }
                }

                dos.close();
            } catch (Throwable t) {
                Log.e("AudioRecord", "Recording Failed");
            }

            return true;
        }
    }
}
