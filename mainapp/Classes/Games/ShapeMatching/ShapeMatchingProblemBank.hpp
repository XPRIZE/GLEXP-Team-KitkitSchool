//
//  ShapeMatchingProblemBank.hpp
//  TodoMath
//
//  Created by Sungwoo Kang on 5/31/17.
//
//

#ifndef ShapeMatchingProblemBank_hpp
#define ShapeMatchingProblemBank_hpp

#include "Base/ProblemBank.h"

class ShapeMatchingProblemBank : public ProblemBank {
public:
    
    virtual Json::Value generateParameters(int level);
};

#endif /* ShapeMatchingProblemBank_hpp */
