//
//  ProblemBank.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 09/03/2018.
//

#include "ThirtyPuzzleProblemBank.hpp"

BEGIN_NS_THIRTYPUZZLE;

ProblemBank* ProblemBank::_instance = 0;

ProblemBank* ProblemBank::getInstance()
{
    if(!_instance) {
        _instance = new ProblemBank();
        _instance->init();
    }
    return _instance;
}

void ProblemBank::init() {
    _maxLevel = 0;
    _problemCount = 0;
}

vector<int> ProblemBank::getLevels() {
    loadData(1);
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}

vector<ThirtyPuzzleLevelStruct> ProblemBank::loadData(int level)
{
    bool isRandom = false;
    
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("ThirtyPuzzle/ThirtyPuzzle_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    vector<ThirtyPuzzleLevelStruct> problems;
    problems.clear();
    
    for (auto row : data) {
        if (row.size() < 1) continue;
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row[0][0] == '#') continue;
        for (auto cell : row) cell = TodoUtil::removeSpaces(TodoUtil::trim(cell));
        
        int rawLevel = TodoUtil::stoi(row[0]);
        int rawProblem = TodoUtil::stoi(row[1]);
        string rawColumns = row[2];
        string rawSuggest = row[3];
        string rawIsRandom = row[4];
        int rawProblemNumber = TodoUtil::stoi(row[5]);

        if (_maxLevel < rawLevel) _maxLevel = rawLevel;

        if (rawLevel != level) continue;
        
        isRandom = rawIsRandom == "Y" ? true : false;

        _problemCount = rawProblemNumber;

        problems.push_back({
            rawLevel,
            rawProblem,
            rawColumns,
            rawSuggest,
            rawIsRandom
        });
        
    }
    
    if (isRandom) random_shuffle(problems.begin(), problems.end(), [](int n) { return rand() % n; });
    problems.erase(problems.begin()+_problemCount, problems.end());
    
    return problems;
}



END_NS_THIRTYPUZZLE;
