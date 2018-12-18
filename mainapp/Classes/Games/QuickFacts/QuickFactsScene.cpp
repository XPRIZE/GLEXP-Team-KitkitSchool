//
//  QuickFactsScene.cpp
//
//  Created by Eunil Park on 18/08/18.
//
//


#include "QuickFactsScene.hpp"
#include "Common/Basic/SoundEffect.h"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"

#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;

namespace QuickFactsSceneSpace {
    const char* defaultFont = "fonts/andikanewbasic-b.ttf";
    
    const string resourcePath = "quickfacts/";
    const string BGMPath = "games/quickfacts/sounds/kitkit_bgm_quickfacts.m4a";
    const string rightSFXPath = "games/quickfacts/sounds/correct_";
    const string wrongSFXPath = "games/quickfacts/sounds/wrong.m4a";
    const string popGemSFXPath = "games/quickfacts/sounds/create_gem.m4a";
    
    const Size gameSize = Size(2560, 1800);
    const Size quizSize = Size(852, 183); //is comboGauge size
    
//
    SoundEffect rightEffect(int combo) {
        return SoundEffect(rightSFXPath + TodoUtil::itos(combo) + ".m4a"); }

    SoundEffect wrongEffect() { return SoundEffect(wrongSFXPath); }
    SoundEffect showGemEffect() { return SoundEffect(popGemSFXPath); }

    const string fontName = "fonts/andika-r.ttf";
    const string fontBoldName = "fonts/andikanewbasic-b.ttf";
    
    const int maxAnswerCount = 15;
    const int maxComboCount = 5;
    
    Color3B quizColorBackGauge =  Color3B(243, 197, 72);
    Color3B quizColorFrontGauge = Color3B(0, 0, 0);
    
    int GEM_PADDING_ROW = 71;
    int GEM_PADDING_COL = 51;
    int fadeLevelForQuiz[6] = { 80, 75, 70, 65, 60, 50};
    int MAX_QUIZ_LINE = 6;
    
