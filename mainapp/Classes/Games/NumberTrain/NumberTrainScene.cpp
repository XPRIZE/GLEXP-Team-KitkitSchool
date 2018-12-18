//
//  NumberTrainScene.cpp
//  todomath
//
//  Created by Gunho Lee on 2017. 3. 21..
//
//

#include "NumberTrainScene.hpp"
#include "NumberTrainHeader.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "NumberTrainProblemBank.hpp"

#include "CCAppController.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Effects/FireworksEffect.hpp"

namespace NumberTrainNamespace {
    const auto bottomHeight = 446;
    const auto trainGuardWidth = 1940;
    const auto railTop = 410+52;
    const auto snapRadius = 200;
    bool isSW = false;
    bool isEN = false;
    
};

using namespace NumberTrainNamespace;

Scene* NumberTrainScene::createScene(int levelID)
{
    auto scene = Scene::create();
    
    auto layer = NumberTrainScene::create();
    layer->setLevel(levelID);
    
    scene->addChild(layer);
    
    return scene;
}

void NumberTrainScene::setLevel(int level)
{
    _currentLevel = level;
    
    NumberTrainProblemBank *problemBank = new NumberTrainProblemBank();
    _problem = problemBank->generateParameters(level);
    
    
    CCLOG("problem size %d",_problem["problems"].size());

    _progressBar = ProgressIndicator::create();
    _progressBar->setMax(_problem["problems"].size());
    //_totalProblemCount = _problem["words"].size();

    // UI 생성
    this->initUI();

}



NumberTrainScene::NumberTrainScene(){
    
    
}

NumberTrainScene::~NumberTrainScene()
{
    
}

bool NumberTrainScene::init(){
    
    if (!Layer::init())
    {
        return false;
    }
    
    float gameHeight = 1800;
    Size winSize = getContentSize();
    setContentSize(Size(winSize.width/winSize.height*gameHeight, gameHeight));
    setScale(winSize.height/gameHeight);
    
    srand((unsigned)time(NULL));
    
    _isReadyToSubmit = false;
    
    _hintNode = nullptr;

    if (LanguageManager::getInstance()->getCurrentLanguageTag() == "en-US") isEN = true; else isSW = true;

    return true;
}

void NumberTrainScene::onEnter(){
    Layer::onEnter();

    handleStart();
    
}

void NumberTrainScene::onExit(){
    
    Layer::onExit();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    GameSoundManager::getInstance()->stopAllEffects();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}


// =========================================================================================================================================== //

void NumberTrainScene::initUI()
{
    
    auto winSize = getContentSize();
    
    
    {
        _top = Node::create();
        auto viewSize = Size(winSize.width, winSize.height-bottomHeight);
        _top->setContentSize(viewSize);
        _top->setPosition(Vec2(0, bottomHeight));
        addChild(_top);
        
        auto bg = Sprite::create("NumberTrain/smallest_largest_bg_top.png");
        auto bgSize = bg->getContentSize();
        auto bgScale = MAX(viewSize.width / bgSize.width, viewSize.height / bgSize.height);
        bg->setScale(bgScale);
        bg->setPosition(viewSize/2);
        _top->addChild(bg);
    }
    
    
    
    {
        auto g1 = Sprite::create("NumberTrain/smallet_largest_rail_blocker.png");
        auto guardSize = g1->getContentSize();
        
        _trainGuard = Node::create();
        _trainGuard->setContentSize(Size(trainGuardWidth+guardSize.width*2, guardSize.height));
        _guardPos = Vec2(0, railTop);
        _trainGuard->setPosition(_guardPos + Vec2(0, -guardSize.height));
        addChild(_trainGuard);
        
        g1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        g1->setPosition(winSize.width/2 - trainGuardWidth/2 - 26, 0);
        _trainGuard->addChild(g1);
        
        auto g2 = Sprite::create("NumberTrain/smallet_largest_rail_blocker.png");
        g2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        g2->setPosition(winSize.width/2 + trainGuardWidth/2 + 26, 0);
        _trainGuard->addChild(g2);
    }
    
    {
        _bottom = Node::create();
        auto viewSize = Size(winSize.width, bottomHeight);
        _bottom->setContentSize(viewSize);
        _bottom->setPosition(Vec2(0, 0));
        addChild(_bottom);
        
        auto bg = Sprite::create("NumberTrain/smallest_largest_bg_bottom.png");
        auto bgSize = bg->getContentSize();
        auto bgScale = MAX(viewSize.width / bgSize.width, viewSize.height / bgSize.height);
        bg->setScale(bgScale);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        bg->setPosition(Vec2(viewSize.width/2, bottomHeight));
        _bottom->addChild(bg);
    }
    
    auto rail = Sprite::create("NumberTrain/smallest_largest_rail.png");
    rail->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    auto railSize = rail->getContentSize();
    auto railScale = winSize.width / railSize.width;
    rail->setScaleX(railScale);
    rail->setPosition(0, railTop+6);
    addChild(rail);

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);

    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);


    /*
    _submitButton = Button::create();
    _submitButton->loadTextures("NumberTrain/smallest_largest_butoon_normal.png",
                                "NumberTrain/smallest_largest_butoon_active.png",
                                "NumberTrain/smallest_largest_butoon_inactive.png");
    _submitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _submitButton->setPosition(Vec2(winSize.width/2+810, bottomHeight+50));
    _submitButton->addClickEventListener([this](Ref*){
        if (_isReadyToSubmit) {
            this->checkAnswer();
        }
    });
    _submitButton->setEnabled(false);
    _submitButton->setVisible(false);
    addChild(_submitButton);
     */
    
}

