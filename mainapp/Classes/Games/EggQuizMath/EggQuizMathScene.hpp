//
//  EggQuizMathScene.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 26/03/2018.
//

#pragma once
#include "EggQuizMathHeader.h"
#include "ProblemBank.hpp"
#include "EggQuizMathButton.hpp"
#include "../EggQuizLiteracy/EggQuizPopup.hpp"

#include "Common/Controls/ProgressIndicator.hpp"


BEGIN_NS_EGGQUIZMATH;

class EggQuizMathScene : public Layer {
public:
    CREATE_FUNC(EggQuizMathScene);
    static Scene* createScene(string level);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    void setLevel(string level);
    void onSolve();
    void onCorrect(const std::string& answer, int answerIndex);
    void onIncorrect(const std::string& correctAnswer, const string& myAnswer, int answerIndex);
    void onComplete();
    std::function<void(bool pass)> onCompleteCallback;

private:
    void initUI();
    void initTextMap();
    void showProblem();
    void updateProgressBar(bool setCurrent = false);

    void passTest();
    void failTest();
    
    string _currentLevel;
    int _currentProblemIndex, _totalProblemCount;
    Node *_gameNode, *_pageNode, *_problemNode;
    Sprite *_paperTop;
    NodeGrid *_pageGrid;
    int _currentPageGridZ;
    bool _isPreTest;

    Size _winSize, _gameSize;
    float _gameScale;
    string _answer;
    string _answerString;
    int _answerInt;
    vector<EggQuizMathButton*> _buttons;
    vector<EggQuizMathTemplateStruct> _problems;
    EggQuizMathTemplateStruct _problem;
    std::vector<bool> _answerResult;
    map<string,float> _duration;
    
    string getText(string key);

    Node* createSingleDigitNumbersView();
    Node* create2DigitNumbersView();
    Node* create3DigitNumbersView();
    Node* createRecognizeNumberView();
    Node* createMissingNumberView();
    Node* createOperationsWithObjects();
    Node* createOperationsWithoutObjects();
    Node* createBiggestAndSmallest();
    Node* createBiggerAndSmaller();
    void biggerAndSmallerAddListenerToButton(Scale9Sprite* button, string answer);
    
    Node* createCompareNumberMagnitudes();
    Node* createUseMagnitudeSymbols();
    Node* createShapes();
    Node* createSoundButton(string fileName, float duration = 1, float size = 100.f, bool autoStart = false);

    vector<Sprite*> _slots;
    vector<Node*> _cards;
    int _maxCardIndex = 0;

    Node* createAnswerMultipleChoiceView(vector<string>& choices, vector<string> paths = {}, int choiceType = 0);
    Node* createAnswerNumberPadView();

    ProgressIndicator *_progressBar;
};

END_NS_EGGQUIZMATH;