    ActionInterval* getActionAppear(float delay, float time);
    ActionInterval* getActionDisAppear(float delay, float time);
    ActionInterval* getActionShake(float delay, float tension);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////action templete
/////////////////////////////////////////////////////////////////////////////////////////////////////////

ActionInterval* QuickFactsSceneSpace::getActionAppear(float delay, float time)
{
    ActionInterval* action = Sequence::create(DelayTime::create(delay), EaseBounceOut::create(ScaleTo::create(time, 1)), nullptr);
    return action;
}

ActionInterval* QuickFactsSceneSpace::getActionDisAppear(float delay, float time)
{
    ActionInterval* action = Sequence::create(DelayTime::create(delay), FadeOut::create(time), nullptr);
    return action;
}

ActionInterval* QuickFactsSceneSpace::getActionShake(float delay, float tension)
{
    ActionInterval* action = Sequence::create(DelayTime::create(delay),
                                              MoveBy::create(0.025f, Vec2(-tension, 0)),
                                              MoveBy::create(0.025f, Vec2(tension, 0)),
                                              MoveBy::create(0.025f, Vec2(tension, 0)),
                                              MoveBy::create(0.025f, Vec2(-tension, 0)),
                                              MoveBy::create(0.025f, Vec2(-tension, 0)),
                                              MoveBy::create(0.025f, Vec2(tension, 0)),
                                              MoveBy::create(0.025f, Vec2(tension, 0)),
                                              MoveBy::create(0.025f, Vec2(-tension, 0)),
                                              nullptr);
    return action;
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace QuickFactsSceneSpace;

Scene* QuickFactsScene::createScene(string levelID)
{
    auto level = TodoUtil::stoi(levelID);
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = QuickFactsScene::create();
    layer->setLevel(level);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool QuickFactsScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    for(int i =0; i<maxComboCount;i++)
        rightEffect(i).preload();
    
    wrongEffect().preload();
    showGemEffect().preload();
    
    auto winSize = getContentSize();
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    
    auto bg = Sprite::create(resourcePath+"bg.png");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize.width * 0.5, winSize.height * 0.5);
    addChild(_gameNode);

    //releate by BackGround
    Vec2 framePos = Vec2(winSize.width/2 - 571, winSize.height/2 - 200);
    framePos = _gameNode->convertToNodeSpace(framePos);
    
    auto bgFrame = Sprite::create(resourcePath+"bg-frame.png");
    float r_gameScale = MAX(gameSize.width/winSize.width, gameSize.height/winSize.height);
    bgFrame->setScale(r_gameScale);
    bgFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgFrame->setPosition(framePos);
    _gameNode->addChild(bgFrame, 10);
    
    _examNode = Node::create();
    _examNode->setContentSize(gameSize*gameScale);
    
    _examNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _examNode->setPosition(_gameNode->getBoundingBox().size/2);
    _gameNode->addChild(_examNode, 4);
    
    _quizNode = Node::create();
    _quizNode->setContentSize(gameSize);
    _quizNode->setScale(r_gameScale);
    
    _quizNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _quizNode->setPosition(_gameNode->getBoundingBox().size/2);
    _gameNode->addChild(_quizNode, 3);
    
    _gaugeSprite = Sprite::create(resourcePath+"gauge_0.png");
    _gaugeSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    _comboGauge = ClippingNode::create();
    AffineTransform transform = AffineTransform::IDENTITY;
    transform = AffineTransformScale(transform, _gaugeSprite->getScale(), _gaugeSprite->getScale());
    _comboGauge->setContentSize(SizeApplyAffineTransform(_gaugeSprite->getContentSize(), transform));
    _comboGauge->setScale(r_gameScale);
    _comboGauge->setAnchorPoint( Vec2(0.5, 0.5) );

    //releate by BackGround
    framePos = Vec2(winSize.width/2 - 571, winSize.height/2 + 488);
    Vec2 gaugePos = _gameNode->convertToNodeSpace(framePos);
    _comboGauge->setPosition(gaugePos);

    _gaugeSprite->setPosition(_comboGauge->getContentSize() * 0.5f);
    _comboGauge->setStencil( _gaugeSprite );
    
    auto gaugeClipper = ClippingNode::create();
    gaugeClipper->setInverted(true);
    gaugeClipper->setContentSize(_comboGauge->getContentSize());
    gaugeClipper->setAlphaThreshold( 0.05f );
    gaugeClipper->addChild(_gaugeSprite);
    
    _Gauge = Node::create();
    _Gauge->retain();
    
    _gaugeStencil = LoadingBar::create(resourcePath+"gauge_0.png");
    _gaugeStencil->setPercent(100);
    _gaugeStencil->setDirection(LoadingBar::Direction::RIGHT);
    _gaugeStencil->setOpacity(255);
    _gaugeStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gaugeStencil->setPosition(_comboGauge->getContentSize()/2);
    _Gauge->addChild(_gaugeStencil);
    
    gaugeClipper->setStencil( _Gauge);
    
    _comboGauge->addChild(gaugeClipper);
    _gameNode->addChild(_comboGauge, 5);
    
    createRightEffect();
    
    scheduleUpdate();
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            QuizAnswerPiece* gem = this->_examples.at(_examples.size()/2);
            if (gem->_touchEnabled) this->onRight(gem);
        });
        
        if( UserManager::getInstance()->isWorksheetTestMode())
        {
            string strSheetNum = "WorkSheetTest : 0/0";
            _worksheetLabel = TodoUtil::createLabel(strSheetNum, 50, Size::ZERO, fontName, Color4B::WHITE);
            Vec2 pos = _progressBar->getPosition();
            _worksheetLabel->setPosition(pos.x ,pos.y - 70);
            addChild(_worksheetLabel);
        }
        
        _lifeTimeSlider = ui::Slider::create();
        _lifeTimeSlider->setTag(1);
        _lifeTimeSlider->setTouchEnabled(true);
        _lifeTimeSlider->loadBarTexture(resourcePath + "gauge_5.png");
        _lifeTimeSlider->loadSlidBallTextures(resourcePath + "gem_1.png", resourcePath+"gem_1.png", "");
        _lifeTimeSlider->loadProgressBarTexture(resourcePath + "gauge_0.png");
        _lifeTimeSlider->setPosition(Vec2(skip->getPositionX()-50, skip->getPositionY() - 150));
        _lifeTimeSlider->setPercent(30);
        _lifeTimeSlider->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _lifeTimeSlider->addEventListener(CC_CALLBACK_2(QuickFactsScene::lifeTimeSliderEvent, this));
        _lifeTimeSlider->setScale(0.2);
        addChild(_lifeTimeSlider, 999);
        
        _lifetTmeValueLabel = TodoUtil::createLabel("30", 50, Size::ZERO, fontName, Color4B::WHITE);
        Vec2 pos = _lifeTimeSlider->getPosition();
        _lifetTmeValueLabel->setPosition(pos.x - 50 ,pos.y - 70);
        addChild(_lifetTmeValueLabel);
        
        _timeSlider = ui::Slider::create();
        _timeSlider->setTag(2);
        _timeSlider->setTouchEnabled(true);
        _timeSlider->loadBarTexture(resourcePath + "gauge_5.png");
        _timeSlider->loadSlidBallTextures(resourcePath + "gem_1.png", resourcePath+"gem_1.png", "");
        _timeSlider->loadProgressBarTexture(resourcePath + "gauge_0.png");
        _timeSlider->setPosition(Vec2(_comboGauge->getPositionX(), _comboGauge->getPositionY() + 300));
        _timeSlider->setPercent(50);
        _timeSlider->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _timeSlider->addEventListener(CC_CALLBACK_2(QuickFactsScene::timeSliderEvent, this));
        _timeSlider->setScale(0.2);
        addChild(_timeSlider, 999);
        
        _timeValueLabel = TodoUtil::createLabel("0.5 sec", 50, Size::ZERO, fontName, Color4B::WHITE);
        pos = _timeSlider->getPosition();
        _timeValueLabel->setPosition(pos.x + 100 ,pos.y - 10);
        addChild(_timeValueLabel);
    }
    
    return true;
}

void QuickFactsScene::onEnter()
{
    Layer::onEnter();

    if( cocos2d::FileUtils::getInstance()->isFileExist(BGMPath) )
    {
        GameSoundManager::getInstance()->playBGM(BGMPath, true);
    }
}

