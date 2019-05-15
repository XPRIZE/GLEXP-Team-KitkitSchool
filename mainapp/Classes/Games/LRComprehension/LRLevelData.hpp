//
//  LRLevelData.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 8/24/18.
//

#ifndef LRLevelData_hpp
#define LRLevelData_hpp

#include "LRComprehensionHeader.hpp"

using namespace std;
using namespace LRComprehension;

class LRProblem
{
public:
    string languageTag;
    int level;
    int worksheet;
    int problem;
    //string questionType;
    LRComprehensionType questionType;
    string script;
    string question;
    string audio;
    string answerType;
    string option1;
    string option2;
    string option3;
    string option4;
    string answer;
};

class LRLevelData
{
public:
    enum class Col
    {
        LanguageTag = 0,
        Level,
        WorkSheet,
        Problem,
        QuestionType,
        Script,
        Question,
        Audio,
        AnswerType,
        Option1,
        Option2,
        Option3,
        Option4,
        Answer,
    };
    
    vector<LRProblem*> problems;

    static LRLevelData* parse();
    vector<LRProblem*> getRandomProblemsByLevel(int level);
    static LRComprehensionType getProblemType(string rowValue);
};

#endif /* ProblemData_hpp */