// =========================================================================================================================================== //

void NumberTrainScene::prepareNewGameWithLevel(Json::Value problemParameter){
    //    std::string curr_lang = LanguageManager::getInstance()->getAppLanguageCode();
    //    CCLOG("current lagnuage:%s", curr_lang.c_str());
    
    _problem = problemParameter;
    
    
}
// =========================================================================================================================================== //

void NumberTrainScene::handleStart(){
    CCLOG("handleStart");
    _totalProblems = _problem["problemCount"].asInt();
    
    // GameController::getInstance()->onBeginWithTotalProblems(_totalProblems);
    
    _currentProblemIndex = 0;
    setProblem(_currentProblemIndex);
    
    
}

void NumberTrainScene::handlePause() {}
void NumberTrainScene::handleResume() {}
void NumberTrainScene::handleQuit(){
    this->stopAllActions();
    this->unscheduleAllCallbacks();
}
void NumberTrainScene::handleAnswerEntered(std::string &answer){
    // GameController::getInstance()->onGameComplete();
}
void NumberTrainScene::showHint() {
    int i=0;
    for (auto t : _trainCars) {
        if (!t->_hasSlot) continue;
        if (t->_cardInSlot == nullptr) {
            
            auto face = Sprite::create("NumberTrain/smallest_largest_1_answerblcok_surface.png");
            face->setCascadeOpacityEnabled(true);
            face->setOpacity(128);
            auto size = face->getContentSize();
            
            auto faceTexture = AnswerPiece::createFace(_correctAnswer[i]);
            faceTexture->setPosition(size/2 + Size(0, 11));
            faceTexture->setCascadeOpacityEnabled(true);
            face->addChild(faceTexture);
            face->setPosition(t->_slotPos);
            
            
            t->addChild(face);
            _hintNode = face;
            
            
            return;
        }
        i++;
    }
    
}
void NumberTrainScene::hideHint() {
    if (_hintNode) {
        _hintNode->removeFromParent();
        _hintNode = nullptr;
    }
    
}

// =========================================================================================================================================== //

void NumberTrainScene::removeObjects()
{
    for (auto c : _answerCards) c->removeFromParent();
    _answerCards.clear();
    
    
    for (auto t : _trainCars) t->removeFromParent();
    _trainCars.clear();
}