void QuickFactsScene::onExit()
{
    GameSoundManager::getInstance()->fadeOutBGM(1);
    
    _quizNode->removeAllChildren();
    _examNode->removeAllChildren();
    _gameNode->removeAllChildren();
    
    for(int i =0; i<_levelData.size(); i++)
    {
        QuickFactsLevelStruct* quiz = _levelData.at(i);
        if(quiz != nullptr)
            delete quiz;
        
    }
    
    _sheetData.clear();
    _levelData.clear();
    
    _sheetNumList.clear();
    
    if(_Gauge != nullptr)
    {
        _Gauge->removeAllChildren();
        _Gauge->release();
    }
    removeAllChildren();
    
    Layer::onExit();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void QuickFactsScene::onEnterTransitionDidFinish()
{
    onStart();
}

void QuickFactsScene::onStart()
{
    loadData(_currentLevel);
    
    selectWorkSheet();
    
    createQuiz();
    _progressBar->setMax((int)_questions.size());
    
    showQuiz();
    showComboGauge();
    
    onNextQuiz();
}

void QuickFactsScene::update(float delta)
{
    if(_gaugeState == GAUGE_PROGRESSING && _comboTime < 0)
    {
        _gaugeState = GAUGE_HOLD;
        
        QuickFactsLevelStruct* quizLevel = _sheetData.at(_currentQuizIndex);
        //comboDown
        string workPath = [this] { stringstream SS;
            SS << "/" << string("Quickfacts");
            SS << "/" << "level-" << _currentLevel << "-" << _currentSheet;
            SS << "/" << "work-" << _currentQuizIndex; return SS.str(); }();
        
        StrictLogManager::shared()->game_Peek_Answer(string("QuickFacts"), workPath, string("timeout"), quizLevel->rightAnswer);
        
        vector<int> pickableIndexList;
        for(int i =0; i<_examples.size(); i++)
        {
            QuizAnswerPiece* gem =  _examples.at(i);
            
            if(isRightAnswer(gem->getNumber()) == false && gem->_touchEnabled){
                pickableIndexList.push_back(i);
            }
        }
        
        if(pickableIndexList.size() == 0)
        {
            for(int i =0; i<_examples.size(); i++)
            {
                QuizAnswerPiece* gem =  _examples.at(i);
                if(gem->_touchEnabled) {
                    gem->runAction(getActionShake(0.1, 5));
                    break;
                }
            }
            
            Sequence* flowAction = Sequence::create(DelayTime::create(0.5), CallFunc::create([this]()
                                                                                             { this->showComboGauge();
                                                                                                 this->onBreakTime();
                                                                                             }), NULL);
            this->runAction(flowAction);
        }
        else
        {
            int randIdx = pickableIndexList.at(random(0, (int)pickableIndexList.size()-1));
            QuizAnswerPiece* gem =  _examples.at(randIdx);
            
            onWrong(gem);
        }
    }
    else if(_gaugeState == GAUGE_PROGRESSING && _comboTime > 0)
    {
        _comboTime -= delta;
        float percent = 100 - (int)(_comboTime/_comboTimeMax * 100);
        _gaugeStencil->setPercent(percent);
    }
    else if(_gaugeState == GAUGE_HOLD)
    {
        float percent = 100 - (int)(_comboTime/_comboTimeMax * 100);
        _gaugeStencil->setPercent(percent);
    }
}

void QuickFactsScene::setLevel(int level)
{
    _currentLevel = level;
}

void QuickFactsScene::loadData(int level)
{
    string P = "Games/" + resourcePath + "quickfacts_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    auto data = TodoUtil::readTSV(S);
   
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

    string LanguageTag;
    int levelID;
    int rowIndex = 0;
    
    _levelData.clear();
    
    while (rowIndex < data.size()) {
        auto row = data[rowIndex++];
        LanguageTag = row[0];
        
        levelID =  TodoUtil::stoi(row[1]);
        if(level != (int)levelID)
            continue;
    
        QuickFactsLevelStruct* quiz = new QuickFactsLevelStruct;
        quiz->sheetNo = TodoUtil::stoi(row[2]);
    
        vector<int>::iterator iter = find(_sheetNumList.begin(), _sheetNumList.end(), quiz->sheetNo);
        if(iter == _sheetNumList.end())
            _sheetNumList.push_back(quiz->sheetNo);
            
        quiz->problemNo = TodoUtil::stoi(row[3]);
        quiz->comboTime = float(TodoUtil::stoi(row[4]));
        quiz->firstValue = row[5];
        quiz->secondValue = row[6];
        quiz->mathSign = row[7];
        quiz->rightAnswer = row[8];
        
        for( int i =0; i<maxAnswerCount; i++)
        {    
            int number = TodoUtil::stoi( row[ 9+i] );
            
            if(number != INT_MIN)
                quiz->examples.push_back( row[ 9+i ] );
        }
        
        _levelData.push_back(quiz);
    }
}

void QuickFactsScene::selectWorkSheet()
{
    if(UserManager::getInstance()->isWorksheetTestMode() == false)
        _currentSheet = _sheetNumList.at(random(0, (int)_sheetNumList.size()-1));
    else
    {
        _currentSheet = _sheetNumList.at(_currentSheetCount++);
    }
    
    _sheetData.clear();
    
    for(int i =0; i<(int)_levelData.size(); i++)
    {
        QuickFactsLevelStruct* data = _levelData.at(i);
        if(data->sheetNo == _currentSheet)
            _sheetData.push_back(data);
    }
}

void QuickFactsScene::createQuiz()
{
    _currentQuizIndex = 0;
    _currentCombo = 0;
    _questions.clear();
    
    if(_worksheetLabel != nullptr)
    {
        _worksheetLabel->setString("WorkSheetTestMode : " + TodoUtil::itos(_currentSheet) + "/" + TodoUtil::itos((int)_sheetNumList.size()));
    }
    
    for (int i =0; i<_sheetData.size(); i++)
    {
        QuickFactsLevelStruct* quiz = _sheetData.at(i);
        
        if(quiz->mathSign == "*") {
            quiz->mathSign = "x";
        }
        
        else if(quiz->mathSign == "/") {
            quiz->mathSign = "÷";
        }
        
        string strFunc = quiz->firstValue + " " + quiz->mathSign + " " +quiz->secondValue + " = ";
        string strAnswer = "?";
        
        QuizLabel* quizLabel = QuizLabel::create();
        
        //quizLabel->setQuizData(strFunc, strAnswer);
        quizLabel->setQuizData(quiz->firstValue, quiz->mathSign, quiz->secondValue, strAnswer);
        quizLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _quizNode->addChild(quizLabel, 4);
        
        _questions.push_back(quizLabel);
    }

}

void QuickFactsScene::showQuiz()
{
    int index = _currentQuizIndex;
    
    _progressBar->setCurrent(index+1, false);
    
    auto winSize = getContentSize();

    Vec2 gaugePos = _quizNode->convertToNodeSpace(_gameNode->convertToWorldSpace
                                                  (_comboGauge->getPosition()));
    float quizPosX = gaugePos.x;
    
    int visibleCount = 0;
    for(int i =index; i<index+MAX_QUIZ_LINE; i++)
    {
        if(i >= _questions.size()) break;
        QuizLabel* quizLabel = _questions.at(i);
        
        Vec2 funcPos = Vec2(quizPosX, 1070 - visibleCount*172 + _gameNode->getBoundingBox().origin.y);
        quizLabel->setPosition(funcPos);
        
        if(i == index)
        {
            Vec2 MidPos = Vec2(quizPosX, gaugePos.y - _comboGauge->getBoundingBox().size.height/2);
            Vec2 DestPos = Vec2(quizPosX, gaugePos.y);
            
            Spawn* actionDisappear = Spawn::create(EaseOut::create(MoveTo::create(0.25f, MidPos), 2.f),                                                                                      FadeTo::create(0.15, 0), NULL);
            
            Spawn* actionAppear = Spawn::create(EaseOut::create(MoveTo::create(0.25f, DestPos), 2.f),FadeTo::create(0.15, 255), NULL);
            
            quizLabel->runAction( Sequence::create( DelayTime::create(0.25f), actionDisappear,
                                                   Spawn::create(actionAppear, CallFunc::create([quizLabel, this](){
                                                    quizLabel->setColor(quizColorBackGauge);
                                                    this->onAttachQuiz(false); }), NULL), NULL));
        }
        else
        {
            int fadeFactor = fadeLevelForQuiz[visibleCount];
            
            Vec2 DestPos = Vec2(quizPosX, 1070 - (visibleCount-1)*172 + _gameNode->getBoundingBox().origin.y);
            quizLabel->runAction(Sequence::create(DelayTime::create(0.25f),
                                                  Spawn::create(EaseOut::create(MoveTo::create(0.5, DestPos), 2.f),
                                                      FadeTo::create(0.5, 255 * (fadeFactor * 0.01)), NULL),
                                                  nullptr));
        }
        visibleCount++;
    }
    
    for(int i =0; i<_gaugeSprite->getChildrenCount(); i++)
    {
        Node* child =  _gaugeSprite->getChildren().at(i);
        child->runAction( Sequence::create( DelayTime::create(0.25f),
                                           Spawn::create(EaseOut::create(MoveBy::create(0.5f,Vec2(0,172)), 2.f), FadeTo::create(0.05, 0), NULL),
                                           CallFunc::create([child, this](){ child->removeFromParentAndCleanup(true);  })
                                           , NULL));
    }

    
}

void QuickFactsScene::showComboGauge()
{
    int comboFileIndex = _currentCombo; //(_currentCombo == maxComboCount) ? 5 : 0;
    
    auto barSprite = Sprite::create(resourcePath+"gauge_"+TodoUtil::itos(comboFileIndex) +".png");
    _gaugeSprite->setSpriteFrame(barSprite->getSpriteFrame());
    
    _comboTime = _comboTimeMax = 1;
    _gaugeStencil->loadTexture(resourcePath+"gauge_"+TodoUtil::itos(comboFileIndex) +".png");
    _gaugeStencil->setPercent(100);
    
    Vec2 destPos = _gaugeSprite->getContentSize() * 0.5f;
    _gaugeSprite->setPosition(destPos);
    _gaugeSprite->setOpacity(255);
    
    for(int i =0; i<_gaugeSprite->getChildrenCount(); i++)
    {
        Node* child =  _gaugeSprite->getChildren().at(i);
        child->setOpacity(255 * 0.8f);
        child->setPosition(destPos);
    }
}

void QuickFactsScene::createExample()
{
    QuickFactsLevelStruct* quizLevel = _sheetData.at(_currentQuizIndex);
    
    auto winSize = getContentSize();
    
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    
    Vec2 gemCenterPos = Vec2(gameSize.width * 0.609, gameSize.height *gameScale* 0.475);
    
    Vec2 Center = gemCenterPos;
    
    _examples.clear();
    _examNode->removeAllChildren();
    
    string workPath = [this] { stringstream SS;
        SS << "/" << string("Quickfacts");
        SS << "/" << "level-" << _currentLevel << "-" << _currentSheet;
        SS << "/" << "work-" << _currentQuizIndex; return SS.str(); }();
    
    int col = 0;
    for(int i =0; i<quizLevel->examples.size(); i++)
    {
        if(col % 3 == 0)
            col = 0;
        
        string gemNum = quizLevel->examples.at(i);
        
        QuizAnswerPiece* gem = QuizAnswerPiece::create();
        gem->_idx = i;
        gem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        gem->setTexture(_currentCombo);
        gem->setNumber(gemNum);
        gem->setScale(0);
        
        int row = (int)quizLevel->examples.size() / 3;
        Vec2 Center = gemCenterPos;
        
        if( row > 0 && row % 2 == 0)
            Center.y += (row/2) * (gem->getContentSize().height + GEM_PADDING_ROW) - (gem->getContentSize().height - GEM_PADDING_ROW) + 5;
        else
            Center.y += (row/2) * (gem->getContentSize().height + GEM_PADDING_ROW) + 5;
        
        gem->setPosition(Center.x + col * (gem->getContentSize().width + GEM_PADDING_COL),
                         Center.y - (i/3) * (gem->getContentSize().height + GEM_PADDING_ROW) );
        
        gem->onSelectGem = [this, gem, workPath]() {
        
            QuickFactsLevelStruct* quizLevel = this->_sheetData.at(_currentQuizIndex);
            StrictLogManager::shared()->game_Peek_Answer(string("QuickFacts"), workPath, gem->getNumber(), quizLevel->rightAnswer);
            
            if (this->isRightAnswer(gem->getNumber())) {
                this->onRight(gem);
            } else {
                this->onWrong(gem);
            }
        };
        
        _examples.push_back(gem);
        _examNode->addChild(gem);
        
        col++;
    }
}

void QuickFactsScene::showExample()
{
    //pop gems!
    for(int i =0; i<_examples.size(); i++)
    {
        float delayByCol = (i % 3) * 0.025;
        float delayByRow = (i / 3) * 0.025;
        
        QuizAnswerPiece* gem = _examples.at(i);
        if(gem != nullptr)
        {
            Sequence* popAction = Sequence::create( getActionAppear(0.25f + delayByRow + delayByCol, 0.2f),
                                                   CallFunc::create([gem](){
                                                    gem->_touchEnabled = true;
                                                    showGemEffect().play();
            }), NULL);
            gem->runAction(popAction);
                   
        }
    }
}

void QuickFactsScene::createRightEffect()
{
    for (int i =0;i<MAX_EXAMPLE_COUNT; i++)
    {
        for (int j =0;j<_IMAGE_COUNT_MAX_COMBO; j++)
        {
            _lightParticles[i][j] = ParticleSystemQuad::create(resourcePath + "effects/code_effect/quick-facts_0"+ TodoUtil::itos(j+1)+".plist");
            _lightParticles[i][j]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _lightParticles[i][j]->setScale(2.1);
            _lightParticles[i][j]->resetSystem();
            _lightParticles[i][j]->setPosition(Vec2(-10000, 10000));
            
            _gameNode->addChild(_lightParticles[i][j], 9);
        }
    }
}

void QuickFactsScene::onBreakTime()
{
    float delay = 0.5f;
    if(UserManager::getInstance()->isDebugMode())
        delay =_timeSlider->getPercent() * 0.01;
    
    for(int i=0; i<_examples.size(); i++)
    {
        QuizAnswerPiece* gem =  _examples.at(i);
        if(gem != nullptr)
        {
            gem->setTexture(0);
            if(gem->_isAnimated == false)
            {
                gem->runAction(Sequence::create(DelayTime::create(delay),
                                                CallFunc::create([gem](){ gem->_touchEnabled = true; } ),
                                                NULL) );
            }
        }
    }
    
    auto seq = CallFunc::create([this](){
        QuickFactsLevelStruct* quiz = this->_sheetData.at(this->_currentQuizIndex);
        this->_comboTime = quiz->comboTime;
        this->_comboTimeMax = quiz->comboTime;
        this->_gaugeState = GAUGE_PROGRESSING;
    } );
    
    runAction(Sequence::create(DelayTime::create(delay), seq, NULL));
}

void QuickFactsScene::onNextQuiz()
{
    _examNode->removeAllChildren();
    
    createExample();
    showExample();
    
    float delay = 0.5f;
    if(UserManager::getInstance()->isDebugMode())
        delay =_timeSlider->getPercent() * 0.01;
    
    auto seq = CallFunc::create([this](){
        QuickFactsLevelStruct* quiz = this->_sheetData.at(this->_currentQuizIndex);
        this->_comboTime = quiz->comboTime;
        this->_comboTimeMax = quiz->comboTime;
        this->_gaugeState = GAUGE_PROGRESSING;
    } );
    
    runAction(Sequence::create(DelayTime::create(0.5), seq, NULL));
}

void QuickFactsScene::disappearQuiz()
{
    float quizPosX =_comboGauge->getPositionX();
    
    QuizLabel* quizLabel = _questions.at(_currentQuizIndex);
    auto winSize = getContentSize();
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    
    Vec2 destPos = Vec2(quizPosX, gameSize.height * gameScale * 0.897);
    
    quizLabel->runAction( Sequence::create(
                                           DelayTime::create(0.3f),
                                           Spawn::create(EaseOut::create(MoveTo::create(0.3f, destPos), 2.f), FadeTo::create(0.01, 0), NULL)
                                           , NULL));
    
}

bool QuickFactsScene::isExistNextQuiz()
{
    if(_currentQuizIndex + 1 >= _sheetData.size())
    {
        return false;
    }
    return true;
}

void QuickFactsScene::onCompleteLevel(float delay)
{
    disappearQuiz();
    
    _currentQuizIndex++;
    _currentCombo = MIN(_currentCombo+1, maxComboCount);
    
    if(_currentQuizIndex >= _sheetData.size())
    {
        if(UserManager::getInstance()->isWorksheetTestMode() && _currentSheetCount != _sheetNumList.size())
        {
            onStart();
        }
        else
        {
            _comboGauge->removeAllChildren();
            _comboGauge->removeFromParentAndCleanup(true);
            
            _quizNode->removeAllChildren();
            
            GameSoundManager::getInstance()->fadeOutBGM(1);
            
            auto CP = CompletePopup::create();
            CP->show(1.5f, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);});
        }
    }
    else {
        showQuiz();
        showComboGauge();
        runAction(Sequence::create(DelayTime::create(delay),
                                   CallFunc::create([this](){ this->onNextQuiz();} ),
                                   NULL));
    }
}

