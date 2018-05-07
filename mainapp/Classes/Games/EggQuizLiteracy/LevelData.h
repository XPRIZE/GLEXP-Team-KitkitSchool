//
//  LevelData.h -- A collection of Worksheets
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Problem.h"
#include <Common/Basic/OneBasedVector.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace EggQuizSceneSpace {

class LevelData {
    
    // language, category, level, set
    typedef tuple<string, char, string, int> ProblemSetKey;
    map<ProblemSetKey, vector<Problem>> Problems;
    
public:

    vector<Problem>& getProblemSet(const string& LanguageTag, char category, string level, int set);
    int numSets(const string& LanguageTag, char category, string level);
    static vector<string> getLevels();
    
public:
    static LevelData defaultData();
    static map<string,float> loadDurationsheet();
    friend istream& operator>>(istream& IS, LevelData& LD);

};

}; // EggQuizSceneSpace
