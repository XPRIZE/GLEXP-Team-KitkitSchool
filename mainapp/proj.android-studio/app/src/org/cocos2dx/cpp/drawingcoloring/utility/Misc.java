package org.cocos2dx.cpp.drawingcoloring.utility;

import android.app.Activity;
import android.content.Intent;

import org.cocos2dx.cpp.drawingcoloring.activity.DrawingColoringActivity;
import org.cocos2dx.cpp.drawingcoloring.core.Const;
import org.cocos2dx.cpp.drawingcoloring.view.ViewDrawingColoring;

import java.io.File;

public class Misc {
    public static void gotoDrawingColoring(Activity activity) {
        Intent intent = new Intent(activity, DrawingColoringActivity.class);
        activity.startActivity(intent);
    }

    public static String getTempFilePath(ViewDrawingColoring.MODE mode) {
        return Const.FOLDER_TEMP + File.separator + (mode == ViewDrawingColoring.MODE.DRAWING ? Const.FILENAME_TEMP_DRAWING : Const.FILENAME_TEMP_COLORING);
    }
}
