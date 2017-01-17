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

#include <cocos2d.h>

USING_NS_CC;



namespace EggQuizSceneSpace {

vector<Problem>& LevelData::getProblemSet(const string& LanguageTag, char category, int level, int set)
{
    
    auto K = ProblemSetKey(LanguageTag, category, level, set);
    auto& S = Problems[K];

    return S;
}

int LevelData::numSets(const string& LanguageTag, char category, int level)
{
    int set = 1;
    
    while (Problems.find(ProblemSetKey(LanguageTag, category, level, set))!=Problems.end()) {
        set++;
    }
    return set-1;
    
}
    

    
    
LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = "EggQuiz/EggQuiz_Levels.tsv";
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
    string LanguageTag;
    char category;
    int level, set, problem;
    
    while (IS >> LanguageTag >> category >> level >> set >> problem)
    {
        auto K = LevelData::ProblemSetKey(LanguageTag, category, level, set);
        auto& S = LD.Problems[K];
        Problem P;
        P.problemNo = problem;
        
        if (!(IS >> P)) {
            CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
            return IS;
        }
        
        S.push_back(P);
        
    }

    
    
    return IS;
}

};//namespace EggQuizSceneSpace