void QuickFactsScene::playSound(string name)
{
    string path = "Games/QuickFacts/Sound/"+name;
    
    GameSoundManager::getInstance()->playEffectSound(path);
}

bool QuickFactsScene::isRightAnswer(string number)
{
    QuickFactsLevelStruct* quizLevel = _sheetData.at(_currentQuizIndex);
    if(quizLevel->rightAnswer == number)
        return true;
    
    return false;
}

void QuickFactsScene::onAttachQuiz(bool isAnimated)
{
    _gaugeSprite->removeAllChildren();
    
    QuizLabel* quizLabel = _questions.at(_currentQuizIndex);
    Vec2 destPos = _gaugeSprite->getContentSize() * 0.5f;
    
    Spawn* actionAppear = Spawn::create( EaseOut::create(MoveTo::create(0.25f, destPos), 2.f),FadeTo::create(0.15, 255 * 0.8), NULL);
    
    auto quizStencil = QuizLabel::create();
    
    quizStencil->setQuizData(quizLabel->_left->getString(), quizLabel->_sign->getString(),
                             quizLabel->_right->getString(), quizLabel->_answer->getString());
    
    quizStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gaugeSprite->addChild(quizStencil, 10);
    
    if(isAnimated == false)
    {
        quizStencil->setPosition(destPos.x, 0);
        quizStencil->setColor(quizColorFrontGauge);
        quizStencil->setOpacity(0);
    
        Sequence* seq = Sequence::create(actionAppear, DelayTime::create(0.1) ,CallFunc::create([quizStencil](){
            quizStencil->setColor(quizColorFrontGauge); }), NULL);
        quizStencil->runAction(seq);
    }
    else
    {
        quizStencil->setPosition(destPos);
        quizStencil->setColor(quizColorFrontGauge);
        quizStencil->setOpacity(255 * 0.8);
    }
}

