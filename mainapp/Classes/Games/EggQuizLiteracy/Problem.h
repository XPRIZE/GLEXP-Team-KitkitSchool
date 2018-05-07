//
//  Problem.h -- A problem for COMPTrace
//  TodoSchool on Jul 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <istream>
#include <vector>
#include <string>

using namespace std;

namespace EggQuizSceneSpace {

    enum QuestionType {
        SOUNDONLY,
        WORD,
        SENTENCE,
        PARAGRAPH,
        IMAGE,
        IMAGESEQ,
        HARDCODE, 
        QuestionType_MAX,
    };
    
    enum AnswerType {
        CHOOSE_3_WORD,
        CHOOSE_3_SENTENCE,
        CHOOSE_3_PARAGRAPH,
        CHOOSE_3_IMAGE,
        CHOOSE_4_NUMBER,
        NUMBERPAD,
        AnswerType_MAX,
    };
    
class Problem {
public:
    // NB(xenosoz, 2016): ProblemID is given from the external world
    //   *after* the instance is created.

    int problemNo;
    
    QuestionType questionType;
    vector<string> questionStrings;
    string questionAudio;
    string questionQuestion;
    
    
    AnswerType answerType;
    vector<string> answerStrings;
    string correctAnswerString;
    
    

    
public:
    static Problem fromInputStream(istream& IS);
    
    bool readQuestion(istream& IS);
    bool readAnswer(istream& IS);
    
    
    friend istream& operator>>(istream& IS, Problem& P);
};

}; // namespace EggQuizSceneSpace

