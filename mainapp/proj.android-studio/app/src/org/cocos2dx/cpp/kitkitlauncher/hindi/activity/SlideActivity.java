package org.cocos2dx.cpp.kitkitlauncher.hindi.activity;

import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.view.View;

import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.kitkitlauncher.hindi.activity.base.BaseActivity;
import org.cocos2dx.cpp.kitkitlauncher.hindi.adapter.SlidePageAdapter;
import org.cocos2dx.cpp.kitkitlauncher.hindi.core.Const;
import org.cocos2dx.cpp.kitkitlauncher.hindi.model.ItemImage;

import java.util.ArrayList;

public class SlideActivity extends BaseActivity implements
        ViewPager.OnPageChangeListener,
        View.OnClickListener {

    private ViewPager mViewPager;
    private View mVLeftArrow;
    private View mVRightArrow;

    private ArrayList<ItemImage> mItems = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_slide);

        mViewPager = (ViewPager) findViewById(R.id.view_pager);
        mViewPager.addOnPageChangeListener(this);

        findViewById(R.id.iv_close).setOnClickListener(this);

        mVLeftArrow = findViewById(R.id.iv_left);
        mVLeftArrow.setOnClickListener(this);

        mVRightArrow = findViewById(R.id.iv_right);
        mVRightArrow.setOnClickListener(this);

        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            mItems = bundle.getParcelableArrayList(Const.KEY_IMAGE_DATA);

            int position = bundle.getInt(Const.KEY_POSITION);
            mViewPager.setAdapter(new SlidePageAdapter(this, mItems));
            mViewPager.setCurrentItem(position);
            setPageInfo(position);
        }
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();

        if (id == R.id.iv_close) {
            finish();

        } else if (id == R.id.iv_left) {
            if (mViewPager.getCurrentItem() > 0) {
                mViewPager.setCurrentItem(mViewPager.getCurrentItem() - 1, true);
            }


        } else if (id == R.id.iv_right) {
            if (mViewPager.getCurrentItem() < mViewPager.getAdapter().getCount() - 1) {
                mViewPager.setCurrentItem(mViewPager.getCurrentItem() + 1, true);
            }

        }
    }

    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

    }

    @Override
    public void onPageSelected(int position) {
        setPageInfo(position);
    }

    @Override
    public void onPageScrollStateChanged(int state) {

    }

    private void setPageInfo(int position) {
        if (mItems.size() > 1) {
            if (position == 0) {
                mVLeftArrow.setVisibility(View.INVISIBLE);
                mVRightArrow.setVisibility(View.VISIBLE);

            } else if (position == mItems.size() - 1) {
                mVLeftArrow.setVisibility(View.VISIBLE);
                mVRightArrow.setVisibility(View.INVISIBLE);

            } else {
                mVLeftArrow.setVisibility(View.VISIBLE);
                mVRightArrow.setVisibility(View.VISIBLE);

            }
        } else {
            mVLeftArrow.setVisibility(View.INVISIBLE);
            mVRightArrow.setVisibility(View.INVISIBLE);

        }
    }
}
