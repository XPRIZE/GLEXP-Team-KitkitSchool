package com.enuma.writingboard.util;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Typeface;

public class Font {
    private static Font _self = null;
    private Typeface mAndika = null;

    public static Font getInstance(Context context) {
        if (null == _self) _self = new Font(context);
        return _self;
    }

    private Font(Context context) {
        AssetManager mgr = context.getAssets();
        mAndika = Typeface.createFromAsset(mgr, "font/andika-r.ttf");

        if (mAndika == null) {
            mAndika = Typeface.DEFAULT;
        }
    }

    public Typeface getAndika() {
        return mAndika;
    }
}