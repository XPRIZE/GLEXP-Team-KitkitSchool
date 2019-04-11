package todoschoollauncher.enuma.com.todoschoollauncher;

import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.os.Bundle;
import android.support.v7.widget.AppCompatImageView;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;

/**
 * Created by ingtellect on 1/3/17.
 */

public class ToolsActivity extends KitKitLoggerActivity {

    private User currentUser;
    private boolean isAnimating;
    private float mScale;
    private View mVBack;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);
        Util.hideSystemUI(this);
        mScale = Util.setScale(this, findViewById(R.id.tools_container));

        mVBack = findViewById(R.id.v_back);
        mVBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        isAnimating = false;

        currentUser = ((LauncherApplication) getApplication()).getCurrentUser();


        ToolsAppView drum = (ToolsAppView) findViewById(R.id.app_drum);
        drum.setItem("drum", getString(R.string.drums), "tools_image_icon_drum", "tools_image_icon_drum_disabled", "30");
//        drum.setUnlocked(true);
//        drum.setEnable(true);

        drum.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {

                    try {
                        //Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.drum");
                        Intent i = new Intent(Intent.ACTION_MAIN);
                        i.setComponent(new ComponentName("com.enuma.drum", "com.enuma.drum.activity.MainActivity"));
                        startActivity(i);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_drum", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }

                } else {
                    unlock(tv);
                }


            }
        });

        ToolsAppView marimba = (ToolsAppView) findViewById(R.id.app_marimba);
        marimba.setItem("marimba", getString(R.string.marimba), "tools_image_icon_marimba", "tools_image_icon_marimba_disabled", "100");
//        marimba.setUnlocked(true);
//        marimba.setEnable(true);

        marimba.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
                        //Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.marimba");
                        Intent i = new Intent(Intent.ACTION_MAIN);
                        i.setComponent(new ComponentName("com.enuma.marimba", "com.enuma.marimba.activity.MainActivity"));
                        startActivity(i);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_marimba", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }

                } else {
                    unlock(tv);
                }
            }
        });


        ToolsAppView blackboard = (ToolsAppView) findViewById(R.id.app_blackboard);
        blackboard.setItem("blackboard", getString(R.string.blackboard), "tools_image_icon_board", "tools_image_icon_board_disabled", "200");
//        blackboard.setUnlocked(true);
//        blackboard.setEnable(true);

        blackboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.enuma.blackboard", "com.enuma.blackboard.activity.MainActivity"));
                        startActivity(intent);
                        //Intent i = getPackageManager().getLaunchIntentForPackage("com.enuma.blackboard");
                        //startActivity(i);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_blackboard", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView drawing = (ToolsAppView) findViewById(R.id.app_drawing);
        drawing.setItem("drawing", getString(R.string.drawing), "tools_image_icon_drawing", "tools_image_icon_drawing_disabled", "300");
