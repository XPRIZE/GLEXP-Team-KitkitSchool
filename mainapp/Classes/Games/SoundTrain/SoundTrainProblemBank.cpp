//
//  SoundTrainProblemBank.cpp
//  KitkitSchool
//
//  Created by JungJaehun on 29/08/2017.
//
//

#include "SoundTrainProblemBank.hpp"

BEGIN_NS_SOUNDTRAIN;

Json::StyledWriter writer;

SoundTrainProblemBank::SoundTrainProblemBank() {
    
    
}

string getJoinString(vector<string> letters) {
    string rt = "";
    for (auto it : letters) {
        if (rt != "") rt += ",";
        rt += it;
    }
    return rt;
}

vector<int> SoundTrainProblemBank::getLevels() {
    loadData(1);
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}


vector<SoundTrainLevelStruct> SoundTrainProblemBank::loadData(int level) {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("SoundTrain/SoundTrain_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    vector<SoundTrainLevelStruct> problems;
    problems.clear();
    int problemCount;
    
    for (auto row : data) {
        if (row.size() < 2) continue;
        if (row[0][0] == '#') continue;
        bool blankCheck = true;
        for (int i=0; i<row.size(); i++) {
            row[i] = TodoUtil::removeSpaces(TodoUtil::trim(row[i]));
            transform(row[i].begin(), row[i].end(), row[i].begin(), ::tolower);
            if (row[i] != "") blankCheck = false;
        }
        if (blankCheck) continue;
        
        int rawLevel = TodoUtil::stoi(row[0]);
        int rawProblemCount = TodoUtil::stoi(row[1]);
        //int rawProblem = TodoUtil::stoi(row[2]);
        vector<string> words;
        
        if (row[4] != "") words.push_back(row[4]);
        if (row[5] != "") words.push_back(row[5]);
        if (row[6] != "") words.push_back(row[6]);
        
        if (_maxLevel < rawLevel) _maxLevel = rawLevel;
        if (rawLevel != level) continue;
        problemCount = rawProblemCount;
        
        problems.push_back({rawLevel, getJoinString(words)});
        
    }
    
    random_shuffle(problems.begin(), problems.end(), [](int n) { return rand() % n; });
    problems.erase(problems.begin()+problemCount, problems.end());
    
    return problems;

}

Json::Value SoundTrainProblemBank::generateProblems(int level) {
    Json::Value problems;
    
    string filepath = LanguageManager::getInstance()->findLocalizedResource("SoundTrain/soundtrain.json");
    string jsonStr = FileUtils::getInstance()->getStringFromFile(filepath);
    
    Json::Value jsonValue;
    Json::Value jsonValueByLevel;
    Json::Value words(Json::arrayValue);
    Json::Reader jsonReader;
    if (!jsonReader.parse(jsonStr, jsonValue)) {
        jsonValue.clear();
    }
    
    //CCLOG("jsonvalue: %s", jsonStr.c_str());
    //CCLOG("size: %d", jsonValue.size());
    
    jsonValueByLevel = jsonValue[TodoUtil::itos(level)];
    vector<string> sampleVec;
    for (int i=0; i<jsonValueByLevel.size(); i++) {
        sampleVec.push_back(jsonValueByLevel[i].asString());
    }
    random_shuffle(sampleVec.begin(), sampleVec.end(), [](int n) { return rand() % n; }); for (int i=0; i<5; i++) { words.append(sampleVec[i]); }

    problems["words"] = words;
    
    CCLOG("%s", writer.write(problems).c_str());
    
    return problems;
}


END_NS_SOUNDTRAIN;
