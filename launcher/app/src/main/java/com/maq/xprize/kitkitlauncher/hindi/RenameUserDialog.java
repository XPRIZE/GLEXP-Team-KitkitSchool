package com.maq.xprize.kitkitlauncher.hindi;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.support.annotation.NonNull;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

import com.maq.kitkitProvider.User;

public class RenameUserDialog extends Dialog {
    private Context mContext;
    private Callback mCallback;

    private EditText mEtName;

    public RenameUserDialog(@NonNull Context context, User user, Callback callback) {
        super(context);

        mContext = context;
        mCallback = callback;

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

        setContentView(R.layout.dialog_rename_user);

        mEtName = (EditText) findViewById(R.id.et_name);
        mEtName.setText(user.getDisplayName());
        mEtName.setSelection(mEtName.length());
        mEtName.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                if (i == EditorInfo.IME_ACTION_DONE) {
                    processConfirm();
                    return true;
                }
                return false;
            }
        });

        findViewById(R.id.tv_cancel).setOnClickListener(mClickListener);
        findViewById(R.id.tv_confirm).setOnClickListener(mClickListener);
    }

    @Override
    public void dismiss() {
        InputMethodManager imm = (InputMethodManager) mContext.getSystemService(Activity.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(mEtName.getWindowToken(), 0);
        super.dismiss();
    }

    private View.OnClickListener mClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int id = view.getId();

            switch(id) {
                case R.id.tv_cancel:
                    dismiss();
                    break;

                case R.id.tv_confirm:
                    processConfirm();
                    break;
            }
        }
    };

    private void processConfirm() {
        if (mCallback != null) {
            mCallback.onSubmit(mEtName.getText().toString());
        }

        dismiss();
    }

    ////////////////////////////////////////////////////////////////////////////////

    public interface Callback {
        void onSubmit(String name);
    }
}

