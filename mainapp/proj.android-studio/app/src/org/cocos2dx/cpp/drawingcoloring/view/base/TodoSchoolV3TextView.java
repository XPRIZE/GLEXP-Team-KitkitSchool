package org.cocos2dx.cpp.drawingcoloring.view.base;

import android.content.Context;
import android.support.annotation.Nullable;
import android.support.v7.widget.AppCompatTextView;
import android.util.AttributeSet;

import org.cocos2dx.cpp.drawingcoloring.utility.Font;

public class TodoSchoolV3TextView extends AppCompatTextView {

    public TodoSchoolV3TextView(Context context) {
        super(context);
        init(context);
    }

    public TodoSchoolV3TextView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public TodoSchoolV3TextView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        if (isInEditMode() == false) {
            setTypeface(Font.getInstance(context).getTodoSchoolV3());
        }
    }
}
