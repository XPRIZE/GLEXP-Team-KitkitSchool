package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

import com.enuma.kitkitlogger.KitKitLoggerActivity;

/**
 * Created by yshong on 2018. 4. 13..
 */

public class VideoPlayerActivity extends KitKitLoggerActivity implements SurfaceHolder.Callback{
    private MediaPlayer mMediaPlayer;
    private SurfaceView mVideoView;
    private int mCurrentPosition;
    private boolean mbPause;
    private String mVideoFileName;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        Util.hideSystemUI(this);
        setContentView(R.layout.activity_video);

        mVideoView = (SurfaceView)findViewById(R.id.videoSurface);
        mMediaPlayer = new MediaPlayer();
        mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mediaPlayer, int i, int i1) {
                finish();
                overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
                return false;
            }
        });

        mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer) {
                finish();
                overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
            }
        });

        mCurrentPosition = 0;
        mbPause = false;

        mVideoView.getHolder().addCallback(this);


        String video = "";
        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            video = bundle.getString("video", "");
        }

        if (video.equals("writing_board")) {
            if (getAppLanguage().equals("sw-TZ")) {
                mVideoFileName = "tutorial_writingboard_sw.mp4";
            } else {
                mVideoFileName = "tutorial_writingboard_en.mp4";
            }

        } else {
            if (getAppLanguage().equals("sw-TZ")) {
                mVideoFileName = "sw_vdo_tut_welcome.m4v";

            } else {
                mVideoFileName = "en_vdo_tut_welcome.m4v";
            }
        }

        playVideo(mVideoFileName);
    }

    @Override
    protected void onPause() {
        super.onPause();
        try {
            mbPause = true;
            mCurrentPosition = mMediaPlayer.getCurrentPosition();
            mMediaPlayer.pause();
            if (mCurrentPosition > 10) {
                mCurrentPosition -= 10;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mbPause == true) {
            playVideo(mVideoFileName);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (mMediaPlayer != null) {
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    @Override
    public void onBackPressed() {
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        if (mMediaPlayer != null) {
            mMediaPlayer.setDisplay(surfaceHolder);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
    }

    private void playVideo(String fileName) {
        try {
            mMediaPlayer.reset();
            mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);

            AssetFileDescriptor fd = getAssets().openFd(fileName);
            mMediaPlayer.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
            mMediaPlayer.prepare();

            if (mbPause) {
                mMediaPlayer.seekTo(mCurrentPosition);
            }

            mMediaPlayer.start();

        } catch (Exception e) {
            Log.e("myLog", "" + e);
            e.printStackTrace();
            finish();
        }
    }
}
