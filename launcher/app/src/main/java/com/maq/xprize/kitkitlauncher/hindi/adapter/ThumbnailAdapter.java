package com.maq.xprize.kitkitlauncher.hindi.adapter;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.widget.RecyclerView;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;

import com.maq.xprize.kitkitlauncher.hindi.R;
import com.maq.xprize.kitkitlauncher.hindi.activity.SlideActivity;
import com.maq.xprize.kitkitlauncher.hindi.core.Const;
import com.maq.xprize.kitkitlauncher.hindi.model.ItemImage;
import com.maq.xprize.kitkitlauncher.hindi.utility.Log;
import com.maq.xprize.kitkitlauncher.hindi.utility.Util;
import com.nostra13.universalimageloader.core.ImageLoader;

import java.io.File;
import java.io.RandomAccessFile;
import java.util.ArrayList;

public class ThumbnailAdapter extends RecyclerView.Adapter<ThumbnailAdapter.ViewHolder> {

    private Activity mActivity;
    private ArrayList<ItemImage> mItems;
    private int mSpanCount = 0;
    private int mGap = 0;
    private float mAspectRatio = 1.0f;

    public ThumbnailAdapter(Activity activity, ArrayList<ItemImage> items, int spanCount, int gap) {
        mActivity = activity;
        mItems = items;
        mSpanCount = spanCount;
        mGap = gap;
        DisplayMetrics displayMetrics = Util.getWindowInfo(activity);
        mAspectRatio = (float)displayMetrics.heightPixels / displayMetrics.widthPixels;
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        FrameLayout layout = new FrameLayout(mActivity);
        layout.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, (int)((parent.getMeasuredWidth() / mSpanCount - mGap * 2) * mAspectRatio)));
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT) {
            layout.setElevation(Util.getPixel(mActivity, 10));
        }

        layout.setBackgroundColor(0x42000000);

        ImageView imageView = new ImageView(mActivity);
        imageView.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
        imageView.setBackgroundColor(Color.WHITE);
        imageView.setId(R.id.iv_thumbnail);
        layout.addView(imageView);

        return new ViewHolder(layout);
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, final int position) {
        final ItemImage item = mItems.get(position);
        holder.mImageView.setImageBitmap(null);
        if (item.mThumbnailPath.isEmpty() == true) {
            item.mThumbnailPath = item.mImagePath;
        }

        ImageLoader.getInstance().displayImage("file:///" + item.mThumbnailPath, holder.mImageView);

        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File file = new File(item.mImagePath);
                Log.i("file path : " + file.getAbsolutePath());
                if (file.exists() == true) {
                    Log.i("before modified : " + item.mLastModified);

                    try {
                        RandomAccessFile randomAccessFile = new RandomAccessFile(file, "rw");
                        long length = randomAccessFile.length();
                        randomAccessFile.setLength(length + 1);
                        randomAccessFile.setLength(length);
                        randomAccessFile.close();
                        Log.i("after modified : " + file.lastModified());

                    } catch (Exception e) {
                        Log.e("", e);
                    }
                }

                Intent intent = new Intent(mActivity, SlideActivity.class);
                intent.putExtra(Const.KEY_IMAGE_DATA, mItems);
                intent.putExtra(Const.KEY_POSITION, position);
                mActivity.startActivity(intent);
            }
        });
    }

    @Override
    public int getItemCount() {
        return mItems.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        ImageView mImageView;

        public ViewHolder(View view) {
            super(view);
            mImageView = (ImageView) view.findViewById(R.id.iv_thumbnail);
        }
    }
}
