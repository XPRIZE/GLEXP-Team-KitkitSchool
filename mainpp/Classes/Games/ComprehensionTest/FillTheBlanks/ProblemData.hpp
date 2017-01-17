//
//  ProblemData.hpp
//  enumaXprize
//
//  Created by timewalker on 13/12/2016.
//
//

#ifndef ProblemData_hpp
#define ProblemData_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

namespace ComprehensionTest
{
    namespace FillTheBlanks
    {
        class Word
        {
        public:
            std::string value;
            bool bBlank;
            
        };
        
        class ProblemData
        {
        public:
            std::string questionText;
            std::string questionImage;
            std::vector<Word*> wordSet;
            std::vector<std::string> wrongWordSet;
            std::string soundPath;
            
            static ProblemData* parse(std::vector<std::string> rawData);
        };
    }
}

#endif /* ProblemData_hpp */
