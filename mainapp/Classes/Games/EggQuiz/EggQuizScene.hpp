//
//  EggQuizScene.hpp
//  KitkitSchool
//
//  Created by Jaehun Jung on 29/08/2018.
//

#pragma once
#include "EggQuizHeader.hpp"
#include "ProblemBank.hpp"

#include "Common/Controls/ProgressIndicator.hpp"

BEGIN_NS_EGGQUIZ;

class Slot;
class EggQuizScene : public Layer {
public:
    
    CREATE_FUNC(EggQuizScene);
    static Scene* createScene(string course, string level);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    void setCourse(string course);
    void setLevel(string level);
    void setParam(string param);
    
    static void resetTryCountFishTest();
    static void increaseTryCountFishTest();
    static int getTryCountFishTest();
    
private:
    Node *_gameNode;
    Size _winSize, _gameSize;
    float _gameScale;
    
    int _currentProblemIndex, _totalProblemCount;
    std::vector<bool> _answerResult;

    string _course, _level;
    int _sheetNo;
    vector<EggQuizTemplateStruct> _problems;
    EggQuizTemplateStruct _problem;
    string _answer, _padNumber;
    int _answerInt;
    
    vector<Slot*> _slots;

    ProgressIndicator *_progressBar;
    void updateProgressBar(bool setCurrent = false);

    void showProblem();
    void checkAnswer(string answer = "");
    void onCorrect();
    void onIncorrect();
    void onComplete();
    void completedFishTest(int score);
    void goNextProblem();
    void checkDragButton();
    void checkDragButtonOver();
    void checkSlotFull(SlotType type = SlotType::Normal);
    void setPressDisabledAll(Node* node);
    float getMinFontSize(vector<string> labels, float fontSize, Size boxSize);
    
    void confirmAnswer(function<void()> onCompleteCallback);
    void fadeInAll(Node* node);
    void fadeOutAll(Node* node, function<void()> onCompleteCallback);
    void setOpacityEnableAll();
    
    void createHeaderView(bool finished = false);

    // math template
    void createSingleDigitNumbersView();
    void createRecognizeNumberView();
    void create2DigitNumbersView();
    void createBiggerAndSmallerView();
    void createMissingNumberView();
    void createOperationsWithObjectsView();
    void createBiggestAndSmallestView();
    void createCompareNumberMagnitudesView();
    void createOperationsWithoutObjectsView();
    void createUseMagnitudeSymbolsView();
    void createShapesView();
    void create3DigitNumbersView();
    void createNumberIdentificationDragView();
    void createMissingNumberDragView();
    void createOperationsDragView();
    void createWordProblemView();
    
    // literacy template
    void createSoundonlyWordView();
    void createWordWordView();
    void createImageWordView();
    void createSentenceWordView();
    void createSentenceSentenceView();
    void createImageSentenceView();
    void createSoundonlySentenceView();
    void createParagraphSentenceView();
    void createImageseqSentenceView();
    void createImageseqImageView();
    void createOrderingSentenceView();
    void createListeningcompSentenceView();
    
    // sub template
    void createPushButton09Template();
    void createDragButton09Template();
    void createAnswerNumberPadView();
    void createPushButtonC3WTemplate(float fontSize=152.f);
    void createPushButtonC3STemplate(float fontSize=100.f);
    void createSequenceTemplate(vector<string> sequenceV, float posY);
    void createSmallSpeakerView(string fileName, Vec2 position = Vec2(0,0));

};


END_NS_EGGQUIZ;
