package com.enuma.voice_engine_a;

/**
 *  NB(xenosoz, 2018): The TTS Engine wrapper (entry point)
 */

import com.enuma.voice_engine_a.core.CompatTtsService;


public class VoiceEngineTtsService extends CompatTtsService
{
    private static final String LIB_NAME = "libttspico.so";

    @Override
    protected String getSoFilename() {
        return LIB_NAME;
    }
}
