//
//  CountingScene.cpp
//
//  Created by Gunho Lee on 6/26/16.
//
//


#include "CountingScene.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Basic/AARect.h"
#include "Common/Repr/AllRepr.h"

#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;
using namespace todoschool::counting;



namespace CountingSceneSpace {
    const char* solveEffect = "Common/Sounds/Effect/UI_Star_Collected.m4a";
    const char* missEffect = "Common/Sounds/Effect/Help.m4a";
    const char* hintEffect = "Common/Sounds/Effect/tokenappear.m4a";
    const char* touchEffect = "Common/Sounds/Effect/tokenappear.m4a";
    const char* dropEffect = "Common/Sounds/Effect/panelput.m4a";
    const int tagForMissAction = 100;
}

using namespace CountingSceneSpace;


Scene* CountingScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CountingScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool CountingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchEnabled = false;
    
    GameSoundManager::getInstance()->preloadEffect(solveEffect);
    GameSoundManager::getInstance()->preloadEffect(missEffect);
    GameSoundManager::getInstance()->preloadEffect(touchEffect);
    GameSoundManager::getInstance()->preloadEffect(dropEffect);
    
    auto winSize = getContentSize();
    
    auto bg = Sprite::create("Counting/counting_image_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    
    _gameNode = Node::create();
    auto gameSize = Size(2560, 1800);
    _gameNode->setContentSize(gameSize);
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    addChild(_gameNode);

    _answerPad = AnswerPadMulti::create();
    _answerPad->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _answerPadPos = Point(gameSize.width-82, gameSize.height/2);
    _answerPad->setPosition(_answerPadPos);
    _answerPad->setAnswerCallback([this](int answer) {
        // NB(xenosoz, 2018): Log for future analysis
        auto workPath = [this] {
            stringstream ss;
            ss << "/" << "Counting";
            ss << "/" << "level-" << _currentLevel;
            ss << "/" << "work-" << _currentProblemID;
            return ss.str();
        }();
        
        StrictLogManager::shared()->game_Peek_Answer("Counting", workPath,
                                                     TodoUtil::itos(answer),
                                                     TodoUtil::itos(_currentProblem.totalCount));
        
        if (_currentProblem.totalCount == answer) {
            onSolve();
        } else {
            onMiss();
        }
    });
    _answerPad->setVisible(false);
    _gameNode->addChild(_answerPad);
    
    _answerPadOutPos = _gameNode->convertToNodeSpace( Point(winSize.width, winSize.height/2) ) + _answerPad->getContentSize();
    _answerPadOutPos.y = gameSize.height/2;

    _objectNode = Node::create();
    _objectNode->setContentSize(Size(gameSize.width/2, gameSize.height-50));
    _gameNode->addChild(_objectNode);
    

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
   
    if (CCAppController::sharedAppController()->isDebug()) {
        auto skip = ui::Button::create();
        skip->setTitleText("CLEAR");
        skip->setTitleFontSize(50);
        skip->setPosition(Vec2(winSize.width-200, winSize.height-100));
        skip->addClickEventListener([this](Ref*) {
            _currentProblemID = _currentWorksheet.endProblemID()-1;
            onSolve();
        });
        addChild(skip);
    }
    
    return true;
}

void CountingScene::onEnter()
{
    Layer::onEnter();

    auto data = LevelData::defaultData();
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto sheet = data.randomSheetFor(lang, _currentLevel);

    _progressBar->setMax((int)sheet.size());
    _currentWorksheet = sheet;
    _currentProblemID = sheet.beginProblemID();
    _currentProblem = sheet.problemByID(_currentProblemID);
}

void CountingScene::onEnterTransitionDidFinish()
{
    onStart();
}


void attachNumber(Node* object, int number) {

    auto n = Button::create();
    n->loadTextureNormal("Counting/counting_image_digit_common_countNumber_BG.png");
    n->setTitleText(TodoUtil::itos(number));
    n->setTitleFontSize(70);
    n->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    n->setPosition(object->getContentSize()/2);
    object->addChild(n, 10, "Number");
}

void detachNumber(Node* object) {
    object->removeChildByName("Number");
}

void CountingScene::onStart()
{
    _objectNode->removeAllChildren();
    
    _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1);
    _answerPad->setAnswerDigitCount((int)TodoUtil::itos(_currentProblem.totalCount).size());


    setTouchEnabled(true);
    
    _objects.clear();
    _touchedObjects = 0;
    _touchedValue = 0;

    float appearTime = 0;
    putObjects(appearTime);
    
    _answerPad->setPosition(_answerPadOutPos);
    _answerPad->setVisible(true);
    auto seq = Sequence::create(DelayTime::create(appearTime+0.2), EaseIn::create(MoveTo::create(0.2, _answerPadPos), 3.0), nullptr);
    //auto seq = Sequence::create(EaseBounceOut::create(MoveTo::create(0.3, _answerPadPos)), nullptr);
    _answerPad->runAction(seq);
}

void CountingScene::onSolve()
{
    
    setTouchEnabled(false);

    GameSoundManager::getInstance()->playAdultVoice(_currentProblem.totalCount);
    scheduleOnce([](float) {  // Sound delay
        GameSoundManager::getInstance()->playEffectSound(solveEffect);
    }, 1.0, "number");
    
    
    if (_currentProblemID + 1 == _currentWorksheet.endProblemID()) {
        _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1, true);

        
        CompletePopup::create()->show(2.0, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
        });

        
    } else {
        _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1, true);
        
        _currentProblemID++;
        _currentProblem = _currentWorksheet.problemByID(_currentProblemID);
        

        
        
        auto seq = Sequence::create(
                                    DelayTime::create(2.0),
                                    EaseOut::create(MoveTo::create(0.2, _answerPadOutPos), 3.0),
                                    DelayTime::create(0.2),
                                    CallFunc::create([this]{ _answerPad->clearAnswer(); this->onStart(); }),
                                    nullptr);
        
        _answerPad->runAction(seq);
        
    }
    
}


void CountingScene::onMiss()
{
    
    setTouchEnabled(false);
    
    GameSoundManager::getInstance()->playEffectSound(missEffect);
    
    
    const float countDelay = .2f;
    //int remainCount = (int)_objects.size();
    //const float countDuration = countDelay * remainCount;
    int tempCount = _touchedObjects;
    int tempValue = _touchedValue;
    
    
    Vector<FiniteTimeAction*> actions;
    actions.pushBack(DelayTime::create(.5f));
    
    // NB(xenosoz, 2016): Clear all children first.
    actions.pushBack(CallFunc::create([this] {
        for (auto object : _objects)
            detachNumber(object);
    }));
    actions.pushBack(DelayTime::create(.5f));

    
    // NB(xenosoz, 2016): Relabel all objects respecting the following rules:
    //   1. keep the user input ordering
    //   2. y-pos of the object (top to bottom)
    //   3. x-pos of the object (left to right)
    vector<size_t> ordering(_objects.size());
    iota(ordering.begin(), ordering.end(), 0);
    sort(ordering.begin(), ordering.end(), [&](size_t lhs, size_t rhs) {
        auto intMax = numeric_limits<int>::max();
        auto valueFn = [&](int t) { return (t != 0 ? t : intMax); };
        auto lvalue = valueFn(_objects[lhs]->value());
        auto rvalue = valueFn(_objects[rhs]->value());

        if (lvalue != rvalue) { return lvalue < rvalue; }

        auto lpos = _objects[lhs]->getPosition();
        auto rpos = _objects[rhs]->getPosition();

        if (lpos.y != rpos.y) { return lpos.y > rpos.y; }
        return lpos.x < rpos.x;
    });
    
    for (size_t index : ordering) {
        auto object = _objects[index];
        int label = object->value();

        if (object->value() == 0) {
            tempCount += 1;
            tempValue += object->weight();
            label = tempValue;
        }

        actions.pushBack(CallFunc::create([this, object, label] {
            GameSoundManager::getInstance()->playEffectSound(hintEffect);
            attachNumber(object, label);
        }));
        actions.pushBack(DelayTime::create(countDelay));
    }
    
    // NB(xenosoz, 2016): Remove children for all nodes and clear the user input.
    actions.pushBack(DelayTime::create(1.5f - countDelay));
    actions.pushBack(CallFunc::create([this] {
        _answerPad->clearAnswer();
        setTouchEnabled(true);
        
        for (auto object : _objects) {
            object->setValue(0);
            detachNumber(object);
        }
        _touchedObjects = 0;
        _touchedValue = 0;
    }));

    auto seq = Sequence::create(actions);
    seq->setTag(tagForMissAction);
    stopActionByTag(seq->getTag());
    runAction(seq);
}

void CountingScene::setLevel(int level)
{
    _currentLevel = level;
}

void CountingScene::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
    _answerPad->setTouchEnabled(enabled);
}