void QuickFactsScene::onWrong(QuizAnswerPiece* gem)
{
    _currentCombo = 0;// 3rd feed, MAX(0, _currentCombo-1);
    
    //playSound
    wrongEffect().play();
    //playEffect
    
    _gaugeState = GAUGE_HOLD;
    
    _gaugeStencil->stopAllActions();
    _gaugeSprite->runAction( Spawn::create(MoveBy::create(0.2, Vec2(0, -10)),FadeOut::create(0.2), NULL));
    
    for(int i =0; i<_gaugeSprite->getChildrenCount(); i++)
    {
        Node* child =  _gaugeSprite->getChildren().at(i);
        child->runAction(Spawn::create(MoveBy::create(0.2, Vec2(0, 10)),FadeOut::create(0.2), NULL));
    }
    
    for(int i =0; i<_examples.size(); i++)
    {
        QuizAnswerPiece* otherGem = _examples.at(i);
        if(otherGem != nullptr)
        {
            otherGem->_touchEnabled = false;
            if(gem != otherGem)
                otherGem->runAction(getActionShake(0.1, 5));
        }
        
    }
    QuizLabel* quizLabel = _questions.at(_currentQuizIndex);
    quizLabel->setColor(quizColorBackGauge);
    quizLabel->setOpacity(255);
    
    gem->PlayActionBreak();
    
    Sequence* flowAction = Sequence::create(DelayTime::create(1.2), CallFunc::create([this]()
                                                                                     { this->showComboGauge();
                                                                                       this->onBreakTime();
                                                                                          }), NULL);
    this->runAction(flowAction);
                                           
}

