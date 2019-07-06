package kitkitschool;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Messenger;
import android.os.SystemClock;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.google.android.vending.expansion.downloader.Constants;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.maq.xprize.booktest.R;
import com.maq.xprize.kitkitlibrary.english.MainActivity;

import java.io.DataInputStream;
import java.io.IOException;
import java.util.Locale;
import java.util.zip.CRC32;

import services.ExpansionDownloaderService;

public class DownloadExpansionFile extends Activity implements IDownloaderClient {
    /**
     * Here is where you place the data that the validator will use to determine
     * if the file was delivered correctly. This is encoded in the source code
     * so the application can easily determine whether the file has been
     * properly delivered without having to talk to the server. If the
     * application is using LVL for licensing, it may make sense to eliminate
     * these checks and to just rely on the server.
     */
    public static final XAPKFile[] xAPKS = {
            new XAPKFile(
                    true, // true signifies a main file
                    5, // the version of the APK that the file was uploaded against
                    1569810587L // the length of the file in bytes
            )
    };
    private static final String LOG_TAG = "LVLDownloader";
    /**
     * Calculating a moving average for the validation speed so we don't get
     * jumpy calculations for time etc.
     */
    static private final float SMOOTHING_FACTOR = 0.005f;
    private ProgressBar mPB;
    private TextView mStatusText;
    private TextView mProgressFraction;
    private TextView mProgressPercent;
    private TextView mAverageSpeed;
    private TextView mTimeRemaining;
    private View mDashboard;
    private View mCellMessage;
    private Button mPauseButton;
    private boolean mStatePaused;
    private int mState;
    private IDownloaderService mRemoteService;
    private IStub mDownloaderClientStub;
    /**
     * Used by the async task
     */
    private boolean mCancelValidation;

    private void setState(int newState) {
        if (mState != newState) {
            mState = newState;
            mStatusText.setText(Helpers.getDownloaderStringResourceIDFromState(newState));
        }
    }

    private void setButtonPausedState(boolean paused) {
        mStatePaused = paused;
        int stringResourceID = paused ? R.string.text_button_resume :
                R.string.text_button_pause;
        mPauseButton.setText(stringResourceID);
    }

    /**
     * Go through each of the APK Expansion files defined in the structure above
     * and determine if the files are present and match the required size. Free
     * applications should definitely consider doing this, as this allows the
     * application to be launched for the first time without having a network
     * connection present. Paid applications that use LVL should probably do at
     * least one LVL check that requires the network to be present, so this is
     * not as necessary.
     *
     * @return true if they are present.
     */
    boolean expansionFilesDelivered() {
        for (XAPKFile xf : xAPKS) {
            String fileName = Helpers.getExpansionAPKFileName(this, xf.mIsMain, xf.mFileVersion);
            if (!Helpers.doesFileExist(this, fileName, xf.mFileSize, false))
                return false;
        }
        return true;
    }

