package org.cocos2dx.cpp;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.google.android.vending.expansion.downloader.Helpers;
import com.maq.xprize.kitkitschool.hindi.R;

import java.io.File;
import java.io.IOException;
import java.util.zip.ZipFile;

import kitkitschool.DownloadExpansionFile;
import utils.Zip;

import static com.maq.xprize.kitkitschool.hindi.R.layout.activity_splash_screen;
import static kitkitschool.DownloadExpansionFile.xAPKS;

public class SplashScreenActivity extends Activity {

    Intent mainActivityIntent = null;
    String expansionFilePath;
    File expansionFile;
    ZipFile expansionZipFile;
    Zip zipHandler;
    File packageNameDir;
    SharedPreferences sharedPref;
    int defaultFileVersion = 0;
    int storedMainFileVersion;
    int storedPatchFileVersion;
    boolean isExtractionRequired = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        View decorView = this.getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        decorView.setSystemUiVisibility(uiOptions);
        setContentView(activity_splash_screen);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            // Permission is not granted
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1);
        } else {
            sharedPref = getSharedPreferences("ExpansionFile", MODE_PRIVATE);
            // Retrieve the stored values of main and patch file version
            storedMainFileVersion = sharedPref.getInt(getString(R.string.mainFileVersion), defaultFileVersion);
            storedPatchFileVersion = sharedPref.getInt(getString(R.string.patchFileVersion), defaultFileVersion);
            isExtractionRequired = isExpansionExtractionRequired(storedMainFileVersion, storedPatchFileVersion);
            // If main or patch file is updated, the extraction process needs to be performed again
            if (isExtractionRequired) {
                System.out.println("Splash onCreate: isExtractionRequired = " + isExtractionRequired);
                new DownloadFile().execute(null, null, null);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions, @NonNull int[] grantResults) {
        // If request is cancelled, the result arrays are empty.
        if (requestCode == 1) {
            if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED
                    && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                sharedPref = getSharedPreferences("ExpansionFile", MODE_PRIVATE);
                // Retrieve the stored values of main and patch file version
                storedMainFileVersion = sharedPref.getInt(getString(R.string.mainFileVersion), defaultFileVersion);
                storedPatchFileVersion = sharedPref.getInt(getString(R.string.patchFileVersion), defaultFileVersion);
                isExtractionRequired = isExpansionExtractionRequired(storedMainFileVersion, storedPatchFileVersion);
                // If main or patch file is updated, the extraction process needs to be performed again
                if (isExtractionRequired) {
                    new DownloadFile().execute(null, null, null);
                }
            } else {
                Toast.makeText(this, "Permission required!", Toast.LENGTH_LONG).show();
                finish();
            }
        }
    }

    private boolean isExpansionExtractionRequired(int storedMainFileVersion, int storedPatchFileVersion) {
        for (DownloadExpansionFile.XAPKFile xf : xAPKS) {
            // If main or patch file is updated set isExtractionRequired to true
            if (xf.mIsMain && xf.mFileVersion != storedMainFileVersion || !xf.mIsMain && xf.mFileVersion != storedPatchFileVersion) {
                return true;
            }
        }
        return false;
    }

    /* function to call the main application after extraction */
    public void toCallApplication() {
        mainActivityIntent = new Intent(this, AppActivity.class);
        startActivity(mainActivityIntent);
        finish();
    }

    public void unzipFile() {
        int totalZipSize = getTotalExpansionFileSize();
        try {
            for (DownloadExpansionFile.XAPKFile xf : xAPKS) {
                if (xf.mIsMain && xf.mFileVersion != storedMainFileVersion || !xf.mIsMain && xf.mFileVersion != storedPatchFileVersion) {
                    expansionFilePath = getExpansionFilePath(xf.mIsMain, xf.mFileVersion);
                    expansionFile = new File(expansionFilePath);
                    expansionZipFile = new ZipFile(expansionFile);
                    zipHandler = new Zip(expansionZipFile, this);
                    packageNameDir = this.getExternalFilesDir(null);
                    if (xf.mIsMain && !packageNameDir.exists()) {
                        packageNameDir.mkdir();
                    }
                    zipHandler.unzip(packageNameDir, totalZipSize, xf.mIsMain, xf.mFileVersion, sharedPref);
                    zipHandler.close();
                }
            }
            toCallApplication();
        } catch (IOException e) {
            System.out.println("Could not extract assets");
            System.out.println("Stack trace:" + e);
        }
    }

    public boolean isStorageSpaceAvailable() {
        long totalExpansionFileSize = 0;
        File internalStorageDir = Environment.getDataDirectory();
        for (DownloadExpansionFile.XAPKFile xf : xAPKS) {
            if (xf.mIsMain && xf.mFileVersion != storedMainFileVersion || !xf.mIsMain && xf.mFileVersion != storedPatchFileVersion) {
                totalExpansionFileSize = xf.mFileSize;
            }
        }
        return totalExpansionFileSize < internalStorageDir.getFreeSpace();
    }

    public int getTotalExpansionFileSize() {
        int totalExpansionFileSize = 0;
        ZipFile zipFile;
        try {
            for (DownloadExpansionFile.XAPKFile xf : xAPKS) {
                if (xf.mIsMain && xf.mFileVersion != storedMainFileVersion || !xf.mIsMain && xf.mFileVersion != storedPatchFileVersion) {
                    expansionFilePath = getExpansionFilePath(xf.mIsMain, xf.mFileVersion);
                    ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 99);
                    expansionFile = new File(expansionFilePath);
                    zipFile = new ZipFile(expansionFile);
                    totalExpansionFileSize += zipFile.size();
                }
            }
        } catch (IOException ie) {
            System.out.println("Couldn't get total expansion file size");
            System.out.println("Stacktrace: " + ie);
        }
        return totalExpansionFileSize;
    }

    public String getExpansionFilePath(boolean isMain, int fileVersion) {
        return getObbDir() + File.separator + Helpers.getExpansionAPKFileName(this, isMain, fileVersion);
    }

    private class DownloadFile extends AsyncTask<String, Integer, String> {
        @Override
        protected String doInBackground(String... sUrl) {
            if (isStorageSpaceAvailable()) {
                unzipFile();
            } else {
                SplashScreenActivity.this.runOnUiThread(new Runnable() {
                    public void run() {
                        Toast.makeText(SplashScreenActivity.this, "Insufficient storage space! Please free up your storage to use this application.", Toast.LENGTH_LONG).show();
                        // Call finish after the toast message disappears
                        new Handler().postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                SplashScreenActivity.this.finish();
                            }
                        }, Toast.LENGTH_LONG);
                    }
                });
            }
            return null;
        }
    }
}