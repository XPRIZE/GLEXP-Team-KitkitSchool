//
//  NumberTrainProblemBank.hpp
//  todomath
//
//  Created by Gunho Lee on 2017. 3. 21..
//
//

#ifndef NumberTrainProblemBank_hpp
#define NumberTrainProblemBank_hpp

#include <stdio.h>
#include "Base/ProblemBank.h"

class NumberTrainProblemBank : public ProblemBank {
public:
    NumberTrainProblemBank();
    ~NumberTrainProblemBank();
    
    virtual Json::Value generateParameters(int level);
};

#endif /* NumberTrainProblemBank_hpp */
