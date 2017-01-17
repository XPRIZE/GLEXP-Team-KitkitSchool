package library.todoschool.enuma.com.todoschoollibrary;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Intent;

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
import java.util.TreeMap;

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


/**
 * Created by ingtellect on 11/18/16.
 */

public class MainActivity extends AppCompatActivity {

    LayoutParams params;
    int viewWidth;
    ArrayList<LinearLayout> layouts;
    int mWidth;

    static boolean useExternalData = false;
    private String TAG = "MainActivity";


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
        String lyrics;
        String info;
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
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
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
                    Log.d("TAG", Arrays.toString(getActivity().getAssets().list(".")));
                    is = getActivity().getAssets().open(videoDataFilename);
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
                        video.thumbnail = RowData[4];
                        video.filename = RowData[5];
                        video.lyrics = RowData[6];
                        video.info = RowData[7];

                        videoArray.add(video);

//                        Log.d("data-test", video.id + " " + video.category + " " + video.title + " " + video.thumbnail + " " + video.filename);

                        video.lyrics = video.lyrics.replaceAll("\\\\n ", "\n");
                        video.info = video.info.replaceAll("\\\\n ", "\n");
                        //Log.d("data-test","lyrics : " + video.lyrics);
                        //Log.d("data-test","info : "+ video.info);

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

            // dynamically add category - doesn't work
//            ViewGroup relativeLayout = (ViewGroup)rootView.findViewById(R.id.video_layout);
//
//            int viewId = 1;
//
//            for(Map.Entry<String, ArrayList<VideoData>> entry : data.entrySet()) {
//                String category = entry.getKey();
//                ArrayList<VideoData> videoList = entry.getValue();
//
//                TextView categoryTitle = new TextView(getContext());
//                categoryTitle.setText(category);
//                categoryTitle.setTextAppearance(R.style.CategoryHeaderText);
//                categoryTitle.setId(viewId++);
//                relativeLayout.addView(categoryTitle);
//
//                PercentRelativeLayout.LayoutParams textViewParams = new PercentRelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
//                        LayoutParams.WRAP_CONTENT);
//                textViewParams.setMarginStart(MainActivity.dpTopx(5,getContext()));
//                if (viewId != 2) {
//                    textViewParams.addRule(RelativeLayout.BELOW, viewId-2);
//                }
//
//                PercentLayoutHelper.PercentLayoutInfo info = textViewParams.getPercentLayoutInfo();
//                info.heightPercent = 0.05f;
//                categoryTitle.setLayoutParams(textViewParams);
//                categoryTitle.requestLayout();
//
//
//                HorizontalScrollView hsv = new HorizontalScrollView(getContext());
//                hsv.setId(viewId++);
//
//                PercentRelativeLayout.LayoutParams hsvParams = new PercentRelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT,
//                        LayoutParams.MATCH_PARENT);
//                hsvParams.addRule(RelativeLayout.BELOW, categoryTitle.getId());
//                PercentLayoutHelper.PercentLayoutInfo hsvParamsInfo = textViewParams.getPercentLayoutInfo();
//                hsvParamsInfo.heightPercent = 0.45f;
//                hsv.setLayoutParams(hsvParams);
//
//                relativeLayout.addView(hsv);
//
//                LinearLayout hsvWrapper = new LinearLayout(getContext());
//
//                LayoutParams hsvWrapperParams = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
//                hsvWrapper.setGravity(Gravity.CENTER_VERTICAL);
//                hsvWrapper.setOrientation(LinearLayout.HORIZONTAL);
//                hsvWrapper.setLayoutParams(hsvWrapperParams);
//                hsv.addView(hsvWrapper);
//                boolean isFirst = true;
//                for (VideoData video : videoList) {
//                    View child = LayoutInflater.from(getContext()).inflate(R.layout.video_item, null);
//                    LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
//                    int left = MainActivity.dpTopx(5,getContext());
//                    int top=MainActivity.dpTopx(2,getContext());;
//                    int right=MainActivity.dpTopx(5,getContext());;
//                    int bottom=MainActivity.dpTopx(5,getContext());;
//                    if (isFirst) {
//                        left = MainActivity.dpTopx(17,getContext());;
//                        isFirst = false;
//                    }
//
//                    layoutParams.setMargins(left,top,right,bottom);
//                    child.setLayoutParams(layoutParams);
//
//                    hsvWrapper.addView(child);
//
//                    child.findViewById(getResources().getIdentifier("roundedrect", "id", getActivity().getPackageName()));
//
//                    ImageButton imageButton = (ImageButton) child.findViewById(getResources().getIdentifier("video_image_button","id",getActivity().getPackageName()));
//                    imageButton.setTag(libPath + File.separator + video.filename);
//                    String thumbnailPath = libPath + File.separator + video.thumbnail;
//                    Drawable thumbnail = Drawable.createFromPath(thumbnailPath);
//                    imageButton.setImageDrawable(thumbnail);
//                    imageButton.setOnClickListener(new View.OnClickListener() {
//                        @Override
//                        public void onClick(View view) {
//                            Intent intent = new Intent(getActivity(), VideoPlayerActivity.class);
//                            Log.d("MainActivity", "tag : "+view.getTag().toString());
//                            intent.putExtra("video",view.getTag().toString());
//                            startActivity(intent);
//                        }
//                    });
//                }
//
//
//            }


