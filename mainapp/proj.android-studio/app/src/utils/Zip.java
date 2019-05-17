package utils;

import org.cocos2dx.cpp.SplashScreenActivity;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class Zip {

    private ZipFile _zipFile;

    public Zip(ZipFile zipFile) {
        this._zipFile = zipFile;
    }

    public Zip(String pathToZipFile) throws IOException {
        this._zipFile = new ZipFile(pathToZipFile);
    }

    public void close() throws IOException {
        _zipFile.close();
    }

    public void unzip(String extractPath) throws IOException {
        File targetDir = new File(extractPath);
        if (!targetDir.exists() && !targetDir.mkdirs()) {
            throw new IOException("Unable to create directory");
        }

        if (!targetDir.isDirectory()) {
            throw new IOException("Unable to extract to a non-directory");
        }

        Enumeration<? extends ZipEntry> zipEntries = _zipFile.entries();
        SplashScreenActivity.countDownTimer.start();
        while (zipEntries.hasMoreElements()) {
            ZipEntry zipEntry = zipEntries.nextElement();
            String path = extractPath + zipEntry.getName();
            if (zipEntry.isDirectory()) {
                /*File newDir = new File(path);
				if(!newDir.mkdirs()){
					throw new IOException("Unable to extract the zip entry " + path);
				}*/
            } else {
                BufferedInputStream inputStream = new BufferedInputStream(_zipFile.getInputStream(zipEntry));

                File outputFile = new File(path);
                File outputDir = new File(outputFile.getParent());

                if (!outputDir.exists() && !outputDir.mkdirs()) {
                    throw new IOException("unable to make directory for entry " + path);
                }

                if (!outputFile.exists() && !outputFile.createNewFile()) {
                    throw new IOException("Unable to create directory for " + path);
                }

                BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream(outputFile));
                try {
                    int currByte;
                    while ((currByte = inputStream.read()) != -1) {
                        outputStream.write(currByte);
                    }
                    File flagFile = new File(extractPath + ".success.txt");
                    flagFile.createNewFile();
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    outputStream.close();
                    inputStream.close();
                }
            }
        }
    }
}
