package org.cocos2dx.cpp.drawingcoloring.view;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.support.annotation.Nullable;
import android.support.v7.widget.AppCompatImageView;
import android.util.AttributeSet;

import com.maq.xprize.kitkitschool.hindi.R;

import org.cocos2dx.cpp.drawingcoloring.utility.Util;

public class ViewPen extends AppCompatImageView {

    private Context mContext;
    private float mNormalMarginTop;
    private float mNormalMarginLeft;

    private Bitmap mBitmapPen;
    private Bitmap mBitmapSelect;
    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG | Paint.DITHER_FLAG);

    public ViewPen(Context context) {
        super(context);
        init(context);
    }

    public ViewPen(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public ViewPen(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    public void init(Context context) {
        mContext = context;
        Point size = Util.getWindowSize((Activity) context);
        boolean isSmallLCD = (size.x <= 1280);

        if (isSmallLCD == true) {
            mNormalMarginTop = mContext.getResources().getDimension(R.dimen.pen_normal_margin_top_s);
            mNormalMarginLeft = mContext.getResources().getDimension(R.dimen.pen_normal_margin_left_s);

        } else {
            mNormalMarginTop = mContext.getResources().getDimension(R.dimen.pen_normal_margin_top);
            mNormalMarginLeft = mContext.getResources().getDimension(R.dimen.pen_normal_margin_left);

        }
    }

    @Override
    protected void drawableStateChanged() {
        super.drawableStateChanged();
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (mBitmapPen != null) {
            if (isSelected() == true) {
                canvas.drawBitmap(mBitmapPen, 0, mNormalMarginTop, mPaint);

            } else {
                canvas.drawBitmap(mBitmapPen, mNormalMarginLeft, mNormalMarginTop, mPaint);

            }
        }

        if (mBitmapSelect != null) {
            if (isSelected() == true) {
                canvas.drawBitmap(mBitmapSelect, 0, 0, mPaint);
            }
        }
    }

    public void setPenImage(int resourceIdPen, Bitmap bitmapSelect) {
        if (resourceIdPen != -1) {
            mBitmapPen = BitmapFactory.decodeResource(mContext.getResources(), resourceIdPen);
        }

        mBitmapSelect = bitmapSelect;
    }
}