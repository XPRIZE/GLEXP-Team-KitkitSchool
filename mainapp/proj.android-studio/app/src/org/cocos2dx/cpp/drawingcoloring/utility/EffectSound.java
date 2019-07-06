package org.cocos2dx.cpp.drawingcoloring.utility;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

import java.io.IOException;
import java.util.HashMap;

public class EffectSound {

    public static int SOUND_CHALK = 0;
    public static int SOUND_NEW_BOARD = 1;
    public static int SOUND_PICK_CRAYON = 2;
    public static int SOUND_SAVE_BOARD = 3;

    private static EffectSound _self = null;
    private SoundPool soundPool;
    private HashMap<Integer, Integer> mSoundPoolMap;
    private HashMap<Integer, Integer> mSoundPoolStreamId;

    private EffectSound(Context context) {
        try {
            AssetManager mgr = context.getAssets();
            soundPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
            mSoundPoolMap = new HashMap<>();
            mSoundPoolStreamId = new HashMap<>();
            mSoundPoolMap.put(SOUND_CHALK, soundPool.load(mgr.openFd("sound/Paint_ChalkLoop2.wav"), 1));
            mSoundPoolMap.put(SOUND_NEW_BOARD, soundPool.load(mgr.openFd("sound/Paint_NewBoard1.wav"), 1));
            mSoundPoolMap.put(SOUND_PICK_CRAYON, soundPool.load(mgr.openFd("sound/Paint_PickCrayon.wav"), 1));
            mSoundPoolMap.put(SOUND_SAVE_BOARD, soundPool.load(mgr.openFd("sound/Paint_SaveArtBoard.wav"), 1));

        } catch (IOException e) {
            Log.e(e.toString());
        }
    }

    public static EffectSound getInstance(Context context) {
        if (null == _self) _self = new EffectSound(context);
        return _self;
    }

    public void startSoundPool(int id, int loop) {
        if (soundPool == null) return;
        int streamId = soundPool.play(mSoundPoolMap.get(id), 1.0f, 1.0f, 1, loop, 1.0f);
        mSoundPoolStreamId.put(id, streamId);
    }

    public void stopSoundPool(int id) {
        if (soundPool == null) return;

        if (mSoundPoolStreamId.get(id) != null) {
            soundPool.stop(mSoundPoolStreamId.get(id));
        }
    }

    public void setVolume(int id, float volume) {
        if (mSoundPoolStreamId.get(id) != null) {
            soundPool.setVolume(id, volume, volume);
        }
    }
}