//
//  ProblemBank.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 09/03/2018.
//

#include "WordNoteProblemBank.hpp"
#include "3rdParty/CCNativeAlert.h"

BEGIN_NS_WORDNOTE;

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
}

vector<int> ProblemBank::getLevels() {
    int workSheet;
    loadData(1, &workSheet);
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}

vector<WordNoteLevelStruct> ProblemBank::loadData(int level, int *currentWorkSheet) {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("WordNote/WordNote_Levels.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    vector<WordNoteLevelStruct> problemsAllWorkSheet, problems;
    problemsAllWorkSheet.clear();
    problems.clear();
    
    int maxWorkSheet = 0;
    
    for (auto row : data) {
        if (row.size() < 1) continue;
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row[0][0] == '#') continue;
        
        for (int i=0; i<row.size(); i++) {
            //row[i] = TodoUtil::removeSpaces(TodoUtil::trim(row[i]));
            TodoUtil::replaceAll(row[i], "\"", "");
        }
        // if (row.size() != 10) { string rows = ""; for (auto word : row) rows += word + " "; NativeAlert::show("undefined row in tsv file", rows, "OK"); }
        string rawLanguageTag = row[0];
        int rawLevel = TodoUtil::stoi(row[1]);
        int rawWorksheet = TodoUtil::stoi(row[2]);
        int rawProblem = TodoUtil::stoi(row[3]);
        string rawWordSentence = row[4];
        string rawCard = row[5];
        string rawActiveCard = row[6];
        string rawSound = row[7];
        string rawImage = row[8];
        string rawHintWordSentence = row[9];
        string rawCanDuplicate = row[10];
        if (_maxLevel < rawLevel) _maxLevel = rawLevel;
        
        if (rawLevel != level) continue;
        
        if (maxWorkSheet < rawWorksheet) {
            maxWorkSheet = rawWorksheet;
        }
        
        problemsAllWorkSheet.push_back({
            rawLanguageTag,
            rawLevel,
            rawWorksheet,
            rawProblem,
            rawWordSentence,
            rawCard,
            rawActiveCard,
            rawSound,
            rawImage,
            rawHintWordSentence == "TRUE" ? true : false,
            rawCanDuplicate == "TRUE" ? true : false
        });
        
    }
    *currentWorkSheet = random<int>(1, maxWorkSheet);
    
    for (auto problem : problemsAllWorkSheet) {
        if (*currentWorkSheet != problem.worksheet) continue;
        problems.push_back(problem);
    }
    
    return problems;
    
}

bool sortWordNoteSoundSheet(const SoundDuration &i, const SoundDuration &j) { return (i.word.size() > j.word.size()); };

vector<SoundDuration> ProblemBank::loadSoundDurationSheet() {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("WordNote/Sounds/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    vector<SoundDuration> soundDurationSheet;
    soundDurationSheet.clear();
    
    for (auto row : data) {
        if (row.size() <= 1) continue;
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row[0][0] == '#') continue;
        
        auto row0Vec = TodoUtil::split(row[0], '.');
        string rowWord = row0Vec[0];
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        soundDurationSheet.push_back({
            rowWord,
            rowDuration
        });
    }
    
    sort(soundDurationSheet.begin(), soundDurationSheet.end(), sortWordNoteSoundSheet);
    soundDurationSheet.push_back({" ", (float)0.3});
    
    return soundDurationSheet;
}

END_NS_WORDNOTE;
