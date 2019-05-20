package com.maq.xprize.marimba.utility;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

import java.io.IOException;
import java.util.HashMap;

public class EffectSound {

    ////////////////////////////////////////////////////////////////////////////////

    public static int SOUND_INTRO = 8;

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
            mSoundPoolMap.put(0, soundPool.load(mgr.openFd("Marimba_NoteC.wav"), 1));
            mSoundPoolMap.put(1, soundPool.load(mgr.openFd("Marimba_NoteD.wav"), 1));
            mSoundPoolMap.put(2, soundPool.load(mgr.openFd("Marimba_NoteE.wav"), 1));
            mSoundPoolMap.put(3, soundPool.load(mgr.openFd("Marimba_NoteF.wav"), 1));
            mSoundPoolMap.put(4, soundPool.load(mgr.openFd("Marimba_NoteG.wav"), 1));
            mSoundPoolMap.put(5, soundPool.load(mgr.openFd("Marimba_NoteA.wav"), 1));
            mSoundPoolMap.put(6, soundPool.load(mgr.openFd("Marimba_NoteB.wav"), 1));
            mSoundPoolMap.put(7, soundPool.load(mgr.openFd("Marimba_NoteC2.wav"), 1));
            mSoundPoolMap.put(SOUND_INTRO, soundPool.load(mgr.openFd("SFX_Marimba_Glissando.wav"), 1));

        } catch (IOException e) {
            Log.e(e.toString());
        }
    }

    public void startSoundPool(int id) {
        if (soundPool == null) return;
        int streamId = soundPool.play(mSoundPoolMap.get(id), 1.0f, 1.0f, 1, 0, 1.0f);
        mSoundPoolStreamId.put(id, streamId);
    }

    public void stopSoundPool(int id) {
        if (soundPool == null) return;

        if (mSoundPoolStreamId.get(id) != null) {
            soundPool.stop(mSoundPoolStreamId.get(id));
        }
    }
}