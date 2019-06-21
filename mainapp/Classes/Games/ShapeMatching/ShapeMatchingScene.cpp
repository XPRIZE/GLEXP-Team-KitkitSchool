//
//  GameScene.cpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#include <Managers/VoiceMoldManager.h>
#include "SimpleAudioEngine.h"
#include "ShapeMatchingScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
//#include "Common/Controls/CompletePopup.hpp"
//#include "Common/Controls/TodoSchoolBackButton.hpp"
//#include "Utils/Random.h"
#include "Utils/SMCardSlotBuilder.hpp"
#include "Utils/Random.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
//#include "Managers/LocalizationManager.hpp"
#include "Utils/TodoUtil.h"

#include "CCAppController.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Common/Controls/CompletePopup.hpp"


#define MAX_CHARACTER_COUNT 20
#define Z_ORDER_BACKGROUND 10
#define Z_ORDER_DEFAULT 30
#define Z_ORDER_PARTICLE 1000

using namespace cocostudio;
using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d;
using namespace ShapeMatching;

namespace ShapeMatching
{
    const char* SOLVE_EFFECT_SOUND = "Common/Sounds/Effect/UI_Star_Collected.m4a";
    const string CARD_MATCHING_EFFECT_SOUND = "ShapeMatching/Sound/Train_Slotin.m4a";
    const string STAR_EFFECT_SOUND = "ShapeMatching/Sound/Quiz_Correct.m4a";

    string soundPathForShape(const string& shapeName)
    {
        stringstream ss;
        ss <<  shapeName ;
        return ss.str();
    }
    
}

//Scene* ShapeMatchingScene::createScene(int levelID)
//{
//    // 'scene' is an autorelease object
//    auto scene = Scene::create();
//
//    // 'layer' is an autorelease object
//    auto layer = ShapeMatchingScene::create();
//    layer->setLevel(levelID);
//
//    // add layer as a child to scene
//    scene->addChild(layer);
//
//    // return the scene
//    return scene;
//}

std::map<std::string,float> ShapeMatchingScene::loadDurationsheet() {
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("ShapeMatching/Sound/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);
    
    std::map<std::string,float> rt;
    rt.clear();
    
    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        rt[row[0]] = rowDuration;
    }
    return rt;
}

Scene* ShapeMatchingScene::createScene(int levelID)
{
    //LOGFN();
    auto scene = Scene::create();
    auto layer = ShapeMatchingScene::create();
    layer->setLevel(levelID);
    scene->addChild(layer);
    return scene;
}

bool ShapeMatchingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //scheduleUpdate();
    
    auto pos = getPosition();
    
    
    _zOrder = Z_ORDER_DEFAULT;
    auto winSize = getContentSize();
    this->setContentSize(winSize);
    
    _gameNode = Node::create();
    _gameSize = Size(2560, 1800);//
    //    auto gameSize = winSize;
    _gameNode->setContentSize(_gameSize);
    
    //    auto bg = Sprite::create("ShapeMatching/Images/matching_bg_bigger.png");
    auto bg = LayerColor::create(Color4B(140,189,51,255));
    bg->setContentSize(winSize);
    //bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //bg->setPosition(winSize/2);
    bg->setPosition(Vec2::ZERO);
    addChild(bg);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _gameSize.height-25));
    _gameNode->addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_gameSize.width/2, _gameSize.height - _progressBar->getContentSize().height));
    _gameNode->addChild(_progressBar);
    
    float gameScale = MIN(winSize.width/_gameSize.width, winSize.height/_gameSize.height);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    //    _gameNode->setPosition(Vec2::ZERO);
    addChild(_gameNode);
    
    // only for play synthesized sentence.
    //_speechButton = TodoSpeechButtonCC::create();
    //_speechButton->setPosition(Vec2(-500,-500));
    //addChild(_speechButton);
    
    //    // Back Button
    //    auto backButton = TodoSchoolBackButton::create();
    //    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //    backButton->setPosition(Vec2(25, winSize.height-25));
    //    addChild(backButton);
    //
    //    // Progress Bar
    //    _progressBar = ProgressIndicator::create();
    //    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    //    _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1);
    //    addChild(_progressBar);
    
    // Next Button
    //    const auto normalColor = Color3B(92, 46, 28);
    //    const auto highlightColor = Color3B(255, 241, 217);
    //    _nextButton = Button::create();
    //    _nextButton->loadTextures("ShapeMatching/Images/matchgame_next_inactivated.png", "ShapeMatching/Images/matchgame_next_activated.png");
    //    _nextButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //    _nextButton->setPosition(Vec2(winSize.width - _nextButton->getContentSize().width - 40, _nextButton->getContentSize().height + 30));
    //    _nextButton->setTitleFontName("fonts/chanakya.ttf");
    //    _nextButton->setTitleFontSize(80.f);
    //    _nextButton->setTitleColor(normalColor);
    //    _nextButton->setTitleText(nextButtonTitle());
    //    _nextButton->addTouchEventListener([=](Ref* sender,Widget::TouchEventType event) {
    //        switch (event)
    //        {
    //            case Widget::TouchEventType::BEGAN:
    //                _nextButton->setTitleColor(highlightColor);
    //                break;
    //            case Widget::TouchEventType::CANCELED:
    //            case Widget::TouchEventType::ENDED:
    //                _nextButton->setTitleColor(normalColor);
    //                break;
    //            default:
    //                break;
    //        }
    //    });
    //    _nextButton->addClickEventListener([this](Ref*) {
    //        advanceToNextProblem();
    //    });
    //    _nextButton->setVisible(false);
    //    addChild(_nextButton);
    
    //preloadShiningParticle();
    //bindingEvents();
    
    isAnimating = false;
    _animationBG = nullptr;
    
    return true;
}
//
void ShapeMatchingScene::onEnter()
{
    Layer::onEnter();
    
    prepareNewGameWithLevel();
    
    onStart();

}

//void ShapeMatchingScene::onEnterTransitionDidFinish()
//{
//    //onStart();
//}

void ShapeMatchingScene::prepareNewGameWithLevel()
{
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto data = LevelData::defaultData();
    auto sheet = data.randomSheetFor(lang, _currentLevelID, &_currentWorksheetID);

    _duration = loadDurationsheet();

    _levelData = data;
    _currentWorksheet = sheet;
    _currentProblemID = sheet.beginProblemID();
    _progressBar->setMax((int)sheet.size());
    
    _colors = {"1", "2"};
    
    _sizes = {"large", "medium", "small"};
    
    _rotation = {0,30,45,90};
    
}

void ShapeMatchingScene::handlePause() {}
void ShapeMatchingScene::handleResume() {}
void ShapeMatchingScene::handleStart(){
    // GameController::getInstance()->onBeginWithTotalProblems(_currentWorksheet.endProblemID());
    onStart();
    
}
void ShapeMatchingScene::handleQuit(){
    this->stopAllActions();
    this->unscheduleAllCallbacks();
}


void ShapeMatchingScene::setLevel(int levelID) {
    _currentLevelID = levelID;
}

void ShapeMatchingScene::onStart()
{
    //GameController::getInstance()->onReady();
    
    _nextTransition = false;
    
    auto problem = _currentWorksheet.problemByID(_currentProblemID);
    _currentProblem = problem;
    
    _progressBar->setCurrent(_currentProblemID, false);
    
    //_nextButton->setVisible(false);
    _currentLevelMaxCardCount = (int)problem.pieces[0].numOfPices;
    //_progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1);
    
    //    if (_currentLevelID == 1)
    //    {
    //        _defaultScaleFactor = 1.0f;
    //        _upScaleFactor = 1.2f;
    //    }
    //    else
    //    {
    _defaultScaleFactor = 0.8f;
    _upScaleFactor = 0.9f;
    //    }
    
    _matchCount = 0;
    initCardList();
    
}

