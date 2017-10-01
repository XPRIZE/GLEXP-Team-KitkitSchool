//
//  PatternTrainScene.cpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "PatternTrainScene.hpp"

#include "CCAppController.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Effects/FireworksEffect.hpp"

#include "PatternTrainProblemBank.hpp"
#include "ChildNodes/Locomotive.hpp"
#include "ChildNodes/FreightCar.hpp"
#include "ChildNodes/Box.hpp"
#include "ChildNodes/Slot.hpp"

BEGIN_NS_PATTERNTRAIN;

const float gameNodeHeight = 1800; // 1536, 1800
const string fontName = "fonts/TodoSchoolV2.ttf";
const auto snapRadius = 150;

const string soundSlotIn = "Common/Sounds/Effect/BlockSlotIn.m4a";
const string soundTouch = "Common/Sounds/Effect/BlockTouch.m4a";
const string soundMiss = "Common/Sounds/Effect/BlockMiss.m4a";
const string soundCombine = "SoundTrain/Sound/TrainCombine.m4a";
const string soundMove = "SoundTrain/Sound/TrainMoves.m4a";
const string soundWhistle1 = "SoundTrain/Sound/Pattern_Train_1.m4a";
const string soundWhistle2 = "SoundTrain/Sound/Pattern_Train_2.m4a";
const string soundWhistle3 = "SoundTrain/Sound/Pattern_Train_3.m4a";
const string soundJump = "Common/Sounds/Effect/SFX_Jump.m4a";
const string soundCome = "NumberTrain/Sounds/Train2.m4a";

bool PatternTrainScene::init() {
    //LOGFN();
    _duplicateFlag = true;
    if (!Layer::init()) return false;
    
    _tutorialBox = nullptr;
    
    this->initUI();
    return true;
}

Scene* PatternTrainScene::createScene(int levelID)
{
    //LOGFN();
    auto scene = Scene::create();
    auto layer = PatternTrainScene::create();
    layer->setLevel(levelID);
    scene->addChild(layer);
    return scene;
}

void PatternTrainScene::setLevel(int level)
{
    LOGFN();
    _currentLevel = level;
    _problemBank = new PatternTrainProblemBank();
    _problem = _problemBank->generateProblems(_currentLevel);
    _progressBar->setMax(_problem["pattern"].size());
    _totalProblemCount = _problem["pattern"].size();
}

void PatternTrainScene::onEnter(){
    LOGFN();
    Layer::onEnter();
    
    _currentProblemIndex = 0;
    PatternTrainScene::setProblem();
}

void PatternTrainScene::onExit() {
    LOGFN();
    
    Layer::onExit();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void PatternTrainScene::onBack() {
    for (auto freightCar : _freightCars) {
        for (auto slot : freightCar->_slotsInFreightCar) {
            if (slot->_replaceable && slot->_boxInSlot) slot->_boxInSlot->_body->setVisible(false);
        }
    }
}

void PatternTrainScene::initUI() {
    LOGFN();
    
    _winSize = getContentSize();
    _gameSize = Size(_winSize.width/_winSize.height*gameNodeHeight, gameNodeHeight);
    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    float gameScale = _winSize.height/gameNodeHeight;
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    addChild(_gameNode);
    
    auto bg = Sprite::create("PatternTrain/_train_pattern_bg.png");
    auto bgSize = bg->getContentSize();
    //auto bgScale = MAX(_gameSize.width / bgSize.width, _gameSize.height / bgSize.height);
    auto bgScaleX = _gameSize.width / bgSize.width;
    auto bgScaleY = _gameSize.height / bgSize.height;
    bg->setScale(bgScaleX, bgScaleY);
    bg->setPosition(_gameSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->addChild(bg);
    
    {
        auto backBtn = ui::Button::create();
        backBtn->loadTextures("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png");
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                onBack();
                TodoSchoolBackButton::popGameScene();
            }
        };
        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
        backBtn->addClickEventListener([this](Ref*){
            onBack();
            TodoSchoolBackButton::popGameScene();
        });
        
        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backBtn->setPosition(Vec2(25, _winSize.height-25));
        _gameNode->addChild(backBtn);
    }

    //_gameNode->addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_gameSize.width/2, _gameSize.height - _progressBar->getContentSize().height));
    _gameNode->addChild(_progressBar);
    
    //GameSoundManager::getInstance()->playEffectSound(soundWhistle2);
    //GameSoundManager::getInstance()->playEffectSound(soundWhistle3);
    
    /*
     GameSoundManager::getInstance()->preloadEffect(soundSlotIn);
     GameSoundManager::getInstance()->preloadEffect(soundTouch);
     GameSoundManager::getInstance()->preloadEffect(soundMiss);
     GameSoundManager::getInstance()->preloadEffect(soundCombine);
     GameSoundManager::getInstance()->preloadEffect(soundMove);
     GameSoundManager::getInstance()->preloadEffect(soundWhistle1);
     GameSoundManager::getInstance()->preloadEffect(soundWhistle2);
     GameSoundManager::getInstance()->preloadEffect(soundWhistle3);
     */
    
}

