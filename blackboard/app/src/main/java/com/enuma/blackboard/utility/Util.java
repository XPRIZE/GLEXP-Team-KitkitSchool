package com.enuma.blackboard.utility;

import android.content.Context;
import android.util.TypedValue;

public class Util {
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
}
