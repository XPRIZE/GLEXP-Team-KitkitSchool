//
//  SentenceBridgeInfo.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#include "SentenceBridgeInfo.hpp"

#include "Utils/TodoUtil.h"


std::vector<std::string> SentenceBridgeInfo::enumerateLevelIDs()
{
	/*
    vector<std::string> ddLevels;
    for (int i = 1; i <= 2; i++) ddLevels.push_back(TodoUtil::itos(i));
    return ddLevels;
	*/

	string P = "Games/SentenceBridge/sentencebridge_level.tsv";
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