void QuickFactsScene::onRight(QuizAnswerPiece* gem)
{
    //playSound
    int comboIndex = MIN(_currentCombo, maxComboCount-1);
    //rightEffect(_currentCombo);
    SoundEffect(rightSFXPath + TodoUtil::itos(comboIndex) + ".m4a").play();
    
    //playEffect
    _gaugeStencil->stopAllActions();
    _gaugeState = GAUGE_HOLD;
    
    gem->PlayActionRight(_currentCombo);
    playRightEffect(gem->_idx, gem);
    
    int touchIdx = gem->_idx;
    int minGem = MAX(0, touchIdx-4);
    int maxGem = MIN(touchIdx+4, (int)_examples.size());
    int combo = _currentCombo;
    
    //upper gems
    for(int i = minGem; i<touchIdx; i++)
    {
        if(touchIdx % 3 == 0 && i == touchIdx - 1) continue;
        if(touchIdx % 3 == 0 && i == touchIdx - 4) continue;
        if(touchIdx % 3 == 2 && i == touchIdx - 2) continue;
        
        QuizAnswerPiece* gem = _examples.at(i);
        if(gem->_touchEnabled)
        {
            runAction(Sequence::create(DelayTime::create(0.05), CallFunc::create([this, gem](){ playRightEffect(gem->_idx, gem);}), NULL));
        }
        Sequence* breakGems = Sequence::create(DelayTime::create(0.05), CallFunc::create([gem, combo](){ gem->PlayActionRight(combo);}), NULL);
        gem->_isAnimated = true;
        gem->runAction(breakGems);
        
    }
    
    //under gems
    for(int i =touchIdx+1; i<maxGem; i++)
    {
        if(touchIdx % 3 == 0 && i == touchIdx + 2) continue;
        if(touchIdx % 3 == 2 && i == touchIdx + 1) continue;
        if(touchIdx % 3 == 2 && i == touchIdx + 4) continue;
        
        QuizAnswerPiece* gem = _examples.at(i);
        
        if(gem->_touchEnabled)
        {
            runAction(Sequence::create(DelayTime::create(0.05), CallFunc::create([this, gem](){ playRightEffect(gem->_idx, gem);}), NULL));
        }
        
        Sequence* breakGems = Sequence::create(DelayTime::create(0.1), CallFunc::create([gem, combo](){ gem->PlayActionRight(combo);}), NULL);
        gem->_isAnimated = true;
        
        gem->runAction(breakGems);
        
    }
    
    //extra gems
    for(int i =0; i<_examples.size(); i++)
    {
        QuizAnswerPiece* gem = _examples.at(i);
        
        if(gem->_isAnimated == false)
        {
            if(gem->_touchEnabled)
            {
                runAction(Sequence::create(DelayTime::create(0.05), CallFunc::create([this, gem](){ playRightEffect(gem->_idx, gem);}), NULL));
            }
            gem->_touchEnabled = false;
            
            Sequence* breakGems = Sequence::create(DelayTime::create(0.15), CallFunc::create([gem, combo](){ gem->PlayActionRight(combo);}), NULL);
            
            gem->runAction(breakGems);
        }
        
        if(gem != nullptr) gem->_touchEnabled = false;
    }

    QuickFactsLevelStruct* quizLevel = _sheetData.at(_currentQuizIndex);
    QuizLabel* quizLabel = _questions.at(_currentQuizIndex);
    quizLabel->actionAnswer(quizLevel->rightAnswer);
    
    for(int i =0; i<_gaugeSprite->getChildrenCount(); i++)
    {
        QuizLabel* child =  dynamic_cast<QuizLabel*>(_gaugeSprite->getChildren().at(i));
        if(child != nullptr) child->actionAnswer(quizLevel->rightAnswer);
    }
    
    float tension = 25.f;
    if(UserManager::getInstance()->isDebugMode()) {
        tension = _lifeTimeSlider->getPercent();
    }
    
    float lifeTime = _lightParticles[0][0]->getDuration() * -1;
    float delay = lifeTime * (100 - tension) * 0.01;
    lifeTime = lifeTime * tension * 0.01;
    
    if( isExistNextQuiz() )
    {
        runAction(Sequence::create(DelayTime::create(lifeTime),
                               CallFunc::create([this, delay](){ this->onCompleteLevel(delay);} ),
                               NULL));
    }
    else
    {
        lifeTime = 0.1 + _lightParticles[0][0]->getDuration() * -1;
        delay = 0;
        runAction(Sequence::create(DelayTime::create(lifeTime),
                                   CallFunc::create([this, delay](){ this->onCompleteLevel(delay);} ),
                                   NULL));
    }
}

