package com.maq.xprize.drawingcoloring.core;

import android.os.Environment;

public class Const {

    public static final String COLORING_IMAGE_PATH_FROM_ASSETS = "coloring";

    public static final String SAVE_PATH = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath();

    public static String FOLDER_TEMP = "";
    public static final String FILENAME_TEMP_DRAWING = "temp_drawing.png";
    public static final String FILENAME_TEMP_COLORING = "temp_coloring.png";

    public static int LCD_GROUP = -1;

    public static String PARAM_BG_COLOR_INDEX = "BG_COLOR_INDEX";

    public static String PARAM_BG_IMAGE_INDEX = "BG_IMAGE_INDEX";

    public static String PARAM_PEN_COLOR_INDEX_IN_DRAWING = "PEN_COLOR_INDEX_IN_DRAWING";

    public static String PARAM_PEN_COLOR_INDEX_IN_COLORING = "PEN_COLOR_INDEX_IN_COLORING";

    public static String PARAM_NEED_SAVE_IN_DRAWING = "NEED_SAVE_IN_DRAWING";
    public static String PARAM_NEED_SAVE_IN_COLORING = "NEED_SAVE_IN_COLORING";
}
