//
//  PatternTrainProblemBank.hpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "PatternTrainHeader.h"

BEGIN_NS_PATTERNTRAIN;

class PatternTrainProblemBank {
public:
    enum shape {
        FIGURE,
        NUMBERALPHABET,
        SIZE
    };
    
    enum choice {
        AB,
        ABC,
        UNLIMITEDABC
    };
    
    enum blank {
        LAST,
        RANDOM,
        ALL
    };
    
    PatternTrainProblemBank();
    Json::Value generateProblems(int level);
    vector<string> getShapeVec(shape shapeNum);

};

END_NS_PATTERNTRAIN;