            for(Map.Entry<String, ArrayList<VideoData>> entry : data.entrySet()) {
                String category = entry.getKey();
                final ArrayList<VideoData> videoList = entry.getValue();

                LinearLayout hsvWrapper;
                TextView categoryTitle;

                if (category.equals("tutorial")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.tutorial_title_textView);
                }
                else if(category.equals("music_video")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv2_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.musicvideo_title_textView);
                }
                else {
                    continue;
                }

                boolean isFirst = true;
                for (VideoData video : videoList) {
                    View child = LayoutInflater.from(getContext()).inflate(R.layout.video_item, null);
                    LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                    int left = MainActivity.dpTopx(9,getContext());
                    int top=MainActivity.dpTopx(2,getContext());
                    int right=MainActivity.dpTopx(5,getContext());
                    int bottom=MainActivity.dpTopx(5,getContext());
                    if (isFirst) {
                        left = MainActivity.dpTopx(5,getContext());
                        categoryTitle.setText(video.categoryName);
                        isFirst = false;
                    }

                    layoutParams.setMargins(left,top,right,bottom);
                    child.setLayoutParams(layoutParams);

                    hsvWrapper.addView(child);

                    child.findViewById(getResources().getIdentifier("roundedrect", "id", getActivity().getPackageName()));
                    child.setClipToOutline(true);

                    TextView videoTitle = (TextView) child.findViewById(R.id.video_title);
                    videoTitle.setText(video.title);

                    ImageButton imageButton = (ImageButton) child.findViewById(getResources().getIdentifier("video_image_button","id",getActivity().getPackageName()));
                    //imageButton.setTag(1, libPath + File.separator + video.filename);
                    imageButton.setTag(video);
                    Drawable thumbnail = null;
                    if (useExternalData) {
                        String thumbnailPath = libPath + File.separator + video.thumbnail;
                        thumbnail = Drawable.createFromPath(thumbnailPath);
                    }
                    else {
                        try {
                            thumbnail = Drawable.createFromStream(getContext().getAssets().open(video.thumbnail), null);
                        }
                        catch (IOException ex) {

                        }
                    }
                    imageButton.setImageDrawable(thumbnail);
                    imageButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Intent intent = new Intent(getActivity(), VideoPlayerActivity.class);
//                            Log.d("MainActivity", "tag : "+view.getTag(1).toString());
                            VideoData video = (VideoData)view.getTag();

                            intent.putExtra("videoArray", videoArray);
                            intent.putExtra("libPath", libPath);

                            int curVideoIndex = videoArray.indexOf(video);
                            intent.putExtra("currentVideoIndex",curVideoIndex);

                            startActivity(intent);
                        }
                    });
                }


            }

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
        TreeMap<String, BookData> bookDataTreeMap;

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {

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
                    is = getContext().getAssets().open(bookDataFilename);
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
                        bookData.thumbnail = RowData[5];
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

            boolean cate4Exist = false;

            for(Map.Entry<String, ArrayList<BookData>> entry : data.entrySet()) {
                String category = entry.getKey();
                ArrayList<BookData> videoList = entry.getValue();

                LinearLayout hsvWrapper;
                TextView categoryTitle;

                if (category.equals("cate_1")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.book1_title_textView);
                } else if (category.equals("cate_2")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv2_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.book2_title_textView);
                } else if (category.equals("cate_3")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv3_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.book3_title_textView);
                } else if (category.equals("cate_4")) {
                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv4_wrapper);
                    categoryTitle = (TextView) rootView.findViewById(R.id.book4_title_textView);
                    cate4Exist = true;
