//
//  TappingScene.cpp
//
//  Created by Gunho Lee on 12/10/16.
//
//


#include "TappingScene.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace TappingSceneSpace {

    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    const Size gameSize = Size(2560, 1800);

    const Rect area1 = Rect(1280-500, 500, 1000, 500);
    const Rect area2 = Rect(1280-1000, 300, 2000, 1000);
    const Rect area3 = Rect(1280-1000, 300, 2000, 1200);
    
    SoundEffect balloonBlowEffect() { return SoundEffect("Tapping/SFX_Balloon-blow-up.m4a"); }
    SoundEffect balloonPopEffect() { return SoundEffect("Tapping/SFX_Balloon-pop.m4a"); }
    SoundEffect bubblePopEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Star_Pop2.m4a"); }
    
    const int balloonEndurance = 10;
    
    
    
    
}

using namespace TappingSceneSpace;


Scene* TappingScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TappingScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TappingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    
    auto winSize = getContentSize();
    
    _bgNode = Node::create();
    _bgNode->setContentSize(winSize);
    addChild(_bgNode);
    

    
    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    addChild(_gameNode);

    

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    

    
    _popCount = 0;
    
    
    
    // cache
    for (int index = 1; index<=10; index++) {
        string bodyName = "Tapping/tg_balloon"+TodoUtil::itos(index)+"_base.png";
        string shadeName = "Tapping/tg_balloon"+TodoUtil::itos(index)+"_shading.png";
        Sprite::create(bodyName);
        Sprite::create(shadeName);
    }
    
    return true;
}

void TappingScene::onEnter()
{
    Layer::onEnter();


   // _progressBar->setMax((int)sheet.size());

}

void TappingScene::onEnterTransitionDidFinish()
{
    onStart();
}

void TappingScene::onStart()
{
    


    
    
    
//    _
//    float appearTime = 0;
//    putObjects(appearTime);
}


void TappingScene::setLevel(int level)
{
    _currentLevel = level;
    
    auto bgName = level==3 ? "Tapping/tappinggame_grassbackground1.jpg" : "Tapping/tg_background1.jpg";
    auto bg = Sprite::create(bgName);
    auto bgSize = bg->getContentSize();
    auto winSize = _bgNode->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    _bgNode->addChild(bg);
    
    
    if (_currentLevel==3) {
        bubblePopEffect().preload();
    } else {
        balloonBlowEffect().preload();
        balloonPopEffect().preload();
    }
    
    if (_currentLevel==1) {
        _progressBar->setMax(3);
        _progressBar->setCurrent(_popCount+1);
        placeBalloon(balloonEndurance, area1);
    } else if (_currentLevel==2) {
        _progressBar->setMax(10);
        _progressBar->setCurrent(_popCount+1);
        for (int i=0; i<5; i++) {
            placeBalloon(balloonEndurance, area2);
        }
    } else if (_currentLevel==3) {
        for (int i=0; i<6; i++) {
            placeBalloon(1, area3, true, random(0.0, 5.0));
        }
    }
    

    
    
}



void TappingScene::placeBalloon(int tapCount, Rect placeArea, bool isBubble, float delay)
{
    
    const int placeTry = 10;

    float maxDist = -1;
    Point bPoint;
    
    for (int i=0; i<placeTry; i++) {
        float x = random(0.f, placeArea.size.width);
        float y = random(0.f, placeArea.size.height);
    
        Point p = Point(placeArea.origin.x + x, placeArea.origin.y+y);
        if (maxDist<0) bPoint = p;
        
        float minDist = -1;
        for (auto v : _balloons) {
            auto dist = p.distance(v->getPosition());
            if (minDist<0 || dist < minDist) {
                minDist = dist;
            }
        }
        
        if (minDist>maxDist) {
            maxDist = minDist;
            bPoint = p;
        }

    }
    
    auto balloon = TappingBalloon::create();
    if (_currentLevel==3) {
        balloon->setupBubble();
        
        
    } else {
        const Color3B colors[] = { Color3B(240, 58, 58), Color3B(240, 95, 165), Color3B(240, 200, 58),
            Color3B(250, 115, 60), Color3B(110, 190, 70), Color3B(110, 40, 130) };
        
        balloon->setupBalloon(tapCount, colors[random(0, 5)]);
        
        
        
    }
    balloon->setPosition(bPoint);
    
    auto duration = random(0.55, 0.95);
    balloon->runAction(RepeatForever::create(Sequence::create(EaseOut::create(MoveBy::create(duration, Vec2(0, 10)), 1.2),
                                                              EaseInOut::create(MoveBy::create(duration*2, Vec2(0, -20)), 1.2),
                                                              EaseIn::create(MoveBy::create(duration, Vec2(0, 10)), 1.2), NULL)));
    

    _gameNode->addChild(balloon);
    _balloons.push_back(balloon);
    
    if (isBubble) {
        balloon->setOpacity(0);
        auto initDelay = DelayTime::create(random(0.0, 0.5) + delay);
        auto fadeinAction = FadeIn::create(0.2);
        auto enableAction = CallFunc::create([balloon](){ balloon->_touchEnabled = true; });
        auto fadeoutAction = FadeOut::create(0.2);
        auto delayAction = DelayTime::create(2.5 + 3.0*(100-_popCount)/100.0);
        auto seq = Sequence::create(initDelay, fadeinAction, enableAction, delayAction, fadeoutAction, CallFunc::create([this, balloon](){
            balloonPoped(balloon, false);
        }), nullptr);
        balloon->runAction(seq);
    } else {
        balloon->setOpacity(0);
        balloon->runAction(Sequence::create(
                                      FadeIn::create(random(0.3, 0.8)),
                                      CallFunc::create([balloon](){ balloon->_touchEnabled = true; }),
                                      nullptr));
    }
    
    balloon->OnPoped = [balloon, this]() {
        balloonPoped(balloon, true);
    };
    
    
    
}

