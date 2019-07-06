package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.support.annotation.NonNull;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.KitkitSchoolApplication;

public class SelectNumberDialog extends Dialog {
    private final int MAX_TABLET_COUNT = 200;
    private Context mContext;
    private RecyclerView mRecyclerView;
    private MODE mMode;
    private int mCount;
    private Callback mCallback;
    public SelectNumberDialog(@NonNull Context context, MODE mode, Callback callback) {
        super(context);

        mContext = context;
        mMode = mode;

        if (mMode == MODE.USER_NO) {
            Activity activity = (Activity) context;
            mCount = ((KitkitSchoolApplication) activity.getApplication()).getDbHandler().numUser();

        } else {
            mCount = MAX_TABLET_COUNT;
        }

        mCallback = callback;

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

        setContentView(R.layout.dialog_select_number);
        Util.setScale(((Activity) context), findViewById(R.id.root));

        if (mMode == MODE.TABLET_NUM) {
            TextView tvTitle = (TextView) findViewById(R.id.tv_title);
            tvTitle.setText(mContext.getString(R.string.select_tablet_no));
            tvTitle.setBackgroundColor(0xff23cce6);
        }

        mRecyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        mRecyclerView.setLayoutManager(new GridLayoutManager(mContext, 5));
        mRecyclerView.setAdapter(new RecyclerAdapter());

        findViewById(R.id.root).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                dismiss();
                return false;
            }
        });

        findViewById(R.id.content).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // do noting
            }
        });
    }

    public enum MODE {
        USER_NO,
        TABLET_NUM
    }

    ////////////////////////////////////////////////////////////////////////////////

    public interface Callback {
        void onSelectedNumber(int number);
    }

    public class RecyclerAdapter extends RecyclerView.Adapter<RecyclerAdapter.ViewHolder> {

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            final View view = LayoutInflater.from(mContext).inflate(R.layout.item_number, parent, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, final int position) {
            if (mMode == MODE.TABLET_NUM) {
                holder.mTvNumber.setBackgroundResource(R.drawable.number_selector2);

            } else {
                holder.mTvNumber.setBackgroundResource(R.drawable.number_selector);

            }

            holder.mTvNumber.setText("" + (position + 1) % mCount);
            holder.mTvNumber.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (mCallback != null) {
                        mCallback.onSelectedNumber((position + 1) % mCount);
                        dismiss();
                    }
                }
            });
        }

        @Override
        public int getItemCount() {
            return mCount;
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            private TextView mTvNumber;

            public ViewHolder(View itemView) {
                super(itemView);
                mTvNumber = (TextView) itemView.findViewById(R.id.tv_number);
            }
        }
    }
}