void PatternTrainScene::setProblem(){
    LOGFN();
    _progressBar->setCurrent(_currentProblemIndex+1, false);
    
    if (_currentProblemIndex == _totalProblemCount) {
        onComplete();
        return;
    }
    
    _selectedBox.clear();
    
    _pattern = TodoUtil::split(_problem["pattern"][_currentProblemIndex].asString().c_str(), '-');
    _blank = _problem["blank"].asInt();
    _choice = _problem["choice"].asInt();
    _shape = _problem["shape"].asInt();
    
    _freightCarCount = 2;
    if (_currentLevel > 3 && _pattern.size() == 2) _freightCarCount = 3;
    
    int i = 0;
    for (string shape : _problemBank->getShapeVec((PatternTrainProblemBank::shape)_shape)) {
        
        switch(i) {
            case 0: _selectedBox["A"] = shape; break;
            case 1: _selectedBox["B"] = shape; break;
            case 2: _selectedBox["C"] = shape; break;
        }
        i++;
    }
    
    CCLOG("selectbox a: %s", _selectedBox["A"].c_str());
    CCLOG("selectbox b: %s", _selectedBox["B"].c_str());
    CCLOG("selectbox c: %s", _selectedBox["C"].c_str());
    
    //float volume = GameSoundManager::getInstance()->getEffectSoundVolume();

    GameSoundManager::getInstance()->playEffectSound(soundWhistle1, false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound(soundWhistle2, false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound(soundWhistle3, false, 1.0f, 0, 0);

    /*
    runAction(Sequence::create(
        CallFunc::create([this](){
            GameSoundManager::getInstance()->setEffectSoundVolume(0);
            GameSoundManager::getInstance()->playEffectSound(soundWhistle1);
            GameSoundManager::getInstance()->playEffectSound(soundWhistle2);
            GameSoundManager::getInstance()->playEffectSound(soundWhistle3);
            GameSoundManager::getInstance()->playEffectSound(soundJump);
        }),
        DelayTime::create(1),
        CallFunc::create([this, volume](){
            GameSoundManager::getInstance()->setEffectSoundVolume(volume);
        }),
        nullptr
    ));
     */
    
    //GameSoundManager::getInstance()->setEffectSoundVolume(test);
    
    
    trainCome();
    if (_currentLevel == 1 && _currentProblemIndex == 0) {
      // tutorial
    } else answerCome();
}

void PatternTrainScene::onComplete() {
    LOGFN();
    auto CP = CompletePopup::create();
    CP->show(0, [this] {
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

Slot *PatternTrainScene::replaceableSlotNearByBox(Box *box) {
    //LOGFN();
    
    Slot *newSlot = nullptr;
    
    for (auto freightCar : _freightCars) {

        for (auto slot : freightCar->_slotsInFreightCar) {

            if (slot->_replaceable) {
                auto slotPos = slot->convertToWorldSpace(slot->_pos);
                auto boxPos = box->getPosition();
                if (slotPos.distance(boxPos)<=snapRadius) newSlot = slot;
                /*
                if (slotPos.distance(boxPos)<=snapRadius) {
                    CCLOG("YES!!!");
                } else CCLOG("NO!!!");
                CCLOG("slotPos x:%f", slotPos.x);
                CCLOG("slotPos y:%f", slotPos.y);
                CCLOG("boxPos x:%f", boxPos.x);
                CCLOG("boxPos y:%f", boxPos.y);
                 */
            }
        }
    }
    return newSlot;
}

Node *PatternTrainScene::answerAreaCannotCatchBox() {
    Node *targetNode = nullptr;
    
    bool judge;
    
    for (auto answerArea : _AnswerAreas) {
        auto answerAreaPos = _gameNode->convertToWorldSpace(answerArea->getPosition());
        judge = true;
        //CCLOG("answerAreaPos %f, %f", answerAreaPos.x, answerAreaPos.y);
        
        for (auto box : _boxes) {
            auto boxPos = _gameNode->convertToWorldSpace(box->getPosition());
            if (answerAreaPos.distance(boxPos) < 200) judge = false;
            
            //CCLOG("boxPos %f, %f", boxPos.x, boxPos.y);
        }
        if (judge) {
            targetNode = answerArea;
            break;
        }
    }
    
    return targetNode;
}

Box* PatternTrainScene::createAnswerBox(string letter, Vec2 pos, bool setVisible) {
    //LOGFN();
    
    auto box = Box::create();
    box->setType(letter, _selectedBox[letter.c_str()], (PatternTrainProblemBank::shape)_shape, true);
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(pos.x, pos.y-500);
    if (!setVisible) box->setVisible(false);
    
    box->_originalPos = pos;
    
    box->onTouchBegan = [box, this]() {
        if (_currentProblemIndex == 0 && _currentLevel == 1) {
            box->_originalPos = Vec2(_winSize.width/2,300);
        }
        box->setGlobalZOrderManual(100);
        GameSoundManager::getInstance()->playEffectSound(soundTouch);
        box->unload(_gameNode);
    };
    
    box->onTouchMoved = [box, this]() {
        Slot *newSlot = nullptr;
        newSlot = replaceableSlotNearByBox(box);
        if (newSlot == nullptr) {
            box->runAction(ScaleTo::create(0.05, 1));
        } else {
            box->runAction(ScaleTo::create(0.05, 0.88));
        }
        /*
         auto answerArea = answerAreaCannotCatchBox();
         if (answerArea != nullptr) {
         duplicateAnswerBox(answerArea);
         }*/
    };
    
    box->onTouchEnded = [box, this](){
        box->setGlobalZOrderManual(1);
        _reservedBoxWork = false;
        _reservedLetter = "";
        _reservedOriginBox = nullptr;
        
        Slot *newSlot = nullptr;
        newSlot = replaceableSlotNearByBox(box);
        
        if (newSlot) {
            auto existingBox = newSlot->_boxInSlot;
            
            if (existingBox) {
                existingBox->unload(_gameNode);
                
                if (box->_targetSlot!=nullptr) {
                    existingBox->loadOnSlot(box->_targetSlot);
                } else {
                    existingBox->_targetSlot = nullptr;
                    existingBox->runAction(MoveTo::create(0.2, existingBox->_originalPos));
                    existingBox->runAction(ScaleTo::create(0.05, 1));
                }
                
            }
            
            if (_currentLevel > 3 && !box->_duplicated) {
                _reservedBoxWork = true;
                _reservedLetter = box->_assignedLetter;
                _reservedNewBoxPos = box->_originalPos;
                _reservedOriginBox = box;
            }
            box->loadOnSlot(newSlot);
            GameSoundManager::getInstance()->playEffectSound(soundSlotIn);
        } else {
            box->_targetSlot = nullptr;
            box->runAction(MoveTo::create(0.2, box->_originalPos));
        }
        this->checkReadyToSubmit();
    };
    
    box->_targetSlot = nullptr;
    _gameNode->addChild(box);
    _boxes.push_back(box);
    
    return box;
}

void PatternTrainScene::generateNewAnswerBox() {
    if (!_reservedBoxWork) return;
    
    _reservedOriginBox->_duplicated = true;
    Box *newBox;
    
    if (_reservedLetter == "A") {
        if (_reservedAnswerA.size()) {
            newBox = _reservedAnswerA[_reservedAnswerA.size()-1];
            _reservedAnswerA.pop_back();
        }
    }
    if (_reservedLetter == "B") {
        if (_reservedAnswerB.size()) {
            newBox = _reservedAnswerB[_reservedAnswerB.size()-1];
            _reservedAnswerB.pop_back();
        }
    }
    if (_reservedLetter == "C") {
        if (_reservedAnswerC.size()) {
            newBox = _reservedAnswerC[_reservedAnswerC.size()-1];
            _reservedAnswerC.pop_back();
        }
    }
    
    if (newBox != nullptr) {
        newBox->runAction(Sequence::create(EaseOut::create(MoveTo::create(0.3, _reservedNewBoxPos), 1.2), nullptr));
    }
}

void PatternTrainScene::answerCome() {
    //LOGFN();
    
    srand((unsigned)time(0));
    
    int i = 0;
    float margin = 50;
    float boxWidth = 264;
    float answerTotalWidth = 0;
    
    vector<string> ab;
    vector<string> abc;
    abc.push_back("A");
    ab.push_back("A");
    abc.push_back("B");
    ab.push_back("B");
    abc.push_back("C");
    
    random_shuffle(abc.begin(), abc.end(), [](int n) { return rand() % n; });
    random_shuffle(ab.begin(), ab.end(), [](int n) { return rand() % n; });
    
    if (_currentLevel > 1) {
        answerTotalWidth += boxWidth*abc.size() + margin*(abc.size()-1);
    } else {
        answerTotalWidth += boxWidth*ab.size() + margin*(ab.size()-1);
    }
    
    float answerBasePosX = _gameSize.width/2 - answerTotalWidth/2;
    float answerBasePosY = 300;
    answerBasePosX += boxWidth/2;
    
    for (auto letter : (_currentLevel>1 ? abc : ab)) {
        
        auto pos = Vec2(answerBasePosX+(boxWidth*i)+(margin*i), answerBasePosY);
        auto startPos = Vec2(pos.x, pos.y-500);
        auto box = createAnswerBox(letter, pos);
        auto box2 = createAnswerBox(letter, pos);
        auto box3 = createAnswerBox(letter, pos);
        box2->setPosition(startPos);
        box3->setPosition(startPos);
        if (letter == "A") {
            _reservedAnswerA.push_back(box2);
            _reservedAnswerA.push_back(box3);
        }
        if (letter == "B") {
            _reservedAnswerB.push_back(box2);
            _reservedAnswerB.push_back(box3);
        }
        if (letter == "C") {
            _reservedAnswerC.push_back(box2);
            _reservedAnswerC.push_back(box3);
        }
        box->runAction(Sequence::create(DelayTime::create(1.3+0.1*i), EaseOut::create(MoveTo::create(0.3, pos), 1.2), nullptr));
        i++;
    }
    
}

float railHeight = 645;
float connectorMargin = 47;
float purgeMargin = 0;
float trainSpeed = 2500;

float PatternTrainScene::getMoveTime(float px) {
    return px / trainSpeed;
}

void PatternTrainScene::trainCome() {
    
    _totalWidth = 0;
    
    auto locomotive = Locomotive::create();
    _gameNode->addChild(locomotive);
    locomotive->setPosition(_gameSize.width, railHeight);
    _locomotive = locomotive;
    
    _totalWidth += locomotive->getContentSize().width + connectorMargin-5;
    _freightCarPos.push_back(_totalWidth);
    
    for (int i=0; i<_freightCarCount; i++) {
        auto freightCar = FreightCar::create();
        freightCar->setType((int)_pattern.size());
        _gameNode->addChild(freightCar);
        freightCar->setPosition(_gameSize.width+_totalWidth, railHeight);
        _freightCars.push_back(freightCar);
        _totalWidth += connectorMargin + freightCar->getContentSize().width;
        
        _freightCarPos.push_back(_totalWidth);
        
        int randomBoxSequence = rand()%_pattern.size();
        
        int j=0;
        for (auto letter : _pattern) {
            auto box = Box::create();
            bool setBlank = false;
            
            if (_freightCarCount == i+1) {
                switch((PatternTrainProblemBank::blank)_blank) {
                    case PatternTrainProblemBank::ALL :
                        setBlank = true;
                        break;
                    case PatternTrainProblemBank::RANDOM :
                        if (randomBoxSequence == j) setBlank = true;
                        break;
                    case PatternTrainProblemBank::LAST :
                        if (_pattern.size() == j+1) setBlank = true;
                        break;
                }
                
            }
            if (setBlank) {
                freightCar->_slotsInFreightCar[j]->setBlank(letter);
                if (_currentLevel == 1 && _currentProblemIndex == 0) {
                    _tutorialBox = PatternTrainScene::createAnswerBox("A", Vec2(0,0), false);
                    _tutorialBox->setGlobalZOrderManual(100);
                    _tutorialBox->setScale(0.88);
                    _tutorialBox->loadOnSlot(freightCar->_slotsInFreightCar[j]);
                    freightCar->loadBox(_tutorialBox, j);
                }
            } else {
                box->setType(letter, _selectedBox[letter.c_str()], (PatternTrainProblemBank::shape)_shape);
                freightCar->loadBox(box, j);
            }
            j++;
        }
    }
    
    float arrivePointLC = _gameSize.width/2 - _totalWidth/2;
    
    runAction(Sequence::create(
        DelayTime::create(0.5),
        CallFunc::create([this, locomotive, arrivePointLC](){
            GameSoundManager::getInstance()->playEffectSound(soundCome);
            locomotive->runAction(EaseOut::create(MoveTo::create(getMoveTime(_winSize.width-arrivePointLC), Vec2(arrivePointLC, railHeight)), 2));
        }),
        nullptr
    ));
    
    int i = 0;
    for (auto freightCar : _freightCars) {
        float startPointFC = _freightCarPos[i] + _winSize.width;
        float arrivePointFC = arrivePointLC + _freightCarPos[i] + purgeMargin*(i+1);
        runAction(Sequence::create(
            DelayTime::create(0.5),
            CallFunc::create([this, freightCar, startPointFC, arrivePointFC](){
                freightCar->runAction(EaseOut::create(MoveTo::create(getMoveTime(startPointFC+-arrivePointFC), Vec2(arrivePointFC, railHeight)), 2));
            }),
            nullptr
        ));
        i++;
    }
    
    if (_currentLevel == 1 && _currentProblemIndex == 0) {
        runAction(Sequence::create(
            DelayTime::create(0.9),
            CallFunc::create([this](){
                _tutorialBox->setVisible(true);
            }),
            DelayTime::create(1.9),
            CallFunc::create([this](){
                _tutorialBox->unload(_gameNode);
                _tutorialBox->_targetSlot = nullptr;
                GameSoundManager::getInstance()->playEffectSound(soundJump);
                _tutorialBox->runAction(JumpBy::create(0.5, Point(-600,-500), 350, 1));
            }),
            nullptr
        ));
    }
}

void PatternTrainScene::checkReadyToSubmit() {
    LOGFN();
    
    checkAnswer();
    /*
    bool isReadyToSubmit = true;
    
    for (auto freightCar : _freightCars) {
        for (auto slot : freightCar->_slotsInFreightCar) {
            if (slot->_replaceable && !slot->_boxInSlot) isReadyToSubmit = false;
        }
    }
    if (isReadyToSubmit) checkAnswer();
    else {
        generateNewAnswerBox();
    }
     */
}

void PatternTrainScene::checkAnswer() {
    LOGFN();
    bool hasIncorrectAnswer = false;
    bool finished = true;
    vector<string> wrongAnswer;
    
    for (auto freightCar : _freightCars) {
        for (auto slot : freightCar->_slotsInFreightCar) {

            if (!slot->_replaceable) continue;
            if (!slot->_boxInSlot) {
                finished = false;
                continue;
            }

            if (slot->_boxInSlot->_assignedLetter != slot->_correctAnswer) {
                wrongAnswer.push_back(slot->_boxInSlot->_assignedLetter);
                auto box = slot->_boxInSlot;
                box->unload(_gameNode);
                box->_targetSlot = nullptr;
                box->runAction(MoveTo::create(0.2, box->_originalPos));
                box->setScale(1);
                GameSoundManager::getInstance()->playEffectSound(soundMiss);
                hasIncorrectAnswer = true;
            }
        }
    }
    
    if (hasIncorrectAnswer) {
        FireworksEffect::miss();
        if (wrongAnswer.size() && find(wrongAnswer.begin(), wrongAnswer.end(), _reservedLetter) == wrongAnswer.end()) generateNewAnswerBox();
    } else if (!finished) generateNewAnswerBox();
    
    
    if (finished && !hasIncorrectAnswer) onCorrect();
    
}

void PatternTrainScene::onCorrect() {

    FireworksEffect::fire();

    answerGo();
    
    int jumpCount = 0;
    for (auto freightCar : _freightCars) {
        jumpCount += freightCar->_slotsInFreightCar.size();
    };
    
    for (auto box : _boxes) {
        box->_enableTouch = false;
    }
    
    float delayJump = 0.25 * jumpCount+0.6;
    float delayCombine = 0.1;
    float delayGo = 2.0;
    
    runAction(Sequence::create(
        CallFunc::create([this](){ trainJump(); }),
        DelayTime::create(delayJump),
        CallFunc::create([this](){ trainCombine(); }),
        DelayTime::create(delayCombine),
        CallFunc::create([this](){ trainGo(); }),
        DelayTime::create(delayGo),
        CallFunc::create([this](){
            removeObjects();
            _currentProblemIndex++;
            setProblem();
        }),
        nullptr
    ));
}

void PatternTrainScene::trainJump() {
    
    int i = 0;
    for (auto freightCar : _freightCars) {
        for (auto slot : freightCar->_slotsInFreightCar) {
            int soundNumber;
            if (slot->_boxInSlot->_assignedLetter == "A") soundNumber = 1;
            else if (slot->_boxInSlot->_assignedLetter == "B") soundNumber = 2;
            else if (slot->_boxInSlot->_assignedLetter == "C") soundNumber = 3;
            runAction(Sequence::create(
                DelayTime::create(0.25*(i+1)),
                CallFunc::create([this, slot, soundNumber](){
                    slot->runAction(JumpBy::create(0.5, Point(0,0), 50, 1));
                    GameSoundManager::getInstance()->playEffectSound("SoundTrain/Sound/Pattern_Train_"+TodoUtil::itos(soundNumber)+".m4a");
                }),
                nullptr
            ));
            i++;
        }
    }
}

void PatternTrainScene::trainGo(){
    CCLOG("trainGo!!");
    GameSoundManager::getInstance()->playEffectSound(soundMove);
    _locomotive->runAction(EaseIn::create(MoveBy::create(1.5, Vec2(_gameNode->getContentSize().width*-1,0)), 1.5));
}

void PatternTrainScene::answerGo() {
    for (auto v : _boxes) {
        if (v->_targetSlot == nullptr) {
            v->runAction(EaseIn::create(MoveTo::create(0.5, Vec2(_gameSize.width/2,-500)), 1.5));
        }
    }
}

void PatternTrainScene::trainCombine() {
    CCLOG("trainCombine!!");
    
    runAction(Sequence::create(
        CallFunc::create([this](){
            //_locomotive->runAction(EaseOut::create(MoveBy::create(0.2, Vec2(purgeMargin, 0)), 2));
        }),
        CallFunc::create([this](){
            for (int i=0; i<_freightCars.size(); i++) {
                auto freightCar = _freightCars[i];
                runAction(Sequence::create(
                    //DelayTime::create(0.21*(i+1)),
                    CallFunc::create([this, freightCar](){
                        freightCar->retain();
                        freightCar->removeFromParent();
                        _locomotive->addChild(freightCar);
                        auto trainWorldSpace = _gameNode->convertToWorldSpace(freightCar->getPosition());
                        auto trainNewNodeSpace = _locomotive->convertToNodeSpace(trainWorldSpace);
                        freightCar->setPosition(trainNewNodeSpace);
                        freightCar->release();
                        //if (i < _freightCars.size()-1) _locomotive->runAction(EaseOut::create(MoveBy::create(0.2, Vec2(purgeMargin, 0)),2.0));
                    }),
                    //DelayTime::create(0.6),
                    CallFunc::create([this](){
                        CCLOG("Combine end!");
                        //GameSoundManager::getInstance()->playEffectSound(soundCombine);
                    }),
                    nullptr
                ));
            }
        }),
        nullptr
    ));
    
}

void PatternTrainScene::removeObjects() {
    for (auto v : _boxes) {
        v->setVisible(false);
        v->removeFromParent();
    }
    for (auto v : _freightCars) v->removeFromParent();
    _freightCars.clear();
    _boxes.clear();
    _pattern.clear();
    _freightCars.clear();
    _freightCarPos.clear();
    _reservedAnswerA.clear();
    _reservedAnswerB.clear();
    _reservedAnswerC.clear();
    _tutorialBox = nullptr;
    
}


END_NS_PATTERNTRAIN;

