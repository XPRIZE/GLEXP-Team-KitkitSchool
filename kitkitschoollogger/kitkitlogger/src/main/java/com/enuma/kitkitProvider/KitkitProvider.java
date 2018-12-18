package com.enuma.kitkitProvider;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.SharedPreferences;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteQueryBuilder;
import android.net.Uri;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.TextUtils;

/**
 * Created by ingtellect on 9/1/17.
 */

public class KitkitProvider extends ContentProvider {

    private static final String AUTHORITY =
            "com.enuma.provider.KitkitContentProvider";

    private static final String USER_TABLE = "users";
    private static final String CURRENT_USER_TABLE = "current_user";
    private static final String SNTP_RESULT_TABLE = "sntp_result";
    private static final String PREFERENCE = "preference";
    private static final String FISHES_TABLE = "fishes";

    public static final Uri CONTENT_URI =
            Uri.parse("content://" + AUTHORITY + "/" + USER_TABLE);

    public static final Uri CURRENT_USER_URI =
            Uri.parse("content://" + AUTHORITY + "/" + CURRENT_USER_TABLE);

    public static final Uri SNTP_RESULT_URI =
            Uri.parse("content://"+ AUTHORITY + "/" + SNTP_RESULT_TABLE);

    public static final Uri PREFERENCE_URI=
            Uri.parse("content://" + AUTHORITY + "/" + PREFERENCE);

    public static final Uri FISHES_URI =
            Uri.parse("content://" + AUTHORITY + "/" + FISHES_TABLE);

    public static final int USER = 1;
    public static final int USER_ID = 2;
    public static final int CURRENT_USER = 3;
    public static final int SNTP_RESULT = 4;
    public static final int PREFERENCE_INFO = 5;
    public static final int FISHES = 6;

    private static final UriMatcher sURIMatcher =
            new UriMatcher(UriMatcher.NO_MATCH);

    static {
        sURIMatcher.addURI(AUTHORITY, USER_TABLE, USER);
        sURIMatcher.addURI(AUTHORITY, USER_TABLE + "/#",
                USER_ID);

        sURIMatcher.addURI(AUTHORITY, CURRENT_USER_TABLE, CURRENT_USER);
        sURIMatcher.addURI(AUTHORITY, SNTP_RESULT_TABLE, SNTP_RESULT);
        sURIMatcher.addURI(AUTHORITY, PREFERENCE, PREFERENCE_INFO);
        sURIMatcher.addURI(AUTHORITY, FISHES_TABLE, FISHES);
    }

    private KitkitDBHandler myDB;

    @Override
    public boolean onCreate() {
        myDB = new KitkitDBHandler(getContext());
        return false;
    }

    @Nullable
    @Override
    public Cursor query(@NonNull Uri uri, @Nullable String[] projection, @Nullable String selection, @Nullable String[] selectionArgs, @Nullable String sortOrder) {
        SQLiteQueryBuilder queryBuilder = new SQLiteQueryBuilder();

        int uriType = sURIMatcher.match(uri);

        switch (uriType) {
            case USER_ID:
                queryBuilder.setTables(KitkitDBHandler.TABLE_USERS);
                queryBuilder.appendWhere(KitkitDBHandler.COLUMN_ID + "="
                        + uri.getLastPathSegment());
                break;
            case USER:
                queryBuilder.setTables(KitkitDBHandler.TABLE_USERS);
                break;
            case CURRENT_USER:
                queryBuilder.setTables(KitkitDBHandler.TABLE_CURRENT_USER);
                break;
            case SNTP_RESULT:
                queryBuilder.setTables(KitkitDBHandler.TABLE_SNTP_RESULT);
                break;
            case PREFERENCE_INFO:
                final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getContext());
                String tabletNumber = prefs.getString("TABLET_NUMBER", "");
                MatrixCursor cursor = new MatrixCursor(new String[]{"TABLET_NUMBER"});
                cursor.addRow(new String[]{String.valueOf(tabletNumber)});
                return cursor;
            case FISHES:
                queryBuilder.setTables(KitkitDBHandler.TABLE_FISHES);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI");
        }

