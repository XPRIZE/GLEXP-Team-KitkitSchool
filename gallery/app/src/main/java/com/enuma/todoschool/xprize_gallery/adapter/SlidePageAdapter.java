package com.enuma.todoschool.xprize_gallery.adapter;

import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.enuma.todoschool.xprize_gallery.R;
import com.enuma.todoschool.xprize_gallery.model.ItemImage;
import com.nostra13.universalimageloader.core.ImageLoader;

import java.util.ArrayList;

public class SlidePageAdapter extends PagerAdapter {

    private Context mContext;
    private ArrayList<ItemImage> mItems;

    public SlidePageAdapter(Context context, ArrayList<ItemImage> items) {
        mContext = context;
        mItems = items;
    }

    @Override
    public int getCount() {
        if (mItems == null) {
            return 0;
        }

        return mItems.size();
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view.equals(object);
    }

    @Override
    public Object instantiateItem(ViewGroup container, final int position) {
        View view = LayoutInflater.from(mContext).inflate(R.layout.layout_slide_page, null);
        container.addView(view, 0);

        final ImageView imageView = (ImageView) view.findViewById(R.id.iv_image);
        final ItemImage item = mItems.get(position);
        ImageLoader.getInstance().displayImage("file:///" + item.mImagePath, imageView);
        return view;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        container.removeView((View) object);
    }
}
