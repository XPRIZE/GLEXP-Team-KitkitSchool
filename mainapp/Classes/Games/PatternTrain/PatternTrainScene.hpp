//
//  PatternTrainScene.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "PatternTrainHeader.h"
#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_PATTERNTRAIN;

class PatternTrainProblemBank;
class FreightCar;
class Box;
class Slot;
class Locomotive;
class PatternTrainScene : public Layer {
public:
    CREATE_FUNC(PatternTrainScene);
    static Scene* createScene(int levelID);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    void setProblem();
    void trainCome();
    void answerCome();
    void onCorrect();
    void onComplete();
    void onIncorrect();
    void onBack();
    void setLevel(int level);
    
private:
    void initUI();
    
    PatternTrainProblemBank *_problemBank;
    int _currentLevel, _currentProblemIndex, _totalProblemCount;
    ProgressIndicator *_progressBar;

    bool _reservedBoxWork ;
    string _reservedLetter;
    Vec2 _reservedNewBoxPos;
    Box *_reservedOriginBox, *_tutorialBox;
    void generateNewAnswerBox();

    Node *_gameNode;
    Vector<Node*>_AnswerAreas;
    Size _winSize, _gameSize;
    Json::Value _problem;
    vector<string> _pattern;
    vector<FreightCar*> _freightCars;
    vector<Box*> _boxes;
    vector<float> _freightCarPos;
    float _totalWidth;
    
    vector<Box*> _reservedAnswerA;
    vector<Box*> _reservedAnswerB;
    vector<Box*> _reservedAnswerC;
    
    int _shape, _choice, _blank, _freightCarCount;
    
    map<string, string> _selectedBox;
    //vector<SoundTrain*> _freight;
    //vector<SoundCard*> _cards;
    // vector<string> _questions, _answers;
    // string _trainName;

    
    void checkReadyToSubmit();
    void checkAnswer();
    void removeObjects();
    Slot* replaceableSlotNearByBox(Box *box);
    Node* answerAreaCannotCatchBox();
    Box* createAnswerBox(string letter, Vec2 pos, bool setVisible = true);
    Locomotive* _locomotive;
    
    void duplicateAnswerBox(Node *answerArea);
    
    bool _duplicateFlag;
    
    float getLocomotiveStartX();
    float getLocomotiveArriveX();
    float getFreightCarStartX(int index);
    float getFreightCarArriveX(int index);
    float getFreightCarSeq(int index);
    float getMoveTime(float px);
    
    void trainJump();
    void trainCombine();
    void trainGo();
    void answerGo();


};

END_NS_PATTERNTRAIN;
