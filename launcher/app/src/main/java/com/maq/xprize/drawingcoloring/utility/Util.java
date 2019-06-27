package com.maq.xprize.drawingcoloring.utility;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Util {
    private static long mMarkTime;

    public static void setMarkTime() {
        mMarkTime = System.currentTimeMillis();
    }

    public static long getElapsedTime() {
        Log.i(":" + (System.currentTimeMillis() - mMarkTime));
        return System.currentTimeMillis() - mMarkTime;
    }

    public static long getElapsedTime(String comment) {
        Log.i(comment + " : " + (System.currentTimeMillis() - mMarkTime) + "msec");
        return System.currentTimeMillis() - mMarkTime;
    }

    public static DisplayMetrics getWindowInfo(Activity activity) {
        DisplayMetrics result = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(result);
        return result;
    }

    /**
     * 단말의 normal, large, xlarge 판별
     * @param context
     * @return 0 : normal, 1 : large, 2 : xlarge
     */
    public static int getLCDGroup(Context context) {
        boolean xlarge = ((context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) == Configuration.SCREENLAYOUT_SIZE_XLARGE);
        boolean large = ((context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) == Configuration.SCREENLAYOUT_SIZE_LARGE);

        if (xlarge == true) {
            return 2;
        } else if (large == true) {
            return 1;
        }

        return 0;
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

    public static int getDip(Context context, int pixel) {
        float dp = 0;
        try {
            DisplayMetrics metrics = context.getResources().getDisplayMetrics();
            dp = pixel / (metrics.densityDpi / 160f);
        } catch (Exception e) {

        }
        return (int) dp;
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

    public static int getResourceId(Context context, String name, String resourceName, String packageName) {
        try {
            return context.getResources().getIdentifier(name, resourceName, packageName);
        } catch (Exception e) {
            Log.e("", e);
            return -1;
        }
    }

    /**
     * <pre>
     * 지정한 FilePath 에서 확장자를 뽑아온다.
     * 확장자를 찾지 못할 경우 빈문자열을 return 한다.
     * </pre>
     *
     * @param filePath
     * @return
     */
    public static String getFileExtension(String filePath) {
        String result = "";

        try {
            result = filePath.substring(filePath.lastIndexOf(".") + 1, filePath.length());
        } catch (Exception e) {
            result = "";
        }

        return result;
    }

    /**
     * <pre>
     * 지정한 파일위치에 Bitmap 객체를 파일로 저장한다.
     * 지정한 파일의 확장자가 png (PNG)일 경우 PNG 포맷으로 저장하고 그 이외일 경우 JPEG 포맷으로 저장한다.
     * quality 는 png 는 100%, jpeg 는 설정값으로 적용된다.
     * </pre>
     *
     * @param bitmap         저장할 비트맵 객체
     * @param outputFilePath 저장할 파일위치
     * @param quality        0 ~ 100%
     * @return
     */
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

    /**
     * <pre>
     * 지정한 파일위치에 Bitmap 객체를 파일로 저장한다.
     * 지정한 파일의 확장자가 png (PNG)일 경우 PNG 포맷으로 저장하고 그 이외일 경우 JPEG 포맷으로 저장한다.
     * quality 는 png 는 100%, jpeg 는 90% 로 설정된다.
     * </pre>
     *
     * @param bitmap         저장할 비트맵 객체
     * @param outputFilePath 저장할 파일위치
     * @return
     */
    public static boolean saveImageFileFromBitmap(Bitmap bitmap, String outputFilePath) {
        return saveImageFileFromBitmap(bitmap, outputFilePath, 90);
    }

    /**
     * <pre>
     * 지정한 view 의 지정 영역을 캡쳐하여 파일로 저장한다.
     * 캡쳐한 이미지는 ARGB8888로 적용된다.
     * 지정한 파일의 확장자가 png (PNG)일 경우 PNG 포맷으로 저장하고 그 이외일 경우 JPEG 포맷으로 저장한다.
     * quality 는 png 는 100%, jpeg 는 설정값으로 설정된다.
     * </pre>
     *
     * @param view
     * @param outputFilePath
     * @param rect           null 일 경우 view 의 전체 영역으로 지정된다.
     * @param quality
     * @return
     */
    public static boolean saveImageFileFromView(View view, String outputFilePath, Rect rect, int quality) {
        boolean result = false;

        if (view == null || view.getWidth() == 0 || view.getHeight() == 0) {
            return false;
        }

        if (rect == null) {
            rect = new Rect();
            rect.left = 0;
            rect.right = view.getWidth();
            rect.top = 0;
            rect.bottom = view.getHeight();
        } else if ((rect.left == rect.right) || (rect.top == rect.bottom)) {
            return false;
        }
        rect.sort();

        Bitmap bitmap = Bitmap.createBitmap(rect.right - rect.left, rect.bottom - rect.top, Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(bitmap);
        canvas.save();
        canvas.translate(-rect.left, -rect.top);
        view.draw(canvas);
        canvas.restore();
        result = saveImageFileFromBitmap(bitmap, outputFilePath, quality);

        if (bitmap != null) {
            bitmap.recycle();
            bitmap = null;
        }
        return result;
    }

    /**
     * Assets에서 특정 Image 파일을 Bitmap 형식으로 return
     * @param context
     * @param path
     * @return
     */
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

    /**
     * Assets에서 특정 폴더의 하위 파일 List 를 얻어온다.
     * @param context
     * @param parentsPath
     * @return
     */
    public static String[] getFileListFromAssets(Context context, String parentsPath) {
        String[] result = null;

        try {
            result = context.getAssets().list(parentsPath);

        } catch (IOException ex) {
            Log.e("", ex);
        }

        return result;
    }

    public static String getTimeFormatString(String timeFormat, long time) {
        String result;
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(timeFormat);
        Date date = new Date(time);
        result = simpleDateFormat.format(date);
        return result;
    }

    public static Point getWindowSize(Activity activity) {
        Point result = new Point();
        activity.getWindowManager().getDefaultDisplay().getRealSize(result);
        return result;
    }

    public static float setScale(Activity activity, View rootView, boolean isBasicHeight1800) {
        Point size = Util.getWindowSize(activity);
        float BASIC_HEIGHT = 1800.f;

        if (isBasicHeight1800 == false) {
            BASIC_HEIGHT = (size.x <= 1280) ?  900.f : 1800.f;
        }

        float fixedSizeWidth = BASIC_HEIGHT * size.x / size.y;
        float fixedSizeHeight = BASIC_HEIGHT;
        float scale = size.y / BASIC_HEIGHT;

        Log.i("display width : " + size.x);
        Log.i("display height : " + size.y);
        Log.i("fixed width : " + fixedSizeWidth);
        Log.i("fixed height : " + fixedSizeHeight);
        Log.i("scale : " + scale);

        ViewGroup.LayoutParams params = rootView.getLayoutParams();
        params.width = (int)(fixedSizeWidth + 0.5f);
        params.height = (int)(fixedSizeHeight + 0.5f);
        rootView.setPivotX(0);
        rootView.setPivotY(0);
        rootView.setScaleX(scale);
        rootView.setScaleY(scale);

        return scale;
    }
}
