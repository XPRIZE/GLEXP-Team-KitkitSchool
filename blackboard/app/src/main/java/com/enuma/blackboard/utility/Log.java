package com.enuma.blackboard.utility;

public final class Log {
    private static final String none_tag = "myLog";

    public static int v(String msg) {
        return android.util.Log.v(none_tag, convertErrMsg(msg));
    }

    public static int v(String tag, String msg) {
        return android.util.Log.v(tag, convertErrMsg(msg));
    }

    public static int v(String msg, Throwable tr) {
        return android.util.Log.v(none_tag, convertErrMsg(msg), tr);
    }

    public static int v(String tag, String msg, Throwable tr) {
        return android.util.Log.v(tag, convertErrMsg(msg), tr);
    }

    public static int d(String msg) {
        return android.util.Log.d(none_tag, convertErrMsg(msg));
    }

    public static int d(String tag, String msg) {
        return android.util.Log.d(tag, convertErrMsg(msg));
    }

    public static int d(String msg, Throwable tr) {
        return android.util.Log.d(none_tag, convertErrMsg(msg), tr);
    }

    public static int d(String tag, String msg, Throwable tr) {
        return android.util.Log.d(tag, convertErrMsg(msg), tr);
    }

    public static int i(String msg) {
        return android.util.Log.i(none_tag, convertErrMsg(msg));
    }

    public static int i(String tag, String msg) {
        return android.util.Log.i(tag, convertErrMsg(msg));
    }

    public static int i(String msg, Throwable tr) {
        return android.util.Log.i(none_tag, convertErrMsg(msg), tr);
    }

    public static int i(String tag, String msg, Throwable tr) {
        return android.util.Log.i(tag, convertErrMsg(msg), tr);
    }

    public static int w(String msg) {
        return android.util.Log.w(none_tag, convertErrMsg(msg));
    }

    public static int w(String tag, String msg) {
        return android.util.Log.w(tag, convertErrMsg(msg));
    }

    public static int w(String msg, Throwable tr) {
        return android.util.Log.w(none_tag, convertErrMsg(msg), tr);
    }

    public static int w(String tag, String msg, Throwable tr) {
        return android.util.Log.w(tag, convertErrMsg(msg), tr);
    }

    public static int e(String msg) {
        return android.util.Log.e(none_tag, convertErrMsg(msg));
    }

    public static int e(String tag, String msg) {
        return android.util.Log.e(tag, convertErrMsg(msg));
    }

    public static int e(String msg, Throwable tr) {
        return android.util.Log.e(none_tag, convertErrMsg(msg), tr);
    }

    public static int e(String tag, String msg, Throwable tr) {
        return android.util.Log.e(tag, convertErrMsg(msg), tr);
    }


    private static String getClassName(String val) {
        String rtn = val;

        try {
            if (rtn.contains(".")) {
                rtn = rtn.substring(rtn.lastIndexOf(".") + 1);
            } else {
                // class_name = class_name.replace(context.getPackageName() + ".", "");
            }

            if (rtn.contains("$")) {
                rtn = rtn.substring(0, rtn.indexOf("$"));
            }
        } catch (Exception e) {
        }

        return rtn;
    }

    private static String convertErrMsg(String val) {
        String rtn = val;
        try {
            StackTraceElement[] a = new Throwable().getStackTrace();

            String prev_method_name = "";
            int prev_line = 0;

            String class_name = getClassName(a[2].getClassName());

            if (a.length > 2) {
                prev_method_name = a[3].getMethodName();
                prev_line = a[3].getLineNumber();
            }

            // if (prev_class_name.length() > 0 && prev_class_name.equalsIgnoreCase(class_name) == false)
            if (prev_method_name.length() > 0) {
                rtn = class_name + "(" + a[2].getMethodName() + ":" + a[2].getLineNumber() + " <- " + prev_method_name + ":" + prev_line + ")" + "\t" + val;
            } else {
                rtn = class_name + "(" + a[2].getMethodName() + ":" + a[2].getLineNumber() + ")" + "\t" + val;
            }
        } catch (Exception e) {
            rtn = val;
        }
        return rtn;
    }
}
