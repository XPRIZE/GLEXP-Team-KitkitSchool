package com.enuma.drawingcoloring.activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.enuma.drawingcoloring.R;
import com.enuma.drawingcoloring.activity.base.BaseActivity;
import com.enuma.drawingcoloring.core.Const;
import com.enuma.drawingcoloring.dialog.DialogSelectColoringBackground;
import com.enuma.drawingcoloring.dialog.DialogSelectDrawingBackground;
import com.enuma.drawingcoloring.utility.EffectSound;
import com.enuma.drawingcoloring.utility.Log;
import com.enuma.drawingcoloring.utility.Preference;
import com.enuma.drawingcoloring.utility.Util;
import com.enuma.drawingcoloring.view.ViewDrawingColoring;
import com.enuma.drawingcoloring.view.ViewPen;
import com.enuma.drawingcoloring.view.base.LockableScrollView;

import java.io.File;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;

public class DrawingColoringActivity extends BaseActivity {

    ////////////////////////////////////////////////////////////////////////////////

    private final int TOTAL_PEN_COLOR_COUNT = 18;
    public static final int TOTAL_BG_COLOR_COUNT = 6;
    private final String TIME_FORMAT = "yyyy-MM-dd-HH-mm-ss";

    ////////////////////////////////////////////////////////////////////////////////

    private BaseActivity mThisActivity;
    protected Preference mPreference;
    private EffectSound mEffectSound;
    ////////////////////////////////////////////////////////////////////////////////

    protected ImageView mVChangeBg;
    private View mVSave;
    private ViewGroup mLayoutDrawing;
    private ViewDrawingColoring mVDrawingColoring;
    protected ImageView mIvColoring;

    private ViewPen[] mVPens = new ViewPen[TOTAL_PEN_COLOR_COUNT];
    private ViewPen[] mVSelectedPensEffect = new ViewPen[TOTAL_PEN_COLOR_COUNT];
    private ImageView mIvSaveEffect;

    ////////////////////////////////////////////////////////////////////////////////

    private int[] PEN_COLORS = new int[TOTAL_PEN_COLOR_COUNT];
    private int[] BG_COLORS = new int[TOTAL_BG_COLOR_COUNT];

    protected ArrayList<String> mImagesColoring;
    protected ArrayList<String> mThumbnailsColoring;
    protected boolean mbNeedSave = false;

    private int mTouchId;
    private float mBeforeTouchPosX, mBeforeTouchPosY;
    private boolean mbSoundingChalk = false;
    private Rect mTempRect = new Rect();

    private View mDecorView;
    private int mUiOption;

    ////////////////////////////////////////////////////////////////////////////////

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mThisActivity = this;
        mPreference = Preference.getInstance(mThisActivity);
        mEffectSound = EffectSound.getInstance(mThisActivity);

