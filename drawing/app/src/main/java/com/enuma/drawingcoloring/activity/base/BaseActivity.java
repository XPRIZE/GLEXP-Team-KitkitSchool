package com.enuma.drawingcoloring.activity.base;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.enuma.drawingcoloring.core.Const;
import com.enuma.drawingcoloring.utility.Util;

public class BaseActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Const.FOLDER_TEMP = getExternalFilesDir(null).getAbsolutePath();
        if (Const.LCD_GROUP == -1) {
            Const.LCD_GROUP = Util.getLCDGroup(this);
        }
    }

}