//                } else if (category.equals("cate_5")) {
//                    hsvWrapper = (LinearLayout) rootView.findViewById(R.id.hsv5_wrapper);
//                    categoryTitle = (TextView) rootView.findViewById(R.id.book5_title_textView);
                } else {
                    continue;
                }


                boolean isFirst = true;
                for (BookData book : videoList) {
                    View child = LayoutInflater.from(getContext()).inflate(R.layout.book_item, null);
                    LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                    int left = MainActivity.dpTopx(9,getContext());
                    int top=MainActivity.dpTopx(2,getContext());
                    int right=MainActivity.dpTopx(5,getContext());
                    int bottom=MainActivity.dpTopx(5,getContext());
                    if (isFirst) {
                        left = MainActivity.dpTopx(5,getContext());
                        categoryTitle.setText(book.categoryName);
                        isFirst = false;
                    }

                    layoutParams.setMargins(left,top,right,bottom);
                    child.setLayoutParams(layoutParams);

                    hsvWrapper.addView(child);

                    child.findViewById(getResources().getIdentifier("roundedrect", "id", getActivity().getPackageName()));
                    child.setClipToOutline(true);

                    TextView bookTitle = (TextView) child.findViewById(R.id.book_title);
                    bookTitle.setText(book.title);

                    TextView bookAuthor = (TextView) child.findViewById(R.id.book_author);
                    bookAuthor.setText(book.author);

                    ImageButton imageButton = (ImageButton) child.findViewById(getResources().getIdentifier("book_image_button","id",getActivity().getPackageName()));
                    imageButton.setTag(book.foldername);
                    Drawable thumbnail = null;
                    if (useExternalData) {
                        String thumbnailPath = libPath + File.separator + book.thumbnail;
                        thumbnail = Drawable.createFromPath(thumbnailPath);
                    }
                    else {
                        try {
                            thumbnail = Drawable.createFromStream(getContext().getAssets().open(book.thumbnail), null);
                        }
                        catch (IOException ex) {

                        }
                    }

                    imageButton.setImageDrawable(thumbnail);
                    imageButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                        Intent intent = getContext().getPackageManager().getLaunchIntentForPackage("com.enuma.booktest");
                        intent.putExtra("book",view.getTag().toString());
                        Log.d("booktest", view.getTag().toString());
                        startActivity(intent);

                        }
                    });
                }


            }

            if (!cate4Exist) {
                View hsv4 = rootView.findViewById(R.id.hsv4);
                TextView categoryTitle = (TextView) rootView.findViewById(R.id.book4_title_textView);
                hsv4.setVisibility(View.GONE);
                categoryTitle.setVisibility(View.GONE);
            }

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

        hideSystemUI();

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


    }

//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.main, menu);
//        return true;
//    }

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


}
