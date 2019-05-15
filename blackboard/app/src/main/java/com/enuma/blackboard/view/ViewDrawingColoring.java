package com.enuma.blackboard.view;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.animation.PropertyValuesHolder;
import android.animation.ValueAnimator;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.RectF;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.DecelerateInterpolator;

import com.enuma.blackboard.R;
import com.enuma.blackboard.utility.EffectSound;
import com.enuma.blackboard.utility.Log;
import com.enuma.blackboard.utility.Util;

/**
 * MODE.DRAWING : 크레용 질감 표현 (Brush Image (5 * 2) 이용)
 */
public class ViewDrawingColoring extends View {

    ////////////////////////////////////////////////////////////////////////////////

    public enum MODE {
        DRAWING,
        ERASE
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
     * 지우개 기준 위치
     */
    private int ERASER_DEFAULT_POS_X = 0;
    private int ERASER_DEFAULT_POS_Y = 0;

    /**
     * 하단 지우개 받침 높이
     */
    private int PADDING_BOTTOM = 0;

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
    private boolean mbSmallLCD;

    ////////////////////////////////////////////////////////////////////////////////

    /**
     * 원본 Brush Alpha 채널 이미지
     */
    private Bitmap mBitmapBrushAlphaChannel;

    /**
     * 실제 사용하는 Brush 이미지
     */
    private Bitmap mBitmapBrush;

    /**
     * Double Buffer
     */
    private Bitmap mBitmapBuffer;
    private Canvas mCanvasBuffer;

    /**
     * Eraser Trace 를 위한 Buffer
     */
    private Bitmap mBitmapTraceBuffer;
    private Canvas mCanvasTraceBuffer;

    /**
     * 지우개
      */
    private Bitmap mBitmapEraser;

