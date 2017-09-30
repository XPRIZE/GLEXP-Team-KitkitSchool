package com.enuma.drawingcoloring.dialog;

import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.enuma.drawingcoloring.R;
import com.enuma.drawingcoloring.activity.DrawingColoringActivity;
import com.enuma.drawingcoloring.utility.Util;

public class DialogSelectDrawingBackground extends Dialog {

    ////////////////////////////////////////////////////////////////////////////////

    private Context mContext;
    private Callback mCallback;

    ////////////////////////////////////////////////////////////////////////////////

    public DialogSelectDrawingBackground(Context context, Callback callback) {
        super(context);

        mContext = context;
        mCallback = callback;
        getWindow().setBackgroundDrawable(new ColorDrawable(0));
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.dialog_drawing_background);
        setCanceledOnTouchOutside(true);
        setUpView();
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void setUpView() {
        findViewById(R.id.layout_root).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    dismiss();
                }
                return true;
            }
        });

        findViewById(R.id.iv_background).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // do nothing
                return true;
            }
        });

        findViewById(R.id.v_close).setOnClickListener(mOnClickListener);
        for (int i = 0; i < DrawingColoringActivity.TOTAL_BG_COLOR_COUNT; ++i) {
            View view = findViewById(Util.getResourceId(mContext, "v_bg_" + i, "id", mContext.getPackageName()));
            view.setTag(new Integer(i));
            view.setOnClickListener(mOnClickListener);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    private View.OnClickListener mOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int id = view.getId();
            Integer colorIndexTag = (Integer) view.getTag();
            if (colorIndexTag != null) {
                if (mCallback != null) {
                    mCallback.onSelectBGColorIndex(colorIndexTag.intValue());
                    dismiss();
                }

            } else {
                if (id == R.id.v_close) {
                    dismiss();
                }
            }
        }
    };

    public interface Callback {
        void onSelectBGColorIndex(int colorIndex);
    }
}
