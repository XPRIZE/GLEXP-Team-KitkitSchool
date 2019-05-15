package org.cocos2dx.cpp.ReadingBird;


import java.io.File;
import java.io.IOException;

import edu.cmu.pocketsphinx.Config;
import edu.cmu.pocketsphinx.Decoder;

public class RecognizerSetup {
    private final Config config;

    public static RecognizerSetup defaultSetup() {
        return new RecognizerSetup(Decoder.defaultConfig());
    }

    public static RecognizerSetup setupFromFile(File configFile) {
        return new RecognizerSetup(Decoder.fileConfig(configFile.getPath()));
    }

    private RecognizerSetup(Config config) {
        this.config = config;
    }

    public Recognizer getRecognizer() throws IOException {
        return new Recognizer(this.config);
    }

    public RecognizerSetup setAcousticModel(File model) {
        return this.setString("-hmm", model.getPath());
    }

    public RecognizerSetup setDictionary(File dictionary) {
        return this.setString("-dict", dictionary.getPath());
    }

    public RecognizerSetup setSampleRate(int rate) {
        return this.setFloat("-samprate", (double)rate);
    }

    public RecognizerSetup setRawLogDir(File dir) {
        return this.setString("-rawlogdir", dir.getPath());
    }

    public RecognizerSetup setKeywordThreshold(float threshold) {
        return this.setFloat("-kws_threshold", (double)threshold);
    }

    public RecognizerSetup setBoolean(String key, boolean value) {
        this.config.setBoolean(key, value);
        return this;
    }

    public RecognizerSetup setInteger(String key, int value) {
        this.config.setInt(key, value);
        return this;
    }

    public RecognizerSetup setFloat(String key, double value) {
        this.config.setFloat(key, value);
        return this;
    }

    public RecognizerSetup setString(String key, String value) {
        this.config.setString(key, value);
        return this;
    }

    static {
        System.loadLibrary("pocketsphinx_jni");
    }
}
