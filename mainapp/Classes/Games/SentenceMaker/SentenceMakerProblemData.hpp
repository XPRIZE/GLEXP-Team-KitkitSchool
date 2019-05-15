//
//  ProblemData.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 31/12/2016.
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

namespace SentenceMaker
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
        std::vector<Word*> wordSet;
        std::vector<std::string> wrongWordSet;
        
        static ProblemData* parse(std::vector<std::string> rawData);
    };
}

#endif /* ProblemData_hpp */
