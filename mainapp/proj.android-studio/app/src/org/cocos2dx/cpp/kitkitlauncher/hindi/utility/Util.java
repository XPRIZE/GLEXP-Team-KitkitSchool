package org.cocos2dx.cpp.kitkitlauncher.hindi.utility;

import android.app.Activity;
import android.content.Context;
import android.database.Cursor;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.View;

import java.lang.ref.WeakReference;

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

    public static DisplayMetrics getWindowInfo(Activity activity) {
        DisplayMetrics result = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(result);
        return result;
    }

    public static String getThumbnailPath(Context context, String path) {
        String result = "";
        long imageId = -1;

        try {
            String[] projection = new String[]{MediaStore.MediaColumns._ID};
            String selection = MediaStore.MediaColumns.DATA + "=?";
            String[] selectionArgs = new String[]{path};
            Cursor cursor = context.getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, projection, selection, selectionArgs, null);
            if (cursor != null && cursor.moveToFirst()) {
                imageId = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
            }
            cursor.close();

            cursor = MediaStore.Images.Thumbnails.queryMiniThumbnail(context.getContentResolver(), imageId, MediaStore.Images.Thumbnails.MINI_KIND, null);
            if (cursor != null && cursor.getCount() > 0) {
                cursor.moveToFirst();
                result = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Images.Thumbnails.DATA));
            }
            cursor.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (TextUtils.isEmpty(result))
            result = path;

        return result;
    }

    public static void hideSystemUI(Activity activity) {
        activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    public interface HandlerInterface {
        void handleUIMessage(Message msg);
    }

    public static class StaticHandler extends Handler {
        private final WeakReference<HandlerInterface> mHandlerInterface;

        public StaticHandler(HandlerInterface handlerInterface) {
            mHandlerInterface = new WeakReference<>(handlerInterface);
        }

        @Override
        public void handleMessage(Message msg) {
            HandlerInterface handlerInterface = mHandlerInterface.get();

            if (handlerInterface != null) {
                try {
                    Log.i("command : " + msg.what);
                    handlerInterface.handleUIMessage(msg);
                } catch (Exception e) {
                    Log.e("" + e);
                }

            }
        }
    }
}
