//
//  PlaceValueInfo.cpp
//  KitkitSchool
//
//  Created by harunom on 8/31/18.
//

#include "PlaceValueInfo.hpp"

#include "Utils/TodoUtil.h"


std::vector<std::string> PlaceValueInfo::enumerateLevelIDs()
{
    vector<std::string> ddLevels;
    
    //tsv 데이터 파일에서 제일 높은 레벨을 가져온다.
    int maxLevel = getMaxLevel();
    
    //레벨 추가
    for (int i = 1; i <= maxLevel; i++) ddLevels.push_back(TodoUtil::itos(i));
    
    return ddLevels;
}

int PlaceValueInfo::getMaxLevel()
{
    string P = "Games/PlaceValue/placevalue_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    auto data = TodoUtil::readTSV(S);
    
    int rowIndex = 0;
    int maxLevel = 1;
    
    while (rowIndex < data.size())
    {
        int level = TodoUtil::stoi(data[rowIndex++][1]);
        
        if (level > maxLevel)
            maxLevel = level;
    }
    
    return maxLevel;
}


