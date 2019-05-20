package com.maq.xprize.writingboard.core;

import android.os.Environment;

import java.io.File;

public class Const {
    public static final String SAVE_IMAGE_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "writingboard" + File.separator + "drawing" + File.separator;
    public static final String SAVE_LOG_IMAGE_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "writingboard" + File.separator + "log_image" + File.separator;
    public static final String IMAGE_PATH_FROM_ASSETS = "image";
}
