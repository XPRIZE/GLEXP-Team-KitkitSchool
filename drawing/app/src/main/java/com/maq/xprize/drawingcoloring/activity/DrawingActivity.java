package com.maq.xprize.drawingcoloring.activity;

import android.Manifest;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;

import com.maq.xprize.drawingcoloring.core.Const;
import com.maq.xprize.drawingcoloring.utility.Preference;
import com.maq.xprize.drawingcoloring.view.ViewDrawingColoring;

public class DrawingActivity extends DrawingColoringActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 99);
        setMode(ViewDrawingColoring.MODE.DRAWING);
        setBGColor(mPreference.getConfig(Const.PARAM_BG_COLOR_INDEX, 0), false);
        setPenColor(mPreference.getConfig(Const.PARAM_PEN_COLOR_INDEX_IN_DRAWING, 1), false);

        mbNeedSave = mPreference.getConfig(Const.PARAM_NEED_SAVE_IN_DRAWING, false);
        checkSaveButton();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mPreference.setConfig(Const.PARAM_NEED_SAVE_IN_DRAWING, mbNeedSave);
    }
}
