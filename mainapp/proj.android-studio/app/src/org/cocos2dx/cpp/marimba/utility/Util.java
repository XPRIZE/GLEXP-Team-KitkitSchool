package org.cocos2dx.cpp.marimba.utility;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;
import android.util.TypedValue;

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

    public static int getResourceId(Context context, String name, String resourceName, String packageName) {
        try {
            return context.getResources().getIdentifier(name, resourceName, packageName);
        } catch (Exception e) {
            Log.e("", e);
            return -1;
        }
    }
}
