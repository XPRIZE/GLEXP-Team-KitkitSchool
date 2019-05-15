//
//  AnimalPuzzleInfo.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 7/14/18.
//

#include "AnimalPuzzleInfo.hpp"

#include "Utils/TodoUtil.h"

std::vector<std::string> AnimalPuzzleInfo::enumerateLevelIDs()
{
    string P = "Games/AnimalPuzzle/AnimalPuzzle_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    int pieceNum;

    auto data = TodoUtil::readTSV(S);
    std::string tsv_Level;
    vector<std::string> ddLevels;
    ddLevels.clear();

    int rowIndex = 0;
    while (rowIndex < data.size()) {
        auto row = data[rowIndex++];

        tsv_Level = row[1];
        pieceNum = TodoUtil::stoi(row[6]);

        if (std::find(ddLevels.begin(), ddLevels.end(), tsv_Level) == ddLevels.end())
        {
            ddLevels.push_back(tsv_Level);
        }

        rowIndex+=pieceNum;

    }

    return ddLevels;
}




