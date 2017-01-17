//
//  GameScene.cpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#include "SimpleAudioEngine.h"
#include "LetterMatchingScene.hpp"
#include "Models/LevelData.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"
#include "ButtonUtility.hpp"
#include "Managers/GameSoundManager.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <Common/Basic/SoundEffect.h>
#include <Games/NumberMatching/Utils/CardSlotBuilder.h>
#include <Managers/LanguageManager.hpp>

#include "CCAppController.hpp"

#define MAX_CHARACTER_COUNT 20
#define Z_ORDER_BACKGROUND 10
#define Z_ORDER_DEFAULT 30
#define Z_ORDER_PARTICLE 1000

using namespace todoschool;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace std;
using namespace todoschool::lettermatching;


namespace LetterMatching
{
    const char* SOLVE_EFFECT_SOUND = "Counting/UI_Star_Collected.m4a";

    string nextButtonTitle() {
        if (LanguageManager::getInstance()->isSwahili()) { return "Inayofuata"; }
        return "Next";
    }
}
using namespace LetterMatching;

Scene* LetterMatchingScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LetterMatchingScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LetterMatchingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    _zOrder = Z_ORDER_DEFAULT;
    
    auto winSize = getContentSize();
    
    auto bg = Sprite::create("NumberMatching/Images/matching_bg_bigger.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
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
    
    // Back Button
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    // Progress Bar
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1);
    addChild(_progressBar);
    
    // Next Button
    const auto normalColor = Color3B(92, 46, 28);
    const auto highlightColor = Color3B(255, 241, 217);
    _nextButton = Button::create();
    _nextButton->loadTextures("NumberMatching/Images/matchgame_next_inactivated.png", "NumberMatching/Images/matchgame_next_activated.png");
    _nextButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _nextButton->setPosition(Vec2(winSize.width - _nextButton->getContentSize().width - 40, _nextButton->getContentSize().height + 30));
    _nextButton->setTitleFontName("fonts/TodoSchoolV2.ttf");
    _nextButton->setTitleFontSize(80.f);
    _nextButton->setTitleColor(normalColor);
    _nextButton->setTitleText(nextButtonTitle());
    _nextButton->addTouchEventListener([=](Ref* sender,Widget::TouchEventType event) {
        switch (event)
        {
            case Widget::TouchEventType::BEGAN:
                _nextButton->setTitleColor(highlightColor);
                break;
            case Widget::TouchEventType::CANCELED:
            case Widget::TouchEventType::ENDED:
                _nextButton->setTitleColor(normalColor);
                break;
            default:
                break;
        }
    });
    _nextButton->addClickEventListener([this](Ref*) {
        advanceToNextProblem();
    });
    _nextButton->setVisible(false);
    addChild(_nextButton);
    

    
    return true;
}

void LetterMatchingScene::onEnter()
{
    Layer::onEnter();
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto data = LevelData::defaultData();
    auto sheet = data.randomSheetFor(lang, _currentLevelID);

    _currentWorksheet = sheet;
    _currentProblemID = sheet.beginProblemID();
    _progressBar->setMax((int)sheet.size());
}

void LetterMatchingScene::onEnterTransitionDidFinish()
{
    onStart();
}

void LetterMatchingScene::setLevel(int levelID) {
    _currentLevelID = levelID;
}

void LetterMatchingScene::onStart()
{
    _nextTransition = false;
    
    auto problem = _currentWorksheet.problemByID(_currentProblemID);
    _currentProblem = problem;

    _nextButton->setVisible(false);
    _currentLevelMaxCardCount = (int)problem.size();
    _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1);
    
    _defaultScaleFactor = 0.8f;
    _upScaleFactor = 0.9f;
    _matchCount = 0;
    initCardList();
}

void LetterMatchingScene::onSolve()
{
    GameSoundManager::getInstance()->playEffectSound(SOLVE_EFFECT_SOUND);
    
    _progressBar->setCurrent(_currentProblemID - _currentWorksheet.beginProblemID() + 1, true);
    
    if (_currentProblemID + 1 == _currentWorksheet.endProblemID())
    {
        CompletePopup::create()->show(1.0f, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
            //TodoSchoolBackButton::popGameScene();
        });
    }
    else
    {
#if true
        advanceToNextProblem();
#else
        _nextButton->setVisible(true);
#endif
    }
}

void LetterMatchingScene::advanceToNextProblem() {
    if (_nextTransition) { return; }
    _nextTransition = true;

    _currentProblemID++;

    runAction(Sequence::create(DelayTime::create(0.2f),
                               CallFunc::create([this]{ onStart(); }), nullptr));
}