void NumberTrainScene::setProblem(int index)
{
    CCLOG("setProblem");

    _progressBar->setCurrent(_currentProblemIndex+1, false);


    auto problem = _problem["problems"][index];
    
    auto type = problem["type"].asString();
    auto choices = problem["choices"];
    auto answers = problem["answers"];
    bool isNumber = true;
    
    removeObjects();
    
    if (type=="ascending") {
        {
            auto car = TrainCar::create();
            car->setType(TrainCar::FRONT);
            auto carSize = car->getContentSize();
            _trainCars.push_back(car);
            car->setAnimal(TrainCar::MOUSE, isNumber);
        }
        
        for (int i=0; i<answers.size(); i++) {
            auto a = answers[i];
            
            auto car = TrainCar::create();
            car->setType(i<answers.size()-1 ? TrainCar::MIDDLE : TrainCar::BACK);
            _trainCars.push_back(car);
            
            if (i==0) {
                auto flag = Sprite::create("NumberTrain/smallet_largest_startpoint_flag.png");
                flag->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                flag->setPosition(Vec2(30, car->getContentSize().height));
                car->addChild(flag);
                
            }
            
        }
        
    } else if (type=="largest") {
    
        {
            auto car = TrainCar::create();
            car->setType(TrainCar::ONE);
            auto carSize = car->getContentSize();
            CCLOG("???carsize width: %f", carSize.width);
            _trainCars.push_back(car);
            car->setAnimal(TrainCar::ELEPHANT, isNumber);
        }
    }
    
    _choices.clear();
    for (auto c : choices) {
        _choices.push_back(c.asInt());
    }
    
    _correctAnswer.clear();
    for (auto a : answers) {
        _correctAnswer.push_back(a.asInt());
    }
    
    _isReadyToSubmit = false;
    //_submitButton->setEnabled(false);
    
    trainCome(1.2);
}


