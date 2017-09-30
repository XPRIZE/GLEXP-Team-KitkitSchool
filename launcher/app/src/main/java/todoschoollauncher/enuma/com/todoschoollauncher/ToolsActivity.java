package todoschoollauncher.enuma.com.todoschoollauncher;

import android.animation.Animator;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.nfc.Tag;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.view.menu.MenuView;
import android.support.v7.widget.AppCompatImageView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

import org.w3c.dom.Text;

import static todoschoollauncher.enuma.com.todoschoollauncher.R.id.textView_numCoin;

/**
 * Created by ingtellect on 1/3/17.
 */

public class ToolsActivity extends KitKitLoggerActivity {

    final String cameraPackageName = "com.android.camera2";
    final String galleryPackageName = "com.android.gallery3d";

    private User currentUser;
    private boolean isAnimating;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });
        isAnimating = false;


//        ImageButton cameraButton = (ImageButton) findViewById(R.id.button_camera);
//        final PackageManager pm = getApplicationContext().getPackageManager();
//        ApplicationInfo ai;
//        try {
//            ai = pm.getApplicationInfo( cameraPackageName, 0);
//        } catch (final PackageManager.NameNotFoundException e) {
//            ai = null;
//        }
//        final String cameraAppName = (String) (ai != null ? pm.getApplicationLabel(ai) : "(unknown)");
//        TextView cameraTextView = (TextView) findViewById(R.id.camera_textView);
//        cameraTextView.setText(cameraAppName);
//
//        cameraButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent i = getPackageManager().getLaunchIntentForPackage(cameraPackageName);
//                startActivity(i);
//                KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
//                logger.logEvent("ToolsActivity","start_camera","",0);
//            }
//        });
//
//
//        final ImageButton gallaryButton = (ImageButton) findViewById(R.id.button_gallery);
//        try {
//            ai = pm.getApplicationInfo( galleryPackageName, 0);
//        } catch (final PackageManager.NameNotFoundException e) {
//            ai = null;
//        }
//        final String galleryAppName = (String) (ai != null ? pm.getApplicationLabel(ai) : "(unknown)");
//        TextView galleryTextView = (TextView) findViewById(R.id.gallery_textView);
//        galleryTextView.setText(galleryAppName);
//
//        gallaryButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent i = getPackageManager().getLaunchIntentForPackage(galleryPackageName);
//                startActivity(i);
//                KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
//                logger.logEvent("ToolsActivity","start_gallery","",0);
//            }
//        });

        currentUser = ((LauncherApplication) getApplication()).getCurrentUser();


        ToolsAppView drum = (ToolsAppView)findViewById(R.id.app_drum);
        drum.setItem("drum", getString(R.string.drums),"tools_image_icon_drum","tools_image_icon_drum_disabled","30");


        drum.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView)v;
                if (tv.isUnlocked()) {
                    Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.drum");
                    startActivity(i);
                    KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
                    logger.logEvent("ToolsActivity","start_drum","",0);
                }
                else {
                    unlock(tv);
                }


            }
        });

        ToolsAppView marimba = (ToolsAppView)findViewById(R.id.app_marimba);
        marimba.setItem("marimba", getString(R.string.marimba),"tools_image_icon_marimba","tools_image_icon_marimba_disabled","100");

        marimba.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView)v;
                if (tv.isUnlocked()) {
                    Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.marimba");
                    startActivity(i);
                    KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
                    logger.logEvent("ToolsActivity","start_marimba","",0);
                }
                else {
                    unlock(tv);
                }
            }
        });


        ToolsAppView blackboard = (ToolsAppView)findViewById(R.id.app_blackboard);
        blackboard.setItem("blackboard", getString(R.string.blackboard),"tools_image_icon_board","tools_image_icon_board_disabled","200");
        blackboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView)v;
                if (tv.isUnlocked()) {
//                    Intent intent = new Intent();
//                    intent.setComponent(new ComponentName("com.enuma.blackboard", "com.enuma.blackboard.activity.MainActivity"));
//                    startActivity(intent);
                    Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.blackboard");
                    startActivity(i);
                    KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
                    logger.logEvent("ToolsActivity","start_blackboard","",0);
                }
                else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView drawing = (ToolsAppView)findViewById(R.id.app_drawing);
        drawing.setItem("drawing", getString(R.string.drawing),"tools_image_icon_drawing","tools_image_icon_drawing_disabled","300");
        drawing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView)v;
                if (tv.isUnlocked()) {

//                    Intent intent = new Intent();
//                    intent.setComponent(new ComponentName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.DrawingActivity"));
//                    startActivity(intent);

                    // Drawing, Coloring 은 Activity 가 2개 이므로 아래와 같이 처리하면 될듯 합니다

                    PackageManager packageManager = getPackageManager();
                    // 실행 flags를 얻기 위한 intent
                    Intent flagsIntent = packageManager.getLaunchIntentForPackage("com.enuma.drawingcoloring");

                    Intent intent = new Intent();
                    intent.setClassName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.DrawingActivity");
                    // intent.getFlags() 를 하면 0이 나옵니다

                    // 얻어온 flags를 setting 해 주면...
                    intent.setFlags(flagsIntent.getFlags());
                    startActivity(intent);


                    KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
                    logger.logEvent("ToolsActivity","start_drawing","",0);
                }
                else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView coloring = (ToolsAppView)findViewById(R.id.app_coloring);
        coloring.setItem("coloring", getString(R.string.coloring),"tools_image_icon_coloring","tools_image_icon_coloring_disabled","500");

        coloring.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView)v;
                if (tv.isUnlocked()) {
//                    Intent intent = new Intent();
//                    intent.setComponent(new ComponentName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.ColoringActivity"));
//                    startActivity(intent);

                    PackageManager packageManager = getPackageManager();
                    // 실행 flags를 얻기 위한 intent
                    Intent flagsIntent = packageManager.getLaunchIntentForPackage("com.enuma.drawingcoloring");

                    Intent intent = new Intent();
                    intent.setClassName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.ColoringActivity");
                    // intent.getFlags() 를 하면 0이 나옵니다

                    // 얻어온 flags를 setting 해 주면...
                    intent.setFlags(flagsIntent.getFlags());
                    startActivity(intent);


                    KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
                    logger.logEvent("ToolsActivity","start_coloring","",0);
                }
                else {
                    unlock(tv);
                }
            }
        });


