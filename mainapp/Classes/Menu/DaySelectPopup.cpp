//
//  DaySelectPopup.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/25/16.
//
//

#include "DaySelectPopup.hpp"

#include "ui/uiButton.h"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"

#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/GameSoundManager.h"

#include "Bird.hpp"
#include "CoopScene.hpp"

#include <time.h>
#include <algorithm>
#include <iterator>

namespace DaySelectPopupSpace {
    
    
    const Size viewSize = Size(2560, 1800);
    const string defaultFont = "fonts/TodoMainCurly.ttf";
    const string folder = "MainScene/DaySelect/";
    
    const Color4B whiteColor = Color4B(255, 252, 236, 255);
    //const Color4B darkColor = Color4B(115, 71, 0, 255);
    
    const int dayLabelTag = 200;
    
    const float dayLabelY = 62;
    const float dayLabelYComp = 56;
    
    
    
    
}

using namespace DaySelectPopupSpace;
using namespace std;

DaySelectPopup* DaySelectPopup::create(Node* parentView, string levelID)
{
    DaySelectPopup *popup = new (std::nothrow) DaySelectPopup();
    if (popup && popup->init(parentView, levelID))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool DaySelectPopup::init(cocos2d::Node *parentView, string levelID)
{

    
    if (!Node::init()) {
        return false;
    }
    
    _parentView = parentView;
    _levelID = levelID;
    
    _touchEnabled = true;
    
    auto winSize = Director::getInstance()->getWinSize();
    
    setContentSize(winSize);

    
    
    _backView = Node::create();
    _backView->setContentSize(winSize);
    auto blocker = EventListenerTouchOneByOne::create();
    blocker->setSwallowTouches(true);
    blocker->onTouchBegan = [](Touch* T, Event* E) {
        return true;
    };
    _backView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(blocker, _backView);
    addChild(_backView);
    
    _bgView = Sprite::create(folder+"daily_bg.jpg");
    auto bgSize = _bgView->getContentSize();
    _bgView->setScale(MAX(winSize.width/bgSize.width, winSize.height/bgSize.height));
    _bgView->setPosition(winSize/2);
    addChild(_bgView);
    
    _mainView = Node::create();
    _mainView->setContentSize(viewSize);
    _mainView->setScale(MIN(winSize.width/viewSize.width, winSize.height/viewSize.height));
    _mainView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _mainView->setPosition(Vec2(winSize.width/2, winSize.height));
    addChild(_mainView);
    

    {
        auto backBtn = ui::Button::create();
        backBtn->loadTextures("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png");
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (!_touchEnabled) return;
            
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                this->dismiss();
            }
        };
        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
        backBtn->addClickEventListener([this](Ref*){
            if (!_touchEnabled) return;
            SoundEffect::menuBackEffect().play();
            
            CoopScene *parentCoop = dynamic_cast<CoopScene*>(_parentView);
            if (parentCoop==nullptr) {
                auto scene = CoopScene::createScene();
                Director::getInstance()->pushScene(TransitionFade::create(0.8, scene));
            
                this->runAction(Sequence::create(DelayTime::create(0.8),
                                                 CallFunc::create([this](){ this->dismiss();}), nullptr));
            } else {
                this->dismiss();
            }
        });
        
        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backBtn->setPosition(Vec2(25, winSize.height-25));
        addChild(backBtn);
    }
    
    
    
    _board = Sprite::create(folder+"daily_window_bg.png");
    _board->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _boardPos = Vec2(122, viewSize.height);
    _board->setPosition(_boardPos);
    _mainView->addChild(_board);
    
    auto boardSize = _board->getContentSize();
    
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    {
        string panelFilename = cur->category=='L' ? "daily_window_title_panel_English.png" : "daily_window_title_panel_math.png";
        if (cur->categoryLevel==0) panelFilename = "daily_window_title_panel_prek.png";
        auto panel = Sprite::create(folder+panelFilename);
        panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        panel->setPosition(Vec2(boardSize.width/2, boardSize.height-300));
        _board->addChild(panel);
        
        auto panelLabel = TodoUtil::createLabel(cur->levelTitle, 70, Size::ZERO, defaultFont, Color4B(255, 252, 236, 255));
        panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        panelLabel->setPosition(panel->getContentSize()/2);
        panel->addChild(panelLabel);
        
    }

    Size btnSize = Size(289, 358);
    Size btnMargin = Size(36, 38);
    int numBtnX = 4;
    
    
    float leftX = (boardSize.width - (numBtnX*btnSize.width + (numBtnX-1)*btnMargin.width))/2;
    float topY = 3*(btnMargin.height+btnSize.height);
    
    
    int x = leftX;
    int y = topY;
    
    auto numCleared = UserManager::getInstance()->numDayCleared(levelID);

    for (int i=0; i<cur->numDays; i++) {
        
        auto day = i+1;
        Sprite *btn = nullptr;

        
        bool cleared = UserManager::getInstance()->isDayCleared(levelID, day);
        
        bool lastday = i==cur->numDays-1;
        bool locked = lastday && (cur->categoryLevel>0) && !cleared && (numCleared<cur->numDays-1);
        
        
        
        string btnFile;
        if (lastday && cur->categoryLevel>0) {
            btnFile = cleared ? "daily_window_icon_challenge_complete.png" : "daily_window_icon_challenge.png";
        } else {
            btnFile = cleared ? "daily_window_icon_todo_complete.png" : "daily_window_icon_todo.png";
        }
        
        btn = Sprite::create(folder+btnFile);
        
        
        auto dayLabel = TodoUtil::createLabel(TodoUtil::itos(day), 65, Size::ZERO, defaultFont, whiteColor);
        dayLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dayLabel->setPosition(btnSize.width/2, cleared ? dayLabelYComp : dayLabelY);
        btn->addChild(dayLabel, 0, dayLabelTag);
        

        btn->setPosition(x+btnSize.width/2, y-btnSize.height/2);

        auto cover = Sprite::createWithTexture(btn->getTexture());
        cover->setColor(Color3B::BLACK);
        cover->setOpacity(64);
        cover->setVisible(false);
        cover->setAnchorPoint(Vec2::ZERO);
        btn->addChild(cover);
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, btn, cover](Touch* T, Event* E) {
            if (!_touchEnabled) return false;
            
            auto P = btn->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (btn->getBoundingBox().containsPoint(pos)) {
                cover->setVisible(true);
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this, btn, cover](Touch* T, Event* E) {
            if (!_touchEnabled) return;
            
            auto P = btn->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            cover->setVisible(btn->getBoundingBox().containsPoint(pos));
        };
        
        listener->onTouchCancelled = [cover](Touch* T, Event* E) {
            cover->setVisible(false);
        };

        
        
        listener->onTouchEnded = [this, btn, day, locked, cover](Touch* T, Event* E) {
            cover->setVisible(false);
            
            if (!_touchEnabled) return;
            
            auto P = btn->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (!btn->getBoundingBox().containsPoint(pos)) return;
            
            if (!locked) {
                SoundEffect::buttonEffect().play();
                this->dayChosen(day);
            }
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, btn);

        
        
        if (i%numBtnX == (numBtnX-1)) {
            x = leftX;
            y -= (btnSize.height+btnMargin.height);
        } else {
            x+=(btnSize.width+btnMargin.width);
        }
        
        
        _board->addChild(btn, 0, day);

    }
    
    
    {
        const Size stageSize = Size(960, 1700);
        
        _stage = Node::create();
        _stage->setContentSize(stageSize);
        _stage->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _stage->setPosition(viewSize);
        _mainView->addChild(_stage);
    
        
        
        
        for (int i=0; i<3; i++) {
        
            auto l = LightBall::create();
            l->setup(1);
            _stage->addChild(l);
            lightFall(l, true);
            
        }
        for (int i=0; i<2; i++) {
            
            auto l = LightBall::create();
            l->setup(2);
            _stage->addChild(l);
            lightFall(l, true);
            
        }
        
        auto beam = Sprite::create(folder+"daily_window_beam.png");
        beam->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        beam->setPosition(stageSize);
        _stage->addChild(beam);
    
        
        auto treetop = Sprite::create(folder+"daily_treetop.png");
        treetop->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        treetop->setPosition(winSize);
        this->addChild(treetop);
    
        
        
        auto treestump = Sprite::create(folder+"daily_treestump.png");
        treestump->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        treestump->setPosition(Vec2(stageSize.width/2, 0));
        _stage->addChild(treestump);
        
        auto treeshadow = Sprite::create(folder+"daily_treestump_charactershadow.png");
        treeshadow->setPosition(Vec2(stageSize.width/2, 438));
        _stage->addChild(treeshadow);
        
        
        Bird* bird = Bird::create(cur->category, cur->categoryLevel, cur->levelID);
        bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(cur->levelID));
        bird->onTouchBegan = [bird](){
            bird->runTouchAnimation();
        };
        

        auto shadowscale = bird->getBoundingBox().size.width / treeshadow->getContentSize().width;
        treeshadow->setScale(MIN(1.0, shadowscale));
    
        bird->setPosition(Vec2(stageSize.width/2, 448));
        _stage->addChild(bird);

    }

    if (UserManager::getInstance()->isDebugMode()) {
        
        auto clear = ui::Button::create();
        clear->setTitleText("Clear");
        clear->setTitleFontName(defaultFont);
        clear->setTitleFontSize(50);
        clear->setPosition(Vec2(winSize.width-300, 200));
        clear->addClickEventListener([this, cur, levelID](Ref*) {
            for (int i=0; i<cur->numDays-1; i++) {
                UserManager::getInstance()->setDayCleared(levelID, i+1);
            }
            this->dismiss();
        });
        addChild(clear);
        
    }
    
    GameSoundManager::getInstance()->playBGM("Common/Music/BGM2_DailySelection.m4a");
    
    return true;
}

