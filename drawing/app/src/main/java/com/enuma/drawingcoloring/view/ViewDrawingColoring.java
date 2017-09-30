package com.enuma.drawingcoloring.view;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.enuma.drawingcoloring.R;
import com.enuma.drawingcoloring.utility.Misc;
import com.enuma.drawingcoloring.utility.Util;

import java.io.File;

/**
 * MODE.DRAWING : 크레용 질감 표현 (Brush Image (5 * 2) 이용)
 * MODE.COLORING : 2017.09.12 DRAWING 과 동일하게 변경 (이전 일반 Path 표현)
 * 임시 작업 파일의 Save, Restore, Delete 기능 제공
 */
public class ViewDrawingColoring extends View {

    ////////////////////////////////////////////////////////////////////////////////

    public enum MODE {
        DRAWING,
        COLORING
    }

    /**
     * Brush 이미지의 수평 갯수
     */
    private final int BRUSH_WIDTH_COUNT = 5;

    /**
     * Brush 이미지의 수직 갯수
     */
    private final int BRUSH_HEIGHT_COUNT = 2;

    /**
     * Brush 이미지의 Brush 갯수
     */
    private final int BRUSH_COUNT = BRUSH_WIDTH_COUNT * BRUSH_HEIGHT_COUNT;

    /**
     * 하나의 Brush Image width
     */
    private int BRUSH_POINT_WIDTH = 0;

    /**
     * 하나의 Brush Image height
     */
    private int BRUSH_POINT_HEIGHT = 0;

    /**
     * Touch 의 Move Event 가 TOUCH_TOLERANCE 이내로 움직이면 무시
     */
    private static final float TOUCH_TOLERANCE = 4;

    ////////////////////////////////////////////////////////////////////////////////

    private Context mContext;

    ////////////////////////////////////////////////////////////////////////////////

    /** 원본 Brush Alpha 채널 이미지 */
    private Bitmap mBitmapBrushAlphaChannel;

    /** 실제 사용하는 Brush 이미지 */
    private Bitmap mBitmapBrush;

    /** Double Buffer */
    private Bitmap mBitmapBuffer;
    private Canvas mCanvasBuffer;

    ////////////////////////////////////////////////////////////////////////////////

    private Callback mCallback;
    private MODE mMode = MODE.DRAWING;
    private int mCurrentColor;
    private int mTouchId;
    private float mTouchPosX, mTouchPosY;

    /** 일반적으로 사용 Paint */
    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    private Paint mPaintDrawing = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);
    private Paint mPaintColoring = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    private Path mPathColoring = new Path();
    private Rect mRect = new Rect();

    ////////////////////////////////////////////////////////////////////////////////

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

        mBitmapBrushAlphaChannel = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.crayon_brush_alpha);
        mBitmapBrush = Bitmap.createBitmap(mBitmapBrushAlphaChannel.getWidth(), mBitmapBrushAlphaChannel.getHeight(), Bitmap.Config.ARGB_8888);
        BRUSH_POINT_WIDTH = mBitmapBrushAlphaChannel.getWidth() / BRUSH_WIDTH_COUNT;
        BRUSH_POINT_HEIGHT = mBitmapBrushAlphaChannel.getHeight() / BRUSH_HEIGHT_COUNT;

        mPaintColoring.setStyle(Paint.Style.STROKE);
        mPaintColoring.setStrokeJoin(Paint.Join.ROUND);
        mPaintColoring.setStrokeCap(Paint.Cap.ROUND);
        mPaintColoring.setStrokeWidth(BRUSH_POINT_WIDTH / 3.0f * 2);
    }

    ////////////////////////////////////////////////////////////////////////////////

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
        }

        canvas.drawBitmap(mBitmapBuffer, 0, 0, mPaint);

//        if (mMode == MODE.COLORING) {
//            if (mPathColoring.isEmpty() == false) {
//                canvas.drawPath(mPathColoring, mPaintColoring);
//            }
//        }
    }

//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        float x;
//        float y;
//
//        switch (event.getAction() & MotionEvent.ACTION_MASK) {
//            case MotionEvent.ACTION_DOWN:
//                mTouchId = event.getPointerId(0);
//                x = event.getX(event.findPointerIndex(mTouchId));
//                y = event.getY(event.findPointerIndex(mTouchId));
//
//                doTouchDown(x, y);
//                invalidate();
//                break;
//
//            case MotionEvent.ACTION_MOVE:
//                if (event.findPointerIndex(mTouchId) != -1) {
//                    x = event.getX(event.findPointerIndex(mTouchId));
//                    y = event.getY(event.findPointerIndex(mTouchId));
//                    doTouchMove(x, y);
//                }
//                invalidate();
//                break;
//
//            case MotionEvent.ACTION_UP:
//                if (event.findPointerIndex(mTouchId) != -1) {
//                    x = event.getX(event.findPointerIndex(mTouchId));
//                    y = event.getY(event.findPointerIndex(mTouchId));
//                    doTouchUp(x, y);
//                }
//                invalidate();
//                break;
//        }
//
//        return true;
//    }

    ////////////////////////////////////////////////////////////////////////////////

    private void doTouchDown(float x, float y) {
        mTouchPosX = x;
        mTouchPosY = y;

//        if (mMode == MODE.DRAWING) {
//            drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) mTouchPosX, (int) mTouchPosY);
//
//        } else {
//            mPathColoring.moveTo(mTouchPosX, mTouchPosY);
//
//        }

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

//            if (mMode == MODE.DRAWING) {
//                drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) x, (int) y);
//
//            } else {
//                mPathColoring.quadTo(mTouchPosX, mTouchPosY, (x + mTouchPosX) / 2, (y + mTouchPosY) / 2);
//
//            }

            drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) x, (int) y);

            mTouchPosX = x;
            mTouchPosY = y;
        }
    }

    private void doTouchUp(float x, float y) {
//        if (mMode == MODE.COLORING) {
//            if (x == mTouchPosX && y == mTouchPosY) {
//                mPathColoring.quadTo(x, y, (x + mTouchPosX) / 2 + 1, (y + mTouchPosY) / 2 + 1);
//
//            } else {
//                mPathColoring.quadTo(x, y, (x + mTouchPosX) / 2, (y + mTouchPosY) / 2);
//
//            }
//
//            mCanvasBuffer.drawPath(mPathColoring, mPaintColoring);
//            mPathColoring.reset();
//        }

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

    ////////////////////////////////////////////////////////////////////////////////

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

//        if (mMode == MODE.DRAWING) {
//            mBitmapBrush.eraseColor(color);
//            Canvas canvas = new Canvas(mBitmapBrush);
//            Paint paint = new Paint(mPaintDrawing);
//            paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_IN));
//            canvas.drawBitmap(mBitmapBrushAlphaChannel, 0, 0, paint);
//
//        } else {
//            mPaintColoring.setColor(color);
//
//        }

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

    ////////////////////////////////////////////////////////////////////////////////

    public interface Callback {
        void onTouchDownForDrawing();
    }
}
