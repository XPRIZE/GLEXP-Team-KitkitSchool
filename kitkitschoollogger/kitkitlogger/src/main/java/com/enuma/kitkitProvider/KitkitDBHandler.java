package com.enuma.kitkitProvider;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/**
 * Created by ingtellect on 9/1/17.
 */

// NB(xenosoz, 2018): SNTP Result table by me.

public class KitkitDBHandler extends SQLiteOpenHelper {
    private ContentResolver myCR;

    private static final int DATABASE_VERSION = 15;
    public static final String DATABASE_NAME = "userDB.db";
    public static final String TABLE_USERS = "users";
    public static final String TABLE_CURRENT_USER = "current_user";
    public static final String TABLE_SNTP_RESULT = "sntp_result_id";
    public static final String TABLE_FISHES = "fishes";

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
    public static final String COLUMN_DISPLAY_NAME = "display_name";
    public static final String COLUMN_OPEN_LIBRARY = "open_library";
    public static final String COLUMN_OPEN_TOOLS = "open_tools";
    public static final String COLUMN_UNLOCK_FISH_BOWL = "unlock_fish_bowl";
    public static final String COLUMN_UNLOCK_WRITING_BOARD = "unlock_writing_board";
    public static final String COLUMN_FINISH_WRITING_BOARD_TUTORIAL = "finish_writing_board_tutorial";

    public static final String COLUMN_SERVER_SPEC = "server_spec";
    public static final String COLUMN_TIME_NOW = "time_now";
    public static final String COLUMN_TIME_SNOW = "time_snow";

    public static final String COLUMN_FISH_ID = "fish_id";
    public static final String COLUMN_FISH_SKIN_NO = "skin_no";
    public static final String COLUMN_FISH_NAME = "name";
    public static final String COLUMN_FISH_CREATE_TIME = "create_time";
    public static final String COLUMN_FISH_POSITION = "position";

    final String CREATE_USER_TABLE = "CREATE TABLE "
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
            + COLUMN_FINISH_LAUNCHER_TUTORIAL + " BOOLEAN,"
            + COLUMN_DISPLAY_NAME + " TEXT,"
            + COLUMN_OPEN_LIBRARY + " BOOLEAN,"
            + COLUMN_OPEN_TOOLS + " BOOLEAN,"
            + COLUMN_UNLOCK_FISH_BOWL + " BOOLEAN,"
            + COLUMN_UNLOCK_WRITING_BOARD + " BOOLEAN,"
            + COLUMN_FINISH_WRITING_BOARD_TUTORIAL + " BOOLEAN"
            + ")";

    final String CREATE_CURRENT_USER_TABLE = "CREATE TABLE "
            + TABLE_CURRENT_USER
            + "("
            + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
            + COLUMN_USERNAME + " TEXT"
            + ")";

    final String CREATE_SNTP_RESULT_TABLE = "CREATE TABLE "
            + TABLE_SNTP_RESULT
            + "("
            + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT"
            + "," + COLUMN_SERVER_SPEC + " TEXT"
            + "," + COLUMN_TIME_NOW + " INTEGER"
            + "," + COLUMN_TIME_SNOW + " TEXT"
            //+ ",UNIQUE(" + COLUMN_SERVER_SPEC + ")"
            + ")";

    final String CREATE_FISH_TABLE = "CREATE TABLE "
            + TABLE_FISHES
            + "("
            + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
            + COLUMN_USERNAME + " TEXT,"
            + COLUMN_FISH_ID + " TEXT,"
            + COLUMN_FISH_SKIN_NO + " INTEGER,"
            + COLUMN_FISH_NAME + " TEXT,"
            + COLUMN_FISH_CREATE_TIME + " TEXT,"
            + COLUMN_FISH_POSITION + " TEXT"
            + ")";

    private String[] arr_sql_table = {
            CREATE_USER_TABLE,
            CREATE_CURRENT_USER_TABLE,
            CREATE_SNTP_RESULT_TABLE,
            CREATE_FISH_TABLE
    };

