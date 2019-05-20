package com.maq.xprize.kitkitlibrary.english;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;

import com.maq.kitkitlogger.KitKitLoggerActivity;
/**
 * Created by ingtellect on 12/23/16.
 */

public class BookDetailActivity extends KitKitLoggerActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
        setContentView(R.layout.activity_bookdetail);

        View roundedRectView = findViewById(R.id.bg);
        roundedRectView.setClipToOutline(true);


    }
}