void ShapeMatchingScene::onSolve()
{
    GameSoundManager::getInstance()->playEffectSound(SOLVE_EFFECT_SOUND);
    
    //_progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1, true);
    
    // GameController::getInstance()->onSolve();
    
    if (_currentProblemID == _currentWorksheet.endProblemID())
    {
        auto seq = Sequence::create(DelayTime::create(1.f), CallFunc::create([this]() {
            
            auto CP = CompletePopup::create();
            CP->show(1.5f, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
            // GameController::getInstance()->onGameComplete();
        }), NULL);
        
        runAction(seq);
        
        
        //        CompletePopup::create()->show(1.0f, [](){
        //            CCAppController::sharedAppController()->handleGameComplete(1);
        //        //    TodoSchoolBackButton::popGameScene();
        //        });
    }
    else
    {
        auto seq = Sequence::create(DelayTime::create(1.7f), CallFunc::create([this]() {
            advanceToNextProblem();
        }), NULL);
        
        runAction(seq);
        
    }
}

void ShapeMatchingScene::advanceToNextProblem() {
    if (_nextTransition) { return; }
    _nextTransition = true;
    
    _currentProblemID++;
    _progressBar->setCurrent(_currentProblemID, false);
    
    runAction(Sequence::create(DelayTime::create(0.2f),
                               CallFunc::create([this]{ onStart(); }), nullptr));
}


void ShapeMatchingScene::initCardList()
{
    // NB(xenosoz, 2016): Create grid first and choose points to reduce collisions.
    
    size_t cardCount = _currentLevelMaxCardCount * 2;
    Rect boardRect = Rect(Point(0.f, 0.f), _gameNode->getContentSize());
    Size cardSize = ShapeMatchingCard::defaultSize();
    
    
    Rect leftRect(boardRect.getMinX(), boardRect.getMinY(),
                  boardRect.size.width/2.f, boardRect.size.height);
    Rect rightRect(boardRect.getMidX(), boardRect.getMinY(),
                   boardRect.size.width/2.f, boardRect.size.height);
    
    size_t leftCount = (size_t)ceil(cardCount / 2.f);
    size_t rightCount = cardCount - leftCount;
    if (random(0, 1) == 0) { std::swap(leftCount, rightCount); }
    
    
    vector<Point> selected;
    auto appendSelected = [&](size_t count, Rect localRect) {
        auto points = shapematching::SMCardSlotBuilder().pointsInBoundary(count, localRect, cardSize);
        auto it = todoschool::sample(points, count);
        selected.insert(selected.end(),
                        make_move_iterator(it.begin()),
                        make_move_iterator(it.end()));
    };
    appendSelected(leftCount, leftRect);
    appendSelected(rightCount, rightRect);
    
    auto pieceInfo = _currentProblem.pieces[0];
    
    
    _shapeCandidates.clear();
    
    for (const auto& group_item : _levelData.groups[pieceInfo.shape]) {
        string group_key_i = group_item.first;
        const vector<string>& shape_keys = group_item.second;
        
        if (shape_keys.size()) {
            string selection = todoschool::Random::shared().sample(shape_keys, 1)[0];
            _shapeCandidates.push_back(selection);
        }
    }
    
    for (int i = 0; i < _currentLevelMaxCardCount; i++)
    {
        
        ValueMap info = pickCards(pieceInfo, i);
        std::string filenameA = info["filenameA"].asString();
        int rotationA = info["rotationA"].asInt();
        std::string filenameB = info["filenameB"].asString();
        int rotationB = info["rotationB"].asInt();
        
        float scaleA = info["scaleA"].asFloat();
        float scaleB = info["scaleB"].asFloat();
        
        auto cardA = createMatchingCard(++_zOrder, selected[i*2+0],
                                        _currentProblemID, i, 1,
                                        filenameA, rotationA, scaleA, pieceInfo.matchSound == "YES", pieceInfo.matchSound != "YES");
        auto cardB = createMatchingCard(++_zOrder, selected[i*2+1],
                                        _currentProblemID, i, 2,
                                        filenameB, rotationB, scaleB, pieceInfo.matchSound == "YES", pieceInfo.matchSound != "YES");
        

        string lang = LanguageManager::getInstance()->getCurrentLocaleCode();
        string shapeString = _shapeCandidates[i];
        shapeString = _levelData.shapes[shapeString].languages[lang];

        cardA->matchSound = soundPathForShape(shapeString);
        cardB->matchSound = soundPathForShape(shapeString);
        
        //        SoundEffect sound;
        //        sound = sound || SoundEffect(pieceInfo.matchSound);
        //        sound = sound || SoundEffect("ShapeMatching/Sound/" + pieceInfo.matchSound);
        //        sound = sound || SoundEffect("ShapeMatching/Images/Number/" + pieceInfo.matchSound);
        //        sound = sound || SoundEffect("ShapeMatching/Sound/star.wav");
        
        //        sound.preload();
        //        cardA->matchSound = sound;
        //        cardB->matchSound = sound;
        matchingCardList.push_back(cardA);
        matchingCardList.push_back(cardB);
        
        
    }
    
    float scaleFactor = 1;
    
    for (auto it : matchingCardList) {
        if (it->getChildByName("shape") == nullptr) continue;
        auto labelSize = it->getChildByName("shape")->getContentSize();
        if (labelSize.width > 580) scaleFactor = MIN(580 / labelSize.width, scaleFactor);
    }
    if (scaleFactor != 1) {
        for (auto it : matchingCardList) {
            if (it->getChildByName("shape") == nullptr) continue;
            it->setShapeScale(scaleFactor);
        }
    }
}

ShapeMatchingCard* ShapeMatchingScene::createMatchingCard(int zOrder, Point pos,
                                                          int level, int id, int type,
                                                          const std::string& cardImageName,
                                                          int rotation, float scale,
                                                          bool hasTouchSound, bool hasMatchingSound)
{
    string lang = LanguageManager::getInstance()->getCurrentLocaleCode();
    string localizedName = _levelData.shapes[cardImageName].languages[lang];
    
    ShapeMatchingCard* card = ShapeMatchingCard::create();
    card->setImage(level, type, id,
                   cardImageName, localizedName,
                   rotation, scale);
    card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    card->setPosition(pos);
    card->setScale(_defaultScaleFactor);
    _gameNode->addChild(card, zOrder);
    card->defaultScale = _defaultScaleFactor;
    card->hasTouchSound = hasTouchSound;
    card->hasMatchingSound = hasMatchingSound;
    bindingEvents(card);
    
    return card;
}

void ShapeMatchingScene::bindingEvents(ShapeMatchingCard* card)
{
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this, card](Touch* touch, Event* event) {
        if (isAnimating) return false;
        
        Point touchPoint = touch->getLocation();
        Point viewPoint = _gameNode->convertToNodeSpace(touchPoint);
        
        if (card->isTouched) return false;
        if (card->isMatchDone) return false;
        if (!card->isTouchedIn(viewPoint)) return false;
        
        card->isTouched = true;
        
        if (card->hasTouchSound){
            //_speechButton->setSpeechSentence(card->matchSound);
            //if(_speechButton->isSelected()){
            //    _speechButton->stopPlaying();
            //}
            //_speechButton->onSpeechPressed(this, ui::Widget::TouchEventType::ENDED);
        }
        GameSoundManager::getInstance()->playEffectSound(CARD_MATCHING_EFFECT_SOUND);
        
        card->runAction(ScaleTo::create(0.1f, _upScaleFactor));
        _gameNode->reorderChild(card, ++_zOrder);
        return true;
        
    };
    
    listener->onTouchMoved = [this, card](Touch* touch, Event* event) {
        if (isAnimating) return false;
        
        card->setPosition(card->getPosition() + touch->getDelta());
        
        if (card->isLinked) {
            if (!shouldKeepAsPair(card->linkedTarget, card)) {
                card->linkedTarget->setLink(nullptr);
                card->setLink(nullptr);
            }
        }
        else {
            for (auto other : matchingCardList) {
                if (other == card) continue;
                if (other->isLinked) continue;
                if (other->isMatchDone) continue;
                
                if (other->id == card->id) {
                    
                    if (isExistPair() == false && shouldBecomePair(other, card)) {
                        other->runAction(EaseElasticOut::create(ScaleTo::create(0.3f, _upScaleFactor)));
                        other->setLink(card);
                        card->setLink(other);
                        _gameNode->reorderChild(other, ++_zOrder);
                        _gameNode->reorderChild(card, ++_zOrder);
                        
                    }
                }
            }
        }
        return true;
    };
    
    
    listener->onTouchEnded =  [this, card](Touch* touch, Event* event) {
        card->isTouched = false;
        
        if (isAnimating) return false;
        
        card->stopParticle();
        
        if (card->isLinked && !card->isMatchDone) {
            isAnimating = true;
            
            float duration = 1.f;
            auto fileNameVec = TodoUtil::split(card->matchSound, '/');
            auto playFileName = fileNameVec.back();
            if (_duration.count(playFileName)) duration = _duration[playFileName];
          VoiceMoldManager::shared()->speak(card->matchSound); //tts implementation for htis module
            
            auto other = card->linkedTarget;
            
            // set animation bg large enough to cover whole screen
            // auto winSize = GameController::getInstance()->getGameWindowSize();
            auto winSize = getContentSize();
            _animationBG = LayerColor::create(Color4B(0, 0, 0, 0.8*255), winSize.width*3, winSize.height*3);
            _animationBG->setPosition(Vec2(-winSize.width,-winSize.height));
            
            int zOrder = MIN(card->getLocalZOrder(),other->getLocalZOrder()) -1;
            _gameNode->addChild(_animationBG, zOrder);
            
            for (auto c : matchingCardList)
            {
                if (c->isMatchDone || c == card || c == other)
                    continue;
                
                c->setLocalZOrder(zOrder - 1);
            }
            
            other->stopParticle();
            
            card->isMatchDone = true;
            other->isMatchDone = true;
            
            auto moveTo = MoveTo::create(0.4f, other->getPosition());
            
            //            card->runAction(Sequence::create(EaseElasticOut::create(moveTo), CallFunc::create([=](){
            ////                other->setPosition(Vec2::ONE * 9999.0f);
            //            }), nullptr));
            card->runAction(EaseElasticOut::create(moveTo));
            
            card->runAction(Sequence::create(DelayTime::create(0.3f), EaseBackIn::create(ScaleTo::create(0.3f, _upScaleFactor)), nullptr));
            
            ShapeMatchingCard* primaryCard, *secondaryCard;
            
            if (card->_type == 1) {
                primaryCard = card;
                secondaryCard = other;
            }
            else {
                primaryCard = other;
                secondaryCard = card;
            }
            
            auto moveCenter1 = MoveTo::create(0.3, Vec2(_gameSize.width/2 - 335, _gameSize.height/2.f));
            auto moveCenter2 = MoveTo::create(0.3, Vec2(_gameSize.width/2 + 335, _gameSize.height/2.f));
            
            auto fadeOut =EaseExponentialIn::create(FadeOut::create(0.3f));
            
            auto fadeOutAndStarEffect = Spawn::create(fadeOut, CallFunc::create([this, primaryCard](){
                GameSoundManager::getInstance()->playEffectSound(STAR_EFFECT_SOUND);
                addStarParticle(primaryCard);
            }), NULL);
            
            auto fadeOutAndStarEffect2 = Spawn::create(fadeOut->clone(), CallFunc::create([this, secondaryCard](){
                addStarParticle(secondaryCard);
            }), NULL);
            
            
            auto seq1 = Sequence::create(DelayTime::create(0.5f),moveCenter1,DelayTime::create(duration), fadeOutAndStarEffect, NULL);
            auto seq2 = Sequence::create(DelayTime::create(0.5f),moveCenter2,DelayTime::create(duration), fadeOutAndStarEffect2, CallFunc::create([this]() {
                //this->addStarParticle(card);
                
                //card->setPosition(Vec2::ONE * 9999.0f);
                
                _matchCount++;
                _animationBG->removeFromParent();
                _animationBG = nullptr;
                isAnimating = false;
                if (_matchCount == _currentLevelMaxCardCount)
                {
                    this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([this](){
                        onSolve();
                    }), nullptr));
                }
            }),nullptr);
            
            if (card->hasMatchingSound){
                //_speechButton->setSpeechSentence(card->matchSound);
                //if(_speechButton->isSelected()){
                //    _speechButton->stopPlaying();
                //}
                //_speechButton->onSpeechPressed(this, ui::Widget::TouchEventType::ENDED);
            }
            
            primaryCard->runAction(seq1);
            secondaryCard->runAction(seq2);
            
            
            //            card->runAction(Sequence::create(DelayTime::create(0.3f), EaseExponentialIn::create(FadeOut::create(0.3f)), nullptr));
            //
            //            card->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create([=](){
            //
            //
            ////                card->matchSound.play();
            //                this->addStarParticle(card);
            //
            //                card->setPosition(Vec2::ONE * 9999.0f);
            //
            //                _matchCount++;
            //                if (_matchCount == _currentLevelMaxCardCount)
            //                {
            //                    this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
            //                        onSolve();
            //                    }), nullptr));
            //                }
            //            }), nullptr));
            
            // NB(xenosoz, 2018): Log for future analysis (#1/2)
            auto workPath = [this] {
                stringstream ss;
                ss << "/" << "ShapeMatching";
                ss << "/" << "level-" << _currentLevelID << "-" << _currentWorksheetID;
                ss << "/" << "work-" << _currentProblemID;
                return ss.str();
            }();
            
            StrictLogManager::shared()->game_Peek_Answer("ShapeMatching", workPath,
                                                         TodoUtil::itos(card->id),
                                                         TodoUtil::itos(other->id));
        }
        else {
            auto nextPos = this->safePointForBoundary(card->getPosition());
            auto rescaleAction = ScaleTo::create(0.1f, _defaultScaleFactor);
            auto reposAction = EaseOut::create(MoveTo::create(.5f, nextPos), 1.5f);
            card->runAction(Spawn::create(rescaleAction, reposAction, nullptr));
            card->runAction(rescaleAction);

            this->stompByNode(card);
            
            // NB(xenosoz, 2018): Log for future analysis (#2/2)
            auto workPath = [this] {
                stringstream ss;
                ss << "/" << "ShapeMatching";
                ss << "/" << "level-" << _currentLevelID << "-" << _currentWorksheetID;
                ss << "/" << "work-" << _currentProblemID;
                return ss.str();
            }();
            
            StrictLogManager::shared()->game_Peek_Answer("ShapeMatching", workPath,
                                                         TodoUtil::itos(card->id),
                                                         "None");
        }
        
        return true;
        
        
    };
    
    listener->onTouchCancelled = [this, card](Touch* touch, Event* event) {
        
        card->isTouched = false;
        
        if (card->isLinked) {
            card->linkedTarget->setLink(nullptr);
            card->setLink(nullptr);
        }
    };
    
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card);
    
    /*
     _listener->onTouchBegan = CC_CALLBACK_2(ShapeMatchingScene::touchBeganHandler, this);
     _listener->onTouchMoved = CC_CALLBACK_2(ShapeMatchingScene::touchMovedHandler, this);
     _listener->onTouchEnded = CC_CALLBACK_2(ShapeMatchingScene::touchEndedHandler, this);
     
     Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
     */
    
}

