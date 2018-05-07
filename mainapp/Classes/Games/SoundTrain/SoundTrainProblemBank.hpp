//
//  SoundTrainProblemBank.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 29/08/2017.
//
//

#pragma once
#include "SoundTrainHeader.h"
#include "SoundTrainProblemBank.hpp"

BEGIN_NS_SOUNDTRAIN;

struct SoundTrainLevelStruct {
    int level;
    string answer;
};

class SoundTrainProblemBank {
public:
    SoundTrainProblemBank();
    vector<SoundTrainLevelStruct> loadData(int level);
    vector<int> getLevels();
    Json::Value generateProblems(int level);
private:
    int _maxLevel = 0;
};

END_NS_SOUNDTRAIN;
