//
//  Problem.h -- A problem for CompTrace
//  TodoSchool on Jul 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Problem.h"
#include <cocos2d.h>
#include "Utils/TodoUtil.h"

USING_NS_CC;


namespace EggQuizSceneSpace {

Problem Problem::fromInputStream(istream &IS) {
    Problem P;
    
    bool ret;
    
    ret = P.readQuestion(IS);
    if (ret) {
        ret = P.readAnswer(IS);
    }
    
    if (!ret) {
        CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    return P;
}

    
bool Problem::readQuestion(istream &IS)
{
    questionType = QuestionType_MAX;
    
    string qType;
    
    if (!(IS >> qType)) return false;
    
    
    if (qType=="SOUNDONLY") questionType = SOUNDONLY;
    else if (qType=="WORD") questionType = WORD;
    else if (qType=="SENTENCE") questionType = SENTENCE;
    else if (qType=="PARAGRAPH") questionType = PARAGRAPH;
    else if (qType=="IMAGE") questionType = IMAGE;
    else if (qType=="IMAGESEQ") questionType = IMAGESEQ;
    else if (qType=="HARDCODE") questionType = HARDCODE;
    else {
        CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
        return false;
    }
    
    string q1, q2, q3;
    string audio, question;
    
    

    {
        if (!(IS >> audio >> q1 >> q2 >> q3 >> question)) return false;
        
        questionAudio = audio;
        questionStrings.push_back(TodoUtil::trim(q1));
        questionStrings.push_back(TodoUtil::trim(q2));
        questionStrings.push_back(TodoUtil::trim(q3));
        questionQuestion = TodoUtil::trim(question);
        
    }

            
    
    
    return true;
    
}

bool Problem::readAnswer(istream &IS)
{
    answerType = AnswerType_MAX;
    
    string aType;
    
    if (!(IS >> aType)) return false;
    

    
    if (aType=="CHOOSE_3_WORD") answerType = CHOOSE_3_WORD;
    else if (aType=="CHOOSE_3_SENTENCE") answerType = CHOOSE_3_SENTENCE;
    else if (aType=="CHOOSE_3_PARAGRAPH") answerType = CHOOSE_3_PARAGRAPH;
    else if (aType=="CHOOSE_3_IMAGE") answerType = CHOOSE_3_IMAGE;
    else if (aType=="CHOOSE_4_NUMBER") answerType = CHOOSE_4_NUMBER;
    else if (aType=="NUMBERPAD") answerType = NUMBERPAD;
    else {
        CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
        return false;
    }
    
    
    string a1, a2, a3, answer;
    

    {
        if (!(IS >> a1 >> a2 >> a3 >> answer)) return false;
        answerStrings.push_back(TodoUtil::trim(a1));
        answerStrings.push_back(TodoUtil::trim(a2));
        answerStrings.push_back(TodoUtil::trim(a3));
        correctAnswerString = TodoUtil::trim(answer);
        
    }
    
    return true;
}
    
    
istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}
}; //namespace EggQuizSceneSpace {
