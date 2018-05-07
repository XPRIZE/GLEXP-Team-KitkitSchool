//
//  SoundTrainScene.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 30/08/2017.
//
//

#pragma once
#include "SoundTrainHeader.h"
#include "SoundCard.hpp"
#include "SoundTrain.hpp"
#include "SoundTrainProblemBank.hpp"

#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_SOUNDTRAIN;

class SoundTrainScene : public Layer {
public:
    CREATE_FUNC(SoundTrainScene);
    static Scene* createScene(int levelID);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    void setProblem();
    void trainCome();
    void trainJump();
    void trainCombine();
    void toggleConnectors();
    void trainGo();
    void cardCome();
    void cardGo();
    void onCorrect();
    void onComplete();
    void onIncorrect();
    void setLevel(int level);

    Node *_gameNode;

private:
    void initUI();
    int _currentLevel, _currentProblemIndex, _totalProblemCount;

    Size _winSize, _gameSize;
    Sprite *_locomotive, *_lionFront, *_lionBack, *_steam, *_soundBubble, *_soundButtonNormal, *_soundButtonActive;
    vector<SoundTrainLevelStruct> _problems;
    vector<SoundTrain*> _freightTrains;
    vector<SoundCard*> _cards;
    vector<string> _questions, _answers;
    string _trainName, _language;

    ProgressIndicator *_progressBar;

    void checkReadyToSubmit();
    void checkAnswer();
    void removeObjects();
    void lionMoveToggle();
    void soundButtonFadeIn();
    void soundButtonFadeOut();
    void soundButtonSpeech();
    void playSound(string letter);

    float getLocomotiveStartX();
    float getLocomotiveArriveX();
    float getTrainStartX(int index);
    float getTrainArriveX(int index);
    float getTrainSeq(int index);
    float getMoveTime(float px);

};

END_NS_SOUNDTRAIN;