void ShapeMatchingScene::addStarParticle(Node* targetNode)
{
    ParticleSystemQuad* _particleEffect = ParticleSystemQuad::create("ShapeMatching/Particle/star_particle.plist");
    _particleEffect->setScale(6.0f);
    _particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _particleEffect->setPosition(targetNode->getPosition());
    _gameNode->addChild(_particleEffect, ++_zOrder);
}

Point ShapeMatchingScene::safePointForBoundary(Point point)
{
    // NB(xenosoz, 2017): Move point inside of the safe boundary (regarding the polar coordinates)
    Rect boardRect = Rect(Point(0.f, 0.f), _gameNode->getContentSize());
    Size cardSize = ShapeMatchingCard::defaultSize();
    
    float alpha = .2f;
    float beta = .1f;
    Rect safeRect = Rect(cardSize.width * alpha,
                         cardSize.height * beta,
                         boardRect.size.width - cardSize.width * alpha * 2,
                         boardRect.size.height - cardSize.height * beta * 2);
    
    float dist = Point(safeRect.getMidX(), safeRect.getMidY()).distance(point);
    float theta = atan2(point.y - safeRect.getMidY(),
                        point.x - safeRect.getMidX());
    
    float rx = (safeRect.size.width / 2.f) / cos(theta);
    float ry = (safeRect.size.height / 2.f) / sin(theta);
    float r = min(abs(rx), abs(ry));
    
    if (dist <= r) {
        // NB(xenosoz, 2017): Point is already in the safe area.
        return point;
    }
    
    if (abs(rx) < abs(ry)) {
        // NB(xenosoz, 2017): Hit the vertical wall
        int sign = ((rx > 0) - (rx < 0));
        return Point((safeRect.size.width / 2.f) * sign + safeRect.getMidX(),
                     r * sin(theta) + safeRect.getMidY());
    }
    
    // NB(xenosoz, 2017): Hit the horizontal wall (or both)
    int sign = ((ry > 0) - (ry < 0));
    Point newPoint = Point(r * cos(theta) + safeRect.getMidX(),
                           (safeRect.size.height / 2.f) * sign + safeRect.getMidY());
    return newPoint;
}


