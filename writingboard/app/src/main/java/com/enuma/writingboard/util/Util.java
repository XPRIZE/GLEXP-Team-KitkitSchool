package com.enuma.writingboard.util;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.TypedValue;
import android.view.View;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import static android.content.Context.ACTIVITY_SERVICE;

public class Util {
    public static int getMemoryClass(Activity activity) {
        ActivityManager activityManager = (ActivityManager) activity.getSystemService(ACTIVITY_SERVICE);
        return activityManager.getMemoryClass();
    }

    public static int getPixel(Context context, int dp) {
        float px = 0;
        try {
            px = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, context.getResources().getDisplayMetrics());
        } catch (Exception e) {

        }
        return (int) px;
    }

    public static int getPixel(Context context, float dp) {
        float px = 0;
        try {
            px = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, context.getResources().getDisplayMetrics());
        } catch (Exception e) {

        }
        return (int) px;
    }

    public static double getDistanceBetween2Point(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
    }

    public static double getRadianAngleBetween2Point(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        return Math.atan2(dx, dy);
    }

    public static Point getWindowSize(Activity activity) {
        Point result = new Point();
        activity.getWindowManager().getDefaultDisplay().getRealSize(result);
        return result;
    }

    public static String getFileExtension(String filePath) {
        String result = "";

        try {
            result = filePath.substring(filePath.lastIndexOf(".") + 1, filePath.length());
        } catch (Exception e) {
            result = "";
        }

        return result;
    }

    public static boolean saveImageFileFromBitmap(Bitmap bitmap, String outputFilePath, int quality) {
        if (bitmap == null) {
            return false;
        }

        File file = new File(outputFilePath);
        File folder = new File(file.getParent());
        if (folder.exists() == false) {
            folder.mkdirs();
        }

        try {
            OutputStream os = new FileOutputStream(file);
            String extension = getFileExtension(outputFilePath);

            if (extension.equalsIgnoreCase("png") == true) {
                bitmap.compress(Bitmap.CompressFormat.PNG, quality, os);
            } else {
                bitmap.compress(Bitmap.CompressFormat.JPEG, quality, os);
            }
        } catch (FileNotFoundException e) {
            Log.e(e.toString());
            return false;
        } catch (Exception e) {
            Log.e(e.toString());
            return false;
        }

        return true;
    }

    public static boolean saveImageFileFromView(View view, String outputFilePath, float scale, int quality) {
        boolean result = false;

        if (view == null || view.getWidth() == 0 || view.getHeight() == 0) {
            return false;
        }

        int width = (int)(view.getWidth() * scale + 0.5f);
        int height = (int)(view.getHeight() * scale + 0.5f);

        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(bitmap);
        canvas.save();
        canvas.scale(scale, scale);
        view.draw(canvas);
        canvas.restore();
        result = saveImageFileFromBitmap(bitmap, outputFilePath, quality);

        if (bitmap != null) {
            bitmap.recycle();
            bitmap = null;
        }
        return result;
    }

    public static Bitmap getBitmapFromFile(String path) {
        Bitmap result = null;

        try {
            FileInputStream fileInputStream = new FileInputStream(path);
            result = BitmapFactory.decodeStream(fileInputStream);

        } catch (IOException ex) {
            Log.e("", ex);
        }

        return result;
    }

    public static Bitmap getBitmapFromAssets(Context context, String path) {
        Bitmap result = null;

        try {
            InputStream is = context.getAssets().open(path);
            result = BitmapFactory.decodeStream(is);

        } catch (IOException ex) {
            Log.e("", ex);
        }

        return result;
    }

    public static String[] getFileListFromAssets(Context context, String parentsPath) {
        String[] result = null;

        try {
            result = context.getAssets().list(parentsPath);

        } catch (IOException ex) {
            Log.e("", ex);
        }

        return result;
    }

    public static void makeFolder(String path) {

        File folder = new File(path);
        if (folder.exists() == false) {
            folder.mkdirs();
        }

    }

    public static void makeNoMedia(String path) {
        makeFolder(path);

        String file = path + "/" + ".nomedia";
        File f = new File(file);
        if (f.exists() == false) {
            try {
                f.createNewFile();

            } catch (Exception e) {
                Log.e(".NOMEDIA create failed. : " + e + "," + f.getAbsolutePath());

            }
        }
    }
}