    /**
     * Go through each of the Expansion APK files and open each as a zip file.
     * Calculate the CRC for each file and return false if any fail to match.
     *
     * @return true if XAPKZipFile is successful
     */
    void validateXAPKZipFiles() {
        AsyncTask<Object, DownloadProgressInfo, Boolean> validationTask = new AsyncTask<Object, DownloadProgressInfo, Boolean>() {

            @Override
            protected void onPreExecute() {
                mDashboard.setVisibility(View.VISIBLE);
                mCellMessage.setVisibility(View.GONE);
                mStatusText.setText(R.string.text_verifying_download);
                mPauseButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        mCancelValidation = true;
                    }
                });
                mPauseButton.setText(R.string.text_button_cancel_verify);
                super.onPreExecute();
            }

            @Override
            protected Boolean doInBackground(Object... params) {
                for (XAPKFile xf : xAPKS) {
                    String fileName = Helpers.getExpansionAPKFileName(
                            DownloadExpansionFile.this,
                            xf.mIsMain, xf.mFileVersion);
                    if (!Helpers.doesFileExist(DownloadExpansionFile.this, fileName,
                            xf.mFileSize, false))
                        return false;
                    fileName = Helpers
                            .generateSaveFileName(DownloadExpansionFile.this, fileName);
                    ZipResourceFile zrf;
                    byte[] buf = new byte[1024 * 256];
                    try {
                        zrf = new ZipResourceFile(fileName);
                        ZipResourceFile.ZipEntryRO[] entries = zrf.getAllEntries();
                        /**
                         * First calculate the total compressed length
                         */
                        long totalCompressedLength = 0;
                        for (ZipResourceFile.ZipEntryRO entry : entries) {
                            totalCompressedLength += entry.mCompressedLength;
                        }
                        float averageVerifySpeed = 0;
                        long totalBytesRemaining = totalCompressedLength;
                        long timeRemaining;
                        /**
                         * Then calculate a CRC for every file in the Zip file,
                         * comparing it to what is stored in the Zip directory.
                         * Note that for compressed Zip files we must extract
                         * the contents to do this comparison.
                         */
                        for (ZipResourceFile.ZipEntryRO entry : entries) {
                            if (-1 != entry.mCRC32) {
                                long length = entry.mUncompressedLength;
                                CRC32 crc = new CRC32();
                                DataInputStream dis = null;
                                try {
                                    dis = new DataInputStream(
                                            zrf.getInputStream(entry.mFileName));

                                    long startTime = SystemClock.uptimeMillis();
                                    while (length > 0) {
                                        int seek = (int) (length > buf.length ? buf.length
                                                : length);
                                        dis.readFully(buf, 0, seek);
                                        crc.update(buf, 0, seek);
                                        length -= seek;
                                        long currentTime = SystemClock.uptimeMillis();
                                        long timePassed = currentTime - startTime;
                                        if (timePassed > 0) {
                                            float currentSpeedSample = (float) seek
                                                    / (float) timePassed;
                                            if (0 != averageVerifySpeed) {
                                                averageVerifySpeed = SMOOTHING_FACTOR
                                                        * currentSpeedSample
                                                        + (1 - SMOOTHING_FACTOR)
                                                        * averageVerifySpeed;
                                            } else {
                                                averageVerifySpeed = currentSpeedSample;
                                            }
                                            totalBytesRemaining -= seek;
                                            timeRemaining = (long) (totalBytesRemaining / averageVerifySpeed);
                                            this.publishProgress(
                                                    new DownloadProgressInfo(
                                                            totalCompressedLength,
                                                            totalCompressedLength
                                                                    - totalBytesRemaining,
                                                            timeRemaining,
                                                            averageVerifySpeed)
                                            );
                                        }
                                        startTime = currentTime;
                                        if (mCancelValidation)
                                            return true;
                                    }
                                    if (crc.getValue() != entry.mCRC32) {
                                        Log.e(Constants.TAG,
                                                "CRC does not match for entry: "
                                                        + entry.mFileName);
                                        Log.e(Constants.TAG,
                                                "In file: " + entry.getZipFileName());
                                        return false;
                                    }
                                } finally {
                                    if (null != dis) {
                                        dis.close();
                                    }
                                }
                            }
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                        return false;
                    }
                }
                return true;
            }

            @Override
            protected void onProgressUpdate(DownloadProgressInfo... values) {
                onDownloadProgress(values[0]);
                super.onProgressUpdate(values);
            }

            @Override
            protected void onPostExecute(Boolean result) {
                if (result) {
                    mDashboard.setVisibility(View.VISIBLE);
                    mCellMessage.setVisibility(View.GONE);
                    mStatusText.setText(R.string.text_validation_complete);
                    mPauseButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            finish();
                        }
                    });
                    mPauseButton.setText(android.R.string.ok);
                } else {
                    mDashboard.setVisibility(View.VISIBLE);
                    mCellMessage.setVisibility(View.GONE);
                    mStatusText.setText(R.string.text_validation_failed);
                    mPauseButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            finish();
                        }
                    });
                    mPauseButton.setText(android.R.string.cancel);
                }
                super.onPostExecute(result);
            }

        };
        validationTask.execute(new Object());
    }

    /**
     * If the download isn't present, we initialize the download UI. This ties
     * all of the controls into the remote service calls.
     */
    private void initializeDownloadUI() {
        mDownloaderClientStub = DownloaderClientMarshaller.CreateStub
                (this, ExpansionDownloaderService.class);
        setContentView(R.layout.download_expansion_file);

        mPB = findViewById(R.id.progressBar);
        mStatusText = findViewById(R.id.statusText);
        mProgressFraction = findViewById(R.id.progressAsFraction);
        mProgressPercent = findViewById(R.id.progressAsPercentage);
        mAverageSpeed = findViewById(R.id.progressAverageSpeed);
        mTimeRemaining = findViewById(R.id.progressTimeRemaining);
        mDashboard = findViewById(R.id.downloaderDashboard);
        mCellMessage = findViewById(R.id.approveCellular);
        mPauseButton = findViewById(R.id.pauseButton);
        Button mWiFiSettingsButton = findViewById(R.id.wifiSettingsButton);

        mPauseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mStatePaused) {
                    mRemoteService.requestContinueDownload();
                } else {
                    mRemoteService.requestPauseDownload();
                }
                setButtonPausedState(!mStatePaused);
            }
        });

        mWiFiSettingsButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
            }
        });

        Button resumeOnCell = findViewById(R.id.resumeOverCellular);
        resumeOnCell.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRemoteService.setDownloadFlags(IDownloaderService.FLAGS_DOWNLOAD_OVER_CELLULAR);
                mRemoteService.requestContinueDownload();
                mCellMessage.setVisibility(View.GONE);
            }
        });

    }

    /**
     * Called when the activity is first create; we wouldn't create a layout in
     * the case where we have the file and are moving to another activity
     * without downloading.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        /*
          Both downloading and validation make use of the "download" UI
         */
        initializeDownloadUI();

        /*
          Before we do anything, are the files we expect already here and
          delivered (presumably by Market) For free titles, this is probably
          worth doing. (so no Market request is necessary)
         */
        if (!expansionFilesDelivered()) {

            try {
                Intent launchIntent = DownloadExpansionFile.this
                        .getIntent();
                Intent intentToLaunchThisActivityFromNotification = new Intent(
                        DownloadExpansionFile
                                .this, DownloadExpansionFile.this.getClass());
                intentToLaunchThisActivityFromNotification.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK |
                        Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intentToLaunchThisActivityFromNotification.setAction(launchIntent.getAction());

                if (launchIntent.getCategories() != null) {
                    for (String category : launchIntent.getCategories()) {
                        intentToLaunchThisActivityFromNotification.addCategory(category);
                    }
                }

                // Build PendingIntent used to open this activity from
                // Notification
                PendingIntent pendingIntent = PendingIntent.getActivity(
                        DownloadExpansionFile.this,
                        0, intentToLaunchThisActivityFromNotification,
                        PendingIntent.FLAG_UPDATE_CURRENT);
                // Request to start the download
                int startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(this,
                        pendingIntent, ExpansionDownloaderService.class);

                if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) {
                    // The DownloaderService has started downloading the files,
                    // show progress
                    initializeDownloadUI();
                } // otherwise, download not needed so we fall through to
            } catch (PackageManager.NameNotFoundException e) {
                Log.e(LOG_TAG, "Cannot find own package! MAYDAY!");
                e.printStackTrace();
            }

        } else {
            validateXAPKZipFiles();
            Intent intent = new Intent(this, MainActivity.class);
            startActivity(intent);
            finish();
        }

    }

    /**
     * Connect the stub to our service on start.
     */
    @Override
    protected void onStart() {
        if (null != mDownloaderClientStub) {
            mDownloaderClientStub.connect(this);
        }
        super.onStart();
    }

    /**
     * Disconnect the stub from our service on stop
     */
    @Override
    protected void onStop() {
        if (null != mDownloaderClientStub) {
            mDownloaderClientStub.disconnect(this);
        }
        super.onStop();
    }

    /**
     * Critical implementation detail. In onServiceConnected we create the
     * remote service and marshaler. This is how we pass the client information
     * back to the service so the client can be properly notified of changes. We
     * must do this every time we reconnect to the service.
     */
    @Override
    public void onServiceConnected(Messenger m) {
        mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
        mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
    }

    /**
     * The download state should trigger changes in the UI --- it may be useful
     * to show the state as being indeterminate at times. This sample can be
     * considered a guideline.
     */
    @Override
    public void onDownloadStateChanged(int newState) {
        setState(newState);
        boolean showDashboard = true;
        boolean showCellMessage = false;
        boolean paused;
        boolean indeterminate;
        switch (newState) {
            case IDownloaderClient.STATE_IDLE:
                // STATE_IDLE means the service is listening, so it's
                // safe to start making calls via mRemoteService.
                paused = false;
                indeterminate = true;
                break;
            case IDownloaderClient.STATE_CONNECTING:
            case IDownloaderClient.STATE_FETCHING_URL:
                showDashboard = true;
                paused = false;
                indeterminate = true;
                break;
            case IDownloaderClient.STATE_DOWNLOADING:
                paused = false;
                showDashboard = true;
                indeterminate = false;
                break;

            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
                paused = true;
                showDashboard = false;
                indeterminate = false;
                break;
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
                showDashboard = false;
                paused = true;
                indeterminate = false;
                showCellMessage = true;
                break;

            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
                paused = true;
                indeterminate = false;
                break;
            case IDownloaderClient.STATE_PAUSED_ROAMING:
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
                paused = true;
                indeterminate = false;
                break;
            case IDownloaderClient.STATE_COMPLETED:
                showDashboard = false;
                paused = false;
                indeterminate = false;
                validateXAPKZipFiles();
                return;
            default:
                paused = true;
                indeterminate = true;
                showDashboard = true;
        }
        int newDashboardVisibility = showDashboard ? View.VISIBLE : View.GONE;
        if (mDashboard.getVisibility() != newDashboardVisibility) {
            mDashboard.setVisibility(newDashboardVisibility);
        }
        int cellMessageVisibility = showCellMessage ? View.VISIBLE : View.GONE;
        if (mCellMessage.getVisibility() != cellMessageVisibility) {
            mCellMessage.setVisibility(cellMessageVisibility);
        }

        mPB.setIndeterminate(indeterminate);
        setButtonPausedState(paused);
    }

    /**
     * Sets the state of the various controls based on the progressinfo object
     * sent from the downloader service.
     */
    @Override
    public void onDownloadProgress(DownloadProgressInfo progress) {
        mAverageSpeed.setText(getString(R.string.kilobytes_per_second,
                Helpers.getSpeedString(progress.mCurrentSpeed)));
        mTimeRemaining.setText(getString(R.string.time_remaining,
                Helpers.getTimeRemaining(progress.mTimeRemaining)));

        mPB.setMax((int) (progress.mOverallTotal >> 8));
        mPB.setProgress((int) (progress.mOverallProgress >> 8));
        mProgressPercent.setText(String.format(Locale.US, "%d %%", progress.mOverallProgress
                * 100 /
                progress.mOverallTotal));
        mProgressFraction.setText(Helpers.getDownloadProgressString
                (progress.mOverallProgress,
                        progress.mOverallTotal));
    }

    @Override
    protected void onDestroy() {
        this.mCancelValidation = true;
        super.onDestroy();
    }

    /**
     * This is a little helper class that demonstrates simple testing of an
     * Expansion APK file delivered by Market. You may not wish to hard-code
     * things such as file lengths into your executable... and you may wish to
     * turn this code off during application development.
     */
    public static class XAPKFile {
        public final boolean mIsMain;
        public final int mFileVersion;
        public final long mFileSize;

        XAPKFile(boolean isMain, int fileVersion, long fileSize) {
            mIsMain = isMain;
            mFileVersion = fileVersion;
            mFileSize = fileSize;
        }
    }

}