//
//  LevelData.cpp -- A collection of Worksheets
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.h"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>
#include "Utils/TodoUtil.h"

#include <cocos2d.h>

USING_NS_CC;



namespace EggQuizSceneSpace {

vector<Problem>& LevelData::getProblemSet(const string& LanguageTag, char category, string level, int set)
{
    
    auto K = ProblemSetKey(LanguageTag, category, level, set);
    auto& S = Problems[K];

    return S;
}

int LevelData::numSets(const string& LanguageTag, char category, string level)
{
    int set = 1;
    
    while (Problems.find(ProblemSetKey(LanguageTag, category, level, set))!=Problems.end()) {
        set++;
    }
    return set-1;
    
}
    
vector<string> LevelData::getLevels() {
    vector<string> rt = {};
    
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("EggQuizLiteracy/EggQuizLiteracy_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    for (auto row : data) {
        if (row.size() < 2) continue;
        if (row[0][0] == '#') continue;
        bool blankCheck = true;
        for (int i=0; i<row.size(); i++) {
            row[i] = TodoUtil::removeSpaces(TodoUtil::trim(row[i]));
            transform(row[i].begin(), row[i].end(), row[i].begin(), ::tolower);
            if (row[i] != "") blankCheck = false;
        }
        if (blankCheck) continue;
        string newLevel = row[2];
        if (find(rt.begin(), rt.end(), newLevel) != rt.end()) continue;

        rt.push_back(newLevel);
    }
    
    return rt;
}
    
map<string,float> LevelData::loadDurationsheet() {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("EggQuizLiteracy/Audio/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    map<string,float> rt;
    rt.clear();
    
    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        transform(row[0].begin(), row[0].end(), row[0].begin(), ::tolower);
        rt[row[0]] = rowDuration;
    }
    
    return rt;
}

LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = "EggQuizLiteracy/EggQuizLiteracy_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    todoschool::CommentStream IS(S);


    auto Facet = [&] {
        auto It = new todoschool::CustomFacet(IS.getloc());
        
        // NB(xenosoz, 2016): U+0020 'SPACE' is not a whitespace anymore.
        It->clearSpaceFlagsFor(" ");
        return It;
    }();
    
    
    auto Loc = locale(IS.getloc(), Facet);
    IS.imbue(Loc);
    
    
    IS >> It;
    
    return It;
}

istream& operator>>(istream& IS, LevelData& LD) {
    string LanguageTag, level;
    char category;
    int set, problem;
    
    while (IS >> LanguageTag >> category >> level >> set >> problem)
    {
        transform(level.begin(), level.end(), level.begin(), ::tolower);
        auto K = LevelData::ProblemSetKey(LanguageTag, category, level, set);
        auto& S = LD.Problems[K];
        Problem P;
        P.problemNo = problem;
        
        if (!(IS >> P)) {
            CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
            return IS;
        }
        
        //if (find(LevelData::_levels.begin(), LevelData::_levels.end(), level) != LevelData::_levels.end()) LevelData::_levels.push_back(level);
        S.push_back(P);
        
    }

    
    
    return IS;
}

};//namespace EggQuizSceneSpace