        setContentView(R.layout.activity_drawing_coloring);
        setImmersiveMode();
        setupView();
        checkSaveButton();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mVDrawingColoring.saveTempBitmapFile();
        if (mbSoundingChalk == true) {
            stopChalkingSound();
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (hasFocus == true) {
            mDecorView.setSystemUiVisibility(mUiOption);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setImmersiveMode() {
        mDecorView = getWindow().getDecorView();
        mUiOption = getWindow().getDecorView().getSystemUiVisibility();
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH )
            mUiOption |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN )
            mUiOption |= View.SYSTEM_UI_FLAG_FULLSCREEN;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT )
            mUiOption |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        mDecorView.setSystemUiVisibility(mUiOption);
    }

    private void setupView() {
        String packageName = getPackageName();

        for (int i = 0; i < TOTAL_PEN_COLOR_COUNT; ++i) {
            PEN_COLORS[i] = ContextCompat.getColor(this, Util.getResourceId(this, "color_" + i, "color", packageName));
        }

        for (int i = 0; i < TOTAL_BG_COLOR_COUNT; ++i) {
            BG_COLORS[i] = ContextCompat.getColor(this, Util.getResourceId(this, "bg_color_" + i, "color", packageName));
        }

        findViewById(R.id.layout_root).setOnTouchListener(mOnTouchListener);

        findViewById(R.id.v_back).setOnClickListener(mOnClickListener);

        mVChangeBg = (ImageView) findViewById(R.id.v_bg);
        mVChangeBg.setOnClickListener(mOnClickListener);

        mVSave = findViewById(R.id.v_save);
        mVSave.setOnClickListener(mOnClickListener);

        mLayoutDrawing = (ViewGroup) findViewById(R.id.layout_drawing);
        mVDrawingColoring = (ViewDrawingColoring) findViewById(R.id.v_drawing_coloring);
        mVDrawingColoring.setCallback(mViewDrawingCoCallback);
        mIvColoring = (ImageView) findViewById(R.id.iv_coloring);

        Bitmap bitmapPen = BitmapFactory.decodeResource(getResources(), R.drawable.drawingpad_crayon_1_white);
        DisplayMetrics displayMetrics = Util.getWindowInfo(mThisActivity);
        Log.i("width dp : " + Util.getDip(mThisActivity, displayMetrics.widthPixels));

        int layoutPenHeight = displayMetrics.heightPixels;

        if (Const.LCD_GROUP == 1) {
            layoutPenHeight -= Util.getPixel(mThisActivity, 85);

        } else {
            layoutPenHeight -= Util.getPixel(mThisActivity, 121);

        }

        if (layoutPenHeight < bitmapPen.getHeight() * (TOTAL_PEN_COLOR_COUNT + 1)) {
            float scale = (float)layoutPenHeight / (bitmapPen.getHeight() * (TOTAL_PEN_COLOR_COUNT + 1));
            View layoutPenGroup = findViewById(R.id.layout_pen_group);
            layoutPenGroup.setPivotX(0);
            layoutPenGroup.setPivotY(0);
            layoutPenGroup.setScaleX(scale);
            layoutPenGroup.setScaleY(scale);
        }

        ((LockableScrollView)findViewById(R.id.scrollView)).setScrollingEnabled(false);

        final int[] RESOURCE_PEN = {
                R.drawable.drawingpad_crayon_1_white,
                R.drawable.drawingpad_crayon_2_black,
                R.drawable.drawingpad_crayon_3_gray,
                R.drawable.drawingpad_crayon_4_brown,
                R.drawable.drawingpad_crayon_5_red,
                R.drawable.drawingpad_crayon_6_orange,
                R.drawable.drawingpad_crayon_7_yelloworange,
                R.drawable.drawingpad_crayon_8_yellow,
                R.drawable.drawingpad_crayon_9_lime,
                R.drawable.drawingpad_crayon_10_green,
                R.drawable.drawingpad_crayon_11_emeraldgreen,
                R.drawable.drawingpad_crayon_12_aqua,
                R.drawable.drawingpad_crayon_13_lightblue,
                R.drawable.drawingpad_crayon_14_blue,
                R.drawable.drawingpad_crayon_15purpleblue,
                R.drawable.drawingpad_crayon_16_magentapurple,
                R.drawable.drawingpad_crayon_17_hotpink,
                R.drawable.drawingpad_crayon_18_lightpink
        };

        Bitmap bitmapPenSelected = BitmapFactory.decodeResource(getResources(), R.drawable.drawingpad_crayon_0_highlight);
        int marginTop = (int)getResources().getDimension(R.dimen.pen_normal_margin_top);

        LinearLayout layoutPen = (LinearLayout) findViewById(R.id.layout_pen);

        for (int i = 0; i < TOTAL_PEN_COLOR_COUNT; ++i) {
            mVPens[i] = new ViewPen(this);
            mVPens[i].setPenImage(RESOURCE_PEN[i], null);
            mVPens[i].setTag(new Integer(i));
//            mVPens[i].setOnClickListener(mOnClickListener);

            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, bitmapPenSelected.getHeight());
            if (i != 0) {
                params.setMargins(0, -marginTop*2, 0, 0);
            }

            layoutPen.addView(mVPens[i], params);
        }

        LinearLayout layoutSelectedPenEffect = (LinearLayout) findViewById(R.id.layout_select_pen_effect);
        for (int i = 0; i < TOTAL_PEN_COLOR_COUNT; ++i) {
            mVSelectedPensEffect[i] = new ViewPen(this);
            mVSelectedPensEffect[i].setPenImage(-1, bitmapPenSelected);

            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, bitmapPenSelected.getHeight());
            if (i != 0) {
                params.setMargins(0, -marginTop*2, 0, 0);
            }

            layoutSelectedPenEffect.addView(mVSelectedPensEffect[i], params);
        }

        mIvSaveEffect = (ImageView) findViewById(R.id.iv_save_effect);
    }

    private void setNeedSave(boolean bNeedSave) {
        mbNeedSave = bNeedSave;
        checkSaveButton();
    }

    private void startSaveAnimation(String bitmapFilePath) {
        Bitmap bitmap = BitmapFactory.decodeFile(bitmapFilePath);
        mIvSaveEffect.setImageBitmap(bitmap);

        Rect rectDrawView = new Rect();
        Rect rectSaveButton = new Rect();
        mLayoutDrawing.getGlobalVisibleRect(rectDrawView);
        mVSave.getGlobalVisibleRect(rectSaveButton);
        int offsetX = rectSaveButton.centerX() - rectDrawView.centerX();
        int offsetY = rectSaveButton.centerY() - rectDrawView.centerY();

        AnimationSet animationSet = (AnimationSet) AnimationUtils.loadAnimation(mThisActivity, R.anim.save);

        TranslateAnimation translateAnimation = new TranslateAnimation(0, offsetX, 0, offsetY);
        animationSet.addAnimation(translateAnimation);
        animationSet.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                mEffectSound.startSoundPool(EffectSound.SOUND_SAVE_BOARD, 0);
                mIvSaveEffect.setVisibility(View.GONE);
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        mIvSaveEffect.setVisibility(View.VISIBLE);
        mIvSaveEffect.startAnimation(animationSet);
    }

    private void startChalkingSound() {
        mbSoundingChalk = true;
        //mEffectSound.startSoundPool(EffectSound.SOUND_CHALK, -1);
    }

    private void stopChalkingSound() {
        mbSoundingChalk = false;
        //mEffectSound.stopSoundPool(EffectSound.SOUND_CHALK);
    }

    ////////////////////////////////////////////////////////////////////////////////

    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int id = view.getId();
            Integer colorIndexTag = (Integer) view.getTag();
            if (colorIndexTag != null) {
//                setPenColor(colorIndexTag.intValue());

            } else {
                if (id == R.id.v_back) {
                    finish();

                } else if (id == R.id.v_bg) {
                    if (mVDrawingColoring.getMode() == ViewDrawingColoring.MODE.DRAWING) {
                        DialogSelectDrawingBackground dialogSelectDrawingBackground = new DialogSelectDrawingBackground(mThisActivity, mSelectDrawingBgCallback);
                        dialogSelectDrawingBackground.show();

                    } else {
                        DialogSelectColoringBackground dialogSelectColoringBackground = new DialogSelectColoringBackground(mThisActivity, mThumbnailsColoring, mSelectColoringBgCallback);
                        dialogSelectColoringBackground.show();

                    }
                } else if (id == R.id.v_save) {
                    Calendar calendar = Calendar.getInstance();
                    String saveFileName = Const.SAVE_PATH + File.separator + Util.getTimeFormatString(TIME_FORMAT, calendar.getTimeInMillis()) + ".jpg";

                    if (Util.saveImageFileFromView(mLayoutDrawing, saveFileName, null, 100) == true) {
                        setNeedSave(false);
                        startSaveAnimation(saveFileName);
                    }
                }
            }
        }
    };

    private DialogSelectDrawingBackground.Callback mSelectDrawingBgCallback = new DialogSelectDrawingBackground.Callback() {
        @Override
        public void onSelectBGColorIndex(int colorIndex) {
            setBGColor(colorIndex, true);

            final int BLACK_COLOR_INDEX = 5;

            if (BLACK_COLOR_INDEX == colorIndex) {
                setPenColor(0, false);

            } else {
                setPenColor(1, false);

            }

            mVDrawingColoring.clearAll();
            setNeedSave(false);
        }
    };

    private DialogSelectColoringBackground.Callback mSelectColoringBgCallback = new DialogSelectColoringBackground.Callback() {
        @Override
        public void onSelectBGImageIndex(int bgIndex) {
            setColoringImage(bgIndex, true);
            setPenColor(1, false);

            mVDrawingColoring.clearAll();
            setNeedSave(false);
        }
    };

    private ViewDrawingColoring.Callback mViewDrawingCoCallback = new ViewDrawingColoring.Callback() {
        @Override
        public void onTouchDownForDrawing() {
            setNeedSave(true);
        }
    };

    private View.OnTouchListener mOnTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            float x = 0;
            float y = 0;

            switch (event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN:
                    mTouchId = event.getPointerId(0);
                    x = event.getX(event.findPointerIndex(mTouchId));
                    y = event.getY(event.findPointerIndex(mTouchId));
                    break;

                case MotionEvent.ACTION_MOVE:
                    if (event.findPointerIndex(mTouchId) != -1) {
                        x = event.getX(event.findPointerIndex(mTouchId));
                        y = event.getY(event.findPointerIndex(mTouchId));
                    }
                    break;

                case MotionEvent.ACTION_UP:
                    if (event.findPointerIndex(mTouchId) != -1) {
                        x = event.getX(event.findPointerIndex(mTouchId));
                        y = event.getY(event.findPointerIndex(mTouchId));
                    }
                    break;
            }

            if (event.findPointerIndex(mTouchId) != -1) {
                for (int i = 0; i < mVPens.length; ++i) {
                    mVPens[i].getGlobalVisibleRect(mTempRect);
                    if (mTempRect.contains((int) x, (int) y) == true) {
                        setPenColor(i, true);
                        break;
                    }
                }

                mVDrawingColoring.getGlobalVisibleRect(mTempRect);

                if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_UP) {
                    stopChalkingSound();

                } else {
                    final float TOUCH_TOLERANCE = 4;

                    if (mTempRect.contains((int)x, (int)y) == true) {
                        if (Math.abs(x - mBeforeTouchPosX) < TOUCH_TOLERANCE &&
                                Math.abs(y - mBeforeTouchPosY) < TOUCH_TOLERANCE) {
                            stopChalkingSound();

                        } else {
                            if (mbSoundingChalk == false) {
                                startChalkingSound();

                            }
                        }
                    } else {
                        stopChalkingSound();
                    }
                }

