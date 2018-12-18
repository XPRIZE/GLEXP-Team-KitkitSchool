//
//  QuickFactsScene.hpp
//
//  Created by Eunil Park. 18/08/18
//
//

#ifndef QuickFactsScene_hpp
#define QuickFactsScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>

#include "Common/Controls/ProgressIndicator.hpp"

#define _IMAGE_COUNT_BREAK_GEM 6
#define _IMAGE_COUNT_MAX_COMBO 5 //is Equal maxComboCount

#define MAX_EXAMPLE_COUNT 15

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

enum GAUGE_STATE
{
    GAUGE_PROGRESSING = 0,
    GAUGE_HOLD,
    GAUGE_FINISH
};

struct QuickFactsLevelStruct;
struct QuizLabel;
struct QuizAnswerPiece;

class QuickFactsScene : public Layer
{
public:
    static Scene* createScene(string levelID);
    virtual bool init() override;
    CREATE_FUNC(QuickFactsScene);

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    
    virtual void update(float delta) override;
    void onStart();
    
    void setLevel(int level);
    void loadData(int level);
    void selectWorkSheet();
    
    void createQuiz();
    void createExample();
    
    void createRightEffect();
    
    void showQuiz();
    void showExample();
    void showComboGauge();
    
    void onAttachQuiz(bool isAnimated);
    void onBreakTime();
    void onNextQuiz();
    bool isExistNextQuiz();
    void onCompleteLevel(float delay);
    
    void onWrong(QuizAnswerPiece* gem);
    void onRight(QuizAnswerPiece* gem);
    
    void playSound(string name);
    void playRightEffect(int idx, QuizAnswerPiece* gem);
    
    void disappearQuiz();
    bool isRightAnswer(string number);
    
    void lifeTimeSliderEvent(Ref *pSender, Slider::EventType type);
    void timeSliderEvent(Ref *pSender, Slider::EventType type);
private:
    Node *_gameNode = nullptr;
    Node *_quizNode = nullptr;
    Node *_examNode = nullptr;
    
    int _currentLevel = 0;
    int _currentSheet = 0;
    int _currentQuizIndex = 0;
    
    int _currentCombo = 0;
    int _currentRightIndex = 0;
    
    float _comboTime = 0;
    float _comboTimeMax = 0;
    //for debug;
    int _currentSheetCount = 0; //for debug, current sheet
    Label* _worksheetLabel = nullptr;
    Label* _displayValueLabel = nullptr;
    cocos2d::ui::Slider* _lifeTimeSlider = nullptr;
    Label* _lifetTmeValueLabel = nullptr;
    cocos2d::ui::Slider* _timeSlider = nullptr;
    Label* _timeValueLabel = nullptr;
    //////////////////////////
    
    GAUGE_STATE _gaugeState = GAUGE_HOLD;
    bool _isFadingBGM = false;

    QuizAnswerPiece* _touchedGem = nullptr;
    ProgressIndicator *_progressBar = nullptr;
    
    Sprite* _gaugeSprite = nullptr;
    ClippingNode* _comboGauge = nullptr;
    LoadingBar* _gaugeStencil = nullptr;
    
    Node*         _Gauge = nullptr;
    
    ParticleSystemQuad*   _lightParticles[MAX_EXAMPLE_COUNT][_IMAGE_COUNT_MAX_COMBO];
    ParticleBatchNode*    _particleBatch[_IMAGE_COUNT_MAX_COMBO] = { nullptr, };
    vector<int> _sheetNumList;
    
    vector<QuickFactsLevelStruct*> _levelData;
    vector<QuickFactsLevelStruct*> _sheetData;
    vector<QuizLabel*> _questions;
    vector<QuizAnswerPiece*> _examples;
    
    std::map<std::string,float> _duration;

};

struct QuickFactsLevelStruct {
    int sheetNo = 0;
    int problemNo = 0;
    float comboTime = 0.f;
    string firstValue;
    string secondValue;
    string mathSign;
    string rightAnswer;
    vector<string> examples;
};

class QuizLabel : public Node {
    
public:
    
    CREATE_FUNC(QuizLabel);
    
    virtual bool init() override;
    
    void setQuizData(string strLeft, string strSign, string strRight, string strAnswer);
    void setQuizData(string quizData, string quizAnswer);
    void actionAnswer(string answer);
    
    Label* _func = nullptr;
    Label* _answer = nullptr;
    
    Label* _left = nullptr;
    Label* _sign = nullptr;
    Label* _right  = nullptr;
};

class QuizAnswerPiece : public Node {
    
public:
    CREATE_FUNC(QuizAnswerPiece);
    
    virtual bool init() override;

    void   setTexture(int combo);
    void   setNumber(string num);
    string getNumber() { return _strNum; }
    std::function<void()> onSelectGem;
    
    int _idx; //index on table;
    
    Label* _number;
    Sprite* _gemSprite;
    Sprite* _stoneSprite[_IMAGE_COUNT_BREAK_GEM];
    Sprite* _shadow;

    Sprite* _clearEffect;
    
    ProgressTimer* _offProgress;
    
    string _strNum;
    bool _touchEnabled = false;
    bool _isAnimated = false;
    
    void PlayActionRight(int combo);
    void PlayActionBreak();

};

#endif /* QuickFactsScene_hpp */