void NumberTrainScene::trainCome(float delay)
{
    CCLOG("trainCome");
    float trainWidth = 0;
    for (auto c : _trainCars) {
        CCLOG("trainWidth: %f",trainWidth);
        trainWidth += c->getContentSize().width;
    }
    CCLOG("trainWidth2: %f",trainWidth);
    
    auto viewSize = _top->getContentSize();

    float margin;
    if (_trainCars.size() != 1) {
        margin = (trainGuardWidth - trainWidth)/(_trainCars.size()-1);
    }

    float offset = 0;
    
    if (_trainCars.size()>1) {
        if (margin>50) {
            offset = (margin-50)*(_trainCars.size()-1) / 2.0;
            margin = 50;
        }
    } else {
        offset = 195;
        margin = 0;
    }
    
    Vec2 pos = Vec2(viewSize.width/2 - trainGuardWidth/2 + offset, 10);
    
    for (int i=0; i<_trainCars.size(); i++) {
        auto car = _trainCars[i];
        CCLOG("for check");
        
        car->setPosition(Vec2(viewSize.width + pos.x + 300, pos.y));
        _top->addChild(car);
        
        car->runAction(Sequence::create(DelayTime::create(delay+0.03*i), EaseOut::create(MoveTo::create(1.0, pos), 4), nullptr));
        
        pos.x += car->getContentSize().width + margin;
        
        if (car->_animalType!=TrainCar::NONE) {
            car->showBubble();
            runAction(Sequence::create(
                DelayTime::create(3.3),
                             CallFunc::create([this, car](){ CCLOG("playing!"); car->soundButtonSpeech(); }),
                nullptr
            ));

        }
        
    }
    
    this->scheduleOnce([this](float){ GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Train2.m4a"); }, delay + 0.3, "trainComeEffect");
    
    _trainGuard->runAction(
        Sequence::create(
            DelayTime::create(delay+0.03*_trainCars.size()+1.2),
            EaseIn::create(MoveTo::create(0.4, _guardPos), 4),
            nullptr
        )
    );
    
    
    answerCome(2.0);
}

void NumberTrainScene::trainGo(float delay)
{
    
    auto guardDown = EaseIn::create(MoveBy::create(0.4, Vec2(0, -_trainGuard->getContentSize().height)), 4);
    _trainGuard->runAction(Sequence::create(DelayTime::create(delay), guardDown, nullptr));
    
    auto viewSize = _top->getContentSize();
    
    for (int i=0; i<_trainCars.size(); i++) {
        auto car = _trainCars[i];
        
        car->runAction(Sequence::create(DelayTime::create(delay+0.02*i+0.7), EaseIn::create(MoveBy::create(1.0, Vec2(-viewSize.width - 300, 0)), 4), nullptr));
        
        this->scheduleOnce([this](float){ GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Train1.m4a"); }, delay + 0.5, "trainGoEffect");
        
        
    }
    
    if (_currentProblemIndex < _totalProblems) {
        this->scheduleOnce([this](float){ setProblem(_currentProblemIndex); }, delay + 2.0, "setProblem");
    } else {
        this->scheduleOnce([this](float){
            removeObjects();
            auto CP = CompletePopup::create();
            CP->show(0, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);
            });

            //GameController::getInstance()->onGameComplete();
        }, delay + 2.0, "onGameComplete");
    }
    
}

void NumberTrainScene::answerCome(float delay)
{
    CCLOG("answerCome");
    auto winSize = getContentSize();
    
    float margin = 80;
    float cardWidth = 222;
    float answerWidth = 80*4+222*5;
    
    margin = (answerWidth - cardWidth*_choices.size())/(_choices.size()-1);
    
    float left = winSize.width/2 - answerWidth/2 + cardWidth/2;
    
    if (_trainCars.size()<=2) {
        
    } else {
        /*
        if (!_submitButton->isVisible()) {
            _submitButton->setVisible(true);
            _submitButton->setPosition(Vec2(winSize.width+810, bottomHeight+50));
            _submitButton->runAction(EaseIn::create(MoveBy::create(0.7, Vec2(-winSize.width/2, 0)), 3.0));
            //_submitButton->setPosition(Vec2(winSize.width/2+810, bottomHeight+50));
        }
         left -= 160;
         */
    }
    
    
    for (int i=0; i<_choices.size(); i++) {
        auto card = AnswerPiece::create();
        card->setNumber(_choices[i]);
        //auto pos = Vec2(winSize.width/2-744+300*i, 215);
        auto pos = Vec2(left + i*margin + i*cardWidth, 215);
        card->_originalPos = pos;
        
        card->onTouchBegan = [card, this]() {
            
            card->unSnap(this);
            GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Matrix_ClickBlock.m4a");
        };
        
        card->onTouchEnded = [card, this](){
            TrainCar *newSlot = nullptr;
            for (auto car : _trainCars) {
                if (car->_hasSlot) {
                    auto slotPos = car->convertToWorldSpace(car->_slotPos);
                    auto cardPos = card->getPosition();
                    if (slotPos.distance(cardPos)<=snapRadius) newSlot = car;
                }
            }
            
            if (newSlot) {
                auto existingCard = newSlot->_cardInSlot;
                
                if (existingCard) {
                    existingCard->unSnap(this);
                    
                    if (card->_targetSlot!=nullptr) {
                        existingCard->snapTarget(card->_targetSlot);
                        
                    } else {
                        
                        existingCard->_targetSlot = nullptr;
                        existingCard->runAction(MoveTo::create(0.2, existingCard->_originalPos));
                    }
                    
                }
                
                card->snapTarget(newSlot);
                
                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Matrix_RightAnswer.m4a");
                
            } else {
                card->_targetSlot = nullptr;
                card->runAction(MoveTo::create(0.2, card->_originalPos));
                
                /*
                Size winSize = getContentSize();
                Vec2 newPos = card->getPosition();
                float margin = 60;
                if (card->getPositionX() > winSize.width - margin) newPos.x = winSize.width - margin;
                if (card->getPositionX() < margin) newPos.x = margin;
                if (card->getPositionY() > winSize.height - margin) newPos.y = winSize.height - margin;
                if (card->getPositionY() < margin) newPos.y = margin;
                if (card->getPosition().distance(newPos) != 0) {
                    card->runAction(EaseOut::create( MoveTo::create( 0.3, newPos ), 1.2));
                }
                  */
                
            }
            
            this->writeActionLog();
            this->checkReadyToSubmit();
        };
        
        
        card->_targetSlot = nullptr;
        auto startPos = pos + Vec2(0, -bottomHeight);
        card->setPosition(startPos);
        addChild(card);
        _answerCards.push_back(card);
        card->runAction(Sequence::create(DelayTime::create(delay+0.1*i), EaseOut::create(MoveTo::create(0.3, pos), 1.2), nullptr));
        
        
    }
    
    //GameController::getInstance()->onReady();
}

void NumberTrainScene::answerGo()
{
    for (auto c : _answerCards) {
        if (!c->_snapped) c->runAction(EaseOut::create(MoveTo::create(0.3, c->_originalPos+Vec2(0, -bottomHeight)), 2.0));
    }
    
    trainGo(1.0);
    
    
    
}

void NumberTrainScene::writeActionLog()
{
    // NB(xenosoz, 2018): Log for future analysis

    stringstream userAnswer;
    stringstream correctAnswer;
        
    userAnswer << "[";
    correctAnswer << "[";
    
    int answerIdx = 0;
    for (auto t : _trainCars) {
        if (!t->_hasSlot) { continue; }
        
        userAnswer << (t->_cardInSlot ? TodoUtil::itos(t->_cardInSlot->_number) : "None");
        userAnswer << ", ";
        
        correctAnswer << _correctAnswer[answerIdx];
        correctAnswer << ", ";
        
        answerIdx += 1;
    }
    
    userAnswer << "]";
    correctAnswer << "]";
    
    
    auto workPath = [this] {
        stringstream ss;
        ss << "/" << "PatternTrain";
        ss << "/" << "level-" << _currentLevel << "-0";
        ss << "/" << "work-" << _currentProblemIndex;
        return ss.str();
    }();
    
    StrictLogManager::shared()->game_Peek_Answer("PatternTrain", workPath,
                                                 userAnswer.str(), correctAnswer.str());
    
}

void NumberTrainScene::checkReadyToSubmit()
{
    checkAnswer();

    /*
    _isReadyToSubmit = true;
    for (auto c : _trainCars) {
        if (c->_hasSlot && !c->_cardInSlot) _isReadyToSubmit = false;
    }
    if (_isReadyToSubmit) checkAnswer();
    
    if (_trainCars.size()<=2 && _isReadyToSubmit) {
        checkAnswer();
    } else {
        _submitButton->setEnabled(_isReadyToSubmit);
    }
    */
    
}

void NumberTrainScene::checkAnswer()
{
    _isReadyToSubmit = false;
    bool finished = true;
    
    int i=0;

    for (auto t : _trainCars) {
        if (!t->_hasSlot) {
            continue;
        } else if (!t->_cardInSlot) {
            finished = false;
        } else {
            auto card = t->_cardInSlot;
            
            CCLOG("answer %d", _correctAnswer[i]);
            CCLOG("card %d", card->_number);
            if (_correctAnswer[i]!=card->_number) {
                t->animShake();
                
                card->unSnap(this);
                card->_targetSlot = nullptr;
                card->runAction(MoveTo::create(0.2, card->_originalPos));
                
                FireworksEffect::miss();
                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/ElephantFail.m4a");
                
                return;
            }
            
        }
        i++;
    }
    
    if (finished) onCorrect();
    
}

void NumberTrainScene::onCorrect()
{
    FireworksEffect::fire();
    GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/MouseSuccess.m4a");
    
    
    for (auto card : _answerCards) {
        card->_enableTouch = false;
    }
    
    
    //GameController::getInstance()->onSolve();
    _currentProblemIndex++;
    
    for (auto c : _trainCars) {
        if (c->_animalType!=TrainCar::NONE) {
            c->hideBubble();
            c->animSuccess();
        }
    }
    answerGo();
}

void NumberTrainScene::onIncorrect()
{
    /*
    FireworksEffect::miss();
    
    int i=0;
    for (auto t : _trainCars) {
        if (t->_animalType!=TrainCar::NONE) t->animFail();
        if (!t->_hasSlot || !t->_cardInSlot) continue;
        
        auto card = t->_cardInSlot;
        if (_correctAnswer[i]!=card->_number) {
            t->animShake();
            
            card->unSnap(this);
            card->_targetSlot = nullptr;
            card->runAction(MoveTo::create(0.2, card->_originalPos));
        }
        i++;
    }
    GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/ElephantFail.m4a");
    //GameSoundManager::shared()->playEffectSound("CoreResources/Sounds/GameSounds/Matrix_WrongMove.m4a");
    
    // GameController::getInstance()->onMiss();
    
    checkReadyToSubmit();
     */
    
}
