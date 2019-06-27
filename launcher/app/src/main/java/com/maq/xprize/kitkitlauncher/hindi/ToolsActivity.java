package com.maq.xprize.kitkitlauncher.hindi;

import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.os.Bundle;
import android.support.v7.widget.AppCompatImageView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.kitkitlogger.KitKitLogger;
import com.maq.kitkitlogger.KitKitLoggerActivity;
import com.maq.xprize.drum.activity.DrumActivity;

/**
 * Created by ingtellect on 1/3/17.
 */

public class ToolsActivity extends KitKitLoggerActivity {

    private User currentUser;
    private boolean isAnimating;
    private float mScale;
    private View mVBack;
    private String packageName = "";
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


        ToolsAppView drum = findViewById(R.id.app_drum);
        drum.setItem("drum", getString(R.string.drums), "tools_image_icon_drum", "tools_image_icon_drum_disabled", "30");
        drum.setUnlocked(true);
        drum.setEnable(true);

        drum.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    packageName = "com.maq.xprize.drum";
                    try {
//                    Calling Drum's main activity
                        Intent intent = new Intent(ToolsActivity.this, DrumActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_drum", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Drum Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }


            }
        });

        ToolsAppView marimba = findViewById(R.id.app_marimba);
        marimba.setItem("marimba", getString(R.string.marimba), "tools_image_icon_marimba", "tools_image_icon_marimba_disabled", "100");
        marimba.setUnlocked(true);
        marimba.setEnable(true);

        marimba.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
//                    Calling Marimba's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.marimba.activity.MainActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_marimba", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Marimba Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }
            }
        });


        ToolsAppView blackboard = findViewById(R.id.app_blackboard);
        blackboard.setItem("blackboard", getString(R.string.blackboard), "tools_image_icon_board", "tools_image_icon_board_disabled", "200");
        blackboard.setUnlocked(true);
        blackboard.setEnable(true);

        blackboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
//                    Calling Black Board's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.blackboard.activity.MainActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_blackboard", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Black Board Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView drawing = findViewById(R.id.app_drawing);
        drawing.setItem("drawing", getString(R.string.drawing), "tools_image_icon_drawing", "tools_image_icon_drawing_disabled", "300");
        drawing.setUnlocked(true);
        drawing.setEnable(true);
        drawing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
//                    Calling Drawing's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.drawingcoloring.activity.DrawingActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_drawing", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Drawing Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }

            }
        });


        ToolsAppView coloring = findViewById(R.id.app_coloring);
        coloring.setItem("coloring", getString(R.string.coloring), "tools_image_icon_coloring", "tools_image_icon_coloring_disabled", "500");
        coloring.setUnlocked(true);
        coloring.setEnable(true);

        coloring.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
//                    Calling Coloring's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.drawingcoloring.activity.ColoringActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_coloring", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Coloring Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }
            }
        });

        ToolsAppView album = findViewById(R.id.app_album);
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
//                  Calling Gallery's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.kitkitlauncher.hindi.activity.GalleryActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_album", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Gallery Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }
            }
        });

        ToolsAppView fishBowl = findViewById(R.id.app_fish_bowl);
        fishBowl.setItem("fish_bowl", getString(R.string.fish_bowl), "tools_image_icon_sea_world", "tools_image_icon_sea_world_disabled", "0");
        fishBowl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    packageName = "com.maq.xprize.FB";
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.maq.xprize.FB", "org.cocos2dx.cpp.AppActivity"));
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_fish_bowl", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Sea World Intent Error:" + e);
                        e.printStackTrace();
                    }

                }
            }
        });

        ToolsAppView writingBoard = findViewById(R.id.app_writing_board);
        writingBoard.setItem("writingboard", getString(R.string.writing_board), "tools_image_icon_writing_board", "tools_image_icon_writing_board_disabled", "200");
        writingBoard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!v.isEnabled()) {
                    return;
                }
                ToolsAppView tv = (ToolsAppView) v;
                if (tv.isUnlocked()) {
                    try {
//                    Calling Writing Board's main activity
                        Intent intent = new Intent(ToolsActivity.this, com.maq.xprize.writingboard.activity.MainActivity.class);
                        startActivity(intent);
                        KitKitLogger logger = ((LauncherApplication) getApplication()).getLogger();
                        logger.logEvent("ToolsActivity", "start_writing_board", "", 0);
                    } catch (Exception e) {
                        Log.e("ToolsActivity", "Writing Board Intent Error:" + e);
                        e.printStackTrace();
                    }
                } else {
                    unlock(tv);
                }
            }
        });

        TextView textViewCoinNum = findViewById(R.id.textView_numCoin);
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

        TextView textViewNumCoin = findViewById(R.id.textView_numCoin);
        textViewNumCoin.setText(String.format("%d", currentUser.getNumStars()));

        ToolsAppView drawing = findViewById(R.id.app_drawing);
        ToolsAppView coloring = findViewById(R.id.app_coloring);
        ToolsAppView album = findViewById(R.id.app_album);
        if (drawing.isUnlocked() || coloring.isUnlocked()) {
            album.setUnlocked(true);
            album.setEnable(true);
        } else {
            album.setUnlocked(false);
            album.setEnable(false);
        }

        ToolsAppView fishBowl = findViewById(R.id.app_fish_bowl);
        if (currentUser.isUnlockFishBowl()) {
            fishBowl.setUnlocked(true);
            fishBowl.setEnable(true);

        } else {
            fishBowl.setUnlocked(false);
            fishBowl.setEnable(false);

        }

        ToolsAppView writingBoard = findViewById(R.id.app_writing_board);
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

        ImageView coinImageInToolbar = findViewById(R.id.image_coin);


        int[] coinLocation = new int[2];
        coinImageInToolbar.getLocationInWindow(coinLocation);
        coinLocation[0] = (int) (coinLocation[0] / mScale + 0.5f);
        coinLocation[1] = (int) (coinLocation[1] / mScale + 0.5f);

        int[] itemLocation = new int[2];
        view.getLocationInWindow(itemLocation);
        itemLocation[0] = (int) (itemLocation[0] / mScale + 0.5f);
        itemLocation[1] = (int) (itemLocation[1] / mScale + 0.5f);

        final ToolsAppView appView = view;

        FrameLayout coinLayout = findViewById(R.id.coin_layout_tools);

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
                        TextView textViewCoinNum = findViewById(R.id.textView_numCoin);
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
        User user = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();

        if (user != null && user.isFinishWritingBoardTutorial() == false) {
            user.setFinishWritingBoardTutorial(true);
            ((LauncherApplication) getApplication()).getDbHandler().updateUser(user);

            Intent i = new Intent(ToolsActivity.this, VideoPlayerActivity.class);
            i.putExtra("video", "writing_board");
            startActivity(i);
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
            return true;
        }

        return false;
    }
}