void CountingScene::putObjects(float& appearTime)
{
    auto areaSize = _objectNode->getContentSize();

    vector<AARect> occupied;

    
    auto putObject = [&](const string& objectSkin, const string& shadowSkin,
                         int weight, int areaLimitIndex = 0, int areaLimitNum = 0)
    {
        
        auto localAreaSize = areaSize;
        auto localAreaBottom = 0;
        if (areaLimitNum>0) {
            localAreaSize.height = areaSize.height/areaLimitNum;
            localAreaBottom = localAreaSize.height*(areaLimitNum-areaLimitIndex-1);
        }
        
        
        auto shadow = Sprite::create(shadowSkin);  // Nullable
        auto object = CountingObject::create(objectSkin, weight);

        auto objectSize = object->getContentSize();
        float bestScore = -numeric_limits<float>::infinity();
        Point bestPos;
        Rect bestRect;
        
        for (int trial = 0; trial < 10; ++trial) {
            auto& os = objectSize;
            float x = random<float>(os.width / 2.f, localAreaSize.width - os.width / 2.f);
            float y = random<float>(os.height / 2.f, localAreaSize.height - os.height / 2.f);
            auto pos = Point(x, y + localAreaBottom);
            
            AARect rect(pos.x - os.width / 2.f,
                        pos.y - os.height / 2.f,
                        os.width,
                        os.height);

            float score = 0.f;
            for (auto obstacle : occupied)
                score -= rect.intersection(obstacle).area();

            if (bestScore <= score) {
                bestScore = score;
                bestPos = pos;
                bestRect = rect;
            }
        }

        auto pos = bestPos;
        auto p0 = Point(pos.x, pos.y+300);
        occupied.push_back(bestRect);


        //
        if (shadow) {
            shadow->setScale(0.5);
            shadow->setPosition(pos);
            shadow->setOpacity(0);
            _objectNode->addChild(shadow);
        }
        
        object->setValue(0);
        object->onTouchEvent = [this, object](Widget::TouchEventType event) {
            if (!_touchEnabled || !object) { return; }
            
            object->getParent()->reorderChild(object, object->getLocalZOrder());
            
            if (object->value() != 0) {
                // NB(xenosoz, 2016): Nonzero value
                //   -> We already finished counting.
                return;
            }
            if (event != Widget::TouchEventType::BEGAN) { return; }
            

            
            _touchedObjects++;
            _touchedValue += object->weight();
            
            object->setValue(_touchedValue);
            attachNumber(object, _touchedValue);
            
            GameSoundManager::getInstance()->playEffectSound(touchEffect);
        };
        
        object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        object->setPosition(p0);
        object->setOpacity(0);
        _objects.push_back(object);
        _objectNode->addChild(object);
        

        //
        auto delay = random(0.05, 0.2);
        const float actionTime = 0.3;
        appearTime+=delay;

        if (shadow) {
            shadow->runAction(Sequence::create(
                                               DelayTime::create(appearTime),
                                               Spawn::create(FadeIn::create(actionTime), EaseBounceOut::create(ScaleTo::create(actionTime, 1.0)), nullptr),
                                               nullptr));
        }
        
        object->runAction(Sequence::create(
                                           DelayTime::create(appearTime),
                                           Spawn::create(FadeIn::create(actionTime), EaseBounceOut::create(MoveTo::create(actionTime, pos)), nullptr),
                                           CallFunc::create([](){
            GameSoundManager::getInstance()->playEffectSound(dropEffect);
        }),
                                           nullptr
                                           ));
    };


    // NB(xenosoz, 2016): Tallies (W=5, W=1)
    int talliesToGo = _currentProblem.tallyCount;
    int tallyVari5 = 1;
    int tallyVari1 = random(1, 2);

    auto tallyObject5 = StringUtils::format("Counting/branch_five_%02d.png",
                                            tallyVari5);
    auto tallyObject1 = StringUtils::format("Counting/branch_one_%02d.png",
                                            tallyVari1);

    auto tallyShadow5 = StringUtils::format("Counting/branch_five_%02d_shadow.png",
                                            tallyVari5);
    auto tallyShadow1 = StringUtils::format("Counting/branch_one_%02d_shadow.png",
                                            tallyVari1);
    
    int talliesAreaLimitNum = ((talliesToGo%5==0) || (talliesToGo<5)) ? 0 : 2;

    while (talliesToGo >= 5) {
        putObject(tallyObject5, tallyShadow5, 5, 0, talliesAreaLimitNum);
        talliesToGo -= 5;
    }
    
    while (talliesToGo >= 1) {
        putObject(tallyObject1, tallyShadow1, 1, 1, talliesAreaLimitNum);
        talliesToGo -= 1;
    }


    // NB(xenosoz, 2016): Stones
    int stonesCount = _currentProblem.stoneCount;
    int stoneVariVector[5] = { 1, 2, 3, 4, 5};
    random_shuffle(std::begin(stoneVariVector), std::end(stoneVariVector));
    int stoneVari = random(0, 4);
    
    int areaLimitNum = (stonesCount<=5) ? 0 : (stonesCount+4)/5;
    int areaLimitIndex = 0;
    
    while (stonesCount>0) {
        
        int stoneStyle = stoneVariVector[stoneVari];
        string stoneObjectSkin = "Counting/stone_0"+TodoUtil::itos(stoneStyle)+".png";
        string stoneShadowSkin = "Counting/stone_0"+TodoUtil::itos(stoneStyle)+"_shadow.png";
        
        int thisStoneCount = MIN(5, stonesCount);
        
        for (int i = 0; i < thisStoneCount; i++)
            putObject(stoneObjectSkin, stoneShadowSkin, 1, areaLimitIndex, areaLimitNum);
        
        stonesCount -= thisStoneCount;
        stoneVari++;
        if (stoneVari>4) stoneVari = 0;
        
        areaLimitIndex++;
    }
    

}

