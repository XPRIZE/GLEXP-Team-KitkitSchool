package com.maq.xprize.kitkitlauncher.hindi;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.support.annotation.NonNull;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.maq.kitkitProvider.User;

import java.util.ArrayList;

public class UserNameListDialog extends Dialog {
    private Context mContext;
    private ArrayList<User> mData;
    private RecyclerView mRecyclerView;

    public UserNameListDialog(@NonNull Context context, ArrayList<User> users) {
        super(context);

        mContext = context;
        mData = users;

        if (mData != null && mData.get(0).getUserName().equals("user0")) {
            User user0 = mData.remove(0);
            mData.add(user0);
        }

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

        setContentView(R.layout.dialog_user_list);
        Util.setScale(((Activity) context), findViewById(R.id.root));

        mRecyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(mContext, LinearLayoutManager.VERTICAL, false));
        mRecyclerView.setAdapter(new RecyclerAdapter());

        findViewById(R.id.root).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                dismiss();
                return false;
            }
        });

        findViewById(R.id.content).setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                // do noting
            }
        });
    }

    ////////////////////////////////////////////////////////////////////////////////

    public class RecyclerAdapter extends RecyclerView.Adapter<RecyclerAdapter.ViewHolder> {

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            final View view = LayoutInflater.from(mContext).inflate(R.layout.item_user_name, parent, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, final int position) {
            User item = mData.get(position);
            holder.mTvNumber.setText(item.getUserName());
            holder.mTvName.setText(item.getDisplayName());
            holder.mTvMaths.setText(""+item.getMaths());
            holder.mTvEnglish.setText(""+item.getEnglish());
            holder.mTvTotal.setText(""+item.getEnglish()+item.getMaths());
            holder.mTvCoins.setText(""+item.get_coins());

            holder.itemView.setBackgroundColor(position % 2 == 0 ? Color.LTGRAY : 0xd5d5d5);
        }

        @Override
        public int getItemCount() {
            if (mData == null) {
                return 0;
            }
            return mData.size();
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            private TextView mTvNumber;
            private TextView mTvName;
            private TextView mTvMaths;
            private TextView mTvEnglish;
            private TextView mTvTotal;
            private TextView mTvCoins;

            public ViewHolder(View itemView) {
                super(itemView);
                mTvNumber = (TextView) itemView.findViewById(R.id.tv_number);
                mTvName = (TextView) itemView.findViewById(R.id.tv_name);
                mTvMaths= (TextView) itemView.findViewById(R.id.tv_maths);
                mTvEnglish = (TextView) itemView.findViewById(R.id.tv_english);
                mTvTotal = (TextView) itemView.findViewById(R.id.tv_total);
                mTvCoins = (TextView) itemView.findViewById(R.id.tv_coins);
            }
        }
    }
}

