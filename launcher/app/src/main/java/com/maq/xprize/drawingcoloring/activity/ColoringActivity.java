package com.maq.xprize.drawingcoloring.activity;

import android.Manifest;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.view.View;

import com.maq.xprize.drawingcoloring.core.Const;
import com.maq.xprize.drawingcoloring.utility.Misc;
import com.maq.xprize.drawingcoloring.view.ViewDrawingColoring;

import java.io.File;

public class ColoringActivity extends DrawingColoringActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 99);
        setMode(ViewDrawingColoring.MODE.COLORING);
        setBGColor(0, false);
        setPenColor(mPreference.getConfig(Const.PARAM_PEN_COLOR_INDEX_IN_COLORING, 1), false);

        mIvColoring.setVisibility(View.VISIBLE);

        int imageIndex = mPreference.getConfig(Const.PARAM_BG_IMAGE_INDEX, 0);
        if (mImagesColoring != null && mImagesColoring.size() > 0) {
            if (mImagesColoring.size() > imageIndex) {
                setColoringImage(imageIndex, false);

            } else {
                // 개발, 리소스 테스트시 오류 예외 처리
                setColoringImage(0, false);
                File tempFile = new File(Misc.getTempFilePath(ViewDrawingColoring.MODE.COLORING));
                if (tempFile.exists() == true) {
                    tempFile.delete();
                }
                mPreference.setConfig(Const.PARAM_NEED_SAVE_IN_COLORING, false);
            }
        }

        mbNeedSave = mPreference.getConfig(Const.PARAM_NEED_SAVE_IN_COLORING, false);
        checkSaveButton();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mPreference.setConfig(Const.PARAM_NEED_SAVE_IN_COLORING, mbNeedSave);
    }
}
