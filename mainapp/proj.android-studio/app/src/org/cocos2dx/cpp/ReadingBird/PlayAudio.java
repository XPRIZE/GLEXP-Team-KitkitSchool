package org.cocos2dx.cpp.ReadingBird;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaPlayer;
import android.media.audiofx.Visualizer;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.FileInputStream;

public class PlayAudio {
    private final int SAMPLE_RATE = 16000;
    private float AUDIO_TRACK_PITCH = 1.2f;

    private Activity mActivity;
    private MediaPlayer mMediaPlayer;
    private AudioTrack mAudioTrack;
    private int mAudioTrackBufferSize;

    private Visualizer mVisualizerMediaPlayer;

    private String mFilePath;
    private Thread mThread;
    private boolean mbPlaying;
    private boolean mbPause;
    private int mCurrentPosition;
    private boolean mbExternalAudioFile = false;
    private String mExternalPath = "";
    private Visualizer.OnDataCaptureListener mOnDataCaptureListener = new Visualizer.OnDataCaptureListener() {
        @Override
        public void onWaveFormDataCapture(Visualizer visualizer, byte[] waveform, int samplingRate) {
        }

        @Override
        public void onFftDataCapture(Visualizer visualizer, byte[] fft, int samplingRate) {
            onPlayVolume(Recognizer.getVolume(fft, fft.length));
        }
    };

    public PlayAudio(Activity activity, boolean bExternalAudioFile, String externalPath) {
        mActivity = activity;
        mbExternalAudioFile = bExternalAudioFile;
        mExternalPath = externalPath;
    }

    public static native void onStop();

    public static native void onPlayVolume(int volume);

    public void playPCM(String filePath) {
        if (mThread != null) {
            return;
        }

        mFilePath = filePath;
        mCurrentPosition = 0;

        if (mAudioTrack == null) {
            int outfrequency = (int) (SAMPLE_RATE * AUDIO_TRACK_PITCH);
            int channelConfiguration = AudioFormat.CHANNEL_OUT_MONO;
            int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;

            mAudioTrackBufferSize = AudioTrack.getMinBufferSize(outfrequency,
                    channelConfiguration, audioEncoding);

            mAudioTrack = new AudioTrack(
                    AudioManager.STREAM_MUSIC, outfrequency,
                    channelConfiguration, audioEncoding, mAudioTrackBufferSize,
                    AudioTrack.MODE_STREAM);

//            LoudnessEnhancer enhancer = new LoudnessEnhancer(mAudioTrack.getAudioSessionId());
//            enhancer.setTargetGain(2500);
//            enhancer.setEnabled(true);
        }

        Log.i("play : " + mFilePath);

        mThread = new PlayThread();
        mThread.start();
    }

    public void play(String filePath) {
        mFilePath = filePath;
        mCurrentPosition = 0;

        if (mMediaPlayer == null) {
            mMediaPlayer = new MediaPlayer();
            mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    onStop();
                    mVisualizerMediaPlayer.setEnabled(false);
                }
            });

            mVisualizerMediaPlayer = new Visualizer(mMediaPlayer.getAudioSessionId());
            mVisualizerMediaPlayer.setCaptureSize(Visualizer.getCaptureSizeRange()[1]);
            mVisualizerMediaPlayer.setDataCaptureListener(mOnDataCaptureListener, Visualizer.getMaxCaptureRate() / 2, false, true);
        }

        Log.i("play : " + mFilePath);

        try {
            mMediaPlayer.reset();

            if (mbExternalAudioFile == false) {
                if (mFilePath.startsWith("localized") == true) {
                    AssetFileDescriptor assetFileDescriptor = mActivity.getAssets().openFd(mFilePath);
                    mMediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(), assetFileDescriptor.getStartOffset(), assetFileDescriptor.getLength());
                    assetFileDescriptor.close();

                } else {
                    mMediaPlayer.setDataSource(filePath);

                }
            } else {
                if (mFilePath.startsWith("localized") == true) {
                    mMediaPlayer.setDataSource(mExternalPath + filePath);

                } else {
                    mMediaPlayer.setDataSource(filePath);

                }
            }

            mMediaPlayer.prepare();
            mVisualizerMediaPlayer.setEnabled(true);
            mMediaPlayer.start();

        } catch (Exception e) {
            Log.e("" + e);
        }

    }

    public void stop() {
        mbPlaying = false;
        mThread = null;
        mCurrentPosition = 0;

        if (mMediaPlayer != null) {
            mMediaPlayer.isPlaying();
            mMediaPlayer.stop();
        }
    }

    public void pause() {
        mbPause = true;

        if (mMediaPlayer != null) {
            if (mMediaPlayer.isPlaying() == true) {
                mMediaPlayer.pause();
                mCurrentPosition = mMediaPlayer.getCurrentPosition();
            }
        }
    }

    public void resume() {
        mbPause = false;

        if (mMediaPlayer != null && mCurrentPosition != 0) {
            mMediaPlayer.seekTo(mCurrentPosition);
            mMediaPlayer.start();
        }
    }

    public void cleanUp() {
        stop();

        if (mMediaPlayer != null) {
            mMediaPlayer.release();
            mMediaPlayer = null;
        }

        if (mAudioTrack != null) {
            mAudioTrack.release();
            mAudioTrack = null;

        }

        if (mVisualizerMediaPlayer != null) {
            mVisualizerMediaPlayer.release();
            mVisualizerMediaPlayer = null;
        }

    }

    private final class PlayThread extends Thread {
        public void run() {
            mbPlaying = true;
            mbPause = false;

            short[] audioData = new short[mAudioTrackBufferSize / 4];

            try {
                DataInputStream findMaxDataInputStream = new DataInputStream(
                        new BufferedInputStream(new FileInputStream(
                                mFilePath)));

                int max = 0;
                while (findMaxDataInputStream.available() > 0) {
                    int value = findMaxDataInputStream.readShort();
                    if (value > max) {
                        max = value;
                    }
                }
                findMaxDataInputStream.close();
                double maxVolume = Math.sqrt(max);

                DataInputStream dataInputStream = new DataInputStream(
                        new BufferedInputStream(new FileInputStream(
                                mFilePath)));

                mAudioTrack.play();
                while (mbPlaying && dataInputStream.available() > 0) {
                    if (mbPause == false) {
                        int i = 0;
                        while (dataInputStream.available() > 0 && i < audioData.length) {
                            audioData[i] = dataInputStream.readShort();
                            i++;
                        }
                        mAudioTrack.write(audioData, 0, audioData.length);
                        int volume = Recognizer.getVolume(audioData, audioData.length);
                        onPlayVolume(Recognizer.getAmplificationVolume(maxVolume, volume));

                    } else {
                        Thread.sleep(100);
                    }
                }

                dataInputStream.close();

            } catch (Exception e) {
                Log.e(e + "");
            }

            try {
                mAudioTrack.stop();
                mbPlaying = false;
                mThread = null;
                onStop();

            } catch (Exception e) {
                Log.e(e + "");
            }

        }
    }
}
