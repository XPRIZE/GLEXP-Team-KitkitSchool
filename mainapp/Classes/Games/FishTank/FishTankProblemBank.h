//
//  FishTankProblemBank.h
//  todomath
//
//  Created by Jesun Kim on 2/25/16.
//
//

#ifndef FishTankProblemBank_h
#define FishTankProblemBank_h

#include "Base/ProblemBank.h"

class FishTankProblemBank : public ProblemBank {
public:
    bool m_isRandom;
    
public:
    FishTankProblemBank();
    ~FishTankProblemBank();
    
    virtual Json::Value generateParameters(int level);
};

#endif /* FishTankProblemBank_h */
