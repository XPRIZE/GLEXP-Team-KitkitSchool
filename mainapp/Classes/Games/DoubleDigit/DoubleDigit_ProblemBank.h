//
//  DoubleDigit_ProblemBank.h
//
//  Created by Sunmi Seol on 12/14/15.
//
//

#ifndef __DoubleDigit_ProblemBank__
#define __DoubleDigit_ProblemBank__

#include "Base/ProblemBank.h"

class DoubleDigit_ProblemBank : public ProblemBank
{
    
public:
    
    DoubleDigit_ProblemBank();
    ~DoubleDigit_ProblemBank();
    
    virtual Json::Value generateParameters(int level);
    
public:
    
    static int getNumberOfProblemsInScreen() { return 6; }
    
private:
};


#endif /* defined(__DoubleDigit_ProblemBank__) */
