package org.cocos2dx.cpp.ReadingBird;

import edu.cmu.pocketsphinx.Hypothesis;

public interface RecognizerListener {
    public void onBeginningOfSpeech();

    public void onEndOfSpeech();

    public void onVolume(int volume);

    public void onResult(Hypothesis var1);

    public void onError(Exception var1);

    public void onTimeout();
}