void DaySelectPopup::lightFall(LightBall *l, bool first)
{
    
    l->stopAllActions();
    l->_ball->stopAllActions();
    
    
    l->setOpacity(255);
    l->_ball->setOpacity(0);
    
    auto p1 = Vec2(random(480-200, 480+200), first ? random(1200, 1700) : 1700);
    auto p2 = Vec2(p1.x, random(450, 1000));
    
    float speed = (l->_type==1) ? random(80.f, 100.f) : random(40.f, 60.f);
    
    auto t = (p1.y-p2.y)/speed;
    
    l->setPosition(p1);
    
    l->runAction(Sequence::create(
      MoveTo::create(t, p2),
      Spawn::create(
                    MoveBy::create(0.5, Vec2(0, -0.5*speed)),
                    FadeOut::create(0.5), nullptr),
      DelayTime::create(random(0.f, 0.3f)),
      CallFunc::create([this, l](){
        this->lightFall(l, false);
    }), nullptr));
    

    auto bt = random(1.1, 1.2);
    
    l->_ball->runAction(RepeatForever::create(Sequence::create(FadeTo::create(bt, 255), DelayTime::create(bt/2), FadeTo::create(bt, 128), DelayTime::create(bt/2), nullptr)));
    
    
}

void DaySelectPopup::show()
{
    _parentView->addChild(this);
    
    this->setVisible(false);
    
    
    
    auto t = 0.3;
    auto winSize = getContentSize();
    auto cover = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
    
    _parentView->addChild(cover);
    cover->runAction(Sequence::create(FadeIn::create(t), CallFunc::create([this](){
        this->setVisible(true);
    }), FadeOut::create(t), CallFunc::create([cover](){
        cover->removeFromParent();
    }), nullptr));
                     
    
    
    
    
    auto boardSize = _board->getContentSize();
    
    _board->setPosition(Vec2(_boardPos.x, viewSize.height+boardSize.height));
    
    _board->runAction(Sequence::create(DelayTime::create(t), EaseOut::create(MoveTo::create(0.5, _boardPos), 3.0), nullptr));
    
}

