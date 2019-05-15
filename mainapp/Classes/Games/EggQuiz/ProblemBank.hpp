//
//  ProblemBank.hpp
//  KitkitSchool-mobile
//
//  Created by Jaehun Jung on 29/08/2018.
//

#pragma once
#include "EggQuizHeader.hpp"

BEGIN_NS_EGGQUIZ;

struct EggQuizLevelStruct {
    string level;
    int workSheet;
    string levelTypeSequence;
};

struct EggQuizTemplateStruct {
    int type;
    string templateName;
    string questionOption1;
    string questionOption2;
    string questionOption3;
    string questionOption4;
    string questionOption5;
    string answer;
    string answerOption1;
};

class ProblemBank {
    static ProblemBank* _instance;
    
public:
    static ProblemBank* getInstance();
    void init();
    vector<string> getLevels(string course);

    vector<EggQuizTemplateStruct> loadProblemData(string course, string level, int *sheetNo);
    map<string,float> loadDurationsheet();
    float getDuration(string fileName);
    
    string checkRandom(string buf);
    vector<string> parseFomula(string fomula);
    int executeFomula(vector<string> fomula);
    string getFomulaString(vector<string> fomula);
    string getJoinString(vector<int> numbers);
    string getJoinString(vector<string> strings, string separator = ",");
    bool checkRegrouping(int number1, char op, int resultCandidate);
    bool hasSameAnswer(string templateName, string questionOption1, string answer);
    
private:
    vector<EggQuizTemplateStruct> _problems;
    map<string,float> _durationSheet;
    vector<string> _levels;
    int _reviewCount = 0;
    bool checkReviewFail(EggQuizTemplateStruct type);
};


END_NS_EGGQUIZ;
