package todoschoollauncher.enuma.com.todoschoollauncher;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.os.Build;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.TextView;

import com.enuma.kitkitProvider.User;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import static com.enuma.kitkitProvider.KitkitDBHandler.DATABASE_NAME;

public class Util {
    public static String TAG = "TodoschoolLancher";
    public static CustomViewGroup mBlockingView;

    public static void hideSystemUI(Activity activity) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
            activity.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
            return;
        }

        activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_FULLSCREEN
                |View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                |View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                |View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                |View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    public static class CustomViewGroup extends ViewGroup {
        public CustomViewGroup(Context context) {
            super(context);
        }

        @Override
        protected void onLayout(boolean changed, int l, int t, int r, int b) {
        }

        @Override
        public boolean onInterceptTouchEvent(MotionEvent ev) {
            return true;
        }
    }

    public static void disableStatusBar(Activity activity) {
        WindowManager manager = ((WindowManager) activity.getApplicationContext().getSystemService(Context.WINDOW_SERVICE));
        WindowManager.LayoutParams localLayoutParams = new WindowManager.LayoutParams();

        localLayoutParams.type = WindowManager.LayoutParams.TYPE_SYSTEM_ERROR;
        localLayoutParams.gravity = Gravity.TOP;
        localLayoutParams.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                // this is to enable the notification to receive touch events
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                // Draws over status bar
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;

        localLayoutParams.width = WindowManager.LayoutParams.MATCH_PARENT;
        localLayoutParams.height = (int) (24 * activity.getResources().getDisplayMetrics().scaledDensity);
        localLayoutParams.format = PixelFormat.TRANSPARENT;
        mBlockingView = new CustomViewGroup(activity);
        manager.addView(mBlockingView, localLayoutParams);
    }

    public static DisplayMetrics getWindowInfo(Activity activity) {
        DisplayMetrics result = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(result);
        return result;
    }

    public static Point getWindowSize(Activity activity) {
        Point result = new Point();
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
            WindowManager windowManager = (WindowManager) activity.getSystemService(Context.WINDOW_SERVICE);
            Display display = windowManager.getDefaultDisplay();
            display.getSize(result);
            return result;
        }

        activity.getWindowManager().getDefaultDisplay().getRealSize(result);
        return result;
    }

    public static float setScale(Activity activity, View rootView) {
        Point size = Util.getWindowSize(activity);
        float BASIC_HEIGHT = 1800.f;

        float fixedSizeWidth = BASIC_HEIGHT * size.x / size.y;
        float fixedSizeHeight = BASIC_HEIGHT;
        float scale = size.y / BASIC_HEIGHT;

        Log.i(TAG, "display width : " + size.x);
        Log.i(TAG, "display height : " + size.y);
        Log.i(TAG, "fixed width : " + fixedSizeWidth);
        Log.i(TAG, "fixed height : " + fixedSizeHeight);
        Log.i(TAG, "scale : " + scale);

        ViewGroup.LayoutParams params = rootView.getLayoutParams();
        params.width = (int)(fixedSizeWidth + 0.5f);
        params.height = (int)(fixedSizeHeight + 0.5f);
        rootView.setPivotX(0);
        rootView.setPivotY(0);
        rootView.setScaleX(scale);
        rootView.setScaleY(scale);

        return scale;
    }

    public static void recycle(View view)
    {
        if (view == null)
        {
            return;
        }

        if (view.getBackground() != null)
        {
            view.getBackground().setCallback(null);
        }

        view.setBackgroundDrawable(null);

        if (view instanceof ViewGroup)
        {
            for (int i = 0; i < ((ViewGroup) view).getChildCount(); i++)
            {
                recycle(((ViewGroup) view).getChildAt(i));
            }

            if ((view instanceof AdapterView) == false)
            {
                ((ViewGroup) view).removeAllViews();
            }
        }

        if (view instanceof ImageView)
        {
            ((ImageView) view).setImageDrawable(null);
        }

        view = null;
    }

    public static void displayUserName(Activity activity, TextView tvUserName) {
        User user = ((LauncherApplication) activity.getApplication()).getDbHandler().getCurrentUser();

        String name = "";
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(activity);
        String displayName = user.getDisplayName();
        String tabletNumber = prefs.getString("TABLET_NUMBER", "");

        if ("user0".equalsIgnoreCase(user.getUserName())) {
            int userCount = ((LauncherApplication)activity.getApplication()).getDbHandler().numUserSeenLauncherTutorial();
            if (userCount >= 2 || (userCount == 1 && user.isFinishLauncherTutorial() == false)) {
                name = user.getUserName();
            }

        } else {
            name = user.getUserName();
        }

        if (!name.isEmpty()) {
            String result = name;

            if (displayName != null && !displayName.isEmpty()) {
                result += ("-" + displayName);
            }

            if (!tabletNumber.isEmpty()) {
                result += ("-t" + tabletNumber);
            }

            tvUserName.setText(result);

        } else {
            tvUserName.setText("");
        }
    }

    public static void copyDBFileToSDCard(Context context) {
        String file_url = "/data/data/" + context.getPackageName() + "/databases/" + DATABASE_NAME;
        File db = new File(file_url);

        if (db.exists()) {
            File outDirectory = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/");

            try {
                if (outDirectory.exists() == false) {
                    outDirectory.mkdirs();
                }

                InputStream in = new FileInputStream(file_url);
                OutputStream out = new FileOutputStream(Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + DATABASE_NAME);

                byte buf[] = new byte[4096];
                int len;
                while ((len = in.read(buf)) > 0) out.write(buf, 0, len);

                out.close();
                in.close();

            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            System.out.println("Can't copy database. Database not created.");
        }
    }
}
