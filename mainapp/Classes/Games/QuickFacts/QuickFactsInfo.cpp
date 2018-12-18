//
//  QuickFactsInfo.cpp
//  KitkitSchool
//
//  Created by Eunil Park on 18/08/18.
//

#include "QuickFactsInfo.hpp"

#include "Utils/TodoUtil.h"


std::vector<std::string> QuickFactsInfo::enumerateLevelIDs()
{
    const string resourcePath = "quickfacts/";
    
    string P = "Games/" + resourcePath + "quickfacts_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    auto data = TodoUtil::readTSV(S);

    string levelID;
    int rowIndex = 0;

    vector<std::string> ddLevels;
    ddLevels.clear();
    
    while (rowIndex < data.size()) {
        auto row = data[rowIndex++];
        
        levelID =  row[1];
        vector<std::string>::iterator iter =  find(ddLevels.begin(), ddLevels.end(), levelID);
        if(iter == ddLevels.end())
            ddLevels.push_back(levelID);
    }
    
    return ddLevels;
}




