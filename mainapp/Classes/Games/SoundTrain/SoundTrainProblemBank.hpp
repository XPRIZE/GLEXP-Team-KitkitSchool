//
//  SoundTrainProblemBank.hpp
//  enumaXprize
//
//  Created by JungJaehun on 29/08/2017.
//
//

#pragma once
#include "SoundTrainHeader.h"

BEGIN_NS_SOUNDTRAIN;

class SoundTrainProblemBank {
public:
    SoundTrainProblemBank();
    Json::Value generateProblems(int level);
};

END_NS_SOUNDTRAIN;
