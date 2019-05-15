package com.enuma.blackboard.utility;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

import java.io.IOException;
import java.util.HashMap;

public class EffectSound {

    ////////////////////////////////////////////////////////////////////////////////

    public static int SOUND_CHALK = 0;
    public static int SOUND_ERASER = 1;
    public static int SOUND_ERASER_POSITION = 2;

    ////////////////////////////////////////////////////////////////////////////////

    private static EffectSound _self = null;
    private SoundPool soundPool;
    private HashMap<Integer, Integer> mSoundPoolMap;
    private HashMap<Integer, Integer> mSoundPoolStreamId;

    public static EffectSound getInstance(Context context) {
        if (null == _self) _self = new EffectSound(context);
        return _self;
    }

    private EffectSound(Context context) {
        try {
            AssetManager mgr = context.getAssets();
            soundPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
            mSoundPoolMap = new HashMap<>();
            mSoundPoolStreamId = new HashMap<>();
            mSoundPoolMap.put(SOUND_CHALK, soundPool.load(mgr.openFd("sound/sfx_crayonloop.wav"), 1));
            mSoundPoolMap.put(SOUND_ERASER, soundPool.load(mgr.openFd("sound/paint_chalkloop2.wav"), 1));
            mSoundPoolMap.put(SOUND_ERASER_POSITION, soundPool.load(mgr.openFd("sound/sfx_crayondrop.wav"), 1));

        } catch (IOException e) {
            Log.e(e.toString());
        }
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
}