//
//  TraceFieldDepot.cpp on Jun 17, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceFieldDepot.h"
#include <Games/NumberTrace/Common/Utils/JsonUtils.h>
#include <Games/NumberTrace/Common/Utils/StringUtils.h>


BEGIN_NS_TRACEFIELD

string TraceFieldDepot::assetPrefix() const {
    string It = "Common/Controls/TraceField";
    return It;
}

string TraceFieldDepot::defaultFont() const {
//    return "fonts/TSAlphabets.ttf";
    return "fonts/kitkitalphabet.ttf";
}

string TraceFieldDepot::cursorFilename() const {
    string It = assetPrefix() + "/TraceField_Cursor2.png";
    return It;
}
    
float TraceFieldDepot::cursorMaxRadius() const {
    return 118.f * 2.1f / 2.f;  // XXX
}

Sprite* TraceFieldDepot::createBackgroundSprite(const Size& ParentSize) const {
    // NB(xenosoz, 2016): Default configurations. In Pixels (px).
    int Width = 1218;
    int Height = 1616;
    int MarginTop = 92;
    int MarginRight = 82;
    int MarginBottom = 92;

    Optional<Json::Value> BackgroundJson = jsonFromFile(assetPrefix() + "/TraceField_Background.json");
    if (BackgroundJson) {
        // NB(xenosoz, 2016): Override default configurations.
        Json::Value& Json = BackgroundJson.value();
        Width = parseIntWithSuffix(Json.get("width", "").asString(), "px").value(Width);
        Height = parseIntWithSuffix(Json.get("height", "").asString(), "px").value(Height);
        MarginTop = parseIntWithSuffix(Json.get("margin-top", "").asString(), "px").value(MarginTop);
        MarginRight = parseIntWithSuffix(Json.get("margin-right", "").asString(), "px").value(MarginRight);
        MarginBottom = parseIntWithSuffix(Json.get("margin-bottom", "").asString(), "px").value(MarginBottom);
    }

    Sprite* It = Sprite::create(assetPrefix() + "/TraceField_Background.png");
    if (It) {
        const Size& Size = It->getContentSize();
        if (Size.width != Width || Size.height != Height) {
            CCLOGWARN("Unexpected sprite size: TraceField_Background.png");
            CCLOGWARN("Given: (width: %f, height: %f)", Size.width, Size.height);
            CCLOGWARN("Expected: (width: %d, height: %d)", Width, Height);
        }

        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        It->setPosition(Point(ParentSize.width - MarginRight, MarginBottom));
    }

    return It;
}

void TraceFieldDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect TraceFieldDepot::soundForTrace() const {
    string P = assetPrefix() + "/Sounds";
    return SoundEffect(P + "/SFX_PENCILLOOP.m4a");
}

Json::Value TraceFieldDepot::jsonForGlyphArchive(string Str) {
    Json::Value JsonValue;
    string GlyphName;
    if (Str == "1" || Str == "2" || Str == "3" || Str == "4" || Str == "5" || Str == "6" || Str == "7" || Str == "8" || Str == "9" || Str == "0")
    {
        GlyphName = "TSAlphabets";
    }
    else
    {
        GlyphName = "kitkitalphabet";
    }
    
    // XXX
    string Filename;
    if (isDigit(Str))
        Filename = (assetPrefix() + "/Glyphs") + ("/" + GlyphName + ".Digit") + ("/" + Str + "/") + (GlyphName + ".Digit." + Str + ".json");
    else if (isUpper(Str))
        Filename = (assetPrefix() + "/Glyphs") + ("/" + GlyphName + ".Latin_Capital") + ("/" + Str + "/") + (GlyphName + ".Latin_Capital." + Str + ".json");
    else if (isLower(Str))
        Filename = (assetPrefix() + "/Glyphs") + ("/" + GlyphName + ".Latin_Small") + ("/" + toUpper(Str) + "/") + (GlyphName + ".Latin_Small." + toUpper(Str) + ".json");
    else if (isPunct(Str))
        Filename = (assetPrefix() + "/Glyphs") + ("/" + GlyphName + ".Special") + ("/" + toURLEncoded(Str) + "/") + (GlyphName + ".Special." + toURLEncoded(Str) + ".json");

    if (Filename.empty()) { return JsonValue; }

    string JsonStr = FileUtils::getInstance()->getStringFromFile(Filename);
    Json::Reader JsonReader;
    if (!JsonReader.parse(JsonStr, JsonValue)) {
        CCLOGERROR("JSON reader parse error: %s", Filename.c_str());
        JsonValue.clear();
    }
 
    return JsonValue;
}

TraceKnotList TraceFieldDepot::knotListForGlyphArchive(string Str) {
    Json::Value JsonValue = jsonForGlyphArchive(Str);
    TraceKnotList KnotList(JsonValue);

    return KnotList;
}
    
vector<SoundEffect> TraceFieldDepot::soundEffectsToPreload() const {
    return {
        soundForTrace(),
    };
}

END_NS_TRACEFIELD
