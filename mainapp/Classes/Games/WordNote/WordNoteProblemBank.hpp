//
//  ProblemBank.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 09/03/2018.
//
#pragma once
#include "WordNoteHeader.h"

BEGIN_NS_WORDNOTE;

struct WordNoteLevelStruct {
    string languageTag;
    int level;
    int worksheet;
    int problem;
    string wordSentence;
    string card;
    string activeCard;
    string sound;
    string image;
    bool hintWordSentence;
    bool canDuplicate;
};

struct SoundDuration {
    string word;
    float duration;
};

class ProblemBank {
    static ProblemBank* _instance;
public:
    static ProblemBank* getInstance();
    void init();
    vector<WordNoteLevelStruct> loadData(int level, int *currentWorkSheet);
    vector<SoundDuration> loadSoundDurationSheet();
    vector<int> getLevels();
private:
    int _maxLevel;

};

END_NS_WORDNOTE;
