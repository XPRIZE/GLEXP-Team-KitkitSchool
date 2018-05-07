package library.todoschool.enuma.com.todoschoollibrary;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.DisplayMetrics;
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

import com.enuma.kitkitlogger.KitKitLogger;
import com.enuma.kitkitlogger.KitKitLoggerActivity;
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
import java.util.Arrays;
import java.util.Map;
import java.util.StringTokenizer;
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
    };

    public static class VideoData implements Serializable
    {
        String id;
        String category;
        String categoryName;
        String title;
        String thumbnail;
        String filename;
        String lyrics = "";
        String info = "";
    }

    public static class BookData
    {
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
            KitKitLogger logger = ((LibraryApplication)getActivity().getApplication()).getLogger();
            logger.tagScreen(TAG);
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            Log.d(TAG,"onCreateView videoFragment");
            setMarkTime();
            Boolean foundVideoFiles;

            TreeMap<String, ArrayList<VideoData>> data = new TreeMap<String, ArrayList<VideoData>>();

            File videoFolder =
                    Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES)
                            .getAbsoluteFile();
            //File videoFolder = getContext().getFilesDir().getAbsoluteFile();

            final String libPath = videoFolder.getAbsolutePath() + File.separator + "Library";



            File libFolder = new File(libPath);
            String videoDataFilename = "library_video_data.tsv";

            String dataPath = libPath + File.separator + videoDataFilename;
            File dataFile = new File(dataPath);
            videoArray = new ArrayList<>();
            try {
                InputStream is;
                if (MainActivity.useExternalData) {
                    is = new FileInputStream(dataFile);
                }
                else {
                    Log.i(TAG, "path : " + (appLanguage +  File.separator + videoDataFilename));
                    is = getActivity().getAssets().open(appLanguage +  File.separator + videoDataFilename);
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
                        video.thumbnail = appLanguage +  File.separator + RowData[4];
                        video.filename = RowData[5];
//                        video.lyrics = RowData[6];
//                        video.info = RowData[7];

                        videoArray.add(video);
                        video.lyrics = video.lyrics.replaceAll("\\\\n ", "\n").replaceAll("\\\\n","\n");
                        video.info = video.info.replaceAll("\\\\n ", "\n").replaceAll("\\\\n","\n");

                        ArrayList<VideoData> videoArrayForMap;

                        if(data.containsKey(video.category)) {
                            videoArrayForMap = data.get(video.category);
                            videoArrayForMap.add(video);
                        }
                        else {
                            videoArrayForMap = new ArrayList<VideoData>();
                            videoArrayForMap.add(video);
                            data.put(video.category, videoArrayForMap);
                        }

                    }
                }
                catch (IOException ex) {
                    // handle exception
                    Log.e(TAG,"IOException");
                }
                finally {
                    try {
                        is.close();
                    }
                    catch (IOException e) {
                        // handle exception
                    }
                }

            }
            catch (FileNotFoundException ex) {
                Log.e(TAG,"File not found exception");

            }
            catch (IOException ex) {

            }


            View rootView = inflater.inflate(R.layout.fragment_video, container, false);

            for(Map.Entry<String, ArrayList<VideoData>> entry : data.entrySet()) {
                String category = entry.getKey();
                final ArrayList<VideoData> videoList = entry.getValue();

                TextView categoryTitle;
                RecyclerView recyclerView;

                if (category.equals("tutorial")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.tutorial_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_video_0);
                }
                else if(category.equals("music_video")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.musicvideo_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_video_1);
                }
                else if(category.equals("literacy_video")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.literacy_video_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_video_2);
                }
                else if(category.equals("math_video")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.math_video_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_video_3);

                } else if(category.equals("alphabet_video")) {
                        categoryTitle = (TextView) rootView.findViewById(R.id.alphabet_video_title_textView);
                        recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_video_4);

                }
                else {
                    continue;
                }

                categoryTitle.setText(videoList.get(0).categoryName);
                recyclerView.setLayoutManager(new LinearLayoutManager(getActivity(), RecyclerView.HORIZONTAL, false));
                recyclerView.addItemDecoration(new MainActivity.RecyclerViewDecoration(
                        MainActivity.dpTopx(7,getContext()),
                        MainActivity.dpTopx(2,getContext()),
                        MainActivity.dpTopx(7,getContext()),
                        MainActivity.dpTopx(5,getContext())));
                recyclerView.setAdapter(new MainActivity.VideoAdapter(getActivity(), libPath, videoList));
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

        public Bitmap loadBookThumbnail(BookData bookData)
        {
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
                thumbnail = BitmapFactory.decodeStream(getContext().getAssets().open(filename), null, options);

            }
            catch (IOException e) {
                Log.e(TAG,"IOException in book thumbnail");
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
            KitKitLogger logger = ((LibraryApplication)getActivity().getApplication()).getLogger();
            logger.tagScreen(TAG);
        }


        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            Log.d(TAG,"onCreateView bookFragment");
            setMarkTime();
            TreeMap<String, ArrayList<BookData>> data = new TreeMap<String, ArrayList<BookData>>();

            File docsFolder =
                    Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS)
                            .getAbsoluteFile();
            //File docsFolder = getContext().getFilesDir().getAbsoluteFile();

            String libPath = docsFolder.getAbsolutePath() + File.separator + "Books";

            File libFolder = new File(libPath);
            String bookDataFilename = "library_book_data.tsv";

            String dataPath = libPath + File.separator + bookDataFilename;
            Log.d("MainActivity",dataPath);
            File dataFile = new File(dataPath);
            try {
                InputStream is;
                if (MainActivity.useExternalData) {
                    is = new FileInputStream(dataFile);
                }
                else {
                    is = getContext().getAssets().open(appLanguage +  File.separator + bookDataFilename);
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
                        bookData.thumbnail = appLanguage +  File.separator + RowData[5];
                        bookData.foldername = RowData[6];

                        ArrayList<BookData> bookArray;

                        if(data.containsKey(bookData.category)) {
                            bookArray = data.get(bookData.category);
                            bookArray.add(bookData);
                        }
                        else {
                            bookArray = new ArrayList<BookData>();
                            bookArray.add(bookData);
                            data.put(bookData.category, bookArray);
                        }

                    }
                }
                catch (IOException ex) {
                    // handle exception
                }
                finally {
                    try {
                        is.close();
                    }
                    catch (IOException e) {
                        // handle exception
                    }
                }

            }
            catch (FileNotFoundException ex) {

            }
            catch (IOException ex) {

            }

            View rootView = inflater.inflate(R.layout.fragment_book, container, false);

            for(Map.Entry<String, ArrayList<BookData>> entry : data.entrySet()) {
                String category = entry.getKey();
                ArrayList<BookData> videoList = entry.getValue();

                TextView categoryTitle;
                RecyclerView recyclerView;

                if (category.equals("cate_2")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book1_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_1);

                } else if (category.equals("cate_3")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book2_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_2);

                } else if (category.equals("cate_4")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book3_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_3);

                } else if (category.equals("cate_5")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book4_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_4);

                } else if (category.equals("cate_6")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book5_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_5);

                } else if (category.equals("cate_7")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book6_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_6);

                } else if (category.equals("cate_8")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book7_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_7);

                } else if (category.equals("cate_9")) {
                    categoryTitle = (TextView) rootView.findViewById(R.id.book8_title_textView);
                    recyclerView = (RecyclerView) rootView.findViewById(R.id.recycler_book_8);

                } else {
                    continue;
                }

                categoryTitle.setVisibility(View.VISIBLE);
                recyclerView.setVisibility(View.VISIBLE);

                categoryTitle.setText(videoList.get(0).categoryName);
                recyclerView.setLayoutManager(new LinearLayoutManager(getActivity(), RecyclerView.HORIZONTAL, false));
                recyclerView.addItemDecoration(new MainActivity.RecyclerViewDecoration(
                        MainActivity.dpTopx(7,getContext()),
                        MainActivity.dpTopx(2,getContext()),
                        MainActivity.dpTopx(7,getContext()),
                        MainActivity.dpTopx(5,getContext())));
                recyclerView.setAdapter(new MainActivity.BookAdapter(getActivity(), libPath, videoList));
            }

            getElapsedTime("book");
            return rootView;
        }
    }

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_FULLSCREEN
                |View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                |View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            hideSystemUI();
        }


    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG,"onCreate()");
        hideSystemUI();

        appLanguage = getAppLanguage();
        Log.i(TAG, "appLanguage : " + appLanguage);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setNavigationIcon(R.drawable.library_icon_back);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onBackPressed();
            }
        });


        TabLayout tabLayout = (TabLayout) findViewById(R.id.tabLayout);
        tabLayout.setTabMode(TabLayout.MODE_FIXED);

        final CustomViewPager viewPager = (CustomViewPager) findViewById(R.id.pager);
        final SectionsPagerAdapter adapter = new SectionsPagerAdapter(getSupportFragmentManager());
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

        viewPager.setCurrentItem(getIntent().getIntExtra("tab",0));

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
        private String mLibraryPath = "";
        public VideoAdapter(Activity activity, String libraryPath, ArrayList<VideoData> items) {
            mActivity = activity;
            mLibraryPath = libraryPath;
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
            holder.itemView.setClipToOutline(true);
            holder.mTvTitle.setText(item.title);

            if (useExternalData) {
                String thumbnailPath = mLibraryPath + File.separator + item.thumbnail;
                ImageLoader.getInstance().displayImage(thumbnailPath, holder.mImageButton);

            } else {
                ImageLoader.getInstance().displayImage("assets://" + item.thumbnail, holder.mImageButton);
            }

            holder.mImageButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(mActivity, VideoPlayerActivity.class);
                    intent.putExtra("videoArray", mItems);
                    intent.putExtra("libPath", mLibraryPath);
                    intent.putExtra("currentVideoIndex", position);

                    mActivity.startActivity(intent);
                    try {
                        KitKitLogger logger = ((LibraryApplication)mActivity.getApplication()).getLogger();
                        logger.logEvent("library","start_video",item.title,0);
                    }
                    catch (NullPointerException ne) {
                        ne.printStackTrace();
                        Log.e(MainActivity.class.getName(), ne.getMessage());
                    }
                    catch (Exception e) {
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
                mTvTitle = (TextView) view.findViewById(R.id.video_title);
                mImageButton = (ImageButton) view.findViewById(R.id.video_image_button);
            }
        }
    }

    public static class BookAdapter extends RecyclerView.Adapter<BookAdapter.ViewHolder> {
        private Activity mActivity;
        private ArrayList<BookData> mItems;
        private String mLibraryPath = "";
        public BookAdapter(Activity activity, String libraryPath, ArrayList<BookData> items) {
            mActivity = activity;
            mLibraryPath = libraryPath;
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

            if (useExternalData) {
                String thumbnailPath = mLibraryPath + File.separator + item.thumbnail;
                ImageLoader.getInstance().displayImage(thumbnailPath, holder.mImageButton);

            } else {
                ImageLoader.getInstance().displayImage("assets://" + item.thumbnail, holder.mImageButton);

            }

            holder.mImageButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.setComponent(new ComponentName("com.enuma.booktest", "org.cocos2dx.cpp.AppActivity"));

                        intent.putExtra("book", item.foldername);
                        Log.d("booktest", item.foldername);
                        mActivity.startActivity(intent);
                    } catch (Exception e) {
                        Toast.makeText(v.getContext(), "Error", Toast.LENGTH_SHORT).show();
                    }



                    try {
                        KitKitLogger logger = ((LibraryApplication)mActivity.getApplication()).getLogger();
                        logger.logEvent("library","start_book",  item.foldername, 0);

                    }
                    catch (NullPointerException ne) {
                        ne.printStackTrace();
                        Log.e(MainActivity.class.getName(), ne.getMessage());
                    }
                    catch (Exception e) {
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

                mTvBookTitle = (TextView) view.findViewById(R.id.book_title);
                mTvBookAuthor = (TextView) view.findViewById(R.id.book_author);
                mImageButton = (ImageButton) view.findViewById(R.id.book_image_button);
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
