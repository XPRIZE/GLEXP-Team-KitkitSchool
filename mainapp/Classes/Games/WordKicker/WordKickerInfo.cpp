//
//  WordKickerInfo.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/14/18.
//

#include "WordKickerInfo.hpp"

#include "Utils/TodoUtil.h"


std::vector<std::string> WordKickerInfo::enumerateLevelIDs()
{
	/*
    vector<std::string> ddLevels;
    for (int i = 1; i <= 2; i++) ddLevels.push_back(TodoUtil::itos(i));
    return ddLevels;
	*/

	string P = "Games/WordKicker/wordkicker_level.tsv";
	string S = FileUtils::getInstance()->getStringFromFile(P);

	auto data = TodoUtil::readTSV(S);
	std::string tsv_Level;
	vector<std::string> ddLevels;
	ddLevels.clear();
	for (auto row : data)
	{
		if (row.size() < 1) continue;
		if (TodoUtil::trim(row[0]).size() <= 0) continue;
		if (row[0][0] == '#') continue;

		tsv_Level = row[1];
		if (std::find(ddLevels.begin(), ddLevels.end(), tsv_Level) == ddLevels.end())
		{
			ddLevels.push_back(tsv_Level);
		}
	}
	return ddLevels;
}


std::vector<std::string> MathKickerInfo::enumerateLevelIDs()
{
    /*
     vector<std::string> ddLevels;
     for (int i = 1; i <= 2; i++) ddLevels.push_back(TodoUtil::itos(i));
     return ddLevels;
     */
    
    string P = "Games/mathKicker/mathkicker_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    auto data = TodoUtil::readTSV(S);
    std::string tsv_Level;
    vector<std::string> ddLevels;
    ddLevels.clear();
    for (auto row : data)
    {
        if (row.size() < 1) continue;
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row[0][0] == '#') continue;
        
        tsv_Level = row[1];
        if (std::find(ddLevels.begin(), ddLevels.end(), tsv_Level) == ddLevels.end())
        {
            ddLevels.push_back(tsv_Level);
        }
    }
    return ddLevels;
}


