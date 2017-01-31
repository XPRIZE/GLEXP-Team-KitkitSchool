//
//  DailyData.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/3/16.
//
//

#include "DailyData.hpp"
#include <Common/Basic/CommentStream.h>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

size_t DailyData::size() const {
    return dailyData.size();
}

DailyData DailyData::defaultData()
{
    DailyData It;
    
    string filePath = "DailyData.tsv";
    string data = FileUtils::getInstance()->getStringFromFile(filePath);
    
    todoschool::CommentStream IS(data);
    IS >> It;
    
    return It;
    
    
}

std::istream& operator>>(std::istream& IS, DailyData& DD)
{
    std::string category;
    int day;
    
    while (IS >> category >> day) {
        auto key = DailyData::DailyDataKey(category, day);
        auto& D = DD.dailyData[key];
        
        int step;
        std::string gameName;
        int level;
        
        IS >> step >> gameName >> level;
        
        D.push_back(DailyData::DailyGameData(gameName, level));
        
    }
    
    return IS;
}

DailyData::DailyGameArray DailyData::getDailyGameArray(DailyDataKey key)
{
    return dailyData[key];
}

