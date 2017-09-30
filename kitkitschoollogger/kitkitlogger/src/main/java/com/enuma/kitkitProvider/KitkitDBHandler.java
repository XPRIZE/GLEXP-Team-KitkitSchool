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

/**
 * Created by ingtellect on 9/1/17.
 */

public class KitkitDBHandler extends SQLiteOpenHelper {
    private ContentResolver myCR;

    private static final int DATABASE_VERSION = 2;
    private static final String DATABASE_NAME = "userDB.db";
    public static final String TABLE_USERS = "users";
    public static final String TABLE_CURRENT_USER = "current_user";


    public static final String COLUMN_ID = "_id";
    public static final String COLUMN_USERNAME = "username";
    public static final String COLUMN_STARS = "stars";
    public static final String COLUMN_FINISH_TUTORIAL = "finish_tutorial";
    public static final String COLUMN_UNLOCK_DRUM = "unlock_drum";
    public static final String COLUMN_UNLOCK_MARIMBA = "unlock_marimba";
    public static final String COLUMN_UNLOCK_DRAWING = "unlock_drawing";
    public static final String COLUMN_UNLOCK_COLORING = "unlock_coloring";
    public static final String COLUMN_UNLOCK_BLACKBOARD = "unlock_blackboard";

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
                + COLUMN_UNLOCK_BLACKBOARD + " BOOLEAN"
                + ")";
        db.execSQL(CREATE_USER_TABLE);

        String CREATE_CURRENT_USER_TABLE = "CREATE TABLE "
                + TABLE_CURRENT_USER
                + "("
                + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                + COLUMN_USERNAME + " TEXT"
                + ")";
        db.execSQL(CREATE_CURRENT_USER_TABLE);

    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.w(KitkitDBHandler.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_USERS);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_CURRENT_USER);
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
                COLUMN_UNLOCK_BLACKBOARD};

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
            cursor.close();
        } else {
            user = null;
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
        return cursor.getString(0);
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

        myCR.update(KitkitProvider.CONTENT_URI, values, selection, null);
    }
}
