package library.todoschool.enuma.com.todoschoollibrary;

import android.support.v4.view.ViewPager;
import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
/**
 * Created by ingtellect on 12/28/16.
 */

public class CustomViewPager extends ViewPager {

    private boolean enabled;

    public CustomViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.enabled = true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (this.enabled) {
            return super.onTouchEvent(event);
        }

        return false;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent event) {
        if (this.enabled) {
            return super.onInterceptTouchEvent(event);
        }

        return false;
    }

    public void setPagingEnabled(boolean enabled) {
        this.enabled = enabled;
    }
}
