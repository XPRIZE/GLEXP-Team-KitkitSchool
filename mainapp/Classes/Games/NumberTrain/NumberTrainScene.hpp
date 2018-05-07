//
//  CircusTrainScene.h
//  todomath
//
//  Created by Gunho Lee on 2017. 3. 21..
//
//

#pragma once

#include <stdio.h>
#include <time.h>
#include <vector>

#include "cocos2d.h"
#include "ui/CocosGUI.h"


#include "AnswerPiece.hpp"
#include "TrainCar.hpp"
#include "3rdParty/json/json.h"

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace std;
using namespace ui;

class NumberTrainScene : public Layer {
public:
    CREATE_FUNC(NumberTrainScene);
    static Scene* createScene(int levelID);

    NumberTrainScene();
    ~NumberTrainScene();


    void setLevel(int level);
    virtual void prepareNewGameWithLevel(Json::Value problemParameter);
    virtual void handleStart();
    virtual void handlePause();
    virtual void handleQuit();
    virtual void handleResume();
    virtual void handleAnswerEntered(std::string &answer);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void showHint();
    virtual void hideHint();
    
    void onCorrect();
    void onIncorrect();
    
private:
    void initUI();

    
    void removeObjects();
    void setProblem(int index);
    
    void trainCome(float delay);
    void trainGo(float delay);
    void answerCome(float delay);
    void answerGo();
    
    ProgressIndicator *_progressBar;
    
    void writeActionLog();
    void checkReadyToSubmit();
    void checkAnswer();
    
    
    bool _isReadyToSubmit;
    
    
    
    
    
private:
    
    Node *_top, *_bottom;
    Node *_trainGuard;
    Node *_hintNode;
    
    
    Button *_submitButton;
    
    vector<TrainCar*> _trainCars;
    vector<AnswerPiece*> _answerCards;
    
    Vec2 _guardPos;
    
    int _currentLevel;
    Json::Value _problem;
    int _totalProblems;
    int _currentProblemIndex;
    vector<int> _choices;
    vector<int> _correctAnswer;
    
};

