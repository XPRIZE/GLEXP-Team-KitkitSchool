package com.maq.xprize.xprize_gallery.activity;

import android.os.Bundle;
import android.os.Environment;
import android.os.Message;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;
import com.maq.xprize.xprize_gallery.R;
import com.maq.xprize.xprize_gallery.activity.base.BaseActivity;
import com.maq.xprize.xprize_gallery.adapter.ThumbnailAdapter;
import com.maq.xprize.xprize_gallery.model.ItemImage;
import com.maq.xprize.xprize_gallery.utility.Log;
import com.maq.xprize.xprize_gallery.utility.Util;
import com.maq.xprize.xprize_gallery.view.RecyclerViewDecoration;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class MainActivity extends BaseActivity {

    ////////////////////////////////////////////////////////////////////////////////

    private final int BG_MSG_READ_DATA = 0;
    private final int MSG_SHOW_PROGRESS = 1;
    private final int MSG_HIDE_PROGRESS = 2;
    private final int MSG_SET_ADAPTER = 3;

    private final int SPAN_COUNT = 4;
    private final int GAP_DP = 10;

    ////////////////////////////////////////////////////////////////////////////////

    private RecyclerView mRecyclerView;
    private View mVProgress;

    ////////////////////////////////////////////////////////////////////////////////

    private ArrayList<ItemImage> mData = new ArrayList<>();
    private long mLastModified = 0L;
    private long mLastModifiedUserFolder = 0L;
    private KitkitDBHandler mKitkitDBHandler;
    private User mUser;

    ////////////////////////////////////////////////////////////////////////////////

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mThisActivity = this;
        mKitkitDBHandler = new KitkitDBHandler(mThisActivity);

        startHandlerThread();
        Util.hideSystemUI(mThisActivity);
        setContentView(R.layout.activity_main);

        setupView();
    }

    @Override
    protected void onResume() {
        super.onResume();
        sendBackgroundCommand(BG_MSG_READ_DATA);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            Util.hideSystemUI(mThisActivity);
        }
    }

    @Override
    public void handleThreadMessage(Message msg) {
        super.handleThreadMessage(msg);

        switch(msg.what) {
            case BG_MSG_READ_DATA:
                readData();
                break;
        }
    }

    @Override
    public void handleUIMessage(Message msg) {
        super.handleUIMessage(msg);

        switch (msg.what) {
            case MSG_SHOW_PROGRESS:
                mVProgress.setVisibility(View.VISIBLE);
                break;

            case MSG_HIDE_PROGRESS:
                mVProgress.setVisibility(View.GONE);
                break;

            case MSG_SET_ADAPTER:
                mRecyclerView.setAdapter(new ThumbnailAdapter(mThisActivity, mData, SPAN_COUNT, Util.getPixel(mThisActivity, GAP_DP)));
                break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setupView() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        mRecyclerView = (RecyclerView) findViewById(R.id.recycler);
        mRecyclerView.setLayoutManager(new GridLayoutManager(mThisActivity, SPAN_COUNT));
        int gap = Util.getPixel(mThisActivity, GAP_DP);
        mRecyclerView.addItemDecoration(new RecyclerViewDecoration(gap, gap, gap, gap));

        mVProgress = findViewById(R.id.progress);
    }

    private void readData() {
        File dcimFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);
        if (dcimFolder.exists() == false) {
            dcimFolder.mkdirs();
        }

        mUser = mKitkitDBHandler.getCurrentUser();
        File userFolder = null;

        if (mUser != null) {
            userFolder = new File(dcimFolder.getAbsolutePath() + File.separator + mUser.getUserName());
        }

        Log.i("dcimFolder.lastModified() : " + dcimFolder.lastModified());

        if ((userFolder != null && userFolder.lastModified() != mLastModifiedUserFolder) || dcimFolder.lastModified() != mLastModified) {
            Util.setMarkTime();
            mHandler.sendEmptyMessage(MSG_SHOW_PROGRESS);

            mLastModified = dcimFolder.lastModified();
            mData.clear();

            if (userFolder != null && userFolder.exists() == true) {
                mLastModifiedUserFolder = userFolder.lastModified();
                addData(userFolder);
            }

            Collections.sort(mData, new Comparator<ItemImage>() {
                @Override
                public int compare(ItemImage o1, ItemImage o2) {
                    return Long.compare(o2.mLastModified, o1.mLastModified);
                }
            });

            Log.i("data size : " + mData.size());
            Util.getElapsedTime("readData");

            mHandler.sendEmptyMessage(MSG_SET_ADAPTER);
            mHandler.sendEmptyMessage(MSG_HIDE_PROGRESS);
        }
    }

    private void addData(File parent) {
        Log.i("parent folder : " + parent.getAbsolutePath());
        String[] children = parent.list();
        if (children != null) {
            for (String child : children) {
                File file = new File(parent.getAbsolutePath() + File.separator + child);
                if (file.isDirectory() == false) {
                    ItemImage itemImage = new ItemImage();
                    itemImage.mImagePath = file.getAbsolutePath();
                    // too slow
//                itemImage.mThumbnailPath = Util.getThumbnailPath(mThisActivity, itemImage.mImagePath);
                    itemImage.mThumbnailPath = itemImage.mImagePath;
                    itemImage.mLastModified = file.lastModified();
                    mData.add(itemImage);
                }
            }
        }
    }
}
