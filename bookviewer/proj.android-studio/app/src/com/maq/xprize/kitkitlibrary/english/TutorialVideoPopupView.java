package com.maq.xprize.kitkitlibrary.english;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import com.maq.xprize.booktest.R;

import java.io.IOException;

/**
 * Created by ingtellect on 9/8/17.
 */

public class TutorialVideoPopupView extends RelativeLayout implements SurfaceHolder.Callback, /*MediaPlayer.OnPreparedListener, */ VideoControllerView.MediaPlayerControl {
    SurfaceView videoSurface;
    MediaPlayer player;
    boolean isPaused;
    int currentPosition;


    VideoControllerView controller;

    public TutorialVideoPopupView(Context context) {
        super(context);
        init(context);
    }

    public TutorialVideoPopupView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public TutorialVideoPopupView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(context);
    }


    public void init(Context context) {
        View v = inflate(context, R.layout.tutorial_video_popup, null);
        addView(v);

        controller = new VideoControllerView(getContext());
        player = new MediaPlayer();
        controller.setMediaPlayer(this);
        controller.setAnchorView((FrameLayout) findViewById(R.id.videoSurfaceContainer));

        videoSurface = findViewById(R.id.videoSurface);
        SurfaceHolder videoHolder = videoSurface.getHolder();
        videoHolder.addCallback(this);

        player.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {

                return false;
            }
        });


        videoSurface.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (controller.isShowing()) {
                    controller.hide();
                } else {
                    controller.show();
                }

                return false;
            }
        });
    }

    public void setVideo(String filename) {

        try {

            player.reset();
            player.setAudioStreamType(AudioManager.STREAM_MUSIC);


            AssetFileDescriptor fd = getContext().getAssets().openFd(filename);
            player.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());

            player.prepare();


        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (IllegalStateException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if (player != null) {
            player.setDisplay(holder);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

        if (player != null) {

        }
    }
    // End SurfaceHolder.Callback

    // Implement MediaPlayer.OnPreparedListener
//    @Override
//    public void onPrepared(MediaPlayer mp) {
//        controller.setMediaPlayer(this);
//        controller.setAnchorView((FrameLayout) findViewById(R.id.videoSurfaceContainer));
//        player.start();
//    }
    // End MediaPlayer.OnPreparedListener

    // Implement VideoMediaController.MediaPlayerControl
    @Override
    public boolean canPause() {
        return true;
    }

    @Override
    public boolean canSeekBackward() {
        return true;
    }

    @Override
    public boolean canSeekForward() {
        return true;
    }

    @Override
    public int getBufferPercentage() {
        return 0;
    }

    @Override
    public int getCurrentPosition() {
        return player.getCurrentPosition();
    }

    @Override
    public int getDuration() {
        return player.getDuration();
    }

    @Override
    public boolean isPlaying() {
        return player.isPlaying();
    }

    @Override
    public void pause() {
        isPaused = true;
        currentPosition = player.getCurrentPosition();
        player.pause();
        ((Activity) getContext()).getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

    }

    @Override
    public void seekTo(int i) {
        player.seekTo(i);
    }

    @Override
    public void start() {
        //((Activity)getContext()).getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        if (isPaused) {
            if (currentPosition < 0) currentPosition = 0;
            if (currentPosition > player.getDuration() - 10)
                currentPosition = player.getDuration() - 10;
            player.seekTo(currentPosition);
            isPaused = false;
        }
//        player.start();

    }


    @Override
    public boolean isFullScreen() {
        return false;
    }

    @Override
    public void toggleFullScreen() {

    }
    // End VideoMediaController.MediaPlayerControl

    @Override
    public void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        if (player != null) {
            player.release();
        }
        player = null;

    }

    public MediaPlayer getPlayer() {
        return player;
    }

}
