package com.maq.xprize.kitkitlibrary.english;

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.provider.SyncStateContract.Helpers;
import android.support.design.widget.TabLayout;
import android.support.v13.app.FragmentPagerAdapter;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.util.TypedValue;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

import com.maq.kitkitlogger.KitKitLogger;
import com.maq.kitkitlogger.KitKitLoggerActivity;
import com.nostra13.universalimageloader.core.ImageLoader;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;


/**
 * Created by ingtellect on 11/18/16.
 */

public class MainActivity extends KitKitLoggerActivity {

    LayoutParams params;
    int viewWidth;
    ArrayList<LinearLayout> layouts;
    int mWidth;

    static boolean useExternalData = false;
    static String appLanguage;
    private static boolean mbSignLanguageMode = false;
    static String pathExternalAsset = "";
    static String pathExternalRaw = "";

    private String TAG = "MainActivity";

    private static LibraryApplication application;


    public static int dpTopx(int dp, Context context) {
        Resources r = context.getResources();
        int px = (int) TypedValue.applyDimension(
                TypedValue.COMPLEX_UNIT_DIP,
                dp,
                r.getDisplayMetrics()
        );
        return px;
    }

    public static class VideoData implements Serializable {
        String id;
        String category;
        String categoryName;
        String title;
        String thumbnail;
        String filename;
        String lyrics = "";
        String info = "";
    }

    public static class BookData {
        String id;
        String category;
        String categoryName;
        String title;
        String author;
        String thumbnail;
        String foldername;
    }

    public static class VideoFragment extends Fragment {

        ArrayList<VideoData> videoArray;
        private String TAG = "VideoFragment";

        public static VideoFragment newInstance() {
            VideoFragment fragment = new VideoFragment();
            Bundle args = new Bundle();
            fragment.setArguments(args);
            return fragment;
        }