void ShapeMatchingScene::stompByNode(Node* node)
{
    // NB(xenosoz, 2017): Prevent cards from stacking in one point (to prevent cheating)
    
    for (auto card : matchingCardList) {
        if (!card || card == node || card->isMatchDone) { continue; }
        
        auto nodePos = node->getPosition();
        auto cardPos = card->getPosition();
        auto dir = (cardPos - nodePos).getNormalized();
        if (dir.length() < .5f) {
            // NB(xenosoz, 2017): Pick random direction for an exact match.
            auto theta = random<float>(0.f, 2*M_PI);
            dir.x = cos(theta);
            dir.y = sin(theta);
        }
        
        float r = [&] {
            auto _ = ShapeMatchingCard::defaultSize();
            return min(_.width/2.f, _.height/2.f);
        }();
        auto d_0 = nodePos.distance(cardPos);
        if (d_0 >= 2*r) { continue; }
        
        auto d_1 = (d_0 * d_0) / (4 * r) + r;
        auto nextPos = nodePos + (dir * d_1 * 1.7f);
        nextPos = this->safePointForBoundary(nextPos);
        
        card->runAction(EaseOut::create(MoveTo::create(.5f, nextPos), 1.5f));
    }
}

bool ShapeMatchingScene::isExistPair()
{
    for (auto card : matchingCardList) {
        if (card->isMatchDone)
            continue;
        
        if (card->isLinked)
            return true;
    }
    
    return false;
}