//        ToolsAppView album = (ToolsAppView)findViewById(R.id.app_album);
//        album.setItem("album",getString(R.string.album),"tools_image_icon_album","tools_image_icon_album_disabled","0");
//        album.setEnable(true);
//        album.setUnlocked(true);
//        album.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Intent i = getPackageManager().getLaunchIntentForPackage(galleryPackageName);
//                startActivity(i);
//                KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
//                logger.logEvent("ToolsActivity","start_gallery","",0);
//            }
//        });

        TextView textViewCoinNum = (TextView)findViewById(R.id.textView_numCoin);
        Typeface f = Typeface.createFromAsset(getAssets(), "TodoMainCurly.ttf");
        textViewCoinNum.setTypeface(f);

    }

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication)getApplication()).getLogger();
        logger.tagScreen("ToolsActivity");


        refreshLock();
    }

    public void refreshLock() {
        currentUser = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();

        TextView textViewNumCoin = (TextView)findViewById(R.id.textView_numCoin);
        textViewNumCoin.setText(String.format("%d",currentUser.getNumStars()));

        ToolsAppView drum = (ToolsAppView)findViewById(R.id.app_drum);
        if (currentUser.isUnlockDrum()) {
            drum.setUnlocked(true);
            drum.setEnable(true);
        }
        else {
            if (currentUser.getNumStars() >= 30) {
                drum.setEnable(true);
            }
            else {
                drum.setEnable(false);
            }
        }

        ToolsAppView marimba = (ToolsAppView)findViewById(R.id.app_marimba);
        if (currentUser.isUnlockMarimba()) {
            marimba.setEnable(true);
            marimba.setUnlocked(true);
        }
        else {
            if (currentUser.getNumStars() >= 100) {
                marimba.setEnable(true);
            }
            else {
                marimba.setEnable(false);
            }
        }


        ToolsAppView blackboard = (ToolsAppView)findViewById(R.id.app_blackboard);
        if (currentUser.isUnlockBlackboard()) {
            blackboard.setUnlocked(true);
            blackboard.setEnable(true);
        }
        else {
            if (currentUser.getNumStars() >= 200) {
                blackboard.setEnable(true);
            }
            else {
                blackboard.setEnable(false);
            }
        }

        ToolsAppView drawing = (ToolsAppView)findViewById(R.id.app_drawing);
        if (currentUser.isUnlockDrawing()) {
            drawing.setUnlocked(true);
            drawing.setEnable(true);
        }
        else {
            if (currentUser.getNumStars() >= 300) {
                drawing.setEnable(true);
            }
            else {
                drawing.setEnable(false);
            }
        }

        ToolsAppView coloring = (ToolsAppView)findViewById(R.id.app_coloring);
        if (currentUser.isUnlockColoring()) {
            coloring.setUnlocked(true);
            coloring.setEnable(true);
        }
        else {
            if (currentUser.getNumStars() >= 500) {
                coloring.setEnable(true);
            }
            else {
                coloring.setEnable(false);
            }
        }
    }

    private void unlock(ToolsAppView view) {

        if (isAnimating) return;

        isAnimating = true;

        ImageView coinImageInToolbar = (ImageView)findViewById(R.id.image_coin);
        int coinLocation[] = new int[2];
        coinImageInToolbar.getLocationInWindow(coinLocation);

        int itemLocation[] = new int[2];
        view.getLocationInWindow(itemLocation);
        final ToolsAppView appView = view;

        FrameLayout coinLayout = (FrameLayout)findViewById(R.id.coin_layout_tools);

        int numAnimation = 10;
        for (int i = 0; i < numAnimation; i++) {
            final AppCompatImageView coin = new AppCompatImageView(this);
            coin.setImageResource(R.drawable.daily_coinstatus_coin);
            coin.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            coinLayout.addView(coin);

            TranslateAnimation anim = new TranslateAnimation(0, itemLocation[0]-coinLocation[0]+view.getWidth()/2-coinImageInToolbar.getWidth()/2, 0, itemLocation[1]-coinLocation[1]+view.getHeight()/3);
            anim.setStartTime(AnimationUtils.currentAnimationTimeMillis()+i*100);
            anim.setDuration(1000);

            if (i == numAnimation - 1) {
                anim.setAnimationListener(new Animation.AnimationListener() {
                    @Override
                    public void onAnimationStart(Animation animation) {}

                    @Override
                    public void onAnimationEnd(Animation animation) {
                        coin.setVisibility(View.GONE);
                        isAnimating = false;

                        switch (appView.getAppID()) {
                            case "drum":
                                currentUser.setUnlockDrum(true);
                                break;
                            case "marimba":
                                currentUser.setUnlockMarimba(true);
                                break;
                            case "drawing":
                                currentUser.setUnlockDrawing(true);
                                break;
                            case "coloring":
                                currentUser.setUnlockColoring(true);
                                break;
                            case "blackboard":
                                currentUser.setUnlockBlackboard(true);
                                break;
                            default:
                                return;
                        }


                        appView.setUnlocked(true);
                        int stars = currentUser.getNumStars();
                        currentUser.setNumStars(stars - appView.getNumCoin());
                        TextView textViewCoinNum = (TextView)findViewById(R.id.textView_numCoin);
                        textViewCoinNum.setText(String.format("%d", (stars - appView.getNumCoin())));
                        KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                        dbHandler.updateUser(currentUser);

                        refreshLock();
                    }
                    @Override
                    public void onAnimationRepeat(Animation animation) {}
                });
            }
            else {
            }
            coin.setAnimation(anim);
        }


    }

}
