/****************************************************************************
Copyright (c) 2015-2017 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.enuma.kitkitProvider.Fish;
import com.enuma.kitkitProvider.KitkitDBHandler;

import java.io.File;
import java.util.ArrayList;

public class AppActivity extends Cocos2dxActivity {
    public static AppActivity _activity;
    public static KitkitDBHandler _dbHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        _activity = this;
        _dbHandler = new KitkitDBHandler(_activity);
    }

    public static String getFishes() {
        String result = "[]";
        if (_dbHandler != null) {
            ArrayList<Fish> fishes = _dbHandler.getFishes();
            result = fishes.toString();
        }
        return result;
    }

    public static boolean deleteFish(int id) {
        if (_dbHandler != null) {
            return _dbHandler.deleteFish(id);
        }
        return false;
    }

    public static String getSavePath() {
        String result = Environment.getExternalStorageDirectory().getAbsolutePath();
        if (_dbHandler != null) {
            result += (File.separator + "sea_world" + File.separator + "images" + File.separator + _dbHandler.getCurrentUser().getUserName());
        }

        File folder = new File(result);
        if (!folder.exists()) {
            folder.mkdirs();
        }
        return result;
    }
}