    /**
     * 일반적으로 사용 Paint
     */
    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    private Paint mPaintDrawing = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);
    private Paint mPaintEraser = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);
    private Paint mPaintTrace = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);
    private Paint mPaintTraceBuffer = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.FILTER_BITMAP_FLAG);

    ////////////////////////////////////////////////////////////////////////////////

    private Callback mCallback;
    private MODE mMode = MODE.DRAWING;
    private int mCurrentColor;
    private int mTouchId;
    private float mTouchPosX, mTouchPosY;
    private float mEraserOffsetX, mEraserOffsetY;
    private RectF mEraserRect = new RectF();
    private RectF mTempRect = new RectF();

    private Property mPropertyEraser = new Property();
    private ValueAnimator mAnimatorTrace = null;
    private EffectSound mEffectSound;
    private boolean mbSoundingChalk = false;

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
        Point size = Util.getWindowSize((Activity)context);
        mbSmallLCD = (size.x <= 1280);

        mEffectSound = EffectSound.getInstance(mContext);

        mTouchPosX = Float.MIN_VALUE;
        mTouchPosY = Float.MIN_VALUE;

        mBitmapBrushAlphaChannel = BitmapFactory.decodeResource(mContext.getResources(), mbSmallLCD ? R.drawable.crayon_brush_alpha_s : R.drawable.crayon_brush_alpha);
        mBitmapBrush = Bitmap.createBitmap(mBitmapBrushAlphaChannel.getWidth(), mBitmapBrushAlphaChannel.getHeight(), Bitmap.Config.ARGB_8888);

        BRUSH_POINT_WIDTH = mBitmapBrushAlphaChannel.getWidth() / BRUSH_WIDTH_COUNT;
        BRUSH_POINT_HEIGHT = mBitmapBrushAlphaChannel.getHeight() / BRUSH_HEIGHT_COUNT;

        mBitmapEraser = BitmapFactory.decodeResource(mContext.getResources(), mbSmallLCD ? R.drawable.tool_blackboard_eraser_s : R.drawable.tool_blackboard_eraser);
        if (mbSmallLCD == true) {
            mEraserRect.set(26 / 2, 14 / 2, 436 / 2, 234 / 2);

        } else {
            mEraserRect.set(26, 14, 436, 234);
        }


        mPaintEraser.setAlpha(255);
        mPaintEraser.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));
        mPaintEraser.setMaskFilter(null);
        mPaintEraser.setStyle(Paint.Style.STROKE);
        mPaintEraser.setStrokeJoin(Paint.Join.ROUND);
        mPaintEraser.setStrokeCap(Paint.Cap.ROUND);
        mPaintEraser.setStrokeWidth(mEraserRect.height());

        mPaintTrace.setColor(0xff353f41);
        mPaintTrace.setAlpha(20);
        mPaintTrace.setXfermode(null);
        mPaintTrace.setMaskFilter(null);
        mPaintTrace.setStyle(Paint.Style.STROKE);
        mPaintTrace.setStrokeJoin(Paint.Join.ROUND);
        mPaintTrace.setStrokeCap(Paint.Cap.ROUND);
        mPaintTrace.setStrokeWidth(mEraserRect.height());
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

            mBitmapTraceBuffer = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            mCanvasTraceBuffer = new Canvas(mBitmapTraceBuffer);
            mBitmapTraceBuffer.eraseColor(Color.TRANSPARENT);

            ERASER_DEFAULT_POS_X = getWidth() - mBitmapEraser.getWidth() - mBitmapEraser.getWidth() / 4;
            if (mbSmallLCD == true) {
                ERASER_DEFAULT_POS_Y = getHeight() - mBitmapEraser.getHeight() - 50 / 2;
            } else {
                ERASER_DEFAULT_POS_Y = getHeight() - mBitmapEraser.getHeight() - 50;
            }

            mPropertyEraser.setX(ERASER_DEFAULT_POS_X);
            mPropertyEraser.setY(ERASER_DEFAULT_POS_Y);

            if (mbSmallLCD == true) {
                PADDING_BOTTOM = 94 / 2;

            } else {
                PADDING_BOTTOM = 94;
            }
        }

        canvas.save();
        canvas.clipRect(0, 0, getWidth(), getHeight() - PADDING_BOTTOM);
        canvas.drawBitmap(mBitmapBuffer, 0, 0, mPaint);

        if (mMode == MODE.ERASE || mPaintTraceBuffer.getAlpha() > 0) {
            canvas.drawBitmap(mBitmapTraceBuffer, 0, 0, mPaintTraceBuffer);
        }

        canvas.restore();
        canvas.drawBitmap(mBitmapEraser, mPropertyEraser.getX(), mPropertyEraser.getY(), mPaint);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mCanvasBuffer == null) {
            Log.i("mCanvasBuffer is null");
            return false;
        }

        float x;
        float y;

        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                mTouchId = event.getPointerId(0);
                x = event.getX(event.findPointerIndex(mTouchId));
                y = event.getY(event.findPointerIndex(mTouchId));

                doTouchDown(x, y);
                invalidate();
                break;

            case MotionEvent.ACTION_MOVE:
                if (event.findPointerIndex(mTouchId) != -1) {
                    x = event.getX(event.findPointerIndex(mTouchId));
                    y = event.getY(event.findPointerIndex(mTouchId));
                    doTouchMove(x, y);
                }
                invalidate();
                break;

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP: {
                int touchIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                if (touchIndex == 0) {
                    if (event.findPointerIndex(mTouchId) != -1) {
                        x = event.getX(event.findPointerIndex(mTouchId));
                        y = event.getY(event.findPointerIndex(mTouchId));
                        doTouchUp(x, y);
                    }
                    invalidate();
                }
            }
            break;
        }

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////

    private void doTouchDown(float x, float y) {
        mTouchPosX = x;
        mTouchPosY = y;

        if (mMode == MODE.DRAWING) {
            mTempRect.set(mEraserRect);
            mTempRect.offset(mPropertyEraser.getX(), mPropertyEraser.getY());
            if (mTempRect.contains(mTouchPosX, mTouchPosY) == true) {
                setMode(MODE.ERASE);
            }
        }

        if (mMode == MODE.DRAWING) {
            drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) mTouchPosX, (int) mTouchPosY);
            mPaintTraceBuffer.setAlpha(0);

        } else {
            if (mAnimatorTrace != null) {
                mAnimatorTrace.end();
            }

            mBitmapTraceBuffer.eraseColor(Color.TRANSPARENT);
            mPaintTraceBuffer.setAlpha(255);

            mEraserOffsetX = mTouchPosX - mPropertyEraser.getX();
            mEraserOffsetY = mTouchPosY - mPropertyEraser.getY();

            mPropertyEraser.setX(mTouchPosX - mEraserOffsetX);
            mPropertyEraser.setY(mTouchPosY - mEraserOffsetY);

            eraseLine(mCanvasBuffer,
                    (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                    (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                    mPaintEraser);

            eraseLine(mCanvasTraceBuffer,
                    (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                    (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                    mPaintTrace);

        }

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
            if (mMode == MODE.DRAWING) {
                drawLineWithBrush(mCanvasBuffer, (int) mTouchPosX, (int) mTouchPosY, (int) x, (int) y);

            } else {
                float oldX = mPropertyEraser.getX();
                float oldY = mPropertyEraser.getY();

                mPropertyEraser.setX(mTouchPosX - mEraserOffsetX);
                mPropertyEraser.setY(mTouchPosY - mEraserOffsetY);

                eraseLine(mCanvasBuffer,
                        (int) oldX + mBitmapEraser.getWidth() / 2, (int) oldY + mBitmapEraser.getHeight() / 2,
                        (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                        mPaintEraser);

                eraseLine(mCanvasTraceBuffer,
                        (int) oldX + mBitmapEraser.getWidth() / 2, (int) oldY + mBitmapEraser.getHeight() / 2,
                        (int) mPropertyEraser.getX() + mBitmapEraser.getWidth() / 2, (int) mPropertyEraser.getY() + mBitmapEraser.getHeight() / 2,
                        mPaintTrace);
            }

            if (mbSoundingChalk == false) {
                startChalkingSound();
            }

            mTouchPosX = x;
            mTouchPosY = y;

        } else {
            stopChalkingSound();
        }
    }

    private void doTouchUp(float x, float y) {
        mTouchPosX = Float.MIN_VALUE;
        mTouchPosY = Float.MIN_VALUE;

        if (mMode == MODE.ERASE) {
            final int TOLERANCE = 10;
            if (Math.abs(ERASER_DEFAULT_POS_X - mPropertyEraser.getX()) > TOLERANCE || Math.abs(ERASER_DEFAULT_POS_Y - mPropertyEraser.getY()) > TOLERANCE) {
                doEraserPositionAnimation(mPropertyEraser, mPropertyEraser.getX(), mPropertyEraser.getY(), ERASER_DEFAULT_POS_X, ERASER_DEFAULT_POS_Y);
                mAnimatorTrace = doTraceAnimation(mPaintTraceBuffer, mPaintTraceBuffer.getAlpha(), 0);

            } else {
                mPropertyEraser.setX(ERASER_DEFAULT_POS_X);
                mPropertyEraser.setY(ERASER_DEFAULT_POS_Y);
            }
        }

        stopChalkingSound();
        setMode(MODE.DRAWING);
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
     *
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

    private void eraseLine(Canvas canvas, int startX, int startY, int endX, int endY, Paint paint) {
        canvas.drawLine(startX, startY, endX, endY, paint);
    }

    private void doEraserPositionAnimation(final Object object, float fromX, float fromY, float toX, float toY) {
        PropertyValuesHolder holderX = PropertyValuesHolder.ofFloat("x", fromX, toX);
        PropertyValuesHolder holderY = PropertyValuesHolder.ofFloat("y", fromY, toY);

        ValueAnimator valueAnimator = ObjectAnimator.ofPropertyValuesHolder(object, holderX, holderY);
        final long ANIMATION_TIME = 200;
        valueAnimator.setDuration(ANIMATION_TIME);
        valueAnimator.setInterpolator(new DecelerateInterpolator(1.0f));
        valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                invalidate();
            }
        });

        valueAnimator.addListener(new Animator.AnimatorListener() {
            @Override
            public void onAnimationStart(Animator animation) {

            }

            @Override
            public void onAnimationEnd(Animator animation) {
                mEffectSound.startSoundPool(EffectSound.SOUND_ERASER_POSITION, 0);

                invalidate();
            }

            @Override
            public void onAnimationCancel(Animator animation) {

            }

            @Override
            public void onAnimationRepeat(Animator animation) {

            }
        });

        valueAnimator.start();
    }

    private ValueAnimator doTraceAnimation(final Object object, int fromAlpha, int toAlpha) {
        ValueAnimator valueAnimator = ObjectAnimator.ofInt(object, "alpha", fromAlpha, toAlpha);
        final long ANIMATION_TIME = 1000;
        valueAnimator.setDuration(ANIMATION_TIME);
        valueAnimator.setInterpolator(new DecelerateInterpolator(1.0f));
        valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(final ValueAnimator animation) {
                invalidate();
            }
        });

        valueAnimator.addListener(new Animator.AnimatorListener() {
            @Override
            public void onAnimationStart(Animator animation) {

            }

            @Override
            public void onAnimationEnd(Animator animation) {
                invalidate();
            }

            @Override
            public void onAnimationCancel(Animator animation) {

            }

            @Override
            public void onAnimationRepeat(Animator animation) {

            }
        });

        valueAnimator.start();
        return valueAnimator;
    }

    private void startChalkingSound() {
        mbSoundingChalk = true;
        if (mMode == MODE.DRAWING) {
            mEffectSound.startSoundPool(EffectSound.SOUND_CHALK, -1);

        } else {
            mEffectSound.startSoundPool(EffectSound.SOUND_ERASER, -1);

        }
    }

    private void stopChalkingSound() {
        mbSoundingChalk = false;
        mEffectSound.stopSoundPool(EffectSound.SOUND_CHALK);
        mEffectSound.stopSoundPool(EffectSound.SOUND_ERASER);
    }

    ////////////////////////////////////////////////////////////////////////////////

    private class Property {
        public float x;
        public float y;

        public void setX(float x) {
            this.x = x;
        }

        public void setY(float y) {
            this.y = y;
        }

        public float getX() {
            return x;
        }

        public float getY() {
            return y;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    public void setCallback(Callback callback) {
        mCallback = callback;
    }

    public void setMode(MODE mode) {
        mMode = mode;

        if (mode != MODE.ERASE) {
            setPenColor(mCurrentColor);
        }
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
        invalidate();
    }

    public void stopSound() {
        stopChalkingSound();
    }

    ////////////////////////////////////////////////////////////////////////////////

    public interface Callback {
        void onTouchDownForDrawing();
    }
}