int ShapeMatchingScene::getMinimumLocalZOrder()
{
    int minOrder = 0;
    
    for (auto card : matchingCardList) {
        if (card->isMatchDone)
            continue;
        
        minOrder = MIN(minOrder, card->getLocalZOrder());
    }
    
    return minOrder;
}

cocos2d::ValueMap ShapeMatchingScene::pickCards(ShapeMatching::Piece pieceInfo, int i)
{
    
    
    std::string filenameA = "";
    std::string filenameB = "";
    std::string shape = "";

    shape = _shapeCandidates[i];

    std::string typeA = pieceInfo.typeA;
    std::string colorA = "";
    
    if (pieceInfo.shape == "Advanced" && _levelData.shapes[shape].groups["Advanced_3D"].size() > 0) {
        colorA = "N/A";
    }
    else if (pieceInfo.colorA == "Random") {
        todoschool::Random::shared().shuffle(_colors);
        colorA = _colors[0];
    }
    else {
        colorA = pieceInfo.colorA;
    }
    
    std::string sizeA = "";
    
    
    if (pieceInfo.shape == "Advanced" && _levelData.shapes[shape].groups["Advanced_3D"].size() > 0) {
        sizeA = "N/A";
    }
    else if(pieceInfo.sizeA == "Random") {
        todoschool::Random::shared().shuffle(_sizes);
        sizeA = _sizes[0];
    }
    else {
        sizeA = pieceInfo.sizeA;
    }
    
    int rotationA = 0;
    
    if (pieceInfo.rotationA == "Random") {
        todoschool::Random::shared().shuffle(_rotation);
        rotationA = _rotation[0];
    }
    else if (pieceInfo.rotationA == ""){
        rotationA = 0;
    }
    else {
        rotationA = TodoUtil::stoi(pieceInfo.rotationA);
    }
    
    std::string typeB = pieceInfo.typeB;
    std::string colorB = "";
    
    if (pieceInfo.shape == "Advanced" && _levelData.shapes[shape].groups["Advanced_3D"].size() > 0) {
        colorB = "N/A";
    }
    else if (pieceInfo.colorB == "Random") {
        if (pieceInfo.colorA == "Random") {
            colorB = _colors[1];
        }
        else {
            todoschool::Random::shared().shuffle(_colors);
            colorB = _colors[0];
        }
    }
    else {
        colorB = pieceInfo.colorB;
    }
    
    std::string sizeB = "";
    
    if(pieceInfo.shape == "Advanced" && _levelData.shapes[shape].groups["Advanced_3D"].size() > 0) {
        sizeB = "N/A";
    }
    else if (pieceInfo.sizeB == "Random") {
        if (pieceInfo.sizeA == "Random") {
            sizeB = _sizes[1];
        }
        else {
            todoschool::Random::shared().shuffle(_sizes);
            sizeB = _sizes[0];
        }
    }
    else {
        sizeB = pieceInfo.sizeB;
    }
    
    
    // no different size of images for 3D. So using scale.
    float scaleA = 1.f;
    float scaleB = 1.f;
    
    if (pieceInfo.shape == "Basic_3D" && sizeA != "N/A") {
        if (sizeA == "medium") {
            scaleA = 0.7f;
        }
        else if (sizeA == "small") {
            scaleA = 0.5f;
        }
        if (sizeB == "medium") {
            scaleB = 0.7f;
        }
        else if (sizeB == "small") {
            scaleB = 0.5f;
        }
        
        sizeA = "N/A";
        sizeB = "N/A";
        
    }
    
    
    int rotationB = 0;
    
    if (pieceInfo.rotationB == "Random") {
        if (pieceInfo.rotationA == "Random") {
            rotationB = _rotation[1];
        }
        else {
            todoschool::Random::shared().shuffle(_rotation);
            rotationB = _rotation[0];
            
        }
    }
    else if (pieceInfo.rotationB == ""){
        rotationB = 0;
    }
    else {
        rotationB = TodoUtil::stoi(pieceInfo.rotationB);
    }
    
    CCLOG("A : %s %s %s %s %d",shape.c_str(), typeA.c_str(), colorA.c_str(), sizeA.c_str(), rotationA);
    CCLOG("B : %s %s %s %s %d",shape.c_str(), typeB.c_str(), colorB.c_str(), sizeB.c_str(), rotationB);
    filenameA = generateCardImageFilename(shape, typeA, colorA, sizeA);
    
    filenameB = generateCardImageFilename(shape, typeB, colorB, sizeB);
    
    CCLOG("A: %s",filenameA.c_str());
    CCLOG("B: %s",filenameB.c_str());
    
    ValueMap info = ValueMap();
    info["filenameA"] = filenameA;
    info["rotationA"] = rotationA;
    info["filenameB"] = filenameB;
    info["rotationB"] = rotationB;
    info["scaleA"] = scaleA;
    info["scaleB"] = scaleB;
    
    return info;
    
}

std::string ShapeMatchingScene::generateCardImageFilename(std::string shape, std::string type, std::string color, std::string size)
{
    std::string imgPrefix = "matchinggame_";
    
    std::string filename = "";
    
    if (type == "Name") {
        return shape;
    }
    else if(type == "Object") {
        filename += imgPrefix + shape + "_object.png";
        return filename;
    }
    else if(type == "Attribute") {
        stringstream ss;
        
        for (auto item : _levelData.shapes[shape].attributes) {
            string key = item.first;
            string value = item.second;
            
            CCLOG(">> %s, %s", key.c_str(), value.c_str());

            if (value != "None") {
                // NB(xenosoz, 2018): I'm expecting a row like "3 Sides" here.
                ss << value << " " << key;
            }
        }
        
        return ss.str();
    }
    
    filename += imgPrefix;
    
    filename += shape;
    
    if (color != "N/A") {
        filename += "_" + color;
    }
    
    if (size != "N/A") {
        filename += "_" + size;
    }
    filename += ".png";
    
    return filename;
    
}
