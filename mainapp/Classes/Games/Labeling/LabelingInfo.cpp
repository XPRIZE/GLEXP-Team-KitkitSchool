//
//  LabelingInfo.cpp
//  KitkitSchool
//
//  Created by Seungbum Son on 9/5/18.
//

#include <algorithm>

#include "LabelingInfo.hpp"

#include "Utils/TodoUtil.h"
#include "Managers/LanguageManager.hpp"


std::vector<std::string> LabelingInfo::enumerateLevelIDs()
{
    vector<std::string> ddLevels;
    
    string P = "Games/Labeling/labeling_levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    auto data = TodoUtil::readTSV(S);
    
    string LanguageTag;
    string levelID;
    
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    int rowIndex = 0;
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[rowIndex++];
        LanguageTag = row[0];
        
        if (LanguageTag != Lang.c_str())
            continue;
        
        levelID = row[1];
        
        if (std::find(ddLevels.begin(), ddLevels.end(), levelID) == ddLevels.end())
            ddLevels.push_back(levelID);
    }
    return ddLevels;
}