//                if (mbSoundingChalk == true) {
//                    mEffectSound.setVolume(EffectSound.SOUND_CHALK, (float)Math.random());
//                }


                mVDrawingColoring.doTouchEvent(event.getAction() & MotionEvent.ACTION_MASK, x - mTempRect.left, y - mTempRect.top);
            }

            mBeforeTouchPosX = x;
            mBeforeTouchPosY = y;
            return true;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////

    protected void setMode(ViewDrawingColoring.MODE mode) {
        mVDrawingColoring.setMode(mode);

        if (mode == ViewDrawingColoring.MODE.COLORING) {
            mVChangeBg.setImageResource(R.drawable.selector_coloring_bg_select);

            mImagesColoring = new ArrayList<>();
            mThumbnailsColoring = new ArrayList<>();

            String[] list = Util.getFileListFromAssets(mThisActivity, Const.COLORING_IMAGE_PATH_FROM_ASSETS);
            if (list != null) {
                for (int i = 0; i < list.length; ++i) {
                    if (list[i].contains("_thumb") == true) {
                        mThumbnailsColoring.add(list[i]);

                    } else {
                        mImagesColoring.add(list[i]);
                    }
                }
            }

            Collections.sort(mImagesColoring, new Comparator<String>() {
                @Override
                public int compare(String s, String t1) {
                    return s.compareTo(t1);
                }
            });

            Collections.sort(mThumbnailsColoring, new Comparator<String>() {
                @Override
                public int compare(String s, String t1) {
                    return s.compareTo(t1);
                }
            });

        } else {
            mVChangeBg.setImageResource(R.drawable.selector_drawing_bg_select);

        }
    }

    /**
     * Pen Color 선택
     *
     * @param colorIndex
     */
    protected void setPenColor(int colorIndex, boolean bEffectSound) {
        if (mVPens[colorIndex].isSelected() == true) {
            return;
        }

        for (int i = 0; i < TOTAL_PEN_COLOR_COUNT; ++i) {
            mVPens[i].setSelected(false);
            mVSelectedPensEffect[i].setSelected(false);
        }

        mVPens[colorIndex].setSelected(true);
        mVSelectedPensEffect[colorIndex].setSelected(true);

        mVDrawingColoring.setPenColor(PEN_COLORS[colorIndex]);

        if (mVDrawingColoring.getMode() == ViewDrawingColoring.MODE.DRAWING) {
            mPreference.setConfig(Const.PARAM_PEN_COLOR_INDEX_IN_DRAWING, colorIndex);

        } else {
            mPreference.setConfig(Const.PARAM_PEN_COLOR_INDEX_IN_COLORING, colorIndex);

        }

        if (bEffectSound == true) {
            mEffectSound.startSoundPool(EffectSound.SOUND_PICK_CRAYON, 0);
        }
    }

    /**
     * Background Color 선택
     *
     * @param colorIndex
     */
    protected void setBGColor(int colorIndex, boolean bEffectSound) {
        mVDrawingColoring.setBackgroundColor(BG_COLORS[colorIndex]);

        if (mVDrawingColoring.getMode() == ViewDrawingColoring.MODE.DRAWING) {
            mPreference.setConfig(Const.PARAM_BG_COLOR_INDEX, colorIndex);
        }

        if (bEffectSound == true) {
            mEffectSound.startSoundPool(EffectSound.SOUND_NEW_BOARD, 0);
        }
    }

    /**
     * Coloring 에서 이미지 선택
     *
     * @param coloringImageIndex
     */
    protected void setColoringImage(int coloringImageIndex, boolean bEffectSound) {
        Bitmap bitmap = Util.getBitmapFromAssets(mThisActivity, Const.COLORING_IMAGE_PATH_FROM_ASSETS + File.separator + mImagesColoring.get(coloringImageIndex));
        if (bitmap != null) {
            mIvColoring.setImageBitmap(bitmap);
            mPreference.setConfig(Const.PARAM_BG_IMAGE_INDEX, coloringImageIndex);

            if (bEffectSound == true) {
                mEffectSound.startSoundPool(EffectSound.SOUND_NEW_BOARD, 0);
            }
        }
    }

    protected void checkSaveButton() {
        mVSave.setEnabled(mbNeedSave);
    }
}
