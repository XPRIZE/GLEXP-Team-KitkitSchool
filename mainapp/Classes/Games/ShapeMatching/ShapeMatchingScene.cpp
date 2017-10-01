//
//  GameScene.cpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#include "SimpleAudioEngine.h"
#include "ShapeMatchingScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
//#include "Common/Controls/CompletePopup.hpp"
//#include "Common/Controls/TodoSchoolBackButton.hpp"
//#include "Utils/Random.h"
#include "Utils/CardSlotBuilder.hpp"
#include "Utils/Random.h"
#include "Managers/LanguageManager.hpp"
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
    const char* SOLVE_EFFECT_SOUND = "ShapeMatching/Sound/UI_Star_Collected.m4a";
    const string CARD_MATCHING_EFFECT_SOUND = "ShapeMatching/Sound/Train_Slotin.m4a";
    const string STAR_EFFECT_SOUND = "ShapeMatching/Sound/Quiz_Correct.m4a";
    
    //    string nextButtonTitle() {
    //        if (LanguageManager::getInstance()->isSwahili()) { return "Inayofuata"; }
    //        return "Next";
    //    }
    
    string swTZ_of_enUS(const string& enUS)
    {
        // XXX(xenosoz, 2017): We're running out of time for Bagamoyo testing
        if (enUS == "circle") { return "duara"; }
        if (enUS == "square") { return "mraba"; }
        if (enUS == "triangle") { return "pembe_tatu"; }
        if (enUS == "rectangle") { return "mstatili"; }
        if (enUS == "star") { return "nyota"; }
        if (enUS == "rhombus") { return "rombasi"; }
        if (enUS == "diamond") { return "almasi"; }
        if (enUS == "oval") { return "duara_dufu"; }
        if (enUS == "hexagon") { return "pembe_sita"; }
        if (enUS == "pentagon") { return "pembe_tano"; }
        if (enUS == "trapezoid") { return "trapeza"; }
        if (enUS == "parallelogram") { return "msambamba"; }
        if (enUS == "octagon") { return "pembe_nane"; }
        if (enUS == "cone") { return "pia"; }
        if (enUS == "sphere") { return "nyanja"; }
        if (enUS == "cylinder") { return "mcheduara"; }
        if (enUS == "cube") { return "mche_mraba"; }
        if (enUS == "rectangular_prism") { return "mche_mstatili"; }
        if (enUS == "triangular_prism") { return "mche_pembe_tatu"; }
        if (enUS == "pyramid") { return "piramidi"; }
        if (enUS == "face") { return "uso"; }
        if (enUS == "faces") { return "nyuso"; }
        if (enUS == "side") { return "upande"; }
        if (enUS == "sides") { return "pande"; }
        if (enUS == "large") { return "kubwa"; }
        if (enUS == "medium") { return "wastani"; }
        if (enUS == "small") { return "dogo"; }
        return "";
    }
    
    string localizeString(const string& enUS)
    {
        // NB(xenosoz, 2017): en_US string -> current language string.
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        if (lang == "en-US") { return enUS; }
        if (lang == "sw-TZ") { return swTZ_of_enUS(enUS); }
        
        return enUS;
    }
    
    string localizedSound(const string& enUS)
    {
        // NB(xenosoz, 2017): en_US string -> current language sound.
        stringstream ss;
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
        ss << "ShapeMatching";
        ss << "/" << "Sound." << lang;
        ss << "/" << swTZ_of_enUS(enUS);
        ss << ".m4a";

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
    //    _nextButton->setTitleFontName("fonts/TodoSchoolV2.ttf");
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
    auto sheet = data.randomSheetFor(lang, _currentLevelID);
    
    _currentWorksheet = sheet;
    _currentProblemID = sheet.beginProblemID();
    _progressBar->setMax((int)sheet.size());
    
    _basic2D = {"circle", "square", "triangle", "rectangle", "star", "rhombus", "oval"};
    _advanced2D = {"hexagon", "pentagon", "trapezoid", "parallelogram", "octagon"};
    
    _basic3D = {"cone", "sphere", "cylinder", "cube"};
    _advanced3D = {"rectangular_prism", "triangular_prism", "pyramid"};
    
    _colors = {"1", "2"};
    
    _sizes = {"large", "medium", "small"};
    
    _rotation = {0,30,45,90};
    
    std::merge(_advanced2D.begin(), _advanced2D.end(), _advanced3D.begin(), _advanced3D.end(), std::back_inserter(_advanced));
    
    _attribute["circle"] = "0 Side";
    _attribute["square"] = "4 Sides";
    _attribute["triangle"] = "3 Sides";
    _attribute["rectangle"] = "4 Sides";
    _attribute["star"] = "10 Sides";
    _attribute["rhombus"] = "4 Sides";
    _attribute["oval"] = "0 Sides";
    _attribute["hexagon"] = "6 Sides";
    _attribute["pentagon"] = "5 Sides";
    _attribute["trapezoid"] = "4 Sides";
    _attribute["parallelogram"] = "4 Sides";
    _attribute["octagon"] = "8 Sides";
    
    _attribute["cone"] = "1 Face";
    _attribute["sphere"] = "0 Faces";
    _attribute["cylinder"] = "2 Faces";
    _attribute["cube"] = "6 Faces";
    _attribute["rectangular_prism"] = "6 Faces";
    _attribute["triangular_prism"] = "5 Faces";
    _attribute["pyramid"] = "5 Faces";
    
    attr2D.insert(pair<std::string, std::string>("0 Sides", "circle"));
    attr2D.insert(pair<std::string, std::string>("4 Sides", "square"));
    attr2D.insert(pair<std::string, std::string>("3 Sides", "triangle"));
    attr2D.insert(pair<std::string, std::string>("4 Sides", "rectangle"));
    attr2D.insert(pair<std::string, std::string>("10 Sides", "star"));
    attr2D.insert(pair<std::string, std::string>("4 Sides", "rhombus"));
    attr2D.insert(pair<std::string, std::string>("0 Sides", "oval"));
    attr2D.insert(pair<std::string, std::string>("6 Sides", "hexagon"));
    attr2D.insert(pair<std::string, std::string>("5 Sides", "pentagon"));
    attr2D.insert(pair<std::string, std::string>("4 Sides", "trapezoid"));
    attr2D.insert(pair<std::string, std::string>("4 Sides", "parallelogram"));
    attr2D.insert(pair<std::string, std::string>("8 Sides", "octagon"));
    
    for(auto it = attr2D.begin(); it != attr2D.end(); it++) {
        if (std::find( attr2DKeys.begin(), attr2DKeys.end(), it->first) == attr2DKeys.end()) {
            attr2DKeys.push_back(it->first);
            //            auto its = attr2D.equal_range(it->first);
            //            for (auto itr = its.first; itr!=its.second; itr++) {
            //                CCLOG("%s ",itr->second.c_str());
            //            }
        }
    }
    
    attr3D.insert(pair<std::string, std::string>("1 Face", "cone"));
    attr3D.insert(pair<std::string, std::string>("0 Faces", "sphere"));
    attr3D.insert(pair<std::string, std::string>("2 Faces", "cylinder"));
    attr3D.insert(pair<std::string, std::string>("6 Faces", "cube"));
    attr3D.insert(pair<std::string, std::string>("6 Faces", "rectangular_prism"));
    attr3D.insert(pair<std::string, std::string>("5 Faces", "triangular_prism"));
    attr3D.insert(pair<std::string, std::string>("5 Faces", "pyramid"));
    
    for(auto it = attr3D.begin(); it != attr3D.end(); it++) {
        if (std::find( attr3DKeys.begin(), attr3DKeys.end(), it->first) == attr3DKeys.end()) {
            attr3DKeys.push_back(it->first);
            //            auto its = attr3D.equal_range(it->first);
            //            for (auto itr = its.first; itr!=its.second; itr++) {
            //                CCLOG("%s ",itr->second.c_str());
            //            }
        }
    }
    
    
    
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
        auto points = shapematching::CardSlotBuilder().pointsInBoundary(count, localRect, cardSize);
        auto it = todoschool::sample(points, count);
        selected.insert(selected.end(),
                        make_move_iterator(it.begin()),
                        make_move_iterator(it.end()));
    };
    appendSelected(leftCount, leftRect);
    appendSelected(rightCount, rightRect);
    
    auto pieceInfo = _currentProblem.pieces[0];
    
    
    _shapeCandidates.clear();
    
    if (pieceInfo.shape == "Basic_2D") {
        todoschool::Random::shared().shuffle(_basic2D);
        for (auto it = _basic2D.begin(); it != _basic2D.end(); it++) {
            _shapeCandidates.push_back(*it);
        }
        
    }
    else if(pieceInfo.shape == "Basic_3D") {
        todoschool::Random::shared().shuffle(_basic3D);
        for (auto it = _basic3D.begin(); it != _basic3D.end(); it++) {
            _shapeCandidates.push_back(*it);
        }
        
    }
    else if(pieceInfo.shape == "Advanced") {
        todoschool::Random::shared().shuffle(_advanced);
        for (auto it = _advanced.begin(); it != _advanced.end(); it++) {
            _shapeCandidates.push_back(*it);
        }
        
    }
    else if(pieceInfo.shape == "Advanced_2D") {
        todoschool::Random::shared().shuffle(_advanced2D);
        for (auto it = _advanced2D.begin(); it != _advanced2D.end(); it++) {
            _shapeCandidates.push_back(*it);
        }
        
    }
    else if(pieceInfo.shape == "Advanced_3D") {
        todoschool::Random::shared().shuffle(_advanced3D);
        for (auto it = _advanced3D.begin(); it != _advanced3D.end(); it++) {
            _shapeCandidates.push_back(*it);
        }
    }
    
    if(_currentLevelID == 11) {
        _shapeCandidates.clear();
        todoschool::Random::shared().shuffle(attr2DKeys);
        std::vector<std::string> shape_candidate_candidate;
        for (auto it = attr2DKeys.begin(); it != attr2DKeys.end(); it++) {
            shape_candidate_candidate.clear();
            auto its = attr2D.equal_range(*it);
            for (auto itr = its.first; itr!=its.second; itr++) {
                shape_candidate_candidate.push_back(itr->second.c_str());
            }
            todoschool::Random::shared().shuffle(shape_candidate_candidate);
            _shapeCandidates.push_back(shape_candidate_candidate[0]);
        }
    }
    else if(_currentLevelID == 12) {
        _shapeCandidates.clear();
        todoschool::Random::shared().shuffle(attr3DKeys);
        std::vector<std::string> shape_candidate_candidate;
        for (auto it = attr3DKeys.begin(); it != attr3DKeys.end(); it++) {
            shape_candidate_candidate.clear();
            auto its = attr3D.equal_range(*it);
            for (auto itr = its.first; itr!=its.second; itr++) {
                shape_candidate_candidate.push_back(itr->second.c_str());
            }
            todoschool::Random::shared().shuffle(shape_candidate_candidate);
            _shapeCandidates.push_back(shape_candidate_candidate[0]);
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
        
        
        string shapeString = _shapeCandidates[i];
        cardA->matchSound = localizedSound(shapeString);
        cardB->matchSound = localizedSound(shapeString);
        
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
}

ShapeMatchingCard* ShapeMatchingScene::createMatchingCard(int zOrder, Point pos,
                                                          int level, int id, int type,
                                                          const std::string& cardImageName, int rotation, float scale, bool hasTouchSound, bool hasMatchingSound)
{
    ShapeMatchingCard* card = ShapeMatchingCard::create();
    card->setImage(level, type, id, cardImageName, rotation, scale);
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
                    
                    if (shouldBecomePair(other, card)) {
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
        if (isAnimating) return false;
        
        card->stopParticle();
        card->isTouched = false;
        
        if (card->isLinked && !card->isMatchDone) {
            isAnimating = true;
            
            
            GameSoundManager::getInstance()->playEffectSound(card->matchSound);
            
            auto other = card->linkedTarget;
            
            // set animation bg large enough to cover whole screen
            // auto winSize = GameController::getInstance()->getGameWindowSize();
            auto winSize = getContentSize();
            _animationBG = LayerColor::create(Color4B(0, 0, 0, 0.8*255), winSize.width*3, winSize.height*3);
            _animationBG->setPosition(Vec2(-winSize.width,-winSize.height));
            
            _gameNode->addChild(_animationBG, MIN(card->getLocalZOrder(),other->getLocalZOrder()) -1);
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
            
            
            auto seq1 = Sequence::create(DelayTime::create(0.5f),moveCenter1,DelayTime::create(0.6f), fadeOutAndStarEffect, NULL);
            auto seq2 = Sequence::create(DelayTime::create(0.5f),moveCenter2,DelayTime::create(0.6f), fadeOutAndStarEffect2, CallFunc::create([this]() {
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
        }
        else {
            auto nextPos = this->safePointForBoundary(card->getPosition());
            auto rescaleAction = ScaleTo::create(0.1f, _defaultScaleFactor);
            auto reposAction = EaseOut::create(MoveTo::create(.5f, nextPos), 1.5f);
            card->runAction(Spawn::create(rescaleAction, reposAction, nullptr));
            
            this->stompByNode(card);
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


cocos2d::ValueMap ShapeMatchingScene::pickCards(ShapeMatching::Piece pieceInfo, int i)
{
    
    
    std::string filenameA = "";
    std::string filenameB = "";
    
    std::string shape = "";
    
    //    if (pieceInfo.shape == "Basic_2D") {
    ////        Random::shared().shuffle(_basic2D);
    //        shape = _basic2D[i];
    //    }
    //    else if(pieceInfo.shape == "Basic_3D") {
    ////        Random::shared().shuffle(_basic3D);
    //        shape = _basic3D[i];
    //    }
    //    else if(pieceInfo.shape == "Advanced") {
    ////        Random::shared().shuffle(_advanced);
    //        shape = _advanced[i];
    //    }
    //    else if(pieceInfo.shape == "Advanced_2D") {
    ////        Random::shared().shuffle(_advanced2D);
    //        shape = _advanced2D[i];
    //    }
    //    else if(pieceInfo.shape == "Advanced_3D") {
    ////        Random::shared().shuffle(_advanced3D);
    //        shape = _advanced3D[i];
    //    }
    
    shape = _shapeCandidates[i];
    
    //    std::string attr;
    //    std::vector<std::string> shape_candidate;
    //
    //    if(_currentLevelID == 11) {
    //        attr = attr2DKeys[i];
    //
    //        auto its = attr2D.equal_range(attr);
    //        for (auto itr = its.first; itr!=its.second; itr++) {
    //            shape_candidate.push_back(itr->second.c_str());
    //        }
    //        Random::shared().shuffle(shape_candidate);
    //        shape = shape_candidate[0];
    //
    //
    //    }
    //    else if(_currentLevelID == 12) {
    //        attr = attr3DKeys[i];
    //
    //        auto its = attr3D.equal_range(attr);
    //        for (auto itr = its.first; itr!=its.second; itr++) {
    //            shape_candidate.push_back(itr->second.c_str());
    //        }
    //        Random::shared().shuffle(shape_candidate);
    //        shape = shape_candidate[0];
    //    }
    
    
    std::string typeA = pieceInfo.typeA;
    
    std::string colorA = "";
    
    if (pieceInfo.shape == "Advanced" && std::find(std::begin(_advanced3D), std::end(_advanced3D), shape) != std::end(_advanced3D)) {
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
    
    
    if(pieceInfo.shape == "Advanced" && std::find(std::begin(_advanced3D), std::end(_advanced3D), shape) != std::end(_advanced3D)) {
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
    
    if (pieceInfo.shape == "Advanced" && std::find(std::begin(_advanced3D), std::end(_advanced3D), shape) != std::end(_advanced3D)) {
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
    
    if(pieceInfo.shape == "Advanced" && std::find(std::begin(_advanced3D), std::end(_advanced3D), shape) != std::end(_advanced3D)) {
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
        return _attribute[shape];
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
