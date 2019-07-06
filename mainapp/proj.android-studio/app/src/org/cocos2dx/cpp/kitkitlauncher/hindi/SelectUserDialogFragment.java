package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.app.LoaderManager;
import android.content.CursorLoader;
import android.content.DialogInterface;
import android.content.Loader;
import android.database.Cursor;
import android.os.Bundle;
import android.support.v4.widget.SimpleCursorAdapter;
import android.widget.ArrayAdapter;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.KitkitProvider;
import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.KitkitSchoolApplication;

/**
 * Created by loiceandia on 03/01/2018.
 */

public class SelectUserDialogFragment extends DialogFragment implements LoaderManager.LoaderCallbacks<Cursor> {

    ArrayAdapter<String> arrayAdapter;
    SimpleCursorAdapter mAdapter;
    String userBundle;


    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        getLoaderManager().initLoader(0, null, this);


    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        if (this.getArguments() != null) {
            userBundle = this.getArguments().getString("user");
        }

        mAdapter = new SimpleCursorAdapter(getActivity(),
                android.R.layout.simple_list_item_2, null,
                new String[]{KitkitDBHandler.COLUMN_USERNAME},
                new int[]{android.R.id.text1}, 0);
        builder.setTitle(R.string.select_user)
                .setAdapter(mAdapter, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Cursor cursor = (Cursor) mAdapter.getItem(which);
                        String userName = cursor.getString(cursor.getColumnIndex(KitkitDBHandler.COLUMN_USERNAME));
                        ((KitkitSchoolApplication) getActivity().getApplication()).getDbHandler().setCurrentUser(userName);
                        if (userBundle != null && userBundle.equals("ABOUT")) {
                            ((AboutActivity) getActivity()).onResume();
                        } else {
                            ((SettingActivity) getActivity()).refreshUI();
                        }
                        cursor.close();
                    }
                });

        return builder.create();
    }

    @Override
    public Loader<Cursor> onCreateLoader(int id, Bundle args) {
        return new CursorLoader(getActivity(),
                KitkitProvider.CONTENT_URI
                , new String[]{KitkitDBHandler.COLUMN_ID, KitkitDBHandler.COLUMN_USERNAME}, null, null, null);

    }

    @Override
    public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
/*            arrayAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.select_dialog_singlechoice);
            data.moveToFirst();
            while (!data.isAfterLast()) {
                arrayAdapter.add(data.getString(0));
                data.moveToNext();
            }*/
        mAdapter.swapCursor(data);
    }

    @Override
    public void onLoaderReset(Loader<Cursor> loader) {
        mAdapter.swapCursor(null);
    }
}
