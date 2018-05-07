package com.enuma.kitkitProvider;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteQueryBuilder;
import android.net.Uri;
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
    public static final Uri CONTENT_URI =
            Uri.parse("content://" + AUTHORITY + "/" + USER_TABLE);

    public static final Uri CURRENT_USER_URI =
            Uri.parse("content://" + AUTHORITY + "/" + CURRENT_USER_TABLE);


    public static final int USER = 1;
    public static final int USER_ID = 2;
    public static final int CURRENT_USER = 3;

    private static final UriMatcher sURIMatcher =
            new UriMatcher(UriMatcher.NO_MATCH);

    static {
        sURIMatcher.addURI(AUTHORITY, USER_TABLE, USER);
        sURIMatcher.addURI(AUTHORITY, USER_TABLE + "/#",
                USER_ID);

        sURIMatcher.addURI(AUTHORITY, CURRENT_USER_TABLE, CURRENT_USER);

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
            default:
                throw new IllegalArgumentException("Unknown URI");
        }

        Cursor cursor = queryBuilder.query(myDB.getReadableDatabase(),
                projection, selection, selectionArgs, null, null,
                sortOrder);
        cursor.setNotificationUri(getContext().getContentResolver(),
                uri);
        return cursor;    }


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
            default:
                throw new IllegalArgumentException("Unknown URI: " +
                        uri);
        }
        getContext().getContentResolver().notifyChange(uri,
                null);
        return rowsUpdated;
    }

}