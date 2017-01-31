//
//  ProblemData.hpp
//  enumaXprize
//
//  Created by timewalker on 09/12/2016.
//
//

#ifndef ProblemData_hpp
#define ProblemData_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <string>

USING_NS_CC;

namespace ComprehensionTest
{
    namespace Matching
    {
        class ProblemData
        {
        public:
            std::string questionText;
            std::string leftType;
            std::string rightType;
            std::vector<std::pair<int, std::string>> leftValue;
            std::vector<std::pair<int, std::string>> rightValue;
            static ProblemData* parse(std::vector<std::string> rawData);
            
        private:
            static bool isWellShuffled(std::vector<std::pair<int, std::string>> leftValue, std::vector<std::pair<int, std::string>> rightValue);
        };
    }
}

#endif /* ProblemData_hpp */