void LetterMatchingScene::initCardList()
{
    // NB(xenosoz, 2016): Create grid first and choose points to reduce collisions.

    size_t cardCount = _currentLevelMaxCardCount * 2;
    Rect boardRect = Rect(Point(0.f, 0.f), _gameNode->getContentSize());
    Size cardSize = LetterMatchingCard::defaultSize();


    Rect leftRect(boardRect.getMinX(), boardRect.getMinY(),
                  boardRect.size.width/2.f, boardRect.size.height);
    Rect rightRect(boardRect.getMidX(), boardRect.getMinY(),
                   boardRect.size.width/2.f, boardRect.size.height);

    size_t leftCount = (size_t)ceil(cardCount / 2.f);
    size_t rightCount = cardCount - leftCount;
    if (random(0, 1) == 0) { std::swap(leftCount, rightCount); }


    vector<Point> selected;
    auto appendSelected = [&](size_t count, Rect localRect) {
        auto points = CardSlotBuilder().pointsInBoundary(count, localRect, cardSize);
        selected.insert(selected.end(),
                        make_move_iterator(points.begin()),
                        make_move_iterator(points.end()));
    };
    appendSelected(leftCount, leftRect);
    appendSelected(rightCount, rightRect);

    for (int i = 0; i < _currentLevelMaxCardCount; i++)
    {
        auto pieceInfo = _currentProblem.pieces[i];

        auto cardA = createMatchingCard(++_zOrder, selected[i*2+0],
                                        _currentProblemID, pieceInfo.pieceID, 1,
                                        pieceInfo.imageNameA);
        auto cardB = createMatchingCard(++_zOrder, selected[i*2+1],
                                        _currentProblemID, pieceInfo.pieceID, 2,
                                        pieceInfo.imageNameB);
        
        // NB(xenosoz, 2016): It seems some LetterMatching resources are in NumberMatching folder
        //   for some reason. I'll just follow that legacy in this time. We need a hero.
        SoundEffect sound;
        sound = sound || SoundEffect(pieceInfo.matchSound);
        sound = sound || SoundEffect("NumberMatching/Sound/" + pieceInfo.matchSound);
        sound = sound || SoundEffect("NumberMatching/Images/Letter/" + pieceInfo.matchSound);
        sound = sound || SoundEffect("NumberMatching/Sound/star.wav");

        sound.preload();
        cardA->matchSound = sound;
        cardB->matchSound = sound;
        matchingCardList.push_back(cardA);
        matchingCardList.push_back(cardB);
    }
}

LetterMatchingCard* LetterMatchingScene::createMatchingCard(int zOrder, Point pos,
                                                            int level, int id, int type,
                                                            const std::string& cardImageName)
{
    LetterMatchingCard* card = LetterMatchingCard::create();
    card->setImage(level, type, id, cardImageName);
    card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    card->setPosition(pos);
    CCLOG("%f, %f", pos.x, pos.y);


    card->setScale(_defaultScaleFactor);
    _gameNode->addChild(card, zOrder);

    card->defaultScale = _defaultScaleFactor;
    bindingEvents(card);
    
    return card;
}

void LetterMatchingScene::bindingEvents(LetterMatchingCard *card)
{
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this, card](Touch* touch, Event* event) {
        
        Point touchPoint = touch->getLocation();
        Point viewPoint = _gameNode->convertToNodeSpace(touchPoint);
        
        if (card->isTouched) return false;
        if (card->isMatchDone) return false;
        if (!card->isTouchedIn(viewPoint)) return false;
        
        card->isTouched = true;
        
        card->runAction(ScaleTo::create(0.1f, _upScaleFactor));
        _gameNode->reorderChild(card, ++_zOrder);
        return true;
        
    };
    
    listener->onTouchMoved = [this, card](Touch* touch, Event* event) {
        
        card->setPosition(card->getPosition() + touch->getDelta());
        
        if (card->isLinked) {
            if (!shouldKeepAsPair(card->linkedTarget, card)) {
                card->linkedTarget->setLink(nullptr);
                card->setLink(nullptr);
            }
        } else {
            
            
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
    };
    
    
    listener->onTouchEnded =  [this, card](Touch* touch, Event* event) {
        
        card->stopParticle();
        card->isTouched = false;
        
        if (card->isLinked) {
            auto other = card->linkedTarget;
            
            other->stopParticle();
            
            
            card->isMatchDone = true;
            other->isMatchDone = true;
            
            auto moveTo = MoveTo::create(0.4f, other->getPosition());
            
            card->runAction(Sequence::create(EaseElasticOut::create(moveTo), CallFunc::create([=](){
                other->setPosition(Vec2::ONE * 9999.0f);
            }), nullptr));
            
            card->runAction(Sequence::create(DelayTime::create(0.3f), EaseBackIn::create(ScaleTo::create(0.3f, _upScaleFactor)), nullptr));
            
            card->runAction(Sequence::create(DelayTime::create(0.3f), EaseExponentialIn::create(FadeOut::create(0.3f)), nullptr));
            
            card->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create([=](){
                card->matchSound.play();
                this->addStarParticle(card);
                
                card->setPosition(Vec2::ONE * 9999.0f);
                
                _matchCount++;
                if (_matchCount == _currentLevelMaxCardCount)
                {
                    this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
                        onSolve();
                    }), nullptr));
                }
            }), nullptr));
        } else {
            
            card->runAction(ScaleTo::create(0.1f, _defaultScaleFactor));
            
            
        }
        
        
        
        
    };
    
    listener->onTouchCancelled = [this, card](Touch* touch, Event* event) {
        
        card->isTouched = false;
        
        if (card->isLinked) {
            card->linkedTarget->setLink(nullptr);
            card->setLink(nullptr);
        }
    };
    
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card);
    

}

void LetterMatchingScene::addStarParticle(Node* targetNode)
{
    ParticleSystemQuad* _particleEffect = ParticleSystemQuad::create("NumberMatching/Particle/star_particle.plist");
    _particleEffect->setScale(6.0f);
    _particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _particleEffect->setPosition(targetNode->getPosition());
    _gameNode->addChild(_particleEffect, ++_zOrder);
}
