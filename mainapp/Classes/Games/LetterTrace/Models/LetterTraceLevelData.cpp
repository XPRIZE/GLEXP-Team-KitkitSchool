//
//  LevelData.cpp -- A collection of Worksheets
//  TodoSchool on Oct 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterTraceLevelData.h"
#include "../Utils/LetterTraceMainDepot.h"
#include <Common/Basic/CommentStream.h>

// for data checking
#include "Managers/UserManager.hpp"
#include "3rdParty/CCNativeAlert.h"
#include "Common/Sounds/CommonSound.hpp"


BEGIN_NS_LETTERTRACE

vector<int> LevelData::levelIDsFor(const string& LanguageTag) {
    vector<int> IDs;
    
    for (auto LevelItem : Levels) {
        auto& Key = LevelItem.first;
        if (Key.first != LanguageTag) { continue; }
        IDs.push_back((int)Key.second);
    }
    
    stable_sort(IDs.begin(), IDs.end());
    return IDs;
}

Worksheet& LevelData::randomSheetFor(const string& LanguageTag, size_t LevelID, int *workSheetID) {
    auto K = LevelKey(LanguageTag, LevelID);
    auto& L = Levels[K];

    *workSheetID = random(L.begin_index(), L.end_index()-1);
    return L.at( *workSheetID);
}

LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = MainDepot().assetPrefix() + "/LetterTrace_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    CommentStream IS(S);
    IS >> It;
    
    if (UserManager::getInstance()->isDebugMode()) {
        It.checkData();
    }
    
    
    return It;
}

void LevelData::checkData()
{
    
    string errorList = "";
    
    for (auto it : Levels ) {
        auto key = it.first;
        auto sheets = it.second;
        auto lang = it.first.first;
        auto level = it.first.second;
        
        for (auto sheet : sheets) {
            for (int i = sheet.beginProblemID(); i<sheet.endProblemID(); i++) {
                auto p = sheet.problemByID(i);
                
                SoundEffect s1, s2;
                string v = "LetterTrace/BonusVideos/" + p.VideoFileName;
                
                if (p.TraceText!="-") s1 =  CommonSound().soundForLetterName(p.TraceText);
                if (p.FullText!="-") s2 = CommonSound().soundForWord(p.FullText);
                
                
                if (p.TraceText!="-" && s1.bad()) errorList += "missing sound for letter : ["+p.TraceText+"] ("+lang+")\n";
                if (p.FullText!="-" && s2.bad()) {
                    errorList += "missing sound for word : ["+p.FullText+"] ("+lang+")\n";
                }
                if (p.VideoFileName!="-" && !FileUtils::getInstance()->isFileExist(v)) errorList += "missing video : ["+p.VideoFileName+"] ("+lang+")\n";

            }
        }
    }
    
    if (errorList.length()>0) {
        NativeAlert::show("Error in LetterTrace_Levels.tsv", errorList, "OK");
    }
    
}

istream& operator>>(istream& IS, LevelData& LD) {
    string LanguageTag;
    size_t LevelID;
    
    while (IS >> LanguageTag >> LevelID)
    {
        auto K = LevelData::LevelKey(LanguageTag, LevelID);
        auto& L = LD.Levels[K];
        
        size_t SheetID;
        if (IS >> SheetID) {
            L.resize_for_index(SheetID);
            auto& S = L.at(SheetID);

            IS >> S;
        }
    }
    
    return IS;
}



END_NS_LETTERTRACE
