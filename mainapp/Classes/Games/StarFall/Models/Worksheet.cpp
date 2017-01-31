//
//  Worksheet.cpp on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Worksheet.h"
#include "../Utils/StarFallDepot.h"
#include <3rdParty/json/json.h>

using namespace std;


namespace todoschool {
namespace starfall {

Worksheet::Worksheet() {
    // NB(xenosoz, 2016): Default dummy data.
    TargetHitCount = 10;
    PreludeDuration = .5f;
    TargetFallingDuration = 10.f;
    WordRegenCooltime = 2.f;
}

size_t Worksheet::size() {
    StarFallDepot Depot;
    Json::Value Data = Depot.jsonForMainGameWork();
    
    Json::Value Levels = Data["levels"];
    return Levels.size();
}

Worksheet Worksheet::forLevelID(size_t LevelID) {
    StarFallDepot Depot;
    Json::Value Data = Depot.jsonForMainGameWork();

    Json::Value Defaults = Data["defaults"];
    Json::Value Levels = Data["levels"];
    int LevelIndex = -1;

    for (int I = 0, E = Levels.size(); I < E; ++I) {
        if (Levels[I].get("level_number", -1).asInt() == (int)LevelID) {
            LevelIndex = I;
            break;
        }
    }

    if (LevelIndex == -1) {
        CCLOGERROR("Undefined level id(%d) in %s", (int)LevelID, __PRETTY_FUNCTION__);
        assert(false);
    }


    Json::Value Level = Levels[LevelIndex];

    auto Value = [&](const string& Key, Json::Value Fallback = Json::Value()) {
        return Level.get(Key, Defaults.get(Key, Fallback));
    };
    auto Int = [&](const string& Key, int DefaultValue) {
        return Value(Key, DefaultValue).asInt();
    };
    auto Float = [&](const string& Key, float DefaultValue) {
        return Value(Key, DefaultValue).asFloat();
    };


    Worksheet W;
    W.TargetHitCount = Int("target_hit_count", W.TargetHitCount);
    W.PreludeDuration = Float("prelude_duration", W.PreludeDuration);
    W.TargetFallingDuration = Float("target_falling_duration", W.TargetFallingDuration);
    W.WordRegenCooltime = Float("word_regen_cooltime", W.WordRegenCooltime);

    for (auto Word : Value("words"))
        W.WordList.push_back(Word.asString());

    W.EnabledSymbols = enabledSymbolsWithWords(W.WordList);
    
    return W;
}

Worksheet Worksheet::dummyLevel1() {
    Worksheet W;

    W.WordList = {"a", "e", "i", "o", "u"};
    W.EnabledSymbols = enabledSymbolsWithWords(W.WordList);
    
    return W;
}

Worksheet Worksheet::dummyLevel2() {
    Worksheet W;
    
    W.WordList = {
        "q", "w", "e", "r", "t",
        "y", "u", "i", "o", "p",
    };
    W.EnabledSymbols = enabledSymbolsWithWords(W.WordList);
    
    return W;
}

Worksheet Worksheet::dummyLevel3() {
    Worksheet W;
    
    W.WordList = {
        "dad", "res", "cat", "eat", "csv",
        "ver", "tea", "rsa", "wet", "wat",
    };
    W.EnabledSymbols = enabledSymbolsWithWords(W.WordList);
    
    return W;
}

SymbolSetType Worksheet::enabledSymbolsWithWords(const std::vector<std::string>& Words) {
    SymbolSetType S;
    
    for (const auto& W : Words)
        for (auto C : W)
            S.insert(std::string{C});
    
    return S;
}

}  // namespace starfall
}  // namespace todoschool