        @Override
        public void onResume() {
            super.onResume();
            KitKitLogger logger = ((LibraryApplication) getActivity().getApplication()).getLogger();
            logger.tagScreen(TAG);
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            Log.d(TAG, "onCreateView videoFragment");
            setMarkTime();

            TreeMap<String, ArrayList<VideoData>> data = new TreeMap<String, ArrayList<VideoData>>();
            String videoDataFilename;
            if (mbSignLanguageMode) {
                videoDataFilename = "library_video_data_SL.tsv";
            } else {
                videoDataFilename = "library_video_data.tsv";
            }

            videoArray = new ArrayList<>();
            try {
                InputStream is;
                if (MainActivity.useExternalData) {
                    is = new FileInputStream(pathExternalAsset + File.separator + appLanguage + File.separator + videoDataFilename);
                } else {
                    Log.i(TAG, "path : " + (appLanguage + File.separator + videoDataFilename));
                    is = getActivity().getAssets().open(appLanguage + File.separator + videoDataFilename);
                }

                BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                try {
                    String line;
                    while ((line = reader.readLine()) != null) {
                        if (line.charAt(0) == '#') continue;
                        String[] RowData = line.split("\t");
                        VideoData video = new VideoData();
                        video.id = RowData[0];
                        video.category = RowData[1];
                        video.categoryName = RowData[2];
                        video.title = RowData[3];
                        video.thumbnail = appLanguage + File.separator + RowData[4];
                        video.filename = RowData[5];
//                        video.lyrics = RowData[6];
//                        video.info = RowData[7];

                        videoArray.add(video);
                        video.lyrics = video.lyrics.replaceAll("\\\\n ", "\n").replaceAll("\\\\n", "\n");
                        video.info = video.info.replaceAll("\\\\n ", "\n").replaceAll("\\\\n", "\n");

                        ArrayList<VideoData> videoArrayForMap;

                        if (data.containsKey(video.category)) {
                            videoArrayForMap = data.get(video.category);
                            videoArrayForMap.add(video);
                        } else {
                            videoArrayForMap = new ArrayList<VideoData>();
                            videoArrayForMap.add(video);
                            data.put(video.category, videoArrayForMap);
                        }

                    }
                } catch (IOException ex) {
                    // handle exception
                    Log.e(TAG, "IOException");
                } finally {
                    try {
                        is.close();
                    } catch (IOException e) {
                        // handle exception
                    }
                }

            } catch (FileNotFoundException ex) {
                Log.e(TAG, "File not found exception");

            } catch (IOException ex) {

            }


            View rootView = inflater.inflate(R.layout.fragment_video, container, false);

            for (Map.Entry<String, ArrayList<VideoData>> entry : data.entrySet()) {
                String category = entry.getKey();
                final ArrayList<VideoData> videoList = entry.getValue();

                TextView categoryTitle;
                RecyclerView recyclerView;

                if (mbSignLanguageMode == false) {
                    if (category.equals("tutorial")) {
                        categoryTitle = rootView.findViewById(R.id.tutorial_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_0);
                    } else if (category.equals("music_video")) {
                        categoryTitle = rootView.findViewById(R.id.musicvideo_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_1);
                    } else if (category.equals("literacy_video")) {
                        categoryTitle = rootView.findViewById(R.id.literacy_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_2);
                    } else if (category.equals("literacy_video_2")) {
                        categoryTitle = rootView.findViewById(R.id.literacy_video_title_textView_2);
                        recyclerView = rootView.findViewById(R.id.recycler_video_2_2);
                    } else if (category.equals("math_video")) {
                        categoryTitle = rootView.findViewById(R.id.math_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_3);
                    } else if (category.equals("math_video_2")) {
                        categoryTitle = rootView.findViewById(R.id.math_video_title_textView_2);
                        recyclerView = rootView.findViewById(R.id.recycler_video_3_2);
                    } else if (category.equals("alphabet_video")) {
                        categoryTitle = rootView.findViewById(R.id.alphabet_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_4);

                    } else {
                        continue;
                    }

                } else {
                    if (category.equals("parents")) {
                        categoryTitle = rootView.findViewById(R.id.tutorial_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_0);
                    } else if (category.equals("story")) {
                        categoryTitle = rootView.findViewById(R.id.musicvideo_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_1);
                    } else if (category.equals("tutorial")) {
                        categoryTitle = rootView.findViewById(R.id.literacy_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_2);
                    } else if (category.equals("music_video")) {
                        categoryTitle = rootView.findViewById(R.id.math_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_3);

                    } else if (category.equals("literacy_video")) {
                        categoryTitle = rootView.findViewById(R.id.alphabet_video_title_textView);
                        recyclerView = rootView.findViewById(R.id.recycler_video_4);

                    } else {
                        continue;
                    }
                }

                categoryTitle.setVisibility(View.VISIBLE);
                recyclerView.setVisibility(View.VISIBLE);

                categoryTitle.setText(videoList.get(0).categoryName);
                recyclerView.setLayoutManager(new LinearLayoutManager(getActivity(), RecyclerView.HORIZONTAL, false));
                recyclerView.addItemDecoration(new MainActivity.RecyclerViewDecoration(
                        MainActivity.dpTopx(7, getActivity()),
                        MainActivity.dpTopx(2, getActivity()),
                        MainActivity.dpTopx(7, getActivity()),
                        MainActivity.dpTopx(5, getActivity())));
                recyclerView.setAdapter(new MainActivity.VideoAdapter(getActivity(), videoList));
            }
            getElapsedTime("video");
            return rootView;

        }
    }


    public static class BookFragment extends Fragment {
        private String TAG = "BookFragment";

        public static BookFragment newInstance() {
            BookFragment fragment = new BookFragment();
            Bundle args = new Bundle();
            fragment.setArguments(args);
            return fragment;
        }

        public Bitmap loadBookThumbnail(BookData bookData) {
            Bitmap thumbnail = null;
            BitmapFactory.Options options = new BitmapFactory.Options();
//            options.inPreferredConfig = Bitmap.Config.RGB_565;
//            //options.inJustDecodeBounds = true;
//            options.inSampleSize = 8;
            try {

                String filename = "";
                filename = bookData.thumbnail;

//                DisplayMetrics metrics = new DisplayMetrics();
//                getActivity().getWindowManager().getDefaultDisplay().getMetrics(metrics);
//                int density = metrics.densityDpi;
//                Log.d("density",new Integer(density).toString());

//                if (density < DisplayMetrics.DENSITY_XHIGH) {
//                    Log.d("filename",filename);

//                    StringTokenizer tokenizer = new StringTokenizer(bookData.thumbnail,".");
//                    filename = tokenizer.nextElement().toString() + "_hdpi." + tokenizer.nextElement().toString();
//
//                }

                Log.i(TAG, "thumbnail : " + thumbnail);
                thumbnail = BitmapFactory.decodeStream(getActivity().getAssets().open(filename), null, options);

            } catch (IOException e) {
                Log.e(TAG, "IOException in book thumbnail");
                return null;
            }

            int imageHeight = options.outHeight;
            int imageWidth = options.outWidth;
            String imageType = options.outMimeType;
            return thumbnail;

        }