void QuickFactsScene::playRightEffect(int idx, QuizAnswerPiece* gem)
{
    int initIdx = MAX(0, _currentCombo-2);
    int comboCnt = MIN(_currentCombo+1, maxComboCount);
    for(int i=initIdx; i<comboCnt; i++)
    {
        ParticleSystemQuad* effect = _lightParticles[idx][i];
        Vec2 posOnGameNode = _gameNode->convertToNodeSpace(
                                                           _gameNode->convertToWorldSpace
                                                           (gem->getPosition()));
        
        effect->setPosition(posOnGameNode);

        effect->runAction(Sequence::create(DelayTime::create(0.2 - i*0.05),
                                                          CallFunc::create([effect](){ effect->resetSystem(); }),NULL));
    }
}

void QuickFactsScene::lifeTimeSliderEvent(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        int maxPercent = slider->getMaxPercent();
        _lifetTmeValueLabel->setString(StringUtils::format("%.0f%%", 100.f * percent / maxPercent));
    }
}

void QuickFactsScene::timeSliderEvent(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        int maxPercent = slider->getMaxPercent();
        _timeValueLabel->setString(StringUtils::format("%.1f sec", (float)percent / maxPercent));
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* QuizLabel */

bool QuizLabel::init() {
    if (!Node::init()) return false;
    
    setContentSize(quizSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    setOpacityModifyRGB(false);
    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);
    setOpacity(0);
    
    return true;
}

void QuizLabel::setQuizData(string quizData, string quizAnswer)
{
    auto winSize = getContentSize();
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    
    setColor(quizColorBackGauge);
    
    _func = TodoUtil::createLabel(quizData, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _func->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _func->setPosition(gameSize.width * 0.222 * gameScale, quizSize.height/2 * gameScale);
    _func->setOpacityModifyRGB(false);
    addChild(_func, 1);
    
    _answer = TodoUtil::createLabel(quizAnswer, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _answer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    _answer->setPosition(gameSize.width * 0.238 * gameScale, quizSize.height/2 * gameScale);
    _answer->setOpacityModifyRGB(false);
    
    addChild(_answer, 1);
}

void QuizLabel::setQuizData(string strLeft, string strSign, string strRight, string strAnswer)
{
    const int padding = 130;
    Vec2 pos = Vec2(quizSize.width * 0.5 - padding * 2, quizSize.height/2);
    setColor(quizColorBackGauge);
    
    _left = TodoUtil::createLabel(strLeft, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _left->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _left->setAlignment(cocos2d::TextHAlignment::CENTER);
    _left->setPosition(pos);
    _left->setOpacityModifyRGB(false);
    addChild(_left, 1);
    
    _sign = TodoUtil::createLabel(strSign, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _sign->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sign->setAlignment(cocos2d::TextHAlignment::CENTER);
    _sign->setPosition(pos.x + padding, pos.y);
    _sign->setOpacityModifyRGB(false);
    addChild(_sign, 1);
    
    _right = TodoUtil::createLabel(strRight, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _right->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _right->setAlignment(cocos2d::TextHAlignment::CENTER);
    _right->setPosition(pos.x + padding * 2, pos.y);
    _right->setOpacityModifyRGB(false);
    addChild(_right, 1);
    
    _func = TodoUtil::createLabel("=", 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _func->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _func->setAlignment(cocos2d::TextHAlignment::CENTER);
    _func->setContentSize(Size(quizSize.width * 0.7, quizSize.height));
    _func->setPosition(pos.x + padding * 3, pos.y);
    _func->setOpacityModifyRGB(false);
    addChild(_func, 1);
    
    _answer = TodoUtil::createLabel(strAnswer, 120, Size::ZERO, fontBoldName, Color4B::WHITE);
    _answer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _answer->setAlignment(cocos2d::TextHAlignment::CENTER);
    _answer->setPosition(pos.x + padding * 4, pos.y);
    _answer->setOpacityModifyRGB(false);
    addChild(_answer, 1);
}

void QuizLabel::actionAnswer(string answer)
{
    GLubyte opacity = _func->getOpacity();
    
    _answer->runAction( Sequence::create(FadeTo::create(0.2, 0),
                                         CallFunc::create([this, answer, opacity](){
                                                this->_answer->setString(answer);
                                            }),
                                         FadeTo::create(0.2, opacity),NULL));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* QuizAnswerPiece */

bool QuizAnswerPiece::init() {
    if (!Node::init()) return false;
    
    auto gemImage = Sprite::create(resourcePath + "gem_0.png");
    Size cSize = gemImage->getContentSize();
    
    setContentSize(cSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _shadow = Sprite::create(resourcePath + "gem_shadow.png");
    _shadow->setPosition(Vec2(cSize.width/2+10, cSize.height/2-15));
    _shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _shadow->setOpacity(255 * 0.55f);
    addChild(_shadow);

    auto offSprite = Sprite::create(resourcePath+"gem_off.png");
    
    _offProgress = ProgressTimer::create(offSprite);
    _offProgress->setType(ProgressTimer::Type::BAR);
    _offProgress->setMidpoint(Vec2(0.5, 0.5));
    _offProgress->setPercentage(0);
    _offProgress->setBarChangeRate(Vec2(1, 1));
    _offProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _offProgress->setPosition(Vec2(cSize.width/2, cSize.height/2));
    _offProgress->setOpacity(0);
    addChild(_offProgress);
    
    for(int i =0; i<_IMAGE_COUNT_BREAK_GEM; i++)
    {
        string offPath = resourcePath + "effects/gem_off/gem_off_" + TodoUtil::itos(i+1) + ".png";
        _stoneSprite[i] = Sprite::create(offPath);
        _stoneSprite[i]->setScale(0);
        _stoneSprite[i]->setPosition(Vec2(cSize.width/2, cSize.height/2));
        addChild(_stoneSprite[i]);
    }
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            
            //pickEffect().play();
            this->setScale(0.85);
            
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (!_touchEnabled) return false;
        
        this->setScale(1);
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
                
            //pickEffect().play();
            
            if(onSelectGem) onSelectGem();
            
            return true;
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void QuizAnswerPiece::setTexture(int combo)
{
    int comboFileIndex = combo; //(combo == maxComboCount) ? 5 : 0;
    if( _gemSprite == nullptr)
    {
        _gemSprite = Sprite::create(resourcePath + "gem_"+ TodoUtil::itos(comboFileIndex) +".png");
        Size cSize = _gemSprite->getContentSize();
        _gemSprite->setPosition(Vec2(cSize.width/2, cSize.height/2));
        _gemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(_gemSprite, 10);
    }
    else
    {
        _gemSprite->setTexture(resourcePath + "gem_"+TodoUtil::itos(comboFileIndex) +".png");
    }
    
}

void QuizAnswerPiece::setNumber(string num)
{
    Size cSize = _gemSprite->getContentSize();
    
    _number = TodoUtil::createLabel(num, 110, Size::ZERO, fontBoldName, Color4B(0, 0, 0, 255 * 0.8));
    _number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _number->setPosition(cSize.width/2, cSize.height/2);
    addChild(_number, 10);
    
    _strNum = num;
}

void QuizAnswerPiece::PlayActionBreak()
{
    _touchEnabled = false;
    _isAnimated = true;
    
    auto toProgress = ProgressTo::create(0.2, 100);
    auto seq = Sequence::create( Spawn::create(toProgress,FadeIn::create(0.1), getActionShake(0.1, 5), NULL),  getActionDisAppear(0.1, 0.2), NULL);
    _offProgress->runAction(seq);
    
    _gemSprite->runAction( Spawn::create(getActionShake(0.1, 5), getActionDisAppear(0.1, 0.2), NULL));
    _shadow->runAction(    Spawn::create(getActionShake(0.1, 5), getActionDisAppear(0.1, 0.2), NULL));
    _number->runAction(    Spawn::create(getActionShake(0.1, 5), getActionDisAppear(0.1, 0.2), NULL));
    
    int radius = _gemSprite->getContentSize().width/3;
    Vec2 center = _gemSprite->getPosition();
    for(int i =0; i<_IMAGE_COUNT_BREAK_GEM; i++)
    {
        Vec2 pos = center + Vec2(radius * cos( MATH_DEG_TO_RAD(i*60)), radius * sin(MATH_DEG_TO_RAD(i*60)));
        
        auto breakGemAction = Sequence::create(DelayTime::create(0.5),
                                    Spawn::create(ScaleTo::create(0.1,1),
                                                  EaseOut::create(MoveTo::create(0.3, pos), 2.f), NULL),
                                    DelayTime::create(0.1),FadeOut::create(0.5), NULL);
        
        _stoneSprite[i]->runAction(breakGemAction);
    }

}

void QuizAnswerPiece::PlayActionRight(int combo)
{
    _isAnimated = true;
    _touchEnabled = false;
    
    Size cSize = _gemSprite->getContentSize();
    
    _gemSprite->runAction(getActionDisAppear(0.1, 0.3));
    _shadow->runAction(getActionDisAppear(0.1, 0.1));
    _number->runAction(getActionDisAppear(0.1, 0.1));
}