    public KitkitDBHandler(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
        myCR = context.getContentResolver();
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        execRawQuery(db, arr_sql_table);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.w(KitkitDBHandler.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion);

        if (oldVersion < newVersion) {
            List<String> arrSql = new ArrayList<String>();

            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_DISPLAY_NAME	 + " TEXT DEFAULT ('');");
            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_OPEN_LIBRARY	 + " BOOLEAN DEFAULT (" + (User.DEFAULT_OPEN_LIBRARY == false ? 0 : 1) + ");");
            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_OPEN_TOOLS + " BOOLEAN DEFAULT (" + (User.DEFAULT_OPEN_TOOLS == false ? 0 : 1) + ");");
            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_UNLOCK_FISH_BOWL + " BOOLEAN DEFAULT (" + 0 + ");");
            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_UNLOCK_WRITING_BOARD + " BOOLEAN DEFAULT (" + 0 + ");");
            arrSql.add("ALTER TABLE " + TABLE_USERS + " ADD COLUMN " + COLUMN_FINISH_WRITING_BOARD_TUTORIAL + " BOOLEAN DEFAULT (" + 0 + ");");

            execRawQuery(db, arr_sql_table); // create table
            String[] sql_array = arrSql.toArray(new String[arrSql.size()]);
            execRawQuery(db, sql_array);
        }
    }

    private void execRawQuery(SQLiteDatabase db, String[] arr_sql) {
        if (arr_sql != null) {
            for (String sql : arr_sql) {
                try {
                    db.execSQL(sql);
                } catch (Exception e) {
                    Log.e(KitkitDBHandler.class.getName(),"SQL ERROR : " + sql);
                }
            }
        }
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
        values.put(COLUMN_OPEN_LIBRARY, user.isOpenLibrary());
        values.put(COLUMN_OPEN_TOOLS, user.isOpenTools());
        values.put(COLUMN_UNLOCK_FISH_BOWL, user.isUnlockFishBowl());
        values.put(COLUMN_UNLOCK_WRITING_BOARD, user.isUnlockWritingBoard());
        values.put(COLUMN_FINISH_WRITING_BOARD_TUTORIAL, user.isFinishWritingBoardTutorial());

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
                COLUMN_FINISH_LAUNCHER_TUTORIAL,
                COLUMN_DISPLAY_NAME,
                COLUMN_OPEN_LIBRARY,
                COLUMN_OPEN_TOOLS,
                COLUMN_UNLOCK_FISH_BOWL,
                COLUMN_UNLOCK_WRITING_BOARD,
                COLUMN_FINISH_WRITING_BOARD_TUTORIAL
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
            user.setDisplayName(cursor.getString(10));
            user.setOpenLibrary("1".equals(cursor.getString(11)));
            user.setOpenTools("1".equals(cursor.getString(12)));
            user.setUnlockFishBowl("1".equals(cursor.getString(13)));
            user.setUnlockWritingBoard("1".equals(cursor.getString(14)));
            user.setFinishWritingBoardTutorial("1".equals(cursor.getString(15)));
            cursor.close();
        } else {
            user = null;
            cursor.close();
        }
        return user;
    }

    public ArrayList<User> getUserList() {
        String[] projection = {COLUMN_ID,
                COLUMN_USERNAME,
                COLUMN_STARS,
                COLUMN_FINISH_TUTORIAL,
                COLUMN_UNLOCK_DRUM,
                COLUMN_UNLOCK_MARIMBA,
                COLUMN_UNLOCK_DRAWING,
                COLUMN_UNLOCK_COLORING,
                COLUMN_UNLOCK_BLACKBOARD,
                COLUMN_FINISH_LAUNCHER_TUTORIAL,
                COLUMN_DISPLAY_NAME,
                COLUMN_OPEN_LIBRARY,
                COLUMN_OPEN_TOOLS,
                COLUMN_UNLOCK_FISH_BOWL,
                COLUMN_UNLOCK_WRITING_BOARD,
                COLUMN_FINISH_WRITING_BOARD_TUTORIAL
        };

        Cursor cursor = myCR.query(KitkitProvider.CONTENT_URI,
                projection, null, null,
                null);

        ArrayList<User> result = new ArrayList<User>();
        if(cursor.moveToFirst()) {
            do {
                User user = new User();
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
                user.setDisplayName(cursor.getString(10));
                user.setOpenLibrary("1".equals(cursor.getString(11)));
                user.setOpenTools("1".equals(cursor.getString(12)));
                user.setUnlockFishBowl("1".equals(cursor.getString(13)));
                user.setUnlockWritingBoard("1".equals(cursor.getString(14)));
                user.setFinishWritingBoardTutorial("1".equals(cursor.getString(15)));
                result.add(user);

            } while(cursor.moveToNext());
        }

        cursor.close();
        return result;
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

            String selection = COLUMN_ID + " = 1";

            ContentValues values = new ContentValues();
            values.put(COLUMN_USERNAME, user.getUserName());
            myCR.update(KitkitProvider.CURRENT_USER_URI, values, selection, null);
        } else {
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
        } catch (NullPointerException ne) {
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
        } catch (NullPointerException ne) {
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
        values.put(COLUMN_DISPLAY_NAME, user.getDisplayName());
        values.put(COLUMN_OPEN_LIBRARY, user.isOpenLibrary());
        values.put(COLUMN_OPEN_TOOLS, user.isOpenTools());
        values.put(COLUMN_UNLOCK_FISH_BOWL, user.isUnlockFishBowl());
        values.put(COLUMN_UNLOCK_WRITING_BOARD, user.isUnlockWritingBoard());
        values.put(COLUMN_FINISH_WRITING_BOARD_TUTORIAL, user.isFinishWritingBoardTutorial());
        Log.i("myLog", "value : " + values.toString());
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
            int id = -1;
            try {
                id = cursor.getInt(cursor.getColumnIndexOrThrow(COLUMN_ID));
            } catch (Exception e) {
                Log.e("myLog", "" + e);
            }

            SntpResult sr = new SntpResult(id, serverSpec, now, snow);
            results.add(sr);
        }
        if (cursor != null) {
            cursor.close();
        }
        return results;
    }

    public String getTabletNumber() {
        Cursor cursor = myCR.query(KitkitProvider.PREFERENCE_URI, null, null, null, null);
        String result = "";
        try {
            cursor.moveToFirst();
            result = cursor.getString(0);
            cursor.close();
        } catch (Exception e) {
        }

        return result;
    }

    public void addFish(String fishID, int skinNo, String fishName, String position) {
        try {
            ContentValues values = new ContentValues();
            values.put(COLUMN_USERNAME, getCurrentUser().getUserName());
            values.put(COLUMN_FISH_ID, fishID);
            values.put(COLUMN_FISH_SKIN_NO, skinNo);
            values.put(COLUMN_FISH_NAME, fishName);
            values.put(COLUMN_FISH_CREATE_TIME, getTimeFormatString(System.currentTimeMillis(), "yyyyMMddHHmmss"));
            values.put(COLUMN_FISH_POSITION, position);

            myCR.insert(KitkitProvider.FISHES_URI, values);
        } catch (Exception e) {
            Log.e(KitkitDBHandler.class.getName(), "" + e);
        }
    }

    public void updateFish(Fish fish) {
        try {
            String selection = COLUMN_ID + " = " + fish._id;

            ContentValues values = new ContentValues();
            values.put(COLUMN_ID, fish._id);
            values.put(COLUMN_USERNAME, fish._userName);
            values.put(COLUMN_FISH_ID, fish._fishID);
            values.put(COLUMN_FISH_SKIN_NO, fish._skinNo);
            values.put(COLUMN_FISH_NAME, fish._name);
            values.put(COLUMN_FISH_CREATE_TIME, fish._createTime);
            values.put(COLUMN_FISH_POSITION, fish._position);

            myCR.update(KitkitProvider.FISHES_URI, values, selection, null);

        } catch (Exception e) {
            Log.e(KitkitDBHandler.class.getName(), "" + e);
        }
    }

    public ArrayList<Fish> getFishes() {
        ArrayList<Fish> result = new ArrayList<>();

        try {
            String[] projection = {
                    COLUMN_ID,
                    COLUMN_USERNAME,
                    COLUMN_FISH_ID,
                    COLUMN_FISH_SKIN_NO,
                    COLUMN_FISH_NAME,
                    COLUMN_FISH_CREATE_TIME,
                    COLUMN_FISH_POSITION
            };

            String selection = COLUMN_USERNAME + " = \"" + getCurrentUser().getUserName() + "\"";
            Cursor cursor = myCR.query(KitkitProvider.FISHES_URI,
                    projection, selection, null,
                    COLUMN_ID + " ASC");

            if(cursor.moveToFirst()) {
                do {
                    Fish fish = new Fish();
                    fish._id = cursor.getInt(0);
                    fish._userName = cursor.getString(1);
                    fish._fishID = cursor.getString(2);
                    fish._skinNo = cursor.getInt(3);
                    fish._name = cursor.getString(4);
                    fish._createTime = cursor.getString(5);
                    fish._position = cursor.getString(6);
                    result.add(fish);

                } while(cursor.moveToNext());
            }

            cursor.close();
        } catch (Exception e) {
            Log.e(KitkitDBHandler.class.getName(), "" + e);
        }

        long currentTime = System.currentTimeMillis();
        String strCurrentTime = getTimeFormatString(currentTime, "yyyyMMddHHmmss");

        for (Fish fish : result) {
            if (currentTime < getCalendarFromString(fish._createTime, "yyyyMMddHHmmss").getTimeInMillis()) {
                fish._createTime = strCurrentTime;
                updateFish(fish);
            }
        }

        return result;
    }

    public boolean deleteFish(int id) {
        boolean result = false;

        try {
            String selection = "_id = \"" + id + "\"";
            int rowsDeleted = myCR.delete(KitkitProvider.FISHES_URI,
                    selection, null);

            if (rowsDeleted > 0)
                result = true;

        } catch (Exception e) {
            Log.e(KitkitDBHandler.class.getName(), "" + e);
        }

        return result;
    }

    public static String getTimeFormatString(long time, String timeFormat) {
        String result;
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(timeFormat, Locale.US);
        Date date = new Date(time);
        result = simpleDateFormat.format(date);
        return result;
    }

    public static Calendar getCalendarFromString(String string, String dateFormat) {
        Calendar calendar = null;
        try {
            SimpleDateFormat format = new SimpleDateFormat(dateFormat);
            Date date = format.parse(string);
            calendar = Calendar.getInstance();
            calendar.setTime(date);

        } catch (Exception e) {
            Log.e(KitkitDBHandler.class.getName(), "" + e);
            calendar = Calendar.getInstance();
        }

        return calendar;
    }
}
