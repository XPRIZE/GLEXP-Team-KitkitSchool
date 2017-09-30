package com.enuma.drawingcoloring.dialog;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.ColorDrawable;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import com.enuma.drawingcoloring.R;
import com.enuma.drawingcoloring.core.Const;
import com.enuma.drawingcoloring.utility.Util;

import java.io.File;
import java.util.ArrayList;

public class DialogSelectColoringBackground extends Dialog {

    ////////////////////////////////////////////////////////////////////////////////

    private final int THUMBNAIL_COUNT_PER_PAGE = 6;

    ////////////////////////////////////////////////////////////////////////////////

    private Context mContext;
    private Callback mCallback;

    ////////////////////////////////////////////////////////////////////////////////

    private ViewPager mVPager;
    private View mVLeftArrow;
    private View mVRightArrow;

    ////////////////////////////////////////////////////////////////////////////////

    public DialogSelectColoringBackground(Context context, ArrayList<String> thumbnailList, Callback callback) {
        super(context);

        mContext = context;
        mCallback = callback;

        getWindow().setBackgroundDrawable(new ColorDrawable(0));
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.dialog_coloring_background);

        WindowManager.LayoutParams params = getWindow().getAttributes();
        params.width = WindowManager.LayoutParams.WRAP_CONTENT;
        params.height = WindowManager.LayoutParams.WRAP_CONTENT;
        getWindow().setAttributes(params);

        setUpView();
        mVPager.setAdapter(new ThumbnailAdapter(mContext, thumbnailList));
        mVPager.addOnPageChangeListener(mPageChangeListener);
        mPageChangeListener.onPageSelected(0);

    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setUpView() {
        findViewById(R.id.v_close).setOnClickListener(mOnClickListener);

        mVLeftArrow = findViewById(R.id.bt_left);
        mVLeftArrow.setOnClickListener(mOnClickListener);

        mVRightArrow = findViewById(R.id.bt_right);
        mVRightArrow.setOnClickListener(mOnClickListener);

        mVPager = (ViewPager) findViewById(R.id.v_pager);
    }

    ////////////////////////////////////////////////////////////////////////////////

    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int id = view.getId();

            Integer imageIndexTag = (Integer) view.getTag();
            if (imageIndexTag != null) {
                if (mCallback != null) {
                    mCallback.onSelectBGImageIndex(imageIndexTag.intValue());
                    dismiss();
                }

            } else {
                if (id == R.id.v_close) {
                    dismiss();

                } else if (id == R.id.bt_left) {
                    if (mVPager.getCurrentItem() > 0) {
                        mVPager.setCurrentItem(mVPager.getCurrentItem() - 1, true);
                    }

                } else if (id == R.id.bt_right) {
                    if (mVPager.getCurrentItem() < mVPager.getAdapter().getCount() - 1) {
                        mVPager.setCurrentItem(mVPager.getCurrentItem() + 1, true);
                    }
                }
            }
        }
    };

    private ViewPager.OnPageChangeListener mPageChangeListener = new ViewPager.OnPageChangeListener() {
        @Override
        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

        }

        @Override
        public void onPageSelected(int position) {
            if (position <= 0) {
                mVLeftArrow.setVisibility(View.INVISIBLE);

            } else {
                mVLeftArrow.setVisibility(View.VISIBLE);

            }

            if (position >= mVPager.getAdapter().getCount() - 1) {
                mVRightArrow.setVisibility(View.INVISIBLE);

            } else {
                mVRightArrow.setVisibility(View.VISIBLE);

            }
        }

        @Override
        public void onPageScrollStateChanged(int state) {

        }
    };
    ////////////////////////////////////////////////////////////////////////////////

    private class ThumbnailAdapter extends PagerAdapter {

        private Context mContext;
        private LayoutInflater mInflater;
        private ArrayList<String> mItem;

        public ThumbnailAdapter(Context context, ArrayList<String> thumbnailList) {
            mContext = context;
            mInflater = LayoutInflater.from(mContext);
            mItem = thumbnailList;
        }

        @Override
        public int getCount() {
            int count = mItem.size() / THUMBNAIL_COUNT_PER_PAGE;
            if (mItem.size() % THUMBNAIL_COUNT_PER_PAGE == 0) {
                return count;
            }
            return count + 1;
        }

        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view.equals(object);
        }

        @Override
        public Object instantiateItem(ViewGroup container, int position) {
            View view = mInflater.inflate(R.layout.content_coloring_background, null);
            String packageName = mContext.getPackageName();
            for (int i = 0; i < THUMBNAIL_COUNT_PER_PAGE; ++i) {
                View layout = view.findViewById(Util.getResourceId(mContext, "layout_bg_" + i, "id", packageName));
                ImageView imageView = (ImageView) view.findViewById(Util.getResourceId(mContext, "iv_bg_" + i, "id", packageName));
                View clickView = view.findViewById(Util.getResourceId(mContext, "v_bg_" + i, "id", packageName));

                int thumbnailIndex = position * THUMBNAIL_COUNT_PER_PAGE + i;
                if (thumbnailIndex >= mItem.size()) {
                    layout.setVisibility(View.INVISIBLE);

                } else {
                    layout.setVisibility(View.VISIBLE);
                    Bitmap bitmap = Util.getBitmapFromAssets(mContext, Const.COLORING_IMAGE_PATH_FROM_ASSETS + File.separator + mItem.get(thumbnailIndex));

                    if (bitmap != null) {
                        imageView.setImageBitmap(bitmap);
                    }

                    clickView.setTag(new Integer(thumbnailIndex));
                    clickView.setOnClickListener(mOnClickListener);
                }
            }

            container.addView(view, 0);
            return view;
        }

        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            container.removeView((View) object);
        }
    }

    public interface Callback {
        void onSelectBGImageIndex(int bgIndex);
    }

}
