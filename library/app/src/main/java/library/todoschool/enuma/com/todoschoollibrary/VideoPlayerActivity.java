package library.todoschool.enuma.com.todoschoollibrary;

import android.animation.Animator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.database.ContentObserver;
import android.graphics.drawable.Drawable;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;


public class VideoPlayerActivity extends KitKitLoggerActivity implements SurfaceHolder.Callback, /*MediaPlayer.OnPreparedListener, */ VideoControllerView.MediaPlayerControl {

    SurfaceView videoSurface;
    MediaPlayer player;
    boolean isPaused;
    int currentPosition;


    VideoControllerView controller;
    SeekBar volumeSeekbar;
    AudioManager audioManager;
    SettingsContentObserver mSettingsContentObserver;
    LinearLayout mLinearLayout, nextLinearLayout;
    ValueAnimator mAnimator;
    ArrayList<MainActivity.VideoData> videoList;
    int curVideoIndex;
    String libPath;
    String libAssetPath;
    ScrollView scrollView;
    MainActivity.VideoData currentVideo;


    public class SettingsContentObserver extends ContentObserver {
        int previousVolume;
        Context context;
        SeekBar seekbar;

        public SettingsContentObserver(Context c, Handler handler, SeekBar seekBar) {
            super(handler);
            context=c;

            AudioManager audio = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            previousVolume = audio.getStreamVolume(AudioManager.STREAM_MUSIC);
            seekbar = seekBar;
        }

        @Override
        public boolean deliverSelfNotifications() {
            return super.deliverSelfNotifications();
        }

        @Override
        public void onChange(boolean selfChange) {
            super.onChange(selfChange);

            AudioManager audio = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            int currentVolume = audio.getStreamVolume(AudioManager.STREAM_MUSIC);
            seekbar.setProgress(currentVolume);
            int delta=previousVolume-currentVolume;

            if(delta>0)
            {
                //Logger.d("Decreased");
                Log.d("onChange","Decreased");
                previousVolume=currentVolume;
            }
            else if(delta<0)
            {
                //Logger.d("Increased");
                Log.d("onChange","Increased");
                previousVolume=currentVolume;
            }
        }
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Util.hideSystemUI(this);

        setContentView(R.layout.activity_video_player);


        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });

        scrollView = (ScrollView)findViewById(R.id.scrollView);
        scrollView.setVisibility(View.GONE);

        Button closeButton = (Button)findViewById(R.id.button_close);
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scrollView.setVisibility(View.GONE);
            }
        });

        /*
        expandButton.setText(R.string.button_more);
        expandButton.setCompoundDrawablesWithIntrinsicBounds(getDrawable(R.drawable.libray_icon_more),null,null,null);

        expandButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Button btn = (Button)v;
                scrollView.setVisibility(View.VISIBLE);
//                if (mLinearLayout.getVisibility()==View.GONE){
//                    expand();
//
//                    btn.setText(R.string.button_less);
//                    btn.setCompoundDrawablesWithIntrinsicBounds(getDrawable(R.drawable.libray_icon_less),null,null,null);
//                }else{
//                    collapse();
//                    btn.setText(R.string.button_more);
//                    btn.setCompoundDrawablesWithIntrinsicBounds(getDrawable(R.drawable.libray_icon_more),null,null,null);
//                }
            }
        });
*/

        player = new MediaPlayer();
        controller = new VideoControllerView(this);
        controller.setMediaPlayer(this);
        controller.setAnchorView((FrameLayout) findViewById(R.id.videoSurfaceContainer));
        initControls();

        videoSurface = (SurfaceView) findViewById(R.id.videoSurface);
        SurfaceHolder videoHolder = videoSurface.getHolder();
        videoHolder.addCallback(this);



        isPaused = true;
        currentPosition = 0;

        Bundle bundle = getIntent().getExtras();
        videoList = (ArrayList< MainActivity.VideoData>)bundle.getSerializable("videoArray");
        libPath = bundle.getString("libPath");
        libAssetPath = bundle.getString("libAssetPath");
        curVideoIndex = bundle.getInt("currentVideoIndex");


        player.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {

                return false;
            }
        });




        videoSurface.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if(controller.isShowing()) {
                    controller.hide();
                }
                else {
                    controller.show();
                }

                return false;
            }
        });


