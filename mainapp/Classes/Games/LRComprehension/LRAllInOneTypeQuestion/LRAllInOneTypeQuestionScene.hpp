//
//  LRAllInOneTypeQuestionScene.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 14/08/2018.
//

#pragma once

#include "LRAllInOneTypeQuestionHeader.hpp"
#include "../LRComprehensionScene.hpp"

BEGIN_NS_LR_ALLINONE_TYPE_QUESTION;

class LRAllInOneTypeQuestionScene : Layer
{
public:
    bool init() override;
    CREATE_FUNC(LRAllInOneTypeQuestionScene);
    static cocos2d::Layer* createLayer(QuestionType questionType, LRComprehensionScene* parent);
    LRAllInOneTypeQuestionScene();
    void onEnter() override;
    
    vector<pair<string, bool>> answerPairVector;
    
    bool isSolved();
    void onSolve();
    bool isCorrect(string answer);
    void solve(string answer);
    int getSolvedCount();
    
private:
    Node *_gameNode;
    LRComprehensionScene* _comprehensionScene;
    QuestionType _questionType;
    
    Vec2 _layerPosition;
    string _questionText;
    string _questionSoundPath;
    string _guideTextOrSoundPath;
    float _questionLabelFontSize;
    string _questionImageFile;
    vector<string> _answers;
    vector<string> _solutions;
    
    void initData();
    void createFixedResources();
    Node* createMultiLine(string text, float fontSize, Size dim, Color4B fontColor);
    Node* createWord(string word, float fontSize, Color4B fontColor);
    string lineWrappnig(string original);
    
    Node* questionLabel;
    Vec2 _questionLabelPosition;
    
    std::string makeWorkPath() const;
};

END_NS_LR_ALLINONE_TYPE_QUESTION;
