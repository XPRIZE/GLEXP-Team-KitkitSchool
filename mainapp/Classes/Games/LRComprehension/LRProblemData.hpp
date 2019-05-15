//
//  LRProblemData.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 8/24/18.
//

#ifndef LRProblemData_hpp
#define LRProblemData_hpp

#include "LRComprehensionHeader.hpp"

using namespace std;
using namespace LRComprehension;

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
        AnswerCount,
        Option1,
        Option2,
        Option3,
        Option4,
        Answer,
    };
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
    int answerCount;
    string option1;
    string option2;
    string option3;
    string option4;
    string answer;

    static vector<LRLevelData*> parse();
    static LRLevelData* getProblem(vector<LRLevelData*> problems, int level);
    static LRComprehensionType getProblemType(string rowValue);
};

#endif /* ProblemData_hpp */