        Cursor cursor = queryBuilder.query(myDB.getReadableDatabase(),
                projection, selection, selectionArgs, null, null,
                sortOrder);
        cursor.setNotificationUri(getContext().getContentResolver(),
                uri);
        return cursor;
    }


    @Nullable
    @Override
    public String getType(@NonNull Uri uri) {
        return null;
    }

    @Nullable
    @Override
    public Uri insert(@NonNull Uri uri, @Nullable ContentValues values) {
        int uriType = sURIMatcher.match(uri);

        SQLiteDatabase sqlDB = myDB.getWritableDatabase();

        long id = 0;
        switch (uriType) {
            case USER:
                id = sqlDB.insert(KitkitDBHandler.TABLE_USERS,
                        null, values);
                break;
            case CURRENT_USER:
                id = sqlDB.insert(KitkitDBHandler.TABLE_CURRENT_USER,
                        null, values);
                break;
            case SNTP_RESULT:
                id = sqlDB.insert(KitkitDBHandler.TABLE_SNTP_RESULT,
                        null, values);
                break;
            case FISHES:
                id = sqlDB.insert(KitkitDBHandler.TABLE_FISHES,
                        null, values);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI: "
                        + uri);
        }
        getContext().getContentResolver().notifyChange(uri, null);
        return Uri.parse(USER_TABLE + "/" + id);
    }

    @Override
    public int delete(@NonNull Uri uri, @Nullable String selection, @Nullable String[] selectionArgs) {
            int uriType = sURIMatcher.match(uri);
            SQLiteDatabase sqlDB = myDB.getWritableDatabase();
            int rowsDeleted = 0;

            switch (uriType) {
                case USER:
                    rowsDeleted = sqlDB.delete(KitkitDBHandler.TABLE_USERS,
                            selection,
                            selectionArgs);
                    break;

                case USER_ID:
                    String id = uri.getLastPathSegment();
                    if (TextUtils.isEmpty(selection)) {
                        rowsDeleted = sqlDB.delete(KitkitDBHandler.TABLE_USERS,
                                KitkitDBHandler.COLUMN_ID + "=" + id,
                                null);
                    } else {
                        rowsDeleted = sqlDB.delete(KitkitDBHandler.TABLE_USERS,
                                KitkitDBHandler.COLUMN_ID + "=" + id
                                        + " and " + selection,
                                selectionArgs);
                    }
                    break;
                case SNTP_RESULT:
                    rowsDeleted = sqlDB.delete(KitkitDBHandler.TABLE_SNTP_RESULT,
                            selection,
                            selectionArgs);
                    break;
                case FISHES:
                    rowsDeleted = sqlDB.delete(KitkitDBHandler.TABLE_FISHES,
                            selection,
                            selectionArgs);
                    break;
                default:
                    throw new IllegalArgumentException("Unknown URI: " + uri);
            }
            getContext().getContentResolver().notifyChange(uri, null);
            return rowsDeleted;
    }

    @Override
    public int update(@NonNull Uri uri, @Nullable ContentValues values, @Nullable String selection, @Nullable String[] selectionArgs) {
        int uriType = sURIMatcher.match(uri);
        SQLiteDatabase sqlDB = myDB.getWritableDatabase();
        int rowsUpdated = 0;

        switch (uriType) {
            case USER:
                rowsUpdated =
                        sqlDB.update(KitkitDBHandler.TABLE_USERS,
                                values,
                                selection,
                                selectionArgs);
                break;
            case USER_ID:
                String id = uri.getLastPathSegment();
                if (TextUtils.isEmpty(selection)) {
                    rowsUpdated =
                            sqlDB.update(KitkitDBHandler.TABLE_USERS,
                                    values,
                                    KitkitDBHandler.COLUMN_ID + "=" + id,
                                    null);
                } else {
                    rowsUpdated =
                            sqlDB.update(KitkitDBHandler.TABLE_USERS,
                                    values,
                                    KitkitDBHandler.COLUMN_ID + "=" + id
                                            + " and "
                                            + selection,
                                    selectionArgs);
                }
                break;
            case CURRENT_USER:
                rowsUpdated =
                        sqlDB.update(KitkitDBHandler.TABLE_CURRENT_USER,
                                values,
                                selection,
                                selectionArgs);
                break;
            case SNTP_RESULT:
                rowsUpdated =
                        sqlDB.update(KitkitDBHandler.TABLE_SNTP_RESULT, values, selection, selectionArgs);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI: " +
                        uri);
            case FISHES:
                rowsUpdated =
                        sqlDB.update(KitkitDBHandler.TABLE_FISHES, values, selection, selectionArgs);
                break;
        }
        getContext().getContentResolver().notifyChange(uri,
                null);
        return rowsUpdated;
    }

}
