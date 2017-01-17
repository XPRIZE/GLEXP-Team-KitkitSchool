//
//  ProblemData.cpp
//  enumaXprize
//
//  Created by HyeonGyu Yu on 31/12/2016.
//
//

#include "ProblemData.hpp"
#include "Utils/TodoUtil.h"

namespace SentenceMaker
{
    ProblemData* ProblemData::parse(std::vector<std::string> rawData)
    {
        ProblemData* pm = new ProblemData();
        
        // wordSet
        for (std::string eachValue : TodoUtil::split(rawData[1], ' '))
        {
            if (TodoUtil::trim(eachValue).size() == 0)
                continue;
            
            if (eachValue.find("]") != std::string::npos)
            {
                // ex) This is the [apple]. <== this value
                if (eachValue.find("].") != std::string::npos)
                {
                    Word* w;
                    std::vector<std::string> eachValueVector = TodoUtil::split(eachValue, ']');
                    TodoUtil::replace(eachValue, "].", "");
                    w = new Word();
                    TodoUtil::replace(eachValueVector[0], "[", "");
                    w->value = eachValueVector[0];
                    w->bBlank = true;
                    pm->wordSet.push_back(w);
                    
                    w = new Word();
                    w->value = eachValueVector[1];
                    w->bBlank = false;
                    pm->wordSet.push_back(w);
                }
                else
                {
                    Word* w = new Word();
                    TodoUtil::replace(eachValue, "[", "");
                    TodoUtil::replace(eachValue, "]", "");
                    
                    w->value = eachValue;
                    w->bBlank = true;
                    pm->wordSet.push_back(w);
                }
            }
            else
            {
                Word* w = new Word();
                w->value = eachValue;
                w->bBlank = false;
                pm->wordSet.push_back(w);
            }
        }
        
        // wrongWordSet
        pm->wrongWordSet = TodoUtil::splitCSV(rawData[2]);
            
        for (int i = 0; i < pm->wrongWordSet.size(); i++)
        {
            pm->wrongWordSet[i] = TodoUtil::trim(pm->wrongWordSet[i]);
        }
        
        return pm;
    }
}