        @Override
        public void onResume() {
            super.onResume();
            KitKitLogger logger = ((LibraryApplication) getActivity().getApplication()).getLogger();
            logger.tagScreen(TAG);
        }


        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            Log.d(TAG, "onCreateView bookFragment");
            setMarkTime();
            TreeMap<String, ArrayList<BookData>> data = new TreeMap<String, ArrayList<BookData>>();

            String bookDataFilename = "library_book_data.tsv";

            try {
                InputStream is;
                if (MainActivity.useExternalData) {
                    is = new FileInputStream(pathExternalAsset + File.separator + appLanguage + File.separator + bookDataFilename);
                } else {
                    is = getActivity().getAssets().open(appLanguage + File.separator + bookDataFilename);
                }


                BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                try {

                    String line;
                    while ((line = reader.readLine()) != null) {
                        if (line.charAt(0) == '#') continue;
                        String[] RowData = line.split("\t");
                        BookData bookData = new BookData();
                        bookData.id = RowData[0];
                        bookData.category = RowData[1];
                        bookData.categoryName = RowData[2];
                        bookData.title = RowData[3];
                        bookData.author = RowData[4];
                        bookData.thumbnail = appLanguage + File.separator + RowData[5];
                        bookData.foldername = RowData[6];

                        ArrayList<BookData> bookArray;

                        if (data.containsKey(bookData.category)) {
                            bookArray = data.get(bookData.category);
                            bookArray.add(bookData);
                        } else {
                            bookArray = new ArrayList<BookData>();
                            bookArray.add(bookData);
                            data.put(bookData.category, bookArray);
                        }

                    }
                } catch (IOException ex) {
                    // handle exception
                } finally {
                    try {
                        is.close();
                    } catch (IOException e) {
                        // handle exception
                    }
                }

            } catch (FileNotFoundException ex) {

            } catch (IOException ex) {

            }

            View rootView = inflater.inflate(R.layout.fragment_book, container, false);

            for (Map.Entry<String, ArrayList<BookData>> entry : data.entrySet()) {
                String category = entry.getKey();
                ArrayList<BookData> videoList = entry.getValue();

                TextView categoryTitle;
                RecyclerView recyclerView;

                if (category.equals("cate_2")) {
                    categoryTitle = rootView.findViewById(R.id.book1_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_1);

                } else if (category.equals("cate_3")) {
                    categoryTitle = rootView.findViewById(R.id.book2_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_2);

                } else if (category.equals("cate_4")) {
                    categoryTitle = rootView.findViewById(R.id.book3_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_3);

                } else if (category.equals("cate_5")) {
                    categoryTitle = rootView.findViewById(R.id.book4_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_4);

                } else if (category.equals("cate_6")) {
                    categoryTitle = rootView.findViewById(R.id.book5_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_5);

                } else if (category.equals("cate_7")) {
                    categoryTitle = rootView.findViewById(R.id.book6_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_6);

                } else if (category.equals("cate_8")) {
                    categoryTitle = rootView.findViewById(R.id.book7_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_7);

                } else if (category.equals("cate_9")) {
                    categoryTitle = rootView.findViewById(R.id.book8_title_textView);
                    recyclerView = rootView.findViewById(R.id.recycler_book_8);

                } else {
                    continue;
                }

                categoryTitle.setVisibility(View.VISIBLE);
                recyclerView.setVisibility(View.VISIBLE);

                categoryTitle.setText(videoList.get(0).categoryName);
                recyclerView.setLayoutManager(new LinearLayoutManager(getActivity(), RecyclerView.HORIZONTAL, false));
                recyclerView.addItemDecoration(new MainActivity.RecyclerViewDecoration(
                        MainActivity.dpTopx(7, getActivity()),
                        MainActivity.dpTopx(2, getActivity()),
                        MainActivity.dpTopx(7, getActivity()),
                        MainActivity.dpTopx(5, getActivity())));
                recyclerView.setAdapter(new MainActivity.BookAdapter(getActivity(), videoList));
            }

            getElapsedTime("book");
            return rootView;
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            Util.hideSystemUI(this);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "onCreate()");
        Util.hideSystemUI(this);

//        appLanguage = getAppLanguage().toLowerCase();
        appLanguage = "en-us";

