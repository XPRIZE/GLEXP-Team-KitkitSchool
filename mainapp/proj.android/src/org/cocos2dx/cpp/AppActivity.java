/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

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

import java.util.UUID;
import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.lang.Exception;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.os.Build.*;
import android.app.ActivityManager;
import android.content.Intent;

import com.enuma.kitkitlogger.KitKitLogger;

import org.cocos2dx.lib.*;
import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    public static AppActivity _activity;

    private Cocos2dxGLSurfaceView glSurfaceView;

    private static String TAG = "KitkitschoolActivity";

    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.d(TAG,"onCreate");
        _activity = this;

        Bundle extras = getIntent().getExtras();
        if(extras != null) {
            if(extras.getBoolean("clearAppData", false)) {
                clearAppData();
            }
        }

    }

    public Cocos2dxGLSurfaceView onCreateView()
    {
        glSurfaceView = new Cocos2dxGLSurfaceView(this);

        this.hideSystemUI();

        // create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    public static void sendToBack() {
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                _activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        _activity.moveTaskToBack(true);
                    }
                });
            }
        });
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG,"onResume");

        Bundle extras = getIntent().getExtras();
        if(extras != null) {
            Log.d(TAG,"onResume extra is not null");
            if(extras.getBoolean("clearAppData", false)) {
                Log.d(TAG,"onResume clearAppData is true");
                clearAppData();
            }
        }
    }



    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
        {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI()
    {
        // Set the IMMERSIVE flag.
        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
        glSurfaceView.setSystemUiVisibility(
                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    public void hideSystemUIOnUIThread()
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                hideSystemUI();
            }
        });
    }


    public static void moveToBackground() {
        _activity.moveTaskToBack(true);
        //moveTaskToBack(true);
    }

    static
    {
        System.loadLibrary("cocos2dcpp");
    }

    public static void staticSetFullScreen() {
        _activity.hideSystemUIOnUIThread();

    }

    public static void logEvent(String installID, String eventString) {

        KitKitLogger logger = ((KitkitSchoolApplication)_activity.getApplication()).getLogger();
        logger.logEvent(eventString);

//        boolean isNew = false;
//        File path = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString() + "/todoschool_" + installID);
//        if(!path.exists()){
//            path.mkdirs();
//        }
//
//        File[] files = path.listFiles();
//
//        String header = "todoschool_" + installID + "_log_";
//        String tail = ".txt";
//        String lastFilename = null;
//        File lastFile = null;
//        int lastNum = 1;
//        for (int i=0; i<files.length; i++){
//            String filename = files[i].getName();
//
//            if(lastFilename == null){
//                lastFilename = filename;
//                lastFile = files[i];
//            }
//
//            if(filename.contains(header)){
//                int num = Integer.parseInt(filename.split("_")[3].split("\\.")[0]);
//                if(num > lastNum){
//                    lastNum = num;
//                    lastFilename = filename;
//                    lastFile = files[i];
//                }
//            }
//        }
//
//
//        if(lastFilename == null){
//            lastFilename = path + "/" + header + "1" + tail;
//            lastFile = new File(lastFilename);
//            if(!lastFile.exists()){
//                try{ lastFile.createNewFile(); }catch(Exception e){}
//                isNew = true;
//            }
//        }
//
//        long size = lastFile.length();
//
//        if(size > 100*1024){
//            lastNum++;
//            lastFilename = path + "/" + header + Integer.toString(lastNum) + tail;
//            lastFile = new File(lastFilename);
//
//            if(!lastFile.exists()){
//                try{ lastFile.createNewFile(); }catch(Exception e){}
//                isNew = true;
//            }
//        }
//
//        try{
//            FileWriter fw = new FileWriter(lastFile.getAbsoluteFile(), true);
//            BufferedWriter bw = new BufferedWriter(fw);
//            bw.write((isNew ? "" : "\r\n") + eventString);
//            bw.close();
//        }catch(Exception e){
//
//        }
    }

    public static String getRandomUUID() {
        return UUID.randomUUID().toString();
    }

    public static String getAppVersion() {
        try {
            String versionName = getContext().getPackageManager()
                    .getPackageInfo(getContext().getPackageName(), 0).versionName;
            return versionName;
        } catch (Exception e) {

        }

        return "";
    }
    private void clearAppData() {
        if (VERSION_CODES.KITKAT <= VERSION.SDK_INT) {
            ((ActivityManager)getContext().getSystemService(ACTIVITY_SERVICE))
                    .clearApplicationUserData();
        } else {
            try {
                String packageName = getApplicationContext().getPackageName();
                Runtime runtime = Runtime.getRuntime();
                runtime.exec("pm clear "+packageName);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


}
