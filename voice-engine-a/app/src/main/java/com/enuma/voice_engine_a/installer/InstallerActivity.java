/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

// package com.svox.langpack.installer;
package com.enuma.voice_engine_a.installer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.enuma.voice_engine_a.R;

public class InstallerActivity extends Activity {
    private static final String TAG = "voice-engine-a";
    private static final int DATA_ROOT_DIRECTORY_REQUEST_CODE = 42;
    private String rootDirectory = "";
    private InstallerActivity self;
    private static boolean sInstallationSuccess = false;
    private static boolean sIsInstalling = false;
    private final static Object sInstallerStateLock = new Object();

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        self = this;

        // NB(xenosoz, 2018): need to ask 23 or higher && 18 or lower -> nothing.
        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        //    requestPermissions(new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
        //}

        Log.d(TAG, "onCreate");

        Intent getRootDirectoryIntent = new Intent();
        getRootDirectoryIntent.setClassName("com.enuma.voice_engine_a", "com.enuma.voice_engine_a.CheckVoiceData");
        startActivityForResult(getRootDirectoryIntent, DATA_ROOT_DIRECTORY_REQUEST_CODE);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data){
        super.onActivityResult(requestCode, resultCode, data);
        Log.d(TAG, "onActivityResult(): " + String.valueOf(requestCode));

        if (requestCode == DATA_ROOT_DIRECTORY_REQUEST_CODE) {
            rootDirectory = data.getStringExtra(TextToSpeech.Engine.EXTRA_VOICE_DATA_ROOT_DIRECTORY);
            // only run the installer if there isn't another one running
            synchronized (sInstallerStateLock) {
                if (!sIsInstalling && !sInstallationSuccess) {
                    sIsInstalling = true;
                    runInstaller();
                    return;
                }

                secureXprizeStatus();
            }
        }
    }

    private void runInstaller(){
        Log.d(TAG, "runInstaller()");

        try {
            Resources res = getResources();
            AssetFileDescriptor langPackFd = res
                    .openRawResourceFd(R.raw.svoxlangpack);
            InputStream stream = langPackFd.createInputStream();

            (new Thread(new unzipper(stream))).start();
        } catch (IOException e) {
            Log.e("PicoLangInstaller", "Unable to open langpack resource.");
            e.printStackTrace();
        }
        setContentView(R.layout.installing);
    }


    private boolean unzipLangPack(InputStream stream) {
        FileOutputStream out;
        byte buf[] = new byte[16384];
        try {
            ZipInputStream zis = new ZipInputStream(stream);
            ZipEntry entry = zis.getNextEntry();
            while (entry != null) {
                Log.d(TAG, "path: " + rootDirectory + "/" + entry.getName());
                if (entry.isDirectory()) {
                    File newDir = new File(rootDirectory + "/" + entry.getName());
                    newDir.mkdir();
                } else {
                    String name = entry.getName();
                    File outputFile = new File(rootDirectory + "/" + name);
                    Log.d(TAG, "outputFile: " + outputFile.toString());

                    //String outputPath = outputFile.getCanonicalPath();
                    String outputPath = outputFile.getAbsolutePath();
                    Log.d(TAG, "outputPath: " + outputPath);

                    name = outputPath
                            .substring(outputPath.lastIndexOf("/") + 1);
                    outputPath = outputPath.substring(0, outputPath
                            .lastIndexOf("/"));
                    Log.d(TAG, "outputPath: " + outputPath + ", name: " + name);

                    File outputDir = new File(outputPath);
                    boolean mkdirsP = outputDir.mkdirs();
                    Log.d(TAG, "mkdirsP: " + String.valueOf(mkdirsP));

                    Log.d(TAG, "create: " + outputPath + ", " + name);
                    outputFile = new File(outputPath, name);
                    outputFile.createNewFile();
                    out = new FileOutputStream(outputFile);

                    int numread = 0;
                    do {
                        numread = zis.read(buf);
                        if (numread <= 0) {
                            break;
                        } else {
                            out.write(buf, 0, numread);
                        }
                    } while (true);
                    out.close();
                }
                entry = zis.getNextEntry();
            }
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    private class unzipper implements Runnable {
        public InputStream stream;

        public unzipper(InputStream is) {
            stream = is;
        }

        public void run() {
            boolean result = unzipLangPack(stream);
            synchronized (sInstallerStateLock) {
                sInstallationSuccess = result;
                sIsInstalling = false;
            }
            if (sInstallationSuccess) {
                runOnUiThread(new successDisplayer());
            } else {
                if (!self.isFinishing()) {
                    // the activity is still running, ask the user to retry.
                    runOnUiThread(new retryDisplayer());
                }
            }
        }
    }

    private void returnToXprize() {
        Log.d(TAG, "Install success: CALL_BY_KITKIT_SCHOOL");

        Intent intent = new Intent();
        intent.setAction("com.enuma.xprize.RETURN_BY_VOICE_ENGINE_A");
        finish();
        startActivity(intent);
    }

    private boolean secureXprizeStatus() {
        String action = getIntent().getAction();
        if ("com.enuma.voice_engine_a.installer.CALL_BY_KITKIT_SCHOOL".equals(action)) {
            returnToXprize();
            return true;
        }
        return false;
    }

    public class successDisplayer implements Runnable {
        public void run() {
            if (secureXprizeStatus()) { return; }
            setContentView(R.layout.success);
        }
    }

    public class retryDisplayer implements Runnable {
        public void run() {
            setContentView(R.layout.retry);
            Button retryButton = (Button) findViewById(R.id.retryButton);
            retryButton.setOnClickListener(new OnClickListener() {
                public void onClick(View arg0) {
                    // only run the installer if there isn't another one running
                    // (we only get here if the installer couldn't complete successfully before)
                    synchronized (sInstallerStateLock) {
                        if (!sIsInstalling) {
                            sIsInstalling = true;
                            runInstaller();
                            return;
                        }

                        secureXprizeStatus();
                    }
                }
            });
        }
    }


}