//        mLinearLayout = (LinearLayout)findViewById(R.id.expandable_layout);
//        mLinearLayout.getViewTreeObserver().addOnPreDrawListener(
//                new ViewTreeObserver.OnPreDrawListener() {
//
//                    @Override
//                    public boolean onPreDraw() {
//                        Log.d("videoactivity","predraw");
//                        mLinearLayout.getViewTreeObserver().removeOnPreDrawListener(this);
//                        mLinearLayout.setVisibility(View.GONE);
//
//                        final int widthSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
//                        final int heightSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
//                        Log.d("videoactivity","width spec : " + widthSpec + "height spec : "+heightSpec);
//                        mLinearLayout.measure(widthSpec, heightSpec);
//
//                        mAnimator = slideAnimator(0, mLinearLayout.getMeasuredHeight());
//                        return true;
//                    }
//                });


        mSettingsContentObserver = new SettingsContentObserver(this,new Handler(), volumeSeekbar);
        getApplicationContext().getContentResolver().registerContentObserver(android.provider.Settings.System.CONTENT_URI, true, mSettingsContentObserver );

    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();



        try {
            MainActivity.VideoData curVideo = videoList.get(curVideoIndex);
            setCurrentVideo(curVideo);
            setNextVideo();
        }
        catch (ArrayIndexOutOfBoundsException ex) {

        }

        if (isPaused) {
            start();
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();

        try {
            KitKitLogger logger = ((LibraryApplication)getApplication()).getLogger();
            logger.logEvent("VideoPlayerActivity","exit_video",currentVideo.title,player.getCurrentPosition());

        }
        catch (NullPointerException ne) {
            KitKitLogger logger = ((LibraryApplication)getApplication()).getLogger();
            logger.logEvent("VideoPlayerActivity","exit_video","",0);
        }

        if (player!=null) {
            player.release();
        }
        player = null;


    }

    @Override
    protected void onPause() {
        super.onPause();
        if(isPlaying()) {
            pause();
        }
    }


    private void setCurrentVideo(MainActivity.VideoData video)
    {
//        String videoPath = bundle.getString("videoPath");
//        String videoTitle = bundle.getString("videoTitle");
//        String lyrics = bundle.getString("videoLyrics");
//        String info = bundle.getString("videoInfo");


        currentVideo = video;
        String videoPath = libPath + File.separator + currentVideo.filename;
        //Log.d("VideoPlayerActivity","cur video path : " + videoPath);
        String videoTitle = currentVideo.title;
        String lyrics = currentVideo.lyrics;
        String info = currentVideo.info;

        TextView titleTV = (TextView) findViewById(R.id.toolbar_title);
        titleTV.setText(videoTitle);

        TextView expandableTitleTV = (TextView)findViewById(R.id.video_title_expandable);
        expandableTitleTV.setText(videoTitle);

        TextView lyricsTV = (TextView) findViewById(R.id.video_lyrics_expandable);
        lyricsTV.setText(lyrics);

        TextView infoTV = (TextView) findViewById(R.id.video_info_expandable);
        infoTV.setText(info);

        try {

            player.reset();
            player.setAudioStreamType(AudioManager.STREAM_MUSIC);

            if (MainActivity.useExternalData) {
                player.setDataSource(videoPath);
            }
            else {
//                int videoInt = getResources().getIdentifier(video.filename,"raw",getPackageName());
//                player.setDataSource(this, Uri.parse("android.resource://"+getPackageName()+"/"+videoInt));
                //player.setDataSource(getAssets().openFd(video.filename));
                //player.setDataSource(getAssets().openFd(video.filename).getFileDescriptor());
                //player.setDataSource(getAssets().openFd(video.filename));

                try {
                    AssetFileDescriptor fd = getAssets().openFd(video.filename);
                    player.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
                }
                catch (FileNotFoundException fe) {
                    StringTokenizer tokenizer = new StringTokenizer(video.filename,".");
                    String videoFilenameWithoutExtention = (String)tokenizer.nextElement();

                    String uriString = "android.resource://" + getPackageName() + "/raw/" + videoFilenameWithoutExtention;
                    player.setDataSource(getApplication(), Uri.parse(uriString));

                }

            }

//            player.setOnPreparedListener(this);
  //          player.prepareAsync();

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

    private void setNextVideo()
    {

        nextLinearLayout = (LinearLayout)findViewById(R.id.next_video_layout);
        nextLinearLayout.setVisibility(View.GONE);

        try {

            final MainActivity.VideoData nextVideo = videoList.get(curVideoIndex+1);


            TextView nextTitleTV = (TextView)findViewById(R.id.next_video_title_textView);
            nextTitleTV.setText(nextVideo.title);

            ImageButton nextVideoButton = (ImageButton)findViewById(R.id.next_video_thumbnail);
            Drawable thumbnail;
            if (MainActivity.useExternalData) {
                String thumbnailPath = libAssetPath + File.separator + nextVideo.thumbnail;
                thumbnail = Drawable.createFromPath(thumbnailPath);
            }
            else {
                thumbnail = Drawable.createFromStream(getAssets().open(nextVideo.thumbnail), null);

            }
            nextVideoButton.setImageDrawable(thumbnail);

            nextVideoButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    curVideoIndex++;
                    setCurrentVideo(nextVideo);
                    setNextVideo();
                    start();
                }
            });


            player.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mediaPlayer) {
                    if (!mediaPlayer.isPlaying()) {
                        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

                        nextLinearLayout.setVisibility(View.VISIBLE);
                        Animation fadeInAnimation = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.fade_in);
                        nextLinearLayout.startAnimation(fadeInAnimation);
                    }
                }
            });

            Button cancelButton = (Button)findViewById(R.id.button_cancel);
            cancelButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    onBackPressed();
                }
            });
        }
        catch (IndexOutOfBoundsException ex) {
            player.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                // do nothing when there is no next video
                public void onCompletion(MediaPlayer mediaPlayer) {
                }
            });

        }
        catch (IOException ex) {

        }


    }



    private void initControls()
    {
        try
        {
            volumeSeekbar = controller.getVolumeSeekBar();
            audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
            volumeSeekbar.setMax(audioManager
                    .getStreamMaxVolume(AudioManager.STREAM_MUSIC));
            volumeSeekbar.setProgress(audioManager
                    .getStreamVolume(AudioManager.STREAM_MUSIC));


            volumeSeekbar.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
            {
                @Override
                public void onStopTrackingTouch(SeekBar arg0)
                {
                }

                @Override
                public void onStartTrackingTouch(SeekBar arg0)
                {
                }

                @Override
                public void onProgressChanged(SeekBar arg0, int progress, boolean arg2)
                {
                    int maxVolume = volumeSeekbar.getMax();
                    float log1=(float)(Math.log(maxVolume-progress)/Math.log(maxVolume));
                    //player.setVolume(1-log1, 1-log1);

                    audioManager.setStreamVolume(AudioManager.STREAM_MUSIC,
                            progress, 0);
                }
            });
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    private void expand() {
        //set Visible
        mLinearLayout.setVisibility(View.VISIBLE);

		/* Remove and used in preDrawListener
		final int widthSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
		final int heightSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
		mLinearLayout.measure(widthSpec, heightSpec);
		mAnimator = slideAnimator(0, mLinearLayout.getMeasuredHeight());
		*/

        mAnimator.start();
    }

    private void collapse() {
        int finalHeight = mLinearLayout.getHeight();

        ValueAnimator mAnimator = slideAnimator(finalHeight, 0);

        mAnimator.addListener(new Animator.AnimatorListener() {
            @Override
            public void onAnimationEnd(Animator animator) {
                //Height=0, but it set visibility to GONE
                mLinearLayout.setVisibility(View.GONE);
            }

            @Override
            public void onAnimationStart(Animator animator) {
            }

            @Override
            public void onAnimationCancel(Animator animator) {
            }

            @Override
            public void onAnimationRepeat(Animator animator) {
            }
        });
        mAnimator.start();
    }


    private ValueAnimator slideAnimator(int start, int end) {

        ValueAnimator animator = ValueAnimator.ofInt(start, end);


        animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator valueAnimator) {
                //Update Height
                int value = (Integer) valueAnimator.getAnimatedValue();

                ViewGroup.LayoutParams layoutParams = mLinearLayout.getLayoutParams();
                layoutParams.height = value;
                mLinearLayout.setLayoutParams(layoutParams);
            }
        });
        return animator;
    }


//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        controller.show();
//        return false;
//    }

    // Implement SurfaceHolder.Callback
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if (player!=null) {
            player.setDisplay(holder);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

        if (player!=null) {

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
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        KitKitLogger logger = ((LibraryApplication)getApplication()).getLogger();
        logger.logEvent("VideoPlayerActivity","pause_video",currentVideo.title,player.getCurrentPosition());

    }

    @Override
    public void seekTo(int i) {
        player.seekTo(i);
    }

    @Override
    public void start() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        if (isPaused) {
            if (currentPosition<0) currentPosition = 0;
            if (currentPosition>player.getDuration()-10) currentPosition = player.getDuration()-10;
            player.seekTo(currentPosition);
            isPaused = false;
        }
        player.start();

        KitKitLogger logger = ((LibraryApplication)getApplication()).getLogger();
        logger.logEvent("VideoPlayerActivity","start_video",currentVideo.title,0);

    }


    @Override
    public boolean isFullScreen() {
        return false;
    }

    @Override
    public void toggleFullScreen() {

    }
    // End VideoMediaController.MediaPlayerControl

}