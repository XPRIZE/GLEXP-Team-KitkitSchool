//
//  EggQuizScene.hpp
//
//  Created by Gunho Lee on 1/1/17.
//
//

#ifndef EggQuizScene_hpp
#define EggQuizScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <functional>

#include "Common/Controls/ProgressIndicator.hpp"

#include "Problem.h"
#include "LevelData.h"

USING_NS_CC;
using namespace cocos2d::ui;


using namespace EggQuizSceneSpace;

class EggQuizScene : public Layer
{
public:
    static Scene* createScene(int level);
    static Scene* createScene(char category, int level, bool isPreTest = false);
    virtual bool init() override;
    CREATE_FUNC(EggQuizScene);

    void setLevel(char category, int level, bool isPreTest);
    
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();
    
    void onSolve();
    void onCorrect(const std::string& answer, int answerIndex);
    void onIncorrect(const std::string& correctAnswer, const string& myAnswer, int answerIndex);
    
    void onComplete();


    std::function<void(bool pass)> onCompleteCallback;
    

private:
    
    bool _isPreTest;

    LevelData _levelData;
    std::vector<Problem> _problemSet;
    std::vector<bool> _answerResult;

    
    
    NodeGrid* createPageGrid();
    
    
    
    Node* createLiteracyQuestionView();
    Node* createLiteracyAnswerView();
    

    
    Node* createMathView();
    
    Node* createMathQuestionView();
    
    Node* createMathCountingView(int num, int step);
    Node* createMathMissingView(int num, int step, bool forLast);
    Node* createTenframeView(int a, int b);
    Node* createExpressView(int a, int b, string op);
    Node* createSmallExpressView(string left, string right);
    Node* createVerticalExpressView(int a, int b, string op);
    
    
    Node* createMathAnswerView(AnswerType type, int answer, vector<int>& choices);
    
    
    Node* createMultiLine(string text, float fontSize, Size dim);
    Node* createSingleLine(string text, float fontSize, Size dim);
    
    
    void showProblem();
    
    void confirmQuit();
    void passTest();
    void failTest();
    
    
    
    
    
    Node *_pageNode;
    NodeGrid *_pageGrid;
    int _currentPageGridZ;
    Node *_gameNode;
    
    
    
    char _quizCategory;
    int _currentLevel;
    int _currentProblemIndex;
    Problem _currentProblemObject;
    
    vector<int> _currentAnswerOrder;
    string _answerString;
    int _answer;
    
    
    
    ProgressIndicator *_progressBar;
};


#endif /* EggQuizScene_hpp */
