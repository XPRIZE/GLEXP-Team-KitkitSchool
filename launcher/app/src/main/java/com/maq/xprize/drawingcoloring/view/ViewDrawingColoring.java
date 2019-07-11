package com.maq.xprize.drawingcoloring.view;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.maq.xprize.drawingcoloring.utility.Misc;
import com.maq.xprize.drawingcoloring.utility.Util;
import com.maq.xprize.kitkitlauncher.hindi.R;

import java.io.File;

public class ViewDrawingColoring extends View {

    public enum MODE {
        DRAWING,
        COLORING
    }
    private final int BRUSH_WIDTH_COUNT = 5;
    private final int BRUSH_HEIGHT_COUNT = 2;
    private final int BRUSH_COUNT = BRUSH_WIDTH_COUNT * BRUSH_HEIGHT_COUNT;
    private int BRUSH_POINT_WIDTH = 0;
    private int BRUSH_POINT_HEIGHT = 0;
    private static final float TOUCH_TOLERANCE = 4;

    private Context mContext;

    private Bitmap mBitmapBrushAlphaChannel;
    private Bitmap mBitmapBrush;
    private Bitmap mBitmapBuffer;
    private Canvas mCanvasBuffer;

    private Callback mCallback;
    private MODE mMode = MODE.DRAWING;
    private int mCurrentColor;
    private int mTouchId;
    private float mTouchPosX, mTouchPosY;

    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    private Paint mPaintDrawing = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);
    private Paint mPaintColoring = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    private Path mPathColoring = new Path();
    private Rect mRect = new Rect();
    private boolean mbInit = false;

    public ViewDrawingColoring(Context context) {
        super(context);
        init(context);
    }

    public ViewDrawingColoring(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public ViewDrawingColoring(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mContext = context;
        mTouchPosX = Float.MIN_VALUE;
        mTouchPosY = Float.MIN_VALUE;

        Point size = Util.getWindowSize((Activity)context);
        boolean isSmallLCD = (size.x <= 1280);

        mBitmapBrushAlphaChannel = BitmapFactory.decodeResource(mContext.getResources(), isSmallLCD ? R.drawable.crayon_brush_alpha_s : R.drawable.crayon_brush_alpha);
        mBitmapBrush = Bitmap.createBitmap(mBitmapBrushAlphaChannel.getWidth(), mBitmapBrushAlphaChannel.getHeight(), Bitmap.Config.ARGB_8888);
        BRUSH_POINT_WIDTH = mBitmapBrushAlphaChannel.getWidth() / BRUSH_WIDTH_COUNT;
        BRUSH_POINT_HEIGHT = mBitmapBrushAlphaChannel.getHeight() / BRUSH_HEIGHT_COUNT;

        mPaintColoring.setStyle(Paint.Style.STROKE);
        mPaintColoring.setStrokeJoin(Paint.Join.ROUND);
        mPaintColoring.setStrokeCap(Paint.Cap.ROUND);
        mPaintColoring.setStrokeWidth(BRUSH_POINT_WIDTH / 3.0f * 2);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (isInEditMode() == true) {
            return;
        }

        if (mBitmapBuffer == null) {
            mBitmapBuffer = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            mCanvasBuffer = new Canvas(mBitmapBuffer);
            mBitmapBuffer.eraseColor(Color.TRANSPARENT);
            mRect.set(0, 0, getWidth(), getHeight());
            restoreTempBitmapFile();
            mbInit = true;
        }

        canvas.drawBitmap(mBitmapBuffer, 0, 0, mPaint);

    }

    private void doTouchDown(float x, float y) {
        mTouchPosX = x;
        mTouchPosY = y;

        drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) mTouchPosX, (int) mTouchPosY);

        if (mCallback != null) {
            mCallback.onTouchDownForDrawing();
        }
    }

    private void doTouchMove(float x, float y) {
        if (mTouchPosX == Float.MIN_VALUE || mTouchPosY == Float.MIN_VALUE) {
            return;
        }

        float dx = Math.abs(x - mTouchPosX);
        float dy = Math.abs(y - mTouchPosY);
        if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
            drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) x, (int) y);
            mTouchPosX = x;
            mTouchPosY = y;
        }
    }

    private void doTouchUp(float x, float y) {
        mTouchPosX = Float.MIN_VALUE;
        mTouchPosY = Float.MIN_VALUE;
    }

    /**
     * Random 한 Brush Point Image
     *
     * @return
     */
    private Bitmap getBrushPointBitmap() {
        Bitmap result;

        int rand = (int) (Math.random() * BRUSH_COUNT);
        result = Bitmap.createBitmap(mBitmapBrush,
                (rand % BRUSH_WIDTH_COUNT) * BRUSH_POINT_WIDTH,
                (rand / BRUSH_WIDTH_COUNT) * BRUSH_POINT_HEIGHT,
                BRUSH_POINT_WIDTH,
                BRUSH_POINT_HEIGHT);

        return result;
    }

    /**
     * 크레용 질감의 Line Drawing
     * @param canvas
     * @param startX
     * @param startY
     * @param endX
     * @param endY
     */
    private void drawLineWithBrush(Canvas canvas, int startX, int startY, int endX, int endY) {
        int distance = (int) Util.getDistanceBetween2Point(startX, startY, endX, endY);
        double angle = Util.getRadianAngleBetween2Point(startX, startY, endX, endY);

        int halfBrushWidth = BRUSH_POINT_WIDTH / 2;
        int halfBrushHeight = BRUSH_POINT_WIDTH / 2;
        int x, y;

        int offset = halfBrushWidth / 2;

        for (int i = 0; i <= distance; i += offset) {
            x = (int) (startX + (Math.sin(angle) * i) - halfBrushWidth);
            y = (int) (startY + (Math.cos(angle) * i) - halfBrushHeight);
            canvas.drawBitmap(getBrushPointBitmap(), x, y, mPaintDrawing);
        }
    }

    /**
     * 임시 파일 삭제
     */
    private void deleteTempBitmapFile() {
        File file = new File(Misc.getTempFilePath(mMode));
        if (file.exists() == true) {
            file.delete();
        }
    }

    /**
     * 임시파일을 Double Buffer 로 적용
     */
    private void restoreTempBitmapFile() {
        File tempFile = new File(Misc.getTempFilePath(mMode));
        if (tempFile.exists() == true) {
            Bitmap bitmap = BitmapFactory.decodeFile(tempFile.getAbsolutePath());
            if (bitmap != null && mBitmapBuffer != null) {
                mCanvasBuffer.drawBitmap(bitmap, 0, 0, mPaint);
            }
        }
    }

    public void setCallback(Callback callback) {
        mCallback = callback;
    }

    public void setMode(MODE mode) {
        mMode = mode;
        setPenColor(mCurrentColor);
    }

    public MODE getMode() {
        return mMode;
    }

    public void setPenColor(int color) {
        mCurrentColor = color;
        mBitmapBrush.eraseColor(color);
        Canvas canvas = new Canvas(mBitmapBrush);
        Paint paint = new Paint(mPaintDrawing);
        paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_IN));
        canvas.drawBitmap(mBitmapBrushAlphaChannel, 0, 0, paint);
    }

    public void clearAll() {
        mBitmapBuffer.eraseColor(Color.TRANSPARENT);
        mPathColoring.reset();
        deleteTempBitmapFile();
        invalidate();
    }

    public void saveTempBitmapFile() {
        if (mBitmapBuffer != null) {
            Util.saveImageFileFromBitmap(mBitmapBuffer, Misc.getTempFilePath(mMode), 100);
        }
    }

    public void doTouchEvent(int action, float x, float y) {
        boolean isInvalidate = false;
        if (mRect.contains((int) x, (int) y) == true || mRect.contains((int) mTouchPosX, (int) mTouchPosY) == true) {
            isInvalidate = true;
        }

        switch (action) {
            case MotionEvent.ACTION_DOWN:
                doTouchDown(x, y);
                break;

            case MotionEvent.ACTION_MOVE:
                doTouchMove(x, y);
                break;

            case MotionEvent.ACTION_UP:
                doTouchUp(x, y);
                break;
        }

        if (isInvalidate == true) {
            invalidate();
        }
    }

    public boolean isInit() {
        return mbInit;
    }

    public interface Callback {
        void onTouchDownForDrawing();
    }
}