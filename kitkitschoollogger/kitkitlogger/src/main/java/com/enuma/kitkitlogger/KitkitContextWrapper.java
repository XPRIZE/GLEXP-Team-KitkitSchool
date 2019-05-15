package com.enuma.kitkitlogger;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.Build;
import android.os.LocaleList;

import java.util.Locale;

/**
 * Created by ingtellect on 8/21/17.
 */

public class KitkitContextWrapper extends ContextWrapper {
    public KitkitContextWrapper(Context base) {
        super(base);
    }

    public static KitkitContextWrapper wrap(Context context, Locale newLocale) {

        Resources res = context.getResources();
        Configuration configuration = res.getConfiguration();

        if (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            configuration.setLocale(newLocale);

            LocaleList localeList = new LocaleList(newLocale);
            LocaleList.setDefault(localeList);
            configuration.setLocales(localeList);

            context = context.createConfigurationContext(configuration);

        } else {
            if (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
                configuration.setLocale(newLocale);
                context = context.createConfigurationContext(configuration);
            } else {
                configuration.locale = newLocale;
                res.updateConfiguration(configuration, null);
            }


        }

        return new KitkitContextWrapper(context);
    }


}