void TappingScene::balloonPoped(TappingBalloon *balloon, bool touched)
{
    balloon->_touchEnabled = false;
    
    auto p = balloon->getPosition();
    
    auto it = std::find(_balloons.begin(), _balloons.end(), balloon);
    if (it!=_balloons.end()) {
        _balloons.erase(it);
    }
    
    
    auto popLabel = [&](int count, float fontSize, Vec2 p) {
        const vector<Color4B> colors = { Color4B(247, 233, 133, 255), Color4B(140, 255, 250, 255),
            Color4B(255, 200, 230, 255), Color4B(100, 252, 154, 255) };
        auto color = colors.at(random(0, 3));
        
        auto label = TodoUtil::createLabel(TodoUtil::itos(count), fontSize, Size::ZERO, defaultFont, color);
        label->setPosition(p);
        _gameNode->addChild(label);
        auto labelSeq = Sequence::create(MoveBy::create(0.7, Vec2(0, 50)), CallFunc::create([label](){
            label->removeFromParent();
        }), NULL);
        label->runAction(labelSeq);
    };
    
    if (_currentLevel==1) {
        balloon->runPopAnimation();
        
        _progressBar->setCurrent(_popCount+1, true);
        _popCount++;
        
        popLabel(_popCount*balloonEndurance, 200, balloon->getCenter());
        
        
        if (_popCount==3) {
            CompletePopup::create()->show(1.0, [](){
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
        } else {
            auto seq = Sequence::create(DelayTime::create(0.7), CallFunc::create([this](){
                _progressBar->setCurrent(_popCount+1);
                placeBalloon(10, area1);
            }), nullptr);
            this->runAction(seq);
        }
    } else if (_currentLevel==2) {
        balloon->runPopAnimation();
        
        _progressBar->setCurrent(_popCount+1, true);
        _popCount++;
        
        popLabel(_popCount*balloonEndurance, 200, balloon->getCenter()  );
        
        if (_popCount==10) {
            CompletePopup::create()->show(1.0, [](){
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
        } else {
            auto seq = Sequence::create(DelayTime::create(0.7), CallFunc::create([this](){
                _progressBar->setCurrent(_popCount+1);
                placeBalloon(10, area2);
            }), nullptr);
            this->runAction(seq);
        }

    } else if (_currentLevel==3) {
        if (touched) {
            
            balloon->runPopAnimation();
            
            _progressBar->setCurrent(_popCount+1, true);
            _popCount++;
            
            popLabel(_popCount, 100, p);
            
            if (_popCount==100) {
                CompletePopup::create()->show(1.0, [](){
                    CCAppController::sharedAppController()->handleGameComplete(1);
                });
            } else {
                auto seq = Sequence::create(DelayTime::create(0.7), CallFunc::create([this](){
                    _progressBar->setCurrent(_popCount+1);
                    placeBalloon(1, area3, true);
                }), nullptr);
                this->runAction(seq);
            }
        } else {
            balloon->removeFromParent();
            
            placeBalloon(1, area3, true, random(1.0, 3.0));
        }
    }
}

///////////// TappingBalloon

void TappingBalloon::setupBubble()
{
    _isBalloon = false;
    
    string bubbleName = "Tapping/bubble_soap_"+TodoUtil::itos(random(1, 4))+".png";
    _body = Sprite::create(bubbleName);
    
    addChild(_body);
    
    auto size = _body->getContentSize();
    _touchRect = Rect(-size.width/2, -size.height/2, size.width, size.height);
    
    _maxTap = 1;
    _tapCount = 0;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (!this->_touchEnabled) return false;
        auto P = convertToNodeSpace(T->getLocation());
        if (_touchRect.containsPoint(P)) {
            _tapCount++;
            if (_tapCount==_maxTap && OnPoped) OnPoped();
            return true;
        }
        return false;
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
}

void TappingBalloon::setupBalloon(int maxTap, cocos2d::Color3B color)
{
    _isBalloon = true;
    
    _body = Sprite::create();
    _body->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    _body->setColor(color);
    addChild(_body);
    
    _shade = Sprite::create();
    addChild(_shade);
    
    _knot = Sprite::create("Tapping/tg_balloon_string_knot.png");
    addChild(_knot);
    
    _maxTap = maxTap;
    _tapCount = 0;
    
    inflateBalloon(0);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (!this->_touchEnabled) return false;
        
        auto P = convertToNodeSpace(T->getLocation());
        if (_touchRect.containsPoint(P)) {
            
            _tapCount++;
            
            auto label = TodoUtil::createLabel(TodoUtil::itos(_tapCount), 100, Size::ZERO, defaultFont, Color4B(102, 47, 21, 255));
            auto p = Point(random(-50.f, 50.f), random(-50.f, 50.f));
            auto pp = Point(_touchRect.origin.x + p.x + ((p.x>0) ? _touchRect.size.width : 0),
                            _touchRect.origin.y + p.y + ((p.y>0) ? _touchRect.size.height : 0));
            
            label->setPosition(pp);
            auto labelSeq = Sequence::create(
                                             Spawn::create(MoveBy::create(0.7, Vec2(0, 50)),
                                                           FadeOut::create(0.7), nullptr),
                                             CallFunc::create([label](){
                label->removeFromParent();
            }), NULL);
            label->runAction(labelSeq);
            
            
            this->addChild(label);
            
            if (_tapCount<_maxTap) {
                balloonBlowEffect().stop();
                balloonBlowEffect().play();
                inflateBalloon(_tapCount);
            } else if (_tapCount==_maxTap && OnPoped) {
                OnPoped();
            }
            return true;
        }
        return false;
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
}

void TappingBalloon::runPopAnimation()
{
    _touchEnabled = false;
    
    if (_isBalloon) {
        
        balloonPopEffect().play();
        balloonBlowEffect().stop();
        
        
        _body->setTexture("Tapping/tg_balloonburst_effect.png");
        _shade->setVisible(false);
        _knot->setVisible(false);
        
        auto popSpwan = Spawn::create(EaseOut::create(ScaleTo::create(0.3, 1.3), 3), EaseOut::create(FadeOut::create(0.3), 3), NULL);
        
        auto popSeq = Sequence::create(popSpwan, CallFunc::create([this](){
            removeFromParent();
        }), NULL);
        
        _body->setScale(0.9);
        _body->runAction(popSeq);
    } else {
        
        bubblePopEffect().play();
        
        auto popSpwan = Spawn::create(EaseOut::create(ScaleTo::create(0.3, 1.3), 3), EaseOut::create(FadeOut::create(0.3), 3), NULL);
        
        auto popSeq = Sequence::create(popSpwan, CallFunc::create([this](){
            removeFromParent();
        }), NULL);
        _body->setScale(0.9);
        _body->runAction(popSeq);
    }
}

Vec2 TappingBalloon::getCenter()
{
    
    auto innerCenter = _touchRect.origin + _touchRect.size/2;
    auto worldPos = convertToWorldSpace(innerCenter);
    auto P = getParent();
    return P->convertToNodeSpace(worldPos);
    
}

void TappingBalloon::inflateBalloon(int count)
{
    

    
    int index = count+1;
    int scale = 1.0;
    if (index>10) {
        scale = 1.0+0.1*(index-10);
        index = 10;
    }
    
    string bodyName = "Tapping/tg_balloon"+TodoUtil::itos(index)+"_base.png";
    string shadeName = "Tapping/tg_balloon"+TodoUtil::itos(index)+"_shading.png";
    

    
    _body->setTexture(bodyName);
    _shade->setTexture(shadeName);
    
    
    Size bSize;
    
    switch (index) {
        case 1: bSize = Size(180, 210); break;
        case 2: bSize = Size(210, 210); break;
        case 3: bSize = Size(270, 270); break;
        case 4: bSize = Size(290, 330); break;
        case 5: bSize = Size(340, 390); break;
        case 6: bSize = Size(385, 445); break;
        case 7: bSize = Size(445, 515); break;
        case 8: bSize = Size(530, 600); break;
        case 9: bSize = Size(625, 705); break;
        case 10:
        default:
            bSize = Size(770, 860); break;

    }
    
    this->_touchEnabled = false;
    _touchRect = Rect(-bSize.width/2, -100, bSize.width, bSize.height);
    
    auto oldScale = getScale();
    
    setScale(oldScale*0.85);
    this->runAction(Sequence::create(
                                     EaseIn::create(ScaleTo::create(0.1, scale), 5.0),
                                     CallFunc::create([this](){
        this->_touchEnabled = true;
    }),
                                     nullptr));
    
                                     
    
    
    
}





                           
                           
