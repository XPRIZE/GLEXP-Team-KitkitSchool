package org.cocos2dx.cpp.kitkitlauncher.hindi;

import android.content.Context;
import android.graphics.Typeface;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.maq.xprize.kitkitschool.hindi.R;

/**
 * Created by ingtellect on 9/12/17.
 */

public class ToolsAppView extends LinearLayout {

    String appID;
    ImageView imageViewAppIcon;
    ImageView imageViewShadow;
    ImageView imageViewCoin;
    TextView textViewAppName;
    TextView textViewCoin;
    String appIconName;
    String appIconNameDisabled;
    String appNameLocalized;
    int numCoin;
    private boolean isUnlocked;


    public ToolsAppView(Context context) {
        super(context);
        init(context);
    }

    public ToolsAppView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public ToolsAppView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    public ToolsAppView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init(context);
    }

    public void init(Context context) {
        View v = inflate(context, R.layout.item_toolapps, null);
        addView(v);

        imageViewAppIcon = (ImageView) v.findViewById(R.id.imageView_appIcon);
        imageViewShadow = (ImageView) v.findViewById(R.id.imageView_shadow);
        imageViewCoin = (ImageView) v.findViewById(R.id.imageView_coin);
        textViewAppName = (TextView) v.findViewById(R.id.textView_appname);
        textViewCoin = (TextView) v.findViewById(R.id.textView_coin);
        isUnlocked = false;

    }

    public void setItem(String appid, String appnameLocalized, String appiconname, String appicondisabled, String coin) {
        appID = appid;
        appNameLocalized = appnameLocalized;
        appIconName = appiconname;
        appIconNameDisabled = appicondisabled;

        if (appNameLocalized.contains("\n")) {
            textViewAppName.setTextSize(TypedValue.COMPLEX_UNIT_PX, 70);
        }

        textViewAppName.setText(appNameLocalized);

        textViewCoin.setText(coin);
        Typeface face = Typeface.createFromAsset(getContext().getAssets(), "TodoMainCurly.ttf");
        textViewCoin.setTypeface(face);

        numCoin = Integer.parseInt(coin);
    }

    public void setEnable(boolean enable) {

        if (enable) {
            imageViewAppIcon.setImageResource(getResources().getIdentifier(appIconName, "drawable", getContext().getPackageName()));
            imageViewShadow.setImageResource(getResources().getIdentifier("tools_image_icon_shadow", "drawable", getContext().getPackageName()));
            imageViewCoin.setImageResource(getResources().getIdentifier("tools_image_icon_coin", "drawable", getContext().getPackageName()));
            textViewCoin.setTextColor(ContextCompat.getColor(getContext(), R.color.coinNumberText));

            this.setAlpha(1);
        } else {
            imageViewAppIcon.setImageResource(getResources().getIdentifier(appIconNameDisabled, "drawable", getContext().getPackageName()));
            imageViewShadow.setImageResource(getResources().getIdentifier("tools_image_icon_shadow_disabled", "drawable", getContext().getPackageName()));
            imageViewCoin.setImageResource(getResources().getIdentifier("tools_image_icon_coin_disabled", "drawable", getContext().getPackageName()));
            textViewCoin.setTextColor(ContextCompat.getColor(getContext(), R.color.coinNumberTextDisabled));

            this.setAlpha(0.4f);
        }

        setEnabled(enable);

    }

    public boolean isUnlocked() {
        return isUnlocked;
    }

    public void setUnlocked(boolean unlocked) {

        LinearLayout coinLayout = (LinearLayout) findViewById(R.id.coin_layout);
        if (unlocked) {
            if (appNameLocalized.contains("\n")) {
                coinLayout.setVisibility(GONE);
                LayoutParams params = (LayoutParams) textViewAppName.getLayoutParams();
                params.bottomMargin = 70;
                textViewAppName.setLayoutParams(params);

            } else {
                coinLayout.setVisibility(INVISIBLE);
            }

        } else {
            coinLayout.setVisibility(VISIBLE);

        }
        isUnlocked = unlocked;
    }

    public String getAppID() {
        return appID;
    }

    public int getNumCoin() {
        return numCoin;
    }

}
