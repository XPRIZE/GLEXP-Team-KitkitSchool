package com.enuma.kitkitProvider;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by ingtellect on 9/1/17.
 */

// NB(xenosoz, 2018): SNTP Result table by me.

public class KitkitDBHandler extends SQLiteOpenHelper {
    private ContentResolver myCR;

    private static final int DATABASE_VERSION = 7;
    private static final String DATABASE_NAME = "userDB.db";
    public static final String TABLE_USERS = "users";
    public static final String TABLE_CURRENT_USER = "current_user";
    public static final String TABLE_SNTP_RESULT = "sntp_result";


    public static final String COLUMN_ID = "_id";
    public static final String COLUMN_USERNAME = "username";
    public static final String COLUMN_STARS = "stars";
    public static final String COLUMN_FINISH_TUTORIAL = "finish_tutorial";
    public static final String COLUMN_UNLOCK_DRUM = "unlock_drum";
    public static final String COLUMN_UNLOCK_MARIMBA = "unlock_marimba";
    public static final String COLUMN_UNLOCK_DRAWING = "unlock_drawing";
    public static final String COLUMN_UNLOCK_COLORING = "unlock_coloring";
    public static final String COLUMN_UNLOCK_BLACKBOARD = "unlock_blackboard";
    public static final String COLUMN_FINISH_LAUNCHER_TUTORIAL = "finish_launcher_tutorial";

    public static final String COLUMN_SERVER_SPEC = "server_spec";
    public static final String COLUMN_TIME_NOW = "time_now";
    public static final String COLUMN_TIME_SNOW = "time_snow";

    public KitkitDBHandler(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
        myCR = context.getContentResolver();
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String CREATE_USER_TABLE = "CREATE TABLE "
                + TABLE_USERS
                + "("
                + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                + COLUMN_USERNAME + " TEXT,"
                + COLUMN_STARS + " INTEGER,"
                + COLUMN_FINISH_TUTORIAL + " BOOLEAN,"
                + COLUMN_UNLOCK_DRUM + " BOOLEAN,"
                + COLUMN_UNLOCK_MARIMBA + " BOOLEAN,"
                + COLUMN_UNLOCK_DRAWING + " BOOLEAN,"
                + COLUMN_UNLOCK_COLORING + " BOOLEAN,"
                + COLUMN_UNLOCK_BLACKBOARD + " BOOLEAN,"
                + COLUMN_FINISH_LAUNCHER_TUTORIAL + " BOOLEAN"
                + ")";
        db.execSQL(CREATE_USER_TABLE);

        String CREATE_CURRENT_USER_TABLE = "CREATE TABLE "
                + TABLE_CURRENT_USER
                + "("
                + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                + COLUMN_USERNAME + " TEXT"
                + ")";
        db.execSQL(CREATE_CURRENT_USER_TABLE);

        String CREATE_SNTP_RESULT_TABLE = "CREATE TABLE "
                + TABLE_SNTP_RESULT
                + "(" + COLUMN_SERVER_SPEC + " TEXT PRIMARY KEY"
                + "," + COLUMN_TIME_NOW + " INTEGER"
                + "," + COLUMN_TIME_SNOW + " TEXT"
                //+ ",UNIQUE(" + COLUMN_SERVER_SPEC + ")"
                + ")";
        db.execSQL(CREATE_SNTP_RESULT_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.w(KitkitDBHandler.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_USERS);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_CURRENT_USER);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_SNTP_RESULT);
        onCreate(db);
    }

    public void addUser(User user) {

        ContentValues values = new ContentValues();
        values.put(COLUMN_USERNAME, user.getUserName());
        values.put(COLUMN_STARS, user.getNumStars());
        values.put(COLUMN_FINISH_TUTORIAL, user.isFinishTutorial());
        values.put(COLUMN_UNLOCK_DRUM, user.isUnlockDrum());
        values.put(COLUMN_UNLOCK_MARIMBA, user.isUnlockMarimba());
        values.put(COLUMN_UNLOCK_DRAWING, user.isUnlockDrawing());
        values.put(COLUMN_UNLOCK_COLORING, user.isUnlockColoring());
        values.put(COLUMN_UNLOCK_BLACKBOARD, user.isUnlockBlackboard());
        values.put(COLUMN_FINISH_LAUNCHER_TUTORIAL, user.isFinishLauncherTutorial());

        myCR.insert(KitkitProvider.CONTENT_URI, values);
    }

    public User findUser(String username) {
        String[] projection = {COLUMN_ID,
                COLUMN_USERNAME,
                COLUMN_STARS,
                COLUMN_FINISH_TUTORIAL,
                COLUMN_UNLOCK_DRUM,
                COLUMN_UNLOCK_MARIMBA,
                COLUMN_UNLOCK_DRAWING,
                COLUMN_UNLOCK_COLORING,
                COLUMN_UNLOCK_BLACKBOARD,
                COLUMN_FINISH_LAUNCHER_TUTORIAL
                };

        String selection = "username = \"" + username + "\"";

        Cursor cursor = myCR.query(KitkitProvider.CONTENT_URI,
                projection, selection, null,
                null);

        User user = new User();

        if (cursor.moveToFirst()) {
            cursor.moveToFirst();
            user.setID(Integer.parseInt(cursor.getString(0)));
            user.setUserName(cursor.getString(1));
            user.setNumStars(Integer.parseInt(cursor.getString(2)));
            user.setFinishTutorial("1".equals(cursor.getString(3)));
            user.setUnlockDrum("1".equals(cursor.getString(4)));
            user.setUnlockMarimba("1".equals(cursor.getString(5)));
            user.setUnlockDrawing("1".equals(cursor.getString(6)));
            user.setUnlockColoring("1".equals(cursor.getString(7)));
            user.setUnlockBlackboard("1".equals(cursor.getString(8)));
            user.setFinishLauncherTutorial("1".equals(cursor.getString(9)));
            cursor.close();
        } else {
            user = null;
            cursor.close();
        }
        return user;
    }

    public boolean deleteUser(String username) {

        boolean result = false;

        String selection = "username = \"" + username + "\"";

        int rowsDeleted = myCR.delete(KitkitProvider.CONTENT_URI,
                selection, null);

        if (rowsDeleted > 0)
            result = true;

        return result;
    }

    public int numUser() {
        String[] projection = {"COUNT(*) AS COUNT"};
        Cursor cursor = myCR.query(KitkitProvider.CONTENT_URI,
                projection, null, null,
                null);
        cursor.moveToFirst();
        int numUser = cursor.getInt(0);
        cursor.close();

        return numUser;

    }

    public int numUserSeenLauncherTutorial() {
        String[] projection = {COLUMN_USERNAME};
        Cursor cursor = myCR.query(KitkitProvider.CONTENT_URI,
                projection, COLUMN_FINISH_LAUNCHER_TUTORIAL + "=1", null,
                null);
        int count = cursor.getCount();
        cursor.close();
        Log.w(KitkitDBHandler.class.getName(), "numUserSeenLauncherTutorial count : " + count);
        return count;
    }


    public boolean currentUserExist() {

        String[] projection = {"COUNT(*) AS COUNT"};
        Cursor cursor = myCR.query(KitkitProvider.CURRENT_USER_URI,
                projection, null, null,
                null);
        cursor.moveToFirst();
        int numUser = cursor.getInt(0);
        cursor.close();

        return numUser > 0;
    }

    public void setCurrentUser(String username) {
        User user = findUser(username);
        setCurrentUser(user);
    }

    public void setCurrentUser(User user) {
        if (currentUserExist()) {

            String selection = COLUMN_ID +" = 1";

            ContentValues values = new ContentValues();
            values.put(COLUMN_USERNAME, user.getUserName());
            myCR.update(KitkitProvider.CURRENT_USER_URI, values, selection, null);
        }
        else {
            ContentValues values = new ContentValues();
            values.put(COLUMN_USERNAME, user.getUserName());
            myCR.insert(KitkitProvider.CURRENT_USER_URI, values);
        }
    }

    public User getCurrentUser() {

        String[] projection = {COLUMN_USERNAME};
        Cursor cursor = myCR.query(KitkitProvider.CURRENT_USER_URI,
                projection, null, null,
                null);
        try {
            cursor.moveToFirst();
        }
        catch (NullPointerException ne) {
            return null;
        }

        String currentUserName = cursor.getString(0);
        User user = findUser(currentUserName);
        cursor.close();
        return user;
    }

    public String getCurrentUsername() {
        String[] projection = {COLUMN_USERNAME};
        Cursor cursor = myCR.query(KitkitProvider.CURRENT_USER_URI,
                projection, null, null,
                null);
        try {
            cursor.moveToFirst();
        }
        catch (NullPointerException ne) {
            return null;
        }
        String result = cursor.getString(0);
        cursor.close();
        return result;
    }

    public void updateUser(User user) {

        String selection = "username = \"" + user.getUserName() + "\"";

        ContentValues values = new ContentValues();
        values.put(COLUMN_USERNAME, user.getUserName());
        values.put(COLUMN_STARS, user.getNumStars());
        values.put(COLUMN_FINISH_TUTORIAL, user.isFinishTutorial());
        values.put(COLUMN_UNLOCK_DRUM, user.isUnlockDrum());
        values.put(COLUMN_UNLOCK_MARIMBA, user.isUnlockMarimba());
        values.put(COLUMN_UNLOCK_DRAWING, user.isUnlockDrawing());
        values.put(COLUMN_UNLOCK_COLORING, user.isUnlockColoring());
        values.put(COLUMN_UNLOCK_BLACKBOARD, user.isUnlockBlackboard());
        values.put(COLUMN_FINISH_LAUNCHER_TUTORIAL, user.isFinishLauncherTutorial());

        myCR.update(KitkitProvider.CONTENT_URI, values, selection, null);
    }

    public void uniqueInsertSntpResult(SntpResult sntpResult) {
        // NB(xenosoz, 2018): myCR.update doesn't do the trick here,
        //   and myCR doesn't support 'INSERT OR IGNORE' statement.
        SntpResult sr = sntpResult;
        String where = COLUMN_SERVER_SPEC + "=" + ("\"" + sr.serverSpec + "\"");

        ContentValues values = new ContentValues();
        values.put(COLUMN_SERVER_SPEC, sr.serverSpec);
        values.put(COLUMN_TIME_NOW, sr.now);
        values.put(COLUMN_TIME_SNOW, sr.snow);

        String[] args = null;
        myCR.delete(KitkitProvider.SNTP_RESULT_URI, where, args);
        myCR.insert(KitkitProvider.SNTP_RESULT_URI, values);
    }

    public List<SntpResult> getSntpResults() {
        ArrayList<SntpResult> results = new ArrayList<SntpResult>();

        Cursor cursor = myCR.query(KitkitProvider.SNTP_RESULT_URI, null, null, null, null);
        while (cursor != null && cursor.moveToNext()) {
            String serverSpec = cursor.getString(cursor.getColumnIndex(COLUMN_SERVER_SPEC));
            long now = cursor.getLong(cursor.getColumnIndex(COLUMN_TIME_NOW));
            String snow = cursor.getString(cursor.getColumnIndex(COLUMN_TIME_SNOW));

            SntpResult sr = new SntpResult(serverSpec, now, snow);
            results.add(sr);
        }
        if (cursor != null) {
            cursor.close();
        }
        return results;
    }
}
