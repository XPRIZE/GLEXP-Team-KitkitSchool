//
//  TraceFieldDepot.h on Jun 17, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ArchiveModels/TraceKnotList.h"
#include "TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceFieldDepot {
public:
    string assetPrefix() const;
    string defaultFont() const;
    string cursorFilename() const;
    float cursorMaxRadius() const;

    Sprite* createBackgroundSprite(const Size& ParentSize) const;

    void preloadSoundEffects() const;
    SoundEffect soundForTrace() const;

    Json::Value jsonForGlyphArchive(string Str);
    TraceKnotList knotListForGlyphArchive(string Str);

private:
    vector<SoundEffect> soundEffectsToPreload() const;
};

END_NS_TRACEFIELD
