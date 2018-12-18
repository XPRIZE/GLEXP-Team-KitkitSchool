//
//  ProblemBank.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 09/03/2018.
//

#pragma once
#include "ThirtyPuzzleHeader.h"

BEGIN_NS_THIRTYPUZZLE;

struct ThirtyPuzzleLevelStruct {
    int level;
    int problem;
    string columns;
    string suggest;
    string isRandom;
    int problemNumber;
};

class ProblemBank {
    static ProblemBank* _instance;
public:
    static ProblemBank* getInstance();
    void init();
    vector<ThirtyPuzzleLevelStruct> loadData(int level);
    vector<int> getLevels();
private:
    int _maxLevel, _problemCount;
    
};

END_NS_THIRTYPUZZLE;
