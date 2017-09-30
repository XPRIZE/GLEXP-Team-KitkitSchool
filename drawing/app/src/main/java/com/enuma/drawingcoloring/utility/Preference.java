package com.enuma.drawingcoloring.utility;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

public class Preference {

    private Context context;
    private SharedPreferences pref = null;

    private Preference(Context context) {
        this.context = context;
        pref = this.context.getSharedPreferences("preference", Activity.MODE_PRIVATE);
    }

    private static Preference preference = null;

    public static Preference getInstance(Context context) {
        if (null == preference) preference = new Preference(context);
        return preference;
    }

    /**
     * config.txt 파일에서 key 에 해당하는 값을 반환한다.
     *
     * @param key          키 값
     * @param defaultValue 반환값이 없을 경우 기본 값
     * @return 문자열 반환 값
     */
    public String getConfig(String key, String defaultValue) {
        String rtn = defaultValue;
        try {
            rtn = pref.getString(key, defaultValue);
        } catch (Exception e) {

        }
        return rtn;
    }

    /**
     * config.txt 파일에서 key 에 해당하는 값을 반환한다.
     *
     * @param key          키 값
     * @param defaultValue 반환값이 없을 경우 기본 값
     * @return Integer 반환 값
     */
    public int getConfig(String key, int defaultValue) {
        int rtn = defaultValue;
        try {
            rtn = pref.getInt(key, defaultValue);
        } catch (Exception e) {

        }
        return rtn;
    }

    /**
     * config.txt 파일에서 key 에 해당하는 값을 반환한다.
     *
     * @param key          키 값
     * @param defaultValue 반환값이 없을 경우 기본 값
     * @return float 반환 값
     */
    public float getConfig(String key, float defaultValue) {
        float rtn = defaultValue;
        try {
            rtn = pref.getFloat(key, defaultValue);
        } catch (Exception e) {

        }
        return rtn;
    }

    /**
     * config.txt 파일에서 key 에 해당하는 값을 반환한다.
     *
     * @param key          키 값
     * @param defaultValue 반환값이 없을 경우 기본 값
     * @return Long 반환 값
     */
    public long getConfig(String key, long defaultValue) {
        long rtn = defaultValue;
        try {
            rtn = pref.getLong(key, defaultValue);
        } catch (Exception e) {

        }
        return rtn;
    }

    /**
     * config.txt 파일에서 key 에 해당하는 값을 반환한다.
     *
     * @param key          키 값
     * @param defaultValue 반환값이 없을 경우 기본 값
     * @return boolean 반환 값
     */
    public boolean getConfig(String key, boolean defaultValue) {
        boolean rtn = defaultValue;
        try {
            rtn = pref.getBoolean(key, defaultValue);
        } catch (Exception e) {

        }
        return rtn;
    }

    /**
     * config.txt 파일에 해당 키, 값을 저장한다.
     *
     * @param key 키 값
     * @param val 저장 값
     */
    public void setConfig(String key, String val) {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.putString(key, val);
            editor.commit();
        } catch (Exception e) {

        }
    }

    /**
     * config.txt 파일에 해당 키, 값을 저장한다.
     *
     * @param key 키 값
     * @param val 저장 값
     */
    public void setConfig(String key, boolean val) {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.putBoolean(key, val);
            editor.commit();
        } catch (Exception e) {

        }
    }

    /**
     * config.txt 파일에 해당 키, 값을 저장한다.
     *
     * @param key 키 값
     * @param val 저장 값
     */
    public void setConfig(String key, Float val) {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.putFloat(key, val);
            editor.commit();
        } catch (Exception e) {

        }
    }

    /**
     * config.txt 파일에 해당 키, 값을 저장한다.
     *
     * @param key 키 값
     * @param val 저장 값
     */
    public void setConfig(String key, int val) {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.putInt(key, val);
            editor.commit();
        } catch (Exception e) {

        }
    }

    /**
     * config.txt 파일에 해당 키, 값을 저장한다.
     *
     * @param key 키 값
     * @param val 저장 값
     */
    public void setConfig(String key, Long val) {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.putLong(key, val);
            editor.commit();
        } catch (Exception e) {

        }
    }

    public void clear() {
        try {
            SharedPreferences.Editor editor = pref.edit();
            editor.clear();
            editor.commit();
        } catch (Exception e) {

        }
    }
}