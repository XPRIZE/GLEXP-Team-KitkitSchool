//
//  LineMatchingProblemBank.h
//  todomath
//
//  Created by KimDonggyu on 2016. 5. 4..
//
//

#ifndef LineMatchingProblemBank_h
#define LineMatchingProblemBank_h

#include <stdio.h>
#include "cocos2d.h"
#include "Base/ProblemBank.h"

USING_NS_CC;

class LineMatchingProblemBank : public ProblemBank {
public:
    LineMatchingProblemBank(){};
    ~LineMatchingProblemBank(){};
    
    virtual Json::Value generateParameters(int level);
};


#endif /* LineMatchingProblemBank_h */
