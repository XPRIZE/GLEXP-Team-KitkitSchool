//
//  ProblemData.cpp
//  KitkitSchool
//
//  Created by timewalker on 13/12/2016.
//
//

#include "ProblemData.hpp"
#include "Utils/TodoUtil.h"

namespace ComprehensionTest
{
    namespace FillTheBlanks
    {
        ProblemData* ProblemData::parse(std::vector<std::string> rawData)
        {
            ProblemData* pm = new ProblemData();
            
            //pm->questionText = rawData[1];
            pm->questionImage = rawData[1];
            
            TodoUtil::replaceAll(rawData[2], "\\\"", "\"");
            
            bool replaceMode = false;
            auto &text = rawData[2];
            
            for (int i = 0; i < text.length(); i++)
            {
                if (text[i] == '[') { replaceMode = true; continue; }
                if (text[i] == ']') { replaceMode = false; continue; }
                
                if (replaceMode && text[i] == ' ')
                {
                    text[i] = '^';
                }
            }
            
            // wordSet
            for (std::string eachValue : TodoUtil::split(text, ' '))
            {
                TodoUtil::replaceAll(eachValue, "^", " ");
                
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
            if (rawData.size()>3) {
                pm->wrongWordSet = TodoUtil::splitCSV(rawData[3]);
                for (int i = 0; i < pm->wrongWordSet.size(); i++)
                {
                    pm->wrongWordSet[i] = TodoUtil::trim(pm->wrongWordSet[i]);
                }
            }
            
            if (rawData.size()>4) {
                pm->soundPath = rawData[4];
            }
            
            return pm;
        }
        
    }
    
}
