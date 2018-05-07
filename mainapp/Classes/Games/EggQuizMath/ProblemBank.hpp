//
//  ProblemBank.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 26/03/2018.
//

#pragma once
#include "EggQuizMathHeader.h"

BEGIN_NS_EGGQUIZMATH;

struct EggQuizMathLevelStruct {
    string level;
    string levelTypeSequence;
};

struct EggQuizMathTemplateStruct {
    int type;
    string templateName;
    string questionOption1;
    string questionOption2;
    string questionOption3;
    string answerRange;
    string answerOption1;
};

class ProblemBank {
    static ProblemBank* _instance;
public:
    static ProblemBank* getInstance();
    void init();
    vector<EggQuizMathTemplateStruct> loadData(string level);
    vector<string> getLevels();
    
    string checkRandom(string buf);
    vector<string> parseFomula(string fomula);
    int executeFomula(vector<string> fomula);
    string getFomulaString(vector<string> fomula);
    string getJoinString(vector<int> numbers);
    string getJoinString(vector<string> strings);
    bool checkRegrouping(int number1, char op, int resultCandidate);
    bool hasSameAnswer(string templateName, string questionOption1, string answerRange);
    map<string,float> loadDurationsheet();

private:
    int _reviewCount = 0;
    vector<string> _levels;
    vector<EggQuizMathTemplateStruct> _problems;
    bool checkReviewFail(EggQuizMathTemplateStruct type);
};

END_NS_EGGQUIZMATH;