void DaySelectPopup::showClear(int day)
{

    _touchEnabled = false;
    
    show();
    
    auto cur = CurriculumManager::getInstance()->findCurriculum(_levelID);
    
    bool lastday = day==cur->numDays;


    string f1, f2;
    if (lastday && cur->categoryLevel>0) {
        f1 = folder+"daily_window_icon_challenge.png";
        f2 = folder+"daily_window_icon_challenge_complete.png" ;
    } else {
        f1 = folder+"daily_window_icon_todo.png";
        f2 = folder+"daily_window_icon_todo_complete.png";
    }

    Sprite *btn = (Sprite*)_board->getChildByTag(day);
    int nextDay = 1;
    for (; nextDay<=cur->numDays; nextDay++) {
        if (nextDay==day) continue;
        if (!UserManager::getInstance()->isDayCleared(_levelID, nextDay)) break;
    }
    
    if (nextDay>cur->numDays) nextDay = -1;
    
    
    Sprite *nextBtn = (Sprite*)_board->getChildByTag(nextDay);
    
    
    Sequence *nextSeq;
    
    if (nextBtn && nextDay>0) {
        

        
      

        
        nextSeq = Sequence::create(CallFunc::create([this, nextDay](){
            auto b = (Sprite*)_board->getChildByTag(nextDay);
            
            auto blinkSeq2 = Sequence::create(FadeOut::create(0.5),
                                              FadeIn::create(0.5),
                                              nullptr);
            auto seqForNextBtn = Sequence::create(Repeat::create(blinkSeq2, 3),
                                                  DelayTime::create(0.5),
                                                  CallFunc::create([this, nextDay](){
                _touchEnabled = true;
                //this->dayChosen(nextDay);
                //this->dismiss();
            }),nullptr);

                                                  
            b->runAction(seqForNextBtn);
        }), nullptr);
        
                                   

    } else {
        nextSeq = Sequence::create(CallFunc::create([](){
            
            auto scene = CoopScene::createScene();
            Director::getInstance()->pushScene(TransitionFade::create(0.8, scene));
        }),
                                   DelayTime::create(0.8),
                                   CallFunc::create([this](){
            _touchEnabled = true;
            this->dismiss();
        }),
                                   nullptr);
     

    }


    btn->setCascadeOpacityEnabled(true);
    btn->setOpacity(0);

    btn->setTexture(f1);
    auto l = btn->getChildByTag(dayLabelTag);
    if (l) l->setPositionY(dayLabelY);
    

    auto blinkSeq = Sequence::create(FadeIn::create(0.5),
                                     FadeOut::create(0.5),
                                     nullptr);
    

    auto seq = Sequence::create(Repeat::create(blinkSeq, 3),
                                CallFunc::create([btn, f2]() {
        btn->setTexture(f2);
        auto l = btn->getChildByTag(dayLabelTag);
        if (l) l->setPositionY(dayLabelYComp);
        
        btn->setOpacity(255);
        SoundEffect::dayClearEffect().play();
        
    }),
                                ScaleTo::create(0.2, 1.2),
                                ScaleTo::create(0.2, 1.0),
                                DelayTime::create(1.0),
                                nextSeq,
                                nullptr);
    
    
    btn->runAction(seq);
    
                                
                                
    

}

void DaySelectPopup::dayChosen(int day)
{
    if (!_touchEnabled) return;
    _touchEnabled = false;
    
    if (onSelectDay) this->onSelectDay(day);

}

void DaySelectPopup::dismiss()
{

    
    auto t = 0.3;
    auto winSize = getContentSize();
    auto cover = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
    
    _parentView->addChild(cover);
    cover->runAction(Sequence::create(FadeIn::create(t), CallFunc::create([this](){
        if (onDismiss) this->onDismiss();
        removeFromParent();
    }), FadeOut::create(t), CallFunc::create([cover](){
        cover->removeFromParent();
    }), nullptr));
}


/////////

void LightBall::setup(int type)
{
    _type = type;
    if (type==1) {
        _ball = Sprite::create(folder+"daily_window_beam_glitter_1.png");
    } else {
        _ball = Sprite::create(folder+"daily_window_beam_glitter_2.png");
    }
    addChild(_ball);
    
    setCascadeOpacityEnabled(true);
    
}
