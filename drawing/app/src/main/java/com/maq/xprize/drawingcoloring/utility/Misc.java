package com.maq.xprize.drawingcoloring.utility;

import android.app.Activity;
import android.content.Intent;

import com.maq.xprize.drawingcoloring.activity.DrawingColoringActivity;
import com.maq.xprize.drawingcoloring.view.ViewDrawingColoring;

import java.io.File;

import com.maq.xprize.drawingcoloring.core.Const;

public class Misc {
    public static void gotoDrawingColoring(Activity activity) {
        Intent intent = new Intent(activity, DrawingColoringActivity.class);
        activity.startActivity(intent);
    }

    public static String getTempFilePath(ViewDrawingColoring.MODE mode) {
        return Const.FOLDER_TEMP + File.separator + (mode == ViewDrawingColoring.MODE.DRAWING ? Const.FILENAME_TEMP_DRAWING : Const.FILENAME_TEMP_COLORING);
    }
}
