//
//  LineMatchingProblemBank.cpp
//  todomath
//
//  Created by KimDonggyu on 2016. 5. 4..
//
//

#include "LineMatchingProblemBank.h"

Json::Value LineMatchingProblemBank::generateParameters(int level) {
    char buf[128];
    sprintf(buf, "LineMatching/Problems/LM_%d.json", level);
    //    sprintf(buf, "LineMatching/Problems/LM_1.json");
    //    sprintf(buf, "LineMatching/Problems/LM_2.json");
    std::string problemsString = cocos2d::FileUtils::getInstance()->getStringFromFile(buf);
    
    if(problemsString.empty() == false){
        Json::Value root;
        Json::Reader reader;
        
        if(reader.parse(problemsString, root)){
            return root;
        }
    }
    return false;
}
