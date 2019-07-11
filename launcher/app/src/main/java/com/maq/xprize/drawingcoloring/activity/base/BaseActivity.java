package com.maq.xprize.drawingcoloring.activity.base;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;

import com.maq.xprize.drawingcoloring.core.Const;
import com.maq.xprize.drawingcoloring.utility.Util;

public class BaseActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        String state = Environment.getExternalStorageState();

        if (Environment.MEDIA_MOUNTED.equals(state) == true) {
            Const.FOLDER_TEMP = getExternalFilesDir(null).getAbsolutePath();

        } else {
            Const.FOLDER_TEMP = getFilesDir().getAbsolutePath();

        }

        if (Const.LCD_GROUP == -1) {
            Const.LCD_GROUP = Util.getLCDGroup(this);
        }
    }

}
