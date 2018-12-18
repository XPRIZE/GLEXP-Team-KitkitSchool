//
//  LRDividedTypeQuestionScene.hpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#pragma once

#include "LRDividedTypeQuestionHeader.hpp"
#include "../LRComprehensionScene.hpp"

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

class LRDividedTypeQuestionScene : Layer
{
public:
    bool init() override;
    CREATE_FUNC(LRDividedTypeQuestionScene);
    static cocos2d::Layer* createLayer(QuestionType questionType, LRComprehensionScene* parent);
    LRDividedTypeQuestionScene();
    void onEnter() override;
    
    vector<pair<string, bool>> answerPairVector;
    
    bool isSolved();
    void onSolve();
    bool isCorrect(string answer);
    void solve(string answer);
    int getSolvedCount();
    //void determineItemType();
    
private:
    Node *_gameNode;
    LRComprehensionScene* _comprehensionScene;
    QuestionType _currentType;
    GuideType _guideType;
    
    Vec2 _layerPosition;
    string _questionText;
    string _questionSoundPath;
    float _questionLabelFontSize;
    string _questionImageFile;
    vector<string> _answers;
    vector<string> _solutions;
    
    string _guideSoundPath;
    string _guideText;
    
    void initData();
    void createFixedResources();
    Node* createMultiLine(string text, float fontSize, Size dim, Color4B fontColor);
    Node* createWord(string word, float fontSize, Color4B fontColor);
    string lineWrappnig(string original);
    
    Node* questionLabel;
    Vec2 _questionLabelPosition;
    
    string makeWorkPath() const;
};

END_NS_LR_DIVIDED_TYPE_QUESTION;
