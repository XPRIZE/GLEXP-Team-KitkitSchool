package com.maq.xprize.kitkitlibrary.english;

import android.Manifest;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ProgressBar;

import com.android.vending.expansion.zipfile.APKExpansionSupport;
import com.google.android.vending.expansion.downloader.Helpers;

import java.io.File;
import java.io.IOException;
import java.util.zip.ZipFile;

import kitkitschool.DownloadExpansionFile;
import utils.Zip;


public class SplashScreenActivity extends AppCompatActivity {

    double progressBarCount = 0;
    ProgressBar progressBar = null;
    public static CountDownTimer countDownTimer = null;
    Intent intent = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       /* if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED &&
                ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                        != PackageManager.PERMISSION_GRANTED) {*/
        // Permission is not granted
        /*}*/
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < 19) { // lower api
            View v = this.getWindow().getDecorView();
            v.setSystemUiVisibility(View.GONE);
        } else if (Build.VERSION.SDK_INT >= 19) {
            //for new api versions.
            View decorView = this.getWindow().getDecorView();
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            decorView.setSystemUiVisibility(uiOptions);
        }
        setContentView(R.layout.activity_splash_screen);
        progressBar = (ProgressBar) findViewById(R.id.p);
        countDownTimer = new CountDownTimer(300000000, 500) {
            public void onTick(long millisUntilFinished) {
                progressBar.setProgress((int) progressBarCount);
                progressBarCount += 0.1;
            }

            public void onFinish() {
            }
        };
        new DownloadFile().execute(null, null, null);
    }

    /*@Override
    public void onRequestPermissionsResult(int requestCode,
                                           String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case 99: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    System.out.println("Permission granted");
                    new DownloadFile().execute(null, null, null);
                } else {
                    System.out.println("Permission not granted");
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                    onDestroy();
                }
            }

            // other 'case' lines to check for other
            // permissions this app might request.
        }
    }*/

    /* function to call the main application after extraction */
    public void toCallApplication() {
        intent = new Intent("android.intent.action.LAUNCHER");
        startActivity(intent);
        finish();
    }

    public void unzipFile() {
        try {
            String filePath = getExpansionFilePath();
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 99);
            File file = new File(filePath);
            ZipFile zipFile = new ZipFile(file);
            Zip _zip = new Zip(zipFile);
            String unzipFilePath = getUnzippedExpansionFilePath();
            File packageNameDir = new File(unzipFilePath);
            if (packageNameDir.exists()) {
                deleteRecursive(packageNameDir);
            }
            packageNameDir.mkdir();
            _zip.unzip(unzipFilePath);
            progressBar.setProgress(100);
            countDownTimer.cancel();
            _zip.close();
            toCallApplication();
        } catch (IOException ie) {
            Log.d("SplashScreenActivity", "Line 118 " + ie.getMessage());
            unzipFile();
        }
        Log.d("SplashScreenActivity", "Line 121");
        return ;
    }

    /*public static boolean isRequiredUnzipExpansionFile(SharedPreferences prefs) {
        return prefs.getInt(EXPANSION_FILE_VERSION_KEY_NAME, 0) != DownloadExpansionFile.xAPK.mFileVersion;
    }*/

    void deleteRecursive(File dirPath) {
        if (dirPath.isDirectory())
            for (File child : dirPath.listFiles())
                deleteRecursive(child);

        dirPath.delete();
    }

    public static String getUnzippedExpansionFilePath() {
//        return getContext().getExternalFilesDir(null) + File.separator;
        return "/storage/emulated/0/Android/data/com.maq.xprize.kitkitlibrary.english/files/";
    }

    public String getExpansionFilePath() {

        return Environment.getExternalStorageDirectory().toString() + APKExpansionSupport.EXP_PATH + Helpers.getPackageName(this) + File.separator +
                Helpers.getExpansionAPKFileName(this, DownloadExpansionFile.xAPK.mIsMain, DownloadExpansionFile.xAPK.mFileVersion);
    }

    private class DownloadFile extends AsyncTask<String, Integer, String> {
        @Override
        protected String doInBackground(String... sUrl) {
            Log.d("SplashScreenActivity", "Line 143");
            unzipFile();
            return null;
        }
    }
}