//        drawing.setUnlocked(true);
//        drawing.setEnable(true);

        drawing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {

                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.putExtra("LANGUAGE", getAppLanguage());
                        intent.setClassName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.DrawingActivity");
                        startActivity(intent);

                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_drawing", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView coloring = (ToolsAppView) findViewById(R.id.app_coloring);
        coloring.setItem("coloring", getString(R.string.coloring), "tools_image_icon_coloring", "tools_image_icon_coloring_disabled", "500");
//        coloring.setUnlocked(true);
//        coloring.setEnable(true);

        coloring.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.putExtra("LANGUAGE", getAppLanguage());
                        intent.setClassName("com.enuma.drawingcoloring", "com.enuma.drawingcoloring.activity.ColoringActivity");
                        startActivity(intent);

                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_coloring", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    unlock(tv);
                }
            }
        });

        ToolsAppView album = (ToolsAppView) findViewById(R.id.app_album);
        album.setItem("album", getString(R.string.album), "tools_image_icon_album", "tools_image_icon_album_disabled", "0");
        album.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.enuma.todoschool.xprize_gallery", "com.enuma.todoschool.xprize_gallery.activity.MainActivity"));
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_album", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }

                }
            }
        });

        ToolsAppView fishBowl = (ToolsAppView) findViewById(R.id.app_fish_bowl);
        fishBowl.setItem("fish_bowl", getString(R.string.fish_bowl), "tools_image_icon_sea_world", "tools_image_icon_sea_world_disabled", "0");
        fishBowl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.enuma.xprize.FB", "org.cocos2dx.cpp.AppActivity"));
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_fish_bowl", "", 0);
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                    }

                }
            }
        });

        ToolsAppView writingBoard = (ToolsAppView) findViewById(R.id.app_writing_board);
        writingBoard.setItem("writingboard", getString(R.string.writing_board), "tools_image_icon_writing_board", "tools_image_icon_writing_board_disabled", "200");
        writingBoard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    if (gotoVideoPlayerForWritingBoard() == false) {
                        try {
                            Intent intent = new Intent(Intent.ACTION_MAIN);
                            intent.setComponent(new ComponentName("com.enuma.writingboard", "com.enuma.writingboard.activity.MainActivity"));
                            intent.putExtra("LANGUAGE", getAppLanguage());
                            startActivity(intent);
                            KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                            logger.logEvent("ToolsActivity", "start_writing_board", "", 0);
                        } catch (Exception e) {
                            Toast.makeText(getApplicationContext(), "Error", Toast.LENGTH_SHORT).show();
                        }
                    }

                } else {
                    unlock(tv);
                }
            }
        });

        TextView textViewCoinNum = (TextView) findViewById(R.id.textView_numCoin);
        Typeface f = Typeface.createFromAsset(getAssets(), "TodoMainCurly.ttf");
        textViewCoinNum.setTypeface(f);

    }

    @Override
    protected void onPause() {
        super.onPause();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(null);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
        logger.tagScreen("ToolsActivity");

        refreshLock();

        if (Util.mBlockingView != null) {
            Util.mBlockingView.setOnTouchListener(mBlockViewTouchListener);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Util.recycle(getWindow().getDecorView());
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    public void refreshLock() {
        currentUser = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();

        TextView textViewNumCoin = (TextView) findViewById(R.id.textView_numCoin);
        textViewNumCoin.setText(String.format("%d", currentUser.getNumStars()));

        ToolsAppView drum = (ToolsAppView) findViewById(R.id.app_drum);
        if (currentUser.isUnlockDrum()) {
            drum.setUnlocked(true);
            drum.setEnable(true);
        } else {
            if (currentUser.getNumStars() >= 30) {
                drum.setEnable(true);
            } else {
                drum.setEnable(false);
            }
        }

        ToolsAppView marimba = (ToolsAppView) findViewById(R.id.app_marimba);
        if (currentUser.isUnlockMarimba()) {
            marimba.setUnlocked(true);
            marimba.setEnable(true);
        } else {
            if (currentUser.getNumStars() >= 100) {
                marimba.setEnable(true);
            } else {
                marimba.setEnable(false);
            }
        }

        ToolsAppView blackboard = (ToolsAppView) findViewById(R.id.app_blackboard);
        if (currentUser.isUnlockBlackboard()) {
            blackboard.setUnlocked(true);
            blackboard.setEnable(true);
        } else {
            if (currentUser.getNumStars() >= 200) {
                blackboard.setEnable(true);
            } else {
                blackboard.setEnable(false);
            }
        }

        ToolsAppView drawing = (ToolsAppView) findViewById(R.id.app_drawing);
        if (currentUser.isUnlockDrawing()) {
            drawing.setUnlocked(true);
            drawing.setEnable(true);
        } else {
            if (currentUser.getNumStars() >= 300) {
                drawing.setEnable(true);
            } else {
                drawing.setEnable(false);
            }
        }

        ToolsAppView coloring = (ToolsAppView) findViewById(R.id.app_coloring);
        if (currentUser.isUnlockColoring()) {
            coloring.setUnlocked(true);
            coloring.setEnable(true);
        } else {
            if (currentUser.getNumStars() >= 500) {
                coloring.setEnable(true);
            } else {
                coloring.setEnable(false);
            }
        }

        ToolsAppView album = (ToolsAppView) findViewById(R.id.app_album);
        if (drawing.isUnlocked() || coloring.isUnlocked()) {
            album.setUnlocked(true);
            album.setEnable(true);
        } else {
            album.setUnlocked(false);
            album.setEnable(false);
        }

        ToolsAppView fishBowl = (ToolsAppView) findViewById(R.id.app_fish_bowl);
        if (currentUser.isUnlockFishBowl()) {
            fishBowl.setUnlocked(true);
            fishBowl.setEnable(true);

        } else {
            fishBowl.setUnlocked(false);
            fishBowl.setEnable(false);

        }

        ToolsAppView writingBoard = (ToolsAppView) findViewById(R.id.app_writing_board);
        if (currentUser.isUnlockWritingBoard()) {
            writingBoard.setUnlocked(true);
            writingBoard.setEnable(true);

        } else {
            if (currentUser.getNumStars() >= 200) {
                writingBoard.setEnable(true);
            } else {
                writingBoard.setEnable(false);
            }
        }
    }

    private void unlock(ToolsAppView view) {

        if (isAnimating) return;

        isAnimating = true;

        ImageView coinImageInToolbar = (ImageView) findViewById(R.id.image_coin);


        int coinLocation[] = new int[2];
        coinImageInToolbar.getLocationInWindow(coinLocation);
        coinLocation[0] = (int)(coinLocation[0] / mScale + 0.5f);
        coinLocation[1] = (int)(coinLocation[1] / mScale + 0.5f);

        int itemLocation[] = new int[2];
        view.getLocationInWindow(itemLocation);
        itemLocation[0] = (int)(itemLocation[0] / mScale + 0.5f);
        itemLocation[1] = (int)(itemLocation[1] / mScale + 0.5f);

        final ToolsAppView appView = view;

        FrameLayout coinLayout = (FrameLayout) findViewById(R.id.coin_layout_tools);

        int numAnimation = 10;
        for (int i = 0; i < numAnimation; i++) {
            final AppCompatImageView coin = new AppCompatImageView(this);
            coin.setImageResource(R.drawable.daily_coinstatus_coin);
            coin.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            coinLayout.addView(coin);

            TranslateAnimation anim = new TranslateAnimation(0, itemLocation[0] - coinLocation[0] + view.getWidth() / 2 - coinImageInToolbar.getWidth() / 2, 0, itemLocation[1] - coinLocation[1] + view.getHeight() / 3);
            anim.setStartTime(AnimationUtils.currentAnimationTimeMillis() + i * 100);
            anim.setDuration(1000);

            if (i == numAnimation - 1) {
                anim.setAnimationListener(new Animation.AnimationListener() {
                    @Override
                    public void onAnimationStart(Animation animation) {
                    }

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
                            case "writingboard":
                                currentUser.setUnlockWritingBoard(true);
                                break;
                            default:
                                return;
                        }


                        appView.setUnlocked(true);
                        int stars = currentUser.getNumStars();
                        currentUser.setNumStars(stars - appView.getNumCoin());
                        TextView textViewCoinNum = (TextView) findViewById(R.id.textView_numCoin);
                        textViewCoinNum.setText(String.format("%d", (stars - appView.getNumCoin())));
                        KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                        dbHandler.updateUser(currentUser);

                        refreshLock();
                    }

                    @Override
                    public void onAnimationRepeat(Animation animation) {
                    }
                });
            } else {
            }
            coin.setAnimation(anim);
        }
  }

    private boolean gotoVideoPlayerForWritingBoard() {
        User user = ((LauncherApplication)getApplication()).getDbHandler().getCurrentUser();

        if (user.isFinishWritingBoardTutorial() == false) {
            user.setFinishWritingBoardTutorial(true);
            ((LauncherApplication)getApplication()).getDbHandler().updateUser(user);

            Intent i = new Intent(ToolsActivity.this, VideoPlayerActivity.class);
            i.putExtra("video", "writing_board");
            startActivity(i);
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
            return true;
        }

        return false;
    }

    private Rect mTempRect = new Rect();
    private boolean mbPressed = false;
    private View.OnTouchListener mBlockViewTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                mVBack.getGlobalVisibleRect(mTempRect);
                if (mTempRect.contains((int) event.getX(), (int) event.getY())) {
                    mbPressed = true;
                    mVBack.dispatchTouchEvent(event);
                } else {
                    mbPressed = false;
                }
            } else {
                if (mbPressed) {
                    mVBack.dispatchTouchEvent(event);
                }

                if (event.getAction() == MotionEvent.ACTION_UP) {
                    mbPressed = false;
                }
            }

            return true;
        }
    };
}