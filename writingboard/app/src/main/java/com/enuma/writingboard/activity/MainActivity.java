package com.enuma.writingboard.activity;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import android.widget.TextView;

import com.enuma.kitkitProvider.KitkitDBHandler;
import com.enuma.kitkitProvider.User;
import com.enuma.writingboard.R;
import com.enuma.writingboard.core.Const;
import com.enuma.writingboard.util.Font;
import com.enuma.writingboard.util.Log;
import com.enuma.writingboard.util.Preference;
import com.enuma.writingboard.util.Util;
import com.enuma.writingboard.view.ViewDrawingColoring;
import com.enuma.writingboard.view.base.SwipeViewPager;
import com.enuma.writingboard.util.EffectSound;

import java.io.File;

public class MainActivity extends Activity {
    private Activity mThisActivity;
    private View mDecorView;
    private int mUiOption;
    private float mScale;
    private boolean mbSmall = false;
    private KitkitDBHandler mKitkitDBHandler;
    private User mUser;
	private EffectSound mEffectSound;

    private SwipeViewPager mViewPager;
    private View mVLeft;
    private View mVRight;
    private TextView mTvPage;
    protected String[] mImages;
    private String mSavePath = "";
    private String mSaveLogImagePath = "";
    private String mAppLanguage = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mThisActivity = this;
        Util.makeNoMedia(Const.SAVE_IMAGE_PATH);
        Util.makeNoMedia(Const.SAVE_LOG_IMAGE_PATH);
        Log.i("memoryClass : " + Util.getMemoryClass(this));

        Point size = Util.getWindowSize(this);
        if (size.x <= 1280) {
            setContentView(R.layout.activity_main_s);
            mbSmall = true;
        } else {
            setContentView(R.layout.activity_main);
        }

        mKitkitDBHandler = new KitkitDBHandler(mThisActivity);
        mUser = mKitkitDBHandler.getCurrentUser();

        if (savedInstanceState == null) {
            Bundle extras = getIntent().getExtras();
            if(extras != null) {
                mAppLanguage = extras.getString("LANGUAGE", "sw-TZ").toLowerCase();
            }
        }

        Log.i("mAppLanguage : " + mAppLanguage);
        if (mAppLanguage.isEmpty()) {
            mAppLanguage = "sw-tz";
        }

        setImmersiveMode();
        readData();
        setupView();
		mEffectSound = EffectSound.getInstance(this);

    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (hasFocus == true) {
            mDecorView.setSystemUiVisibility(mUiOption);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        Preference.getInstance(this).setConfig(getPagePreferenceKey(), mViewPager.getCurrentItem());
        saveImage();
    }

    ////////////////////////////////////////////////////////////////////////////////

    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int id = view.getId();

            if (id == R.id.v_back) {
                finish();

            } else if (id == R.id.v_left) {
                moveLeft();

            } else if (id == R.id.v_right) {
                moveRight();

            }
        }
    };

    private class PagerAdapterPage extends PagerAdapter {
        private LayoutInflater mInflater;

        public PagerAdapterPage(Context context) {
            super();
            mInflater = LayoutInflater.from(context);
        }

        @Override
        public Object instantiateItem(ViewGroup container, int position) {
            View vPage;
            vPage = mInflater.inflate(mbSmall ? R.layout.item_page_s : R.layout.item_page, null);
            vPage.setTag("page_" + position);
            ImageView ivBackground = (ImageView) vPage.findViewById(R.id.iv_background);
            ivBackground.setImageBitmap(Util.getBitmapFromAssets(mThisActivity, Const.IMAGE_PATH_FROM_ASSETS + File.separator + mAppLanguage + File.separator + mImages[position]));

            ViewDrawingColoring viewDrawingColoring = (ViewDrawingColoring) vPage.findViewById(R.id.drawing);
            viewDrawingColoring.setPenColor(Color.BLACK);
            viewDrawingColoring.loadDrawingImage(mSavePath + position + ".png");
            container.addView(vPage);
            return vPage;
        }

        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            container.removeView((View) object);
        }

        @Override
        public int getCount() {
            return mImages.length;
        }

        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view == object;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setImmersiveMode() {
        mDecorView = getWindow().getDecorView();
        mUiOption = getWindow().getDecorView().getSystemUiVisibility();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
            mUiOption |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            mUiOption |= View.SYSTEM_UI_FLAG_FULLSCREEN;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
            mUiOption |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        mDecorView.setSystemUiVisibility(mUiOption);
    }

    private void setScale(View rootView) {
        Point size = Util.getWindowSize(this);
        float BASIC_HEIGHT = (size.x <= 1280) ? 900.f : 1800.f;

        float fixedSizeWidth = BASIC_HEIGHT * size.x / size.y;
        float fixedSizeHeight = BASIC_HEIGHT;
        mScale = size.y / BASIC_HEIGHT;

        Log.i("display width : " + size.x);
        Log.i("display height : " + size.y);
        Log.i("fixed width : " + fixedSizeWidth);
        Log.i("fixed height : " + fixedSizeHeight);
        Log.i("scale : " + mScale);

        ViewGroup.LayoutParams params = rootView.getLayoutParams();
        params.width = (int) (fixedSizeWidth + 0.5f);
        params.height = (int) (fixedSizeHeight + 0.5f);
        rootView.setPivotX(0);
        rootView.setPivotY(0);
        rootView.setScaleX(mScale);
        rootView.setScaleY(mScale);
    }

    private void setupView() {
        mVLeft = findViewById(R.id.v_left);
        mVLeft.setOnClickListener(mOnClickListener);

        mVRight = findViewById(R.id.v_right);
        mVRight.setOnClickListener(mOnClickListener);

        mTvPage = (TextView) findViewById(R.id.tv_page);
        findViewById(R.id.v_back).setOnClickListener(mOnClickListener);
        mTvPage.setAlpha(0.55f);
        mTvPage.setTypeface(Font.getInstance(this).getAndika());

        mViewPager = (SwipeViewPager) findViewById(R.id.viewpager);
        mViewPager.setPagingEnabled(false);
        mViewPager.setAdapter(new PagerAdapterPage(this));
        mViewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
            }

            @Override
            public void onPageSelected(int position) {
                setPageText(position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {
                int position = mViewPager.getCurrentItem();
                if (state == ViewPager.SCROLL_STATE_IDLE) {
                    setDrawPageEnable(position, true);

                }
            }
        });

        int page = Preference.getInstance(this).getConfig(getPagePreferenceKey(), 0);
        mViewPager.setCurrentItem(page);
        setDrawPageEnable(page, true);
        setPageText(page);
        setScale(findViewById(R.id.layout_root));

        checkButtonState();
    }

    private void moveLeft() {
        int position = mViewPager.getCurrentItem();
        if (position > 0) {
            saveImage();
            setDrawPageEnable(position, false);
            mViewPager.setCurrentItem(position - 1, true);
        }
        checkButtonState();
		mEffectSound.stopSoundPool(EffectSound.SOUND_CHALK);
		mEffectSound.stopSoundPool(EffectSound.SOUND_ERASER);

    }

    private void moveRight() {
        int position = mViewPager.getCurrentItem();
        if (position < mImages.length - 1) {
            saveImage();
            setDrawPageEnable(position, false);
            mViewPager.setCurrentItem(position + 1, true);
        }
		mEffectSound.stopSoundPool(EffectSound.SOUND_CHALK);
		mEffectSound.stopSoundPool(EffectSound.SOUND_ERASER);

        checkButtonState();
    }

    private void checkButtonState() {
        int position = mViewPager.getCurrentItem();

        if (position <= 0) {
            if (mVLeft.getVisibility() == View.VISIBLE) {
                showLeftButton(false);
            }
        } else {
            if (mVLeft.getVisibility() == View.INVISIBLE) {
                showLeftButton(true);
            }
        }

        if (position >= mImages.length - 1) {
            if (mVRight.getVisibility() == View.VISIBLE) {
                showRightButton(false);
            }
        } else {
            if (mVRight.getVisibility() == View.INVISIBLE) {
                showRightButton(true);
            }
        }
    }

    private void setPageText(int pageIndex) {
        mTvPage.setText((pageIndex + 1) + " / " + mImages.length);
    }

    private void readData() {
        if (mUser == null) {
            mSavePath = Const.SAVE_IMAGE_PATH + File.separator;
            mSaveLogImagePath = Const.SAVE_LOG_IMAGE_PATH + File.separator;

        } else {
            mSavePath = Const.SAVE_IMAGE_PATH + File.separator + mUser.getUserName() + File.separator;
            mSaveLogImagePath = Const.SAVE_LOG_IMAGE_PATH+ File.separator + mUser.getUserName() + File.separator;
        }

        mImages = Util.getFileListFromAssets(this, Const.IMAGE_PATH_FROM_ASSETS + File.separator + mAppLanguage);
    }

    private void saveImage() {
        int position = mViewPager.getCurrentItem();
        View page = mViewPager.findViewWithTag("page_" + position);
        if (page != null) {

            ViewDrawingColoring viewDrawingColoring = (ViewDrawingColoring) page.findViewById(R.id.drawing);
            viewDrawingColoring.saveDrawingImage(mSavePath + position + ".png");
            float scale = 640.0f / page.getWidth();
            Util.saveImageFileFromView(page, mSaveLogImagePath + position + ".jpg", scale, 70);
        }
    }

    private void setDrawPageEnable(int position, boolean bEnable) {
        View page = mViewPager.findViewWithTag("page_" + position);
        if (page != null) {
            ViewDrawingColoring viewDrawingColoring = (ViewDrawingColoring) page.findViewById(R.id.drawing);
            if (viewDrawingColoring != null) {
                viewDrawingColoring.setTouchEnable(bEnable);
            }
        }
    }

    private void showLeftButton(final boolean bShow) {
        float start = bShow ? -mVLeft.getWidth() : 0;
        float end = bShow ? 0 : -mVLeft.getWidth();
        TranslateAnimation translation = new TranslateAnimation(start, end, 0, 0);
        translation.setDuration(200);
        translation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
                mVLeft.setVisibility(View.VISIBLE);
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                if (!bShow) {
                    mVLeft.setVisibility(View.INVISIBLE);
                }
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        mVLeft.startAnimation(translation);
    }

    private void showRightButton(final boolean bShow) {
        float start = bShow ? mVRight.getWidth() : 0;
        float end = bShow ? 0 : mVRight.getWidth();

        TranslateAnimation translation = new TranslateAnimation(start, end, 0, 0);
        translation.setDuration(200);
        translation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
                mVRight.setVisibility(View.VISIBLE);
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                if (!bShow) {
                    mVRight.setVisibility(View.INVISIBLE);
                }
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        mVRight.startAnimation(translation);
    }

    private String getPagePreferenceKey() {
        String result = "PAGE";
        if (mUser != null) {
            result += "_" + mUser.getUserName();
        }

        return result;
    }
}
