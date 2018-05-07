//
//  ProblemData.cpp
//  KitkitSchool
//
//  Created by timewalker on 09/12/2016.
//
//

#include "ProblemData.hpp"
#include "Utils/TodoUtil.h"

#include "3rdParty/CCNativeAlert.h"

namespace ComprehensionTest
{
    namespace Matching
    {
        ProblemData* ProblemData::parse(std::vector<std::string> rawData)
        {
            int index;
            
            ProblemData* result = new ProblemData();
            
            //result->questionText = rawData[1];
            
//            result->leftType = rawData[2];
//            result->rightType = rawData[3];
            
            result->leftValue.clear();
            
            index = 1;
            for (auto eachValue : TodoUtil::splitCSV(rawData[1]))
            {
                result->leftValue.push_back(std::pair<int, std::string>(index++, TodoUtil::trim(eachValue)));
            }
            
            
            index = 1;
            result->rightValue.clear();
            for (auto eachValue : TodoUtil::splitCSV(rawData[2]))
            {
                result->rightValue.push_back(std::pair<int, std::string>(index++, TodoUtil::trim(eachValue)));
            }
            
            {
                auto value = result->leftValue[0].second;
                auto isImage = (value.find(".png") != std::string::npos) || (value.find(".jpg") != std::string::npos);
                result->leftType = (isImage) ? "image" : "text";
            }
            
            {
                auto value = result->rightValue[0].second;
                auto isImage = (value.find(".png") != std::string::npos) || (value.find(".jpg") != std::string::npos);
                result->rightType = (isImage) ? "image" : "text";
            }
            
          
            if (result->leftValue.size() != result->rightValue.size()) {
                NativeAlert::show("Error in Comp Matching problem", "left/right does not match - " + TodoUtil::itos(result->leftValue.size()) + "!=" + TodoUtil::itos(result->rightValue.size()) + " ["+rawData[4]+"] - ["+rawData[5]+"]", "OK");
              
                return result;
            }
            
            while (!isWellShuffled(result->leftValue, result->rightValue))
            {
                random_shuffle(result->leftValue.begin(), result->leftValue.end());
                random_shuffle(result->rightValue.begin(), result->rightValue.end());
            }
    
            return result;
        }
        
        bool ProblemData::isWellShuffled(std::vector<std::pair<int, std::string>> leftValue, std::vector<std::pair<int, std::string>> rightValue)
        {
            bool bWellShuffled = false;
            
            // assume that leftValue and rightValue count is equal
            for (int i = 0; i < leftValue.size(); i++)
            {
                if (leftValue[i].first != rightValue[i].first)
                    bWellShuffled = true;
            }
            return bWellShuffled;
        }
    }
}