        mbSignLanguageMode = isSignLanguageMode();
        if (appLanguage.startsWith("en") == false) {
            mbSignLanguageMode = false;
        }

        Log.i(TAG, "appLanguage : " + appLanguage + ", mbSignLanguageMode : " + mbSignLanguageMode);

        checkExternalData();
        setContentView(R.layout.activity_main);

        Toolbar toolbar = findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });


        TabLayout tabLayout = findViewById(R.id.tabLayout);
        tabLayout.setTabMode(TabLayout.MODE_FIXED);

        final CustomViewPager viewPager = findViewById(R.id.pager);
        final SectionsPagerAdapter adapter = new SectionsPagerAdapter(getFragmentManager());
        viewPager.setAdapter(adapter);
        viewPager.setPagingEnabled(false);
        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));
        tabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
            @Override
            public void onTabSelected(TabLayout.Tab tab) {
                //int position = tab.getPosition();
                // Switch to view for this tab
                viewPager.setCurrentItem(tab.getPosition());
            }

            @Override
            public void onTabUnselected(TabLayout.Tab tab) {

            }

            @Override
            public void onTabReselected(TabLayout.Tab tab) {

            }
        });


        Display display = getWindowManager().getDefaultDisplay();
        mWidth = display.getWidth(); // deprecated
        viewWidth = mWidth / 3;
        layouts = new ArrayList<LinearLayout>();
        params = new LayoutParams(viewWidth, LayoutParams.WRAP_CONTENT);

        viewPager.setCurrentItem(getIntent().getIntExtra("tab", 0));

    }

    private boolean isSignLanguageMode() {
        boolean result = false;
        try {
            Context context = createPackageContext("com.maq.xprize.kitkitlauncher", 0);
            //this seems working but Context.MODE_MULTI_PROCESS is deprecated since SDK 23. If it has problem, need to change to ContentProvider for sharing data.
            SharedPreferences pref = context.getSharedPreferences("sharedPref", Context.MODE_MULTI_PROCESS);
            result = pref.getBoolean("sign_language_mode_on", false);

        } catch (Exception e) {
            Log.e(TAG, "e : " + e);
        }

        return result;
    }

    private void checkExternalData() {
        useExternalData = true;
        pathExternalAsset = Environment.getDataDirectory() + "/data/" + getPackageName() + "/files/en-us/assets/";
        pathExternalRaw = Environment.getDataDirectory() + "/data/" + getPackageName() + "/files/en-us/raw/";
        pathExternalAsset = "storage/emulated/0/Android/data/" + getPackageName() + "/files/en-us/assets";
        pathExternalRaw = "storage/emulated/0/Android/data/" + getPackageName() + "/files/en-us/res/raw";
        Log.i(TAG, pathExternalAsset + "\n" + pathExternalRaw);
    }

    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            // getItem is called to instantiate the fragment for the given page.
            // Return a PlaceholderFragment (defined as a static inner class below).
            if (position == 0) {
                return VideoFragment.newInstance();
            } else {
                return BookFragment.newInstance();
            }

        }

        @Override
        public int getCount() {
            // Show 2 total pages.
            return 2;
        }

    }

    public static class VideoAdapter extends RecyclerView.Adapter<VideoAdapter.ViewHolder> {
        private Activity mActivity;
        private ArrayList<VideoData> mItems;

        public VideoAdapter(Activity activity, ArrayList<VideoData> items) {
            mActivity = activity;
            mItems = items;
        }

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(mActivity).inflate(R.layout.video_item, null);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, final int position) {
            final VideoData item = mItems.get(position);

            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    holder.itemView.setClipToOutline(true);
                }
            }

            holder.mTvTitle.setText(item.title);
            holder.mImageButton.setImageBitmap(null);

            if (useExternalData) {
                String thumbnailPath = "file://" + pathExternalAsset + File.separator + item.thumbnail;
                ImageLoader.getInstance().displayImage(thumbnailPath, holder.mImageButton);

            } else {
                ImageLoader.getInstance().displayImage("assets://" + item.thumbnail, holder.mImageButton);
            }

            holder.mImageButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(mActivity, VideoPlayerActivity.class);
                    intent.putExtra("videoArray", mItems);
                    intent.putExtra("libPath", pathExternalRaw);
                    intent.putExtra("libAssetPath", pathExternalAsset);
                    intent.putExtra("currentVideoIndex", position);

                    mActivity.startActivity(intent);
                    try {
                        KitKitLogger logger = ((LibraryApplication) mActivity.getApplication()).getLogger();
                        logger.logEvent("library", "start_video", item.title, 0);
                    } catch (NullPointerException ne) {
                        ne.printStackTrace();
                        Log.e(MainActivity.class.getName(), ne.getMessage());
                    } catch (Exception e) {
                        Log.e(MainActivity.class.getName(), e.getMessage());
                    }
                }
            });
        }

        @Override
        public int getItemCount() {
            return mItems.size();
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            public TextView mTvTitle;
            public ImageButton mImageButton;

            public ViewHolder(View view) {
                super(view);
                mTvTitle = view.findViewById(R.id.video_title);
                mImageButton = view.findViewById(R.id.video_image_button);
            }
        }
    }

    public static class BookAdapter extends RecyclerView.Adapter<BookAdapter.ViewHolder> {
        private Activity mActivity;
        private ArrayList<BookData> mItems;

        public BookAdapter(Activity activity, ArrayList<BookData> items) {
            mActivity = activity;
            mItems = items;
        }

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(mActivity).inflate(R.layout.book_item, null);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, final int position) {
            final BookData item = mItems.get(position);
            holder.mTvBookTitle.setText(item.title);
            holder.mTvBookAuthor.setText(item.author);
            holder.mImageButton.setImageBitmap(null);

            if (useExternalData) {
                String thumbnailPath = "file://" + pathExternalAsset + File.separator + item.thumbnail;
                ImageLoader.getInstance().displayImage(thumbnailPath, holder.mImageButton);

            } else {
                ImageLoader.getInstance().displayImage("assets://" + item.thumbnail, holder.mImageButton);

            }

            holder.mImageButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.maq.xprize.booktest", "org.cocos2dx.cpp.AppActivity"));

                        intent.putExtra("book", item.foldername);
                        Log.d("booktest", item.foldername);
                        mActivity.startActivity(intent);
                    } catch (Exception e) {
                        Log.d("MainActivity", "Line 732 " + e.getMessage());
                        // Display a toast before redirecting user to Google Play Store
                        Toast.makeText(v.getContext(), "You need Book Viewer app to read books", Toast.LENGTH_LONG).show();
                        (new Handler())
                                .postDelayed(
                                        new Runnable() {
                                            public void run() {
                                                // Redirect user to Google Play Store to download Book Viewer app
                                                mActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=com.maq.xprize.booktest")));
                                            }
                                        }, 3500);
                    }

                    try {
                        KitKitLogger logger = ((LibraryApplication) mActivity.getApplication()).getLogger();
                        logger.logEvent("library", "start_book", item.foldername, 0);

                    } catch (NullPointerException ne) {
                        ne.printStackTrace();
                        Log.e(MainActivity.class.getName(), ne.getMessage());
                    } catch (Exception e) {
                        Log.e(MainActivity.class.getName(), e.getMessage());
                    }
                }
            });
        }

        @Override
        public int getItemCount() {
            return mItems.size();
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            public TextView mTvBookTitle;
            public TextView mTvBookAuthor;
            public ImageButton mImageButton;

            public ViewHolder(View view) {
                super(view);

                mTvBookTitle = view.findViewById(R.id.book_title);
                mTvBookAuthor = view.findViewById(R.id.book_author);
                mImageButton = view.findViewById(R.id.book_image_button);
            }
        }
    }

    public static class RecyclerViewDecoration extends RecyclerView.ItemDecoration {
        private int mLeft;
        private int mTop;
        private int mRight;
        private int mBottom;

        public RecyclerViewDecoration(int left, int top, int right, int bottom) {
            mLeft = left;
            mTop = top;
            mRight = right;
            mBottom = bottom;
        }

        @Override
        public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
            super.getItemOffsets(outRect, view, parent, state);
            outRect.left = mLeft;
            outRect.top = mTop;
            outRect.right = mRight;
            outRect.bottom = mBottom;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    private static long mMarkTime;

    private static void setMarkTime() {
        mMarkTime = System.currentTimeMillis();
    }

    private static long getElapsedTime(String comment) {
        Log.i("myLog", comment + " : " + (System.currentTimeMillis() - mMarkTime) + "msec");
        return System.currentTimeMillis() - mMarkTime;
    }

}