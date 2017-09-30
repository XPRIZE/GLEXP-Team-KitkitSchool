package com.enuma.drawingcoloring.activity;

import android.os.Bundle;

import com.enuma.drawingcoloring.core.Const;
import com.enuma.drawingcoloring.utility.Preference;
import com.enuma.drawingcoloring.view.ViewDrawingColoring;

public class DrawingActivity extends DrawingColoringActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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
