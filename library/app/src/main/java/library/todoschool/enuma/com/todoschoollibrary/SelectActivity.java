package library.todoschool.enuma.com.todoschoollibrary;

import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;

import com.enuma.kitkitlogger.KitKitLoggerActivity;

/**
 * Created by ingtellect on 9/7/17.
 */

public class SelectActivity extends KitKitLoggerActivity {

    private String TAG = "SelectActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "onCreate()");
        hideSystemUI();

        setContentView(R.layout.activity_select);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });

    }
    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_FULLSCREEN
                |View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                |View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            hideSystemUI();
        }


    }

    public void onClickBook(View v) {
        Intent intent = new Intent(this, MainActivity.class);
        intent.putExtra("tab",1);
        startActivity(intent);
    }

    public void onClickVideo(View v) {
        Intent intent = new Intent(this, MainActivity.class);
        intent.putExtra("tab",0);
        startActivity(intent);

    }

    @Override
    public void onAttachedToWindow() {
        super.onAttachedToWindow();

/*
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        boolean showTutorial = prefs.getBoolean("Show_tutorial", true);
        if (showTutorial) {
            showTutorialVideo(null);
            SharedPreferences.Editor editor = prefs.edit();
            editor.putBoolean("Show_tutorial",false);
            editor.apply();
        }
*/
    }


    public void showTutorialVideo(View v) {
        VideoDialogFragment fragment = new VideoDialogFragment();
        fragment.show(getFragmentManager(),"video");
    }


    static public class VideoDialogFragment extends DialogFragment {

        TutorialVideoPopupView popupView;
        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {

            View view = inflater.inflate(R.layout.dialog_video, container, false);
            popupView = (TutorialVideoPopupView)view.findViewById(R.id.video_view);

            String filename = "How to use the tablet.mp4";
            popupView.setVideo(filename);

            ImageButton closeBtn = (ImageButton)view.findViewById(R.id.btn_close);
            closeBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    getDialog().dismiss();
                }
            });
            return view;
        }

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {

//            Dialog dialog = super.onCreateDialog(savedInstanceState);
            final Dialog dialog = new Dialog(getActivity(), R.style.DialogFullScreen);
            dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
            dialog.setCanceledOnTouchOutside(false);

            dialog.setOnShowListener(new DialogInterface.OnShowListener() {
                @Override
                public void onShow(DialogInterface dialogInterface) {
                    getActivity().getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);


                    popupView.start();
                    Dialog dialogObj = Dialog.class.cast(dialogInterface);
                    dialogObj.getWindow().getDecorView().setSystemUiVisibility(
                            getActivity().getWindow().getDecorView().getSystemUiVisibility());
                    dialogObj.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);

                    MediaPlayer player = popupView.getPlayer();

                    player.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                        @Override
                        public void onCompletion(MediaPlayer mediaPlayer) {
                            if (!mediaPlayer.isPlaying()) {
                                dialog.dismiss();
                            }
                        }
                    });

                }
            });
            dialog.getWindow().setLayout(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT);
            dialog.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
            dialog.getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);

            return dialog;
        }

        @Override
        public void onResume() {
            ViewGroup.LayoutParams params = getDialog().getWindow().getAttributes();
            params.width = WindowManager.LayoutParams.MATCH_PARENT;
            params.height = WindowManager.LayoutParams.MATCH_PARENT;
            getDialog().getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);

            super.onResume();
        }


    }


}
