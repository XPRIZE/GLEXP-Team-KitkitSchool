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

import org.cocos2dx.lib.*;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.util.Log;

import com.enuma.kitkitlogger.KitKitLogger;

import java.io.File;

public class AppActivity extends Cocos2dxActivity {

    public static AppActivity _activity;
    public static String _launchString;
    private Cocos2dxGLSurfaceView _glSurfaceView;

    private static String TAG = "BookTestActivity";

    public static String getDocumentsPath() {
        File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS);
        return path.getAbsolutePath();


    }

    public static String getLaunchString() {
        return _launchString;
    }

    public static void clearLaunchString() {
        _launchString = "";
    }

    public static void sendToBack() {
        Log.d("AppActivity", "sendToBack - booktest");

        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.d("AppActivity", "sendToBack onUIThread 1 - booktest");
                _activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Log.d("AppActivity", "sendToBack onUIThread 2 - booktest");
                        //_activity.moveTaskToBack(true);
                        _activity.finish();
                    }
                });
            }
        });
    }

    public  boolean isStoragePermissionGranted() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG,"Permission is granted");
                return true;
            } else {

                Log.v(TAG,"Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
                return false;
            }
        }
        else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG,"Permission is granted");
            return true;
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        _activity = this;

        Log.d("AppActivity", "onCreate - booktest");
        isStoragePermissionGranted();
        Intent intent = getIntent();

        if (intent.hasExtra("book")) {
            _launchString = intent.getStringExtra("book");
            Log.d("AppActivity", "onCreate has book extra - " + _launchString);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

//        Intent intent = getIntent();
//
//        if (intent.hasExtra("book")) {
//            _launchString = intent.getStringExtra("book");
//            Log.d("AppActivity", "onCreate has book extra - " + _launchString);
//        }


    }

    public Cocos2dxGLSurfaceView onCreateView() {
        if (null == _glSurfaceView) {
            _glSurfaceView = new Cocos2dxGLSurfaceView(this);
        }

        this.hideSystemUI();

        // create stencil buffer
        _glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return _glSurfaceView;
    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI() {

        if (_glSurfaceView == null) return;
        // Set the IMMERSIVE flag.
        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
        _glSurfaceView.setSystemUiVisibility(
                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                        | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    public void hideSystemUIOnUIThread() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                hideSystemUI();
            }
        });
    }

    public static void staticSetFullScreen() {
        _activity.hideSystemUIOnUIThread();

    }

    public static void logEvent(String eventString) {

        KitKitLogger logger = ((BookApplication) _activity.getApplication()).getLogger();
        logger.logEvent(eventString);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED){
            Log.v(TAG,"Permission: "+permissions[0]+ "was "+grantResults[0]);
        }
    }
}
