//
//  ShapeMatchingProblemBank.cpp
//  TodoMath
//
//  Created by Sungwoo Kang on 5/31/17.
//
//

#include "ShapeMatchingProblemBank.hpp"

Json::Value ShapeMatchingProblemBank::generateParameters(int level)
{
    Json::Value a;
    a["level"] = level;
    
    return a;
}
