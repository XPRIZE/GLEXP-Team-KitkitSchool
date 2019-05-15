//
//  DailyScene.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 09/10/17.
//
//

#include "DailyScene.hpp"
#include "GameSelectScene.hpp"

#include "CCAppController.hpp"
#include "CustomDirector.h"

#include "ui/UIButton.h"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"

#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/LogManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"

#include "Common/Controls/PopupBase.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CoinTab.hpp"
#include "Common/Controls/TouchEventLogger.h"

#include "Bird.hpp"




#include <time.h>
#include <algorithm>
#include <iterator>

namespace DailySceneSpace {
    
    
    const Size viewSize = Size(2560, 1800);
    
    const string defaultFont = "fonts/TodoMainCurly.ttf";
    const string folder = "MainScene/DailyScene/";
    
    const Color4B whiteColor = Color4B(255, 252, 236, 255);
    //const Color4B darkColor = Color4B(120, 16, 0, 255);
    const Size boardSize = Size(1200, 1200);
    
    const Size startBtnSize = Size(846, 582);
    const Size startBtnLabelSize = Size(700, 250);
    const Vec2 startBtnLabelPos = Vec2(423, 385);
    
    const int dayLabelTag = 200;
    
    Vec2 tabPos;
  
    
    
//    const float dayLabelY = 62;
//    const float dayLabelYComp = 56;
    
    
    
    
}

using namespace DailySceneSpace;
using namespace std;

Scene* DailyScene::createScene(string levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = DailyScene::create(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



DailyScene* DailyScene::create(string levelID)
{
    DailyScene *popup = new (std::nothrow) DailyScene();
    if (popup && popup->init(levelID))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool DailyScene::init(string levelID)
{

    
    if (!Node::init()) {
        return false;
    }
    

    StrictLogManager::shared()->dayChoice_Begin(levelID);
    _levelID = levelID;
    _currentDay = 0;
    _doneClearedDay = false;
    
    _touchEnabled = true;
    
    _cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    _freechoiceGames.clear();
    
    
    auto winSize = Director::getInstance()->getWinSize();
    setContentSize(winSize);

    
    
//    _backView = Node::create();
//    _backView->setContentSize(winSize);
//    auto blocker = EventListenerTouchOneByOne::create();
//    blocker->setSwallowTouches(true);
//    blocker->onTouchBegan = [](Touch* T, Event* E) {
//        return true;
//    };
//    _backView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(blocker, _backView);
//    addChild(_backView);
    
    _bgView = Sprite::create(folder+"daily_bg.png");
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
       // auto cl = LayerColor::create(Color4B(255, 0, 0, 64), viewSize.width, viewSize.height);
       // _mainView->addChild(cl);
    }
    

    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    backButton->onBack = [this] {
        StrictLogManager::shared()->dayChoice_End_Quit(_levelID);
    };
    addChild(backButton);
    
    
    

    _mangoBoard = Node::create();
    _mangoBoard->setContentSize(boardSize);
    _mangoBoard->setPosition(Vec2(184, winSize.height-280-boardSize.height));
    _mainView->addChild(_mangoBoard);
    
    
    
    
    {
        string panelFilename = _cur->category=='L' ? "daily_panel_literacy.png" : "daily_panel_math.png";
        if (_cur->categoryLevel==0) panelFilename = "daily_panel_prek.png";
        _panel = Sprite::create(folder+panelFilename);
        _panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _panel->setPosition(Vec2(viewSize.width/2, viewSize.height));
        _mainView->addChild(_panel);
        
        auto panelLabel = TodoUtil::createLabel(_cur->levelTitle, 70, Size::ZERO, defaultFont, Color4B(255, 252, 236, 255));
        panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        panelLabel->setPosition(_panel->getContentSize()/2 - Size(0, 42));
        _panel->addChild(panelLabel);
        
    }


    
    
    
    {
        const Size stageSize = Size(960, winSize.height);
        
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
        
        auto beam = Sprite::create(folder+"daily_beam.png");
        beam->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        beam->setPosition(stageSize);
        _stage->addChild(beam);
    
        
        auto treetop = Sprite::create(folder+"daily_treetop_right.png");
        treetop->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        treetop->setPosition(winSize);
        this->addChild(treetop);
    
        
        
//        auto treestump = Sprite::create(folder+"daily_treetrunk.png");
//        treestump->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//        treestump->setPosition(Vec2(stageSize.width/2, 0));
//        _stage->addChild(treestump);
        
//        _birdShadow = Sprite::create(folder+"daily_birdshadow.png");
//        _birdShadow->setPosition(Vec2(stageSize.width/2, 318));
//        _stage->addChild(_birdShadow);
        
        auto buttonShadow = Sprite::create(folder+"daily_button_shadow.png");
        buttonShadow->setPosition(Vec2(stageSize.width-540, 200));
        _stage->addChild(buttonShadow);
        

        _startBtnNode = Node::create();
        _startBtnNode->setContentSize(startBtnSize);
        _startBtnNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        auto startBtnPos = Vec2(stageSize.width-108-startBtnSize.width/2, 190);
        _startBtnNode->setPosition(startBtnPos);
        _stage->addChild(_startBtnNode);
        
        
        auto btnPanel = Sprite::create(folder+"daily_button_woodpanel.png");
        btnPanel->setPosition(startBtnSize/2);
        _startBtnNode->addChild(btnPanel);
        
        const auto btnNormal = folder+"daily_button_normal.png";
        const auto btnPressed = folder+"daily_button_active.png";
        
        _startBtnSprite = Sprite::create(btnNormal);
        _startBtnSprite->setPosition(startBtnSize/2);
        _startBtnNode->addChild(_startBtnSprite);
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            if (!_touchEnabled) return false;
            if (!_startBtnSprite->isVisible()) return false;
            
            
            auto P = _startBtnNode->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (_startBtnNode->getBoundingBox().containsPoint(pos)) {
                this->setStartButtonPressed(true);
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this](Touch* T, Event* E) {
            if (!_touchEnabled) return;
            
            auto P = _startBtnNode->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            this->setStartButtonPressed(_startBtnNode->getBoundingBox().containsPoint(pos));
        };
        
        listener->onTouchCancelled = [this](Touch* T, Event* E) {
            this->setStartButtonPressed(false);
        };
        
        
        
        listener->onTouchEnded = [this](Touch* T, Event* E) {
            this->setStartButtonPressed(false);
            
            if (!_touchEnabled) return;
            
            auto P = _startBtnNode->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (!_startBtnNode->getBoundingBox().containsPoint(pos)) return;
            
             _bird->runTouchAnimation();
            SoundEffect::buttonEffect().play();
            this->dayChosen(_currentDay);
           
            
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _startBtnNode);

        
        
        
        
        _startBtnGlow = Sprite::create(folder+"daily_button_effect_glow.png");
        _startBtnGlow->setPosition(startBtnLabelPos);
        _startBtnNode->addChild(_startBtnGlow);
        _startBtnGlow->runAction(RepeatForever::create(Sequence::create(
                                                                       EaseOut::create(ScaleTo::create(0.8, 1.2), 2.0),
                                                                       EaseIn::create(ScaleTo::create(0.8, 0.7), 2.0), NULL)));
        
        
        
        
        _bird = Bird::create(_cur->category, _cur->categoryLevel, _cur->levelID);
        _bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        _bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(_cur->levelID));
        _bird->onTouchBegan = [this](){
            if (!_touchEnabled) return;
            if (!_startBtnSprite->isVisible()) return;
            
            _bird->runTouchAnimation();
            SoundEffect::buttonEffect().play();
            this->dayChosen(_currentDay);
        };
        
        
        _bird->setPosition(Vec2(startBtnPos.x, startBtnPos.y + startBtnSize.height));
        _stage->addChild(_bird);
        
    }
    
    
    _choiceTab = Node::create();
    _choiceTab->setContentSize(Size(winSize.width, 385));
    tabPos = Vec2(0, -250);
    _choiceTab->setPosition(tabPos);
    addChild(_choiceTab);
    
  
    {
        _coinTab = CoinTab::create();
        _coinTab->setPosition(winSize - Size(50, 50));
        addChild(_coinTab);
    }

    if (UserManager::getInstance()->isDebugMode()) {
        
        {
            auto clear = ui::Button::create();
            clear->setTitleText("Clear All but Last");
            clear->setTitleFontName(defaultFont);
            clear->setTitleFontSize(50);
            clear->setPosition(Vec2(winSize.width-300, 400));
            clear->addClickEventListener([this, levelID](Ref*) {
                for (int i=0; i<_cur->numDays-1; i++) {
                    UserManager::getInstance()->setDayCleared(levelID, i+1);
                }
                
                StrictLogManager::shared()->dayChoice_End_Quit(_levelID);
                this->dismiss();
            });
            addChild(clear);
        }
        
        {
            auto clearLast = ui::Button::create();
            clearLast->setTitleText("ClearLast");
            clearLast->setTitleFontName(defaultFont);
            clearLast->setTitleFontSize(50);
            clearLast->setPosition(Vec2(winSize.width-300, 200));
            clearLast->addClickEventListener([this, levelID](Ref*) {
                _currentDay = _cur->numDays;
                UserManager::getInstance()->setDayCleared(levelID, _cur->numDays);
                refreshScene();
            });
            addChild(clearLast);
        }

    }
    
    GameSoundManager::getInstance()->playBGM("Common/Music/BGM2_DailySelection.m4a");
    
    return true;
}


void DailyScene::refreshData()
{
    if (_doneClearedDay) return;
    
    std::map<std::string, int> lastAvailable;
    for (auto game : _freechoiceGames) {
        lastAvailable[game.first] = game.second.maxAvailable;
    }
    
    _freechoiceGames.clear();
    
    _currentDay = UserManager::getInstance()->getCurrentDay(_levelID);

    if (_currentDay<=0) _currentDay = 1;
    _maxAvailableDay = _currentDay;
    
    
    int newDay = _currentDay;
    int minAvailableDay = 0;
    
    for (int i=0; i<_cur->numDays; i++) {
        
        auto day = i+1;
        
        
        bool cleared = UserManager::getInstance()->isDayCleared(_levelID, day);
        
        if (newDay==day && cleared) newDay++;
        if (minAvailableDay==0 && !cleared) minAvailableDay = day;
        
        auto dayCur = _cur->getDayCurriculum(day);
        
        if (cleared && day<_cur->numDays && day>=_maxAvailableDay) {
            _maxAvailableDay = day;
        }
        
        
        
        for (auto game : dayCur->games) {
            if (game.gameLevel<=0) continue;
            if (game.gameName == "EggQuizLiteracy") continue;
            if (game.gameName == "EggQuizMath") continue;
            if (game.gameName == "Comprehension") continue;
            
            bool isNew = false;
            if (cleared) {
                auto last = lastAvailable.find(game.gameName);
                if (last!=lastAvailable.end()) {
                    if (last->second < game.gameLevel) {
                        isNew = true;
                    }
                }
            }
            
            auto it = _freechoiceGames.find(game.gameName);
            
            if (it!=_freechoiceGames.end()) {
                FreeChoiceInfo &v = it->second;
                if (game.gameLevel>v.maxShow) {
                    v.maxShow = game.gameLevel;
                }
                
                if (cleared && game.gameLevel>v.maxAvailable) {
                    v.maxAvailable = game.gameLevel;
                }
                if (isNew) v.isNew = isNew;
                
            } else {
                FreeChoiceInfo v;
                v.maxAvailable = cleared ? game.gameLevel : 0;
                v.maxShow = game.gameLevel;
                v.isNew = isNew;
                
                _freechoiceGames[game.gameName] = v;
            }
            

        }

    }
    
    
    if (newDay!=_currentDay) {
        if (newDay>_cur->numDays && minAvailableDay>0) newDay = minAvailableDay;
        
        UserManager::getInstance()->setCurrentDay(_levelID, newDay);
        _currentDay = newDay;
    }
    
    _maxAvailableDay = MAX(_currentDay, _maxAvailableDay);
    if (UserManager::getInstance()->checkIfNextDayIsAvailable(_levelID, _currentDay)) {
        _maxAvailableDay = MAX(_currentDay+1, _maxAvailableDay);
    }
    _maxAvailableDay = MIN(_maxAvailableDay, _cur->numDays);
    
}


bool DailyScene::Mango::init()
{
    if (!Node::init()) return false;
    
    _currentStatus = 0;
    
    return true;
}

void DailyScene::Mango::setupShape(int day, bool crown)
{
    _isCrown = crown;
    
    if (crown) {
        _body = Sprite::create(folder+"daily_day_crown_done.png");
    } else {
        _body = Sprite::create(folder+"daily_day_mango_done.png");
    }
    
    auto size = _body->getContentSize();
    this->setContentSize(size);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _body->setPosition(size/2);
    
    
    _backGlow = Sprite::create(folder+"daily_mango_active_glow.png");
    _backGlow->setVisible(false);
    _backGlow->setPosition(size/2);
    this->addChild(_backGlow);
    
    this->addChild(_body);
    
    _labelGlow = Sprite::create(folder+"daily_mango_active_number_glow.png");
    _labelGlow->setVisible(false);
    this->addChild(_labelGlow);
    
    
    _label = TodoUtil::createLabel(TodoUtil::itos(day), 65, Size::ZERO, defaultFont, Color4B(49, 16, 0, 255));
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_label);
    
    _label->setPosition(size.width/2, size.height/2);
    

    if (crown) {
        _label->setPosition(size.width/2, size.height/2 - 30);
    } else {
        _label->setPosition(size.width/2, size.height/2);
    }
    _labelGlow->setPosition(_label->getPosition());
    
    
    
    _cover = Sprite::createWithTexture(_body->getTexture());
    _cover->setColor(Color3B::BLACK);
    _cover->setOpacity(64);
    _cover->setVisible(false);
    _cover->setAnchorPoint(Vec2::ZERO);
    this->addChild(_cover);

}

void DailyScene::Mango::setTouched(bool touched)
{
    _cover->setVisible(!touched);
}


void DailyScene::Mango::setStatus(int status, bool animate)
{
    string btnFile;
    Color4B btnColor;
    

    if (status==_currentStatus) return;
    
    if (status==0) {
        btnFile = _isCrown ? "daily_day_crown_done.png" : "daily_day_mango_done.png";
        btnColor = Color4B(49, 16, 0, 255);
    } else if (status==1) {
        btnFile = _isCrown ? "daily_day_crown_active.png" : "daily_day_mango_active.png";
        btnColor = Color4B::WHITE;
    } else {
        btnFile = _isCrown ? "daily_day_crown_unavailable.png" : "daily_day_mango_unavilable.png";
        btnColor = Color4B(6, 71, 0, 255);
    }
    
    if (animate) {
        _body->setTexture(folder+btnFile);
        _label->setTextColor(btnColor);
    } else {
        _body->setTexture(folder+btnFile);
        _label->setTextColor(btnColor);
    }
    
    if (status==1) {
        _backGlow->setVisible(true);
        _backGlow->runAction(RepeatForever::create(Sequence::create(
                                                                     EaseOut::create(ScaleTo::create(0.8, 1.2), 2.0),
                                                                     EaseIn::create(ScaleTo::create(0.8, 0.7), 2.0), NULL)));
        _labelGlow->setVisible(true);
        _labelGlow->runAction(RepeatForever::create(Sequence::create(
                                                                  EaseOut::create(ScaleTo::create(0.8, 1.1), 2.0),
                                                                  EaseIn::create(ScaleTo::create(0.8, 0.9), 2.0), NULL)));
    } else {
        _labelGlow->setVisible(false);
        _labelGlow->stopAllActions();
        
        _backGlow->setVisible(false);
        _backGlow->stopAllActions();
        
    }
    
 
    _currentStatus = status;
    
}


void DailyScene::setupMangoBoard()
{
    //auto cur = CurriculumManager::getInstance()->findCurriculum(_levelID);
    
    _mangoBoard->removeAllChildren();
    
    Size btnSize = Size(154, 164);
    Size btnMargin = Size(52, 66);
    int numBtnX = 6;
    
    
    float leftX = 0;
    //(boardSize.width - (numBtnX*btnSize.width + (numBtnX-1)*btnMargin.width))/2;
    float topY = boardSize.height; // 4*btnSize.height+3*btnMargin.height;
    
    
    
    int x = leftX;
    int y = topY;
    
    

    for (int i=0; i<_cur->numDays; i++) {
        
        auto day = i+1;
        
        
        
        bool cleared = UserManager::getInstance()->isDayCleared(_levelID, day);
        

        
        auto dayCur = _cur->getDayCurriculum(day);
        bool crown = (day == _cur->numDays); //(dayCur->isEggQuiz);
        
        
        Mango *mango = Mango::create();
        mango->setupShape(day, crown);
        
        if (day==_currentDay) {
            mango->setStatus(1);
        } else if (cleared) {
            mango->setStatus(0);
        } else {
            mango->setStatus(2);
        }
        
        
        mango->setPosition(x+btnSize.width/2, y-btnSize.height/2);
        if (crown) {
            mango->setPosition(x+btnSize.width/2+28, y-btnSize.height/2+6);
        }

        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, mango, day](Touch* T, Event* E) {
            if (!_touchEnabled) return false;
            if (day > _maxAvailableDay) return false;
            
            
            auto P = mango->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (mango->getBoundingBox().containsPoint(pos)) {
                mango->setTouched(true);
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this, mango](Touch* T, Event* E) {
            if (!_touchEnabled) return;
            
            auto P = mango->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            mango->setTouched(mango->getBoundingBox().containsPoint(pos));
        };
        
        listener->onTouchCancelled = [mango](Touch* T, Event* E) {
            mango->setTouched(false);
        };
        
        
        
        listener->onTouchEnded = [this, mango, day](Touch* T, Event* E) {
            mango->setTouched(false);
            
            if (!_touchEnabled) return;
            
            auto P = mango->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (!mango->getBoundingBox().containsPoint(pos)) return;
            
            if (day <= _maxAvailableDay) {
                SoundEffect::buttonEffect().play();
                this->dayChosen(day);
            }
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, mango);
        
        
        
        if (i%numBtnX == (numBtnX-1)) {
            x = leftX;
            y -= (btnSize.height+btnMargin.height);
        } else {
            x+=(btnSize.width+btnMargin.width);
        }
        
        
        _mangoBoard->addChild(mango, 0, day);
        
    }

}

void DailyScene::setupFreechoiceTab()
{

    
    _choiceUp = false;
    
    _choiceTab->removeAllChildren();
    
    
    auto cs = _choiceTab->getContentSize();
    auto t = ui::Scale9Sprite::create(folder+"daily_bottom_tap_bg.png");
    t->setPreferredSize(cs);
    t->setPosition(cs/2);
    _choiceTab->addChild(t);
    
    _choiceTabArrow = Sprite::create(folder+"daily_bottom_tap_arrow_up.png");
    _choiceTabArrow->setPosition(Vec2(cs.width/2, cs.height - 55));
    _choiceTab->addChild(_choiceTabArrow);
    
    
    Rect box = Rect(0, 250, cs.width, 385-250);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, t, box](Touch* T, Event* E) {
        if (!_touchEnabled) return false;
        
        auto P = t->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (box.containsPoint(pos))
        {
            if (_choiceUp) {
                StrictLogManager::shared()->dayChoice_CloseFreeChoiceMenu();
            }
            else {
                StrictLogManager::shared()->dayChoice_OpenFreeChoiceMenu();
            }

            this->showChoiceTab(!_choiceUp);
            return true;
        }
        return false;
    };
    
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, t);
    
    {
        
        
        
        auto btnSize = Size(203, 199);
        
        float x = 90+btnSize.width/2;
        float xStep = btnSize.width+36;
        
        for (auto it : _freechoiceGames) {
            
            auto gameBtn = Node::create();
            
            gameBtn->setContentSize(btnSize);
            gameBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            gameBtn->setPosition(Vec2(x, 134));
            x+=xStep;
            
            _choiceTab->addChild(gameBtn);
            
            
            std::string iconFilename = StringUtils::format("icons/game_icon_%s.png",it.first.c_str());
            Node *ic = Sprite::create(iconFilename);
            if (ic==nullptr) {
                ic = Node::create();
                ic->setContentSize(Size(190, 190));
            }
            ic->setScale(190.0 / 250.0);
            ic->setPosition(btnSize/2);
            gameBtn->addChild(ic);
            
            if (it.second.isNew) {
                auto s = Sprite::create(folder+"daily_freechoice_circleicon_new.png");
                s->setPosition(btnSize/2);
                gameBtn->addChild(s);
                
                auto effect1 = Sprite::create(folder+"daily_freechoice_circleicon_new_effect_1.png");
                effect1->setPosition(btnSize/2);
                gameBtn->addChild(effect1);
                effect1->runAction(RepeatForever::create(RotateBy::create(0.3, 30)));
                
                auto effect2 = Sprite::create(folder+"daily_freechoice_circleicon_new_effect_2.png");
                effect2->setPosition(btnSize/2);
                gameBtn->addChild(effect2, ic->getLocalZOrder()-10);
                effect2->runAction(RepeatForever::create(Sequence::create(
                                                                               EaseOut::create(ScaleTo::create(0.8, 1.2), 2.0),
                                                                               EaseIn::create(ScaleTo::create(0.8, 0.7), 2.0), NULL)));
                
            } else {
                auto s = Sprite::create(folder+"daily_freechoice_circleicon_available.png");
                s->setPosition(btnSize/2);
                gameBtn->addChild(s);
            }
            
            if (it.second.maxAvailable>0) {
                
                
                auto *listener = EventListenerTouchOneByOne::create();
                listener->setSwallowTouches(true);
                listener->onTouchBegan = [this, gameBtn, it](Touch* T, Event* E) {
                    if (!_touchEnabled) return false;
                    
                    auto P = gameBtn->getParent();
                    auto pos = P->convertToNodeSpace(T->getLocation());
                    if (gameBtn->getBoundingBox().containsPoint(pos)) {
                        
                        StrictLogManager::shared()->dayChoice_OpenFreeChoiceLevelPopup(it.first, it.second.maxAvailable, it.second.maxShow);
                        showFreechoicePopup(it.first, it.second.maxAvailable, it.second.maxShow);
                        
                        return true;
                    }
                    return false;
                };
                
                
                
                this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, gameBtn);
            } else {
                auto cover = Sprite::create(folder+"daily_freechoice_circleicon_unavailable.png");
                cover->setPosition(btnSize/2);
                gameBtn->addChild(cover);
            }
            
            
        }
    }

}

void DailyScene::onEnter()
{
    Node::onEnter();
    
    auto panelSize = _panel->getContentSize();
    Vec2 panelPos = Vec2(viewSize.width/2, viewSize.height-panelSize.height);
    
    _panel->setPosition(Vec2(viewSize.width/2, viewSize.height));
    
    _panel->runAction(Sequence::create(DelayTime::create(0.3), EaseOut::create(MoveTo::create(0.5, panelPos), 3.0), nullptr));
    
   
    refreshScene();
    
    
    auto coin = UserManager::getInstance()->getStars();
    auto showingCoin = CoinTab::_numCoin;
    if (coin!=showingCoin) {
        _coinTab->addCoin(coin-showingCoin, Vec2(viewSize.width/2, 250));
        
    }
    

}

void DailyScene::refreshScene()
{
    if (_doneClearedDay) {
        
        setupMangoBoard();
        setupFreechoiceTab();
        
        if (_currentDay>1) {
            showChoiceTab(true);
        }
        _doneClearedDay = false;
        
        return;
    }
    
    auto oldCurrentDay = _currentDay;
    
    refreshData();

    
    if (oldCurrentDay>0 && oldCurrentDay!=_currentDay) {
        setupFreechoiceTab();
        showClear(oldCurrentDay);
    } else {
        
        
        if (_currentDay <= _cur->numDays) {
            _startBtnSprite->setVisible(true);
            setStartButtonText(_currentDay);
        } else {
            _startBtnSprite->setVisible(false);
            _startBtnGlow->setVisible(false);
        }
        
        
        setupMangoBoard();
        setupFreechoiceTab();
        
        if (_currentDay>1) {
            showChoiceTab(true);
        }
        
    }
    
}

void DailyScene::onExit()
{
    Node::onExit();
    _touchEnabled = true;
    
}


void DailyScene::lightFall(LightBall *l, bool first)
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


void DailyScene::showFreechoicePopup(std::string gameName, int maxPlayable, int maxAvailable)
{
    
    Size popupSize = Size(1540, 1404);
    auto winSize = Director::getInstance()->getWinSize();
    
    auto popup = PopupBase::create(this, winSize);
    popup->showFromTop = true;
    popup->setAllowDismissByTouchingOutside(true);
    
    
    {
        
        auto bg = Sprite::create(folder+"daily_freechoice_popup_window.png");
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        bg->setPosition(Vec2(winSize.width/2, winSize.height));
        popup->addChild(bg);
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [bg](Touch* T, Event* E) {
            auto P = bg->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (bg->getBoundingBox().containsPoint(pos)) {
                return true;
            }
            return false;
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);
        
    }
    
    

    {
        auto panel = Sprite::create(folder+"daily_freechoice_popup_panel.png");
        panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        panel->setPosition(Vec2(winSize.width/2, winSize.height-120));
        popup->addChild(panel);
        
//        auto l = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString(gameName), 70, Size::ZERO, defaultFont, Color4B(255, 210, 74, 255));
        auto l = TodoUtil::createLabelMultilineToFit(LanguageManager::getInstance()->getLocalizedString(gameName), 70, Size(900,0), defaultFont, Color4B(255, 210, 74, 255));
        l->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        l->setPosition(panel->getContentSize()/2);
        panel->addChild(l);
    }
    
    //string s1 = LanguageManager::getInstance()->getLocalizedString("Are you ready for") + " " + cur->levelTitle + "?";
    
    {
        auto filename = "MainScene/FreeChoiceThumbnail/freechoice_game_"+gameName+".png";
        Node *thumb = Sprite::create(filename);
        if (thumb==nullptr) thumb = TodoUtil::createLabel(gameName, 120, Size(800, 500), defaultFont, Color4B::BLACK);
        
        thumb->setPosition(Vec2(winSize.width/2, winSize.height-600));
        popup->addChild(thumb);
    }
    
    {
        auto cancelBtn = ui::Button::create();
        cancelBtn->loadTextures(folder+"daily_freechoice_close_normal.png", folder+"daily_freechoice_close_active.png");
        cancelBtn->setZoomScale(0);
        
        cancelBtn->addClickEventListener([popup](Ref*) {
            StrictLogManager::shared()->dayChoice_CloseFreeChoiceLevelPopup();
            popup->dismiss(true);
        });
        cancelBtn->setPosition(Vec2(1985, 1600));
        popup->addChild(cancelBtn);
    }
    
    {
        int numLevel = maxAvailable;
        
        const float stepX = 140;
        int numX = MIN(10, numLevel);
        int numY = (numLevel+9)/10;
        float leftX = (winSize.width - stepX*(numX-1))/2.0;
        float topY = 630 + stepX*(numY-1)/2.0;
        
        float x = leftX;
        float y = topY;
        
        for (int i=0; i<numLevel; i++) {
            
            auto gameLevel = i+1;
            
            
            auto b = ui::Button::create();
            b->loadTextures(folder+"daily_freechoice_popup_level_normal.png",
                            folder+"daily_freechoice_popup_level_done.png",
                            folder+"daily_freechoice_popup_level_unavailable.png");
            
            b->setEnabled(gameLevel<=maxPlayable);
            b->setPosition(Vec2(x, y));
            popup->addChild(b);
            if (i%10==9) {
                x = leftX;
                y -= stepX;
            } else {
                x += stepX;
            }
            
            
            auto l = TodoUtil::createLabel(TodoUtil::itos(i+1), 50, Size::ZERO, defaultFont, Color4B::WHITE);
            l->setPosition(b->getContentSize()/2 + Size(2, -6));
            b->addChild(l);
            
            b->addClickEventListener([gameName, gameLevel, popup](Ref*) {
                StrictLogManager::shared()->dayChoice_ChooseFreeChoiceGame(gameName, gameLevel);
                CCAppController::sharedAppController()->startFreeChoiceGame(gameName, gameLevel);
                popup->dismiss(true);
            });
//            
//            auto *listener = EventListenerTouchOneByOne::create();
//            listener->setSwallowTouches(true);
//            listener->onTouchBegan = [b, gameName, gameLevel](Touch* T, Event* E) {
//                auto P = b->getParent();
//                auto pos = P->convertToNodeSpace(T->getLocation());
//                if (b->getBoundingBox().containsPoint(pos)) {
//                    CCAppController::sharedAppController()->startGame(gameName, gameLevel);
//                    
//                    return true;
//                }
//                return false;
//            };
//            
//            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, b);
            
            
            
            
            
            
            
        }
    }
    
    
    
    popup->show(this, true);


}
//
//void DailyScene::show()
//{
//    _parentView->addChild(this);
//    
//    this->setVisible(false);
//    
//    
//    
//    auto t = 0.3;
//    auto winSize = getContentSize();
//    auto cover = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
//    
//    _parentView->addChild(cover);
//    cover->runAction(Sequence::create(FadeIn::create(t), CallFunc::create([this](){
//        this->setVisible(true);
//    }), FadeOut::create(t), CallFunc::create([cover](){
//        cover->removeFromParent();
//    }), nullptr));
//                     
//    
//    
//    
//    
//    auto panelSize = _panel->getContentSize();
//    Vec2 panelPos = Vec2(viewSize.width/2, viewSize.height-panelSize.height);
//    
//    _panel->setPosition(Vec2(viewSize.width/2, viewSize.height));
//    
//    _panel->runAction(Sequence::create(DelayTime::create(t), EaseOut::create(MoveTo::create(0.5, panelPos), 3.0), nullptr));
//    
//    LogManager::getInstance()->logEvent("DailyScene", "show", _levelID, 0);
//    
//}
//

void DailyScene::setStartButtonPressed(bool pressed)
{
    const auto btnNormal = folder+"daily_button_normal.png";
    const auto btnPressed = folder+"daily_button_active.png";
    

    _startBtnSprite->setTexture(pressed ? btnPressed : btnNormal);
    
}

void DailyScene::setStartButtonText(int day)
{
    auto lang = LanguageManager::getInstance()->getCurrentLanguageCode();
    
    std::string btnText = TodoUtil::itos(day);
    
    if (lang=="en") {
        btnText = "Day " + btnText;
    } else { // sw
        if (day==1) btnText = "Siku ya kwanza";
        else if (day==2) btnText = "Siku ya pili";
        else if (day==3) btnText = "Siku ya tatu";
        else btnText = "Siku ya " + btnText;
    }
    
    if (_startBtnLabel) _startBtnLabel->removeFromParent();
    _startBtnLabel = TodoUtil::createLabelMultilineToFit(btnText, 110, startBtnLabelSize, defaultFont, Color4B::WHITE, TextHAlignment::CENTER);
    _startBtnLabel->setPosition(startBtnLabelPos);
    _startBtnNode->addChild(_startBtnLabel);
    
    
}

void DailyScene::showChoiceTab(bool show)
{
    if (_choiceUp==show) return;
    
    auto arrow = show ? "daily_bottom_tap_arrow_down.png" : "daily_bottom_tap_arrow_up.png";
    _choiceTabArrow->setTexture(folder+arrow);
    
    _choiceUp = show;
    auto pos = show ? Vec2(tabPos.x, 0) : tabPos;
    _choiceTab->runAction(EaseOut::create(MoveTo::create(0.3, pos), 2.0));

}


void DailyScene::showClear(int day)
{

    _touchEnabled = false;
    
    

    
    auto cur = CurriculumManager::getInstance()->findCurriculum(_levelID);
    
    bool lastday = day==cur->numDays;

    
    Mango *mango = (Mango*)_mangoBoard->getChildByTag(day);
    int nextDay = _currentDay;
    if (nextDay>cur->numDays || nextDay<=0) nextDay = -1;
    
    
    Sprite *nextBtn = nextDay>=0 ? (Sprite*)_mangoBoard->getChildByTag(nextDay) : nullptr;
    Sequence *nextSeq;
    
    if (nextBtn && nextDay>0) {

        nextSeq = Sequence::create(CallFunc::create([this, nextDay](){
            auto nextMango = (Mango*)_mangoBoard->getChildByTag(nextDay);
            
            auto seqForNextBtn = Sequence::create(
                                                  DelayTime::create(0.1),
                                                  ScaleTo::create(0.3, 1.2),
                                                  CallFunc::create([this, nextDay, nextMango](){
                
                nextMango->setStatus(1);
                
                
                _startBtnNode->setVisible(true);
                this->setStartButtonText(nextDay);
                
//                auto btnSpawn = Spawn::create(EaseOut::create(ScaleTo::create(0.5, 1.0), 2.0),
//                                              EaseOut::create(MoveTo::create(0.5, startBtnPos), 2.0),
//                                              FadeTo::create(0.5, 255),
//                                              nullptr);
                _startBtnNode->runAction(Sequence::create(EaseOut::create(ScaleTo::create(0.5, 1.0, 1.0), 2.0),
                                                      DelayTime::create(0.1),
                                                      CallFunc::create([this, nextDay](){
                    _touchEnabled = true;
                    //this->dayChosen(nextDay);
                }),
                                                      ScaleTo::create(0.3, 1.0),
                                                      nullptr));
                
                
                //this->dismiss();
            }),nullptr);

                                                  
            nextMango->runAction(seqForNextBtn);
        }), nullptr);
        
                                   

    } else {
        nextSeq = Sequence::create(DelayTime::create(0.8),
                                   CallFunc::create([this](){
            StrictLogManager::shared()->dayChoice_End_Complete(_levelID);
            _touchEnabled = true;
            this->dismiss();
        }),
                                   nullptr);
     

    }


    mango->setStatus(1);

    

    auto seq = Sequence::create(DelayTime::create(0.3),
                                ScaleTo::create(0.6, 0.8),
                                ScaleTo::create(0.3, 1.2),
                                
                                CallFunc::create([mango, this]() {
        mango->setStatus(0);
        SoundEffect::dayClearEffect().play();
        
        _bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(_cur->levelID));
        _bird->loadAnimation();
        
        showChoiceTab(true);
        
    }),
                                ScaleTo::create(0.2, 1.0),
                                DelayTime::create(0.1),
                                nextSeq,
                                nullptr);
    
    
    mango->runAction(seq);
    
    

    _startBtnNode->runAction(Sequence::create(
                                          DelayTime::create(0.3),
                                              EaseOut::create(ScaleTo::create(0.5, 0.0, 1.0), 2.0),
                                          CallFunc::create([this](){ _startBtnNode->setVisible(false); }),
                                          nullptr));

    _bird->runAction(Sequence::create(
                                      EaseOut::create(MoveBy::create(1.2, Vec2(0, 150)), 2.0),
                                      CallFunc::create([this](){ _bird->runTouchAnimation(); }),
                                      EaseIn::create(MoveBy::create(1.2, Vec2(0, -150)), 2.0),
                                      nullptr));
    

}

void DailyScene::dayChosen(int day)
{
    if (!_touchEnabled) return;
    _touchEnabled = false;
    
    
    {

        UserManager::getInstance()->setCurrentDay(_levelID, day);
        
        
        _doneClearedDay = UserManager::getInstance()->isDayCleared(_levelID, day);
        if (!_doneClearedDay) {
            if (_currentDay!=day) {
                
                {
                    Mango *m1 = (Mango*)_mangoBoard->getChildByTag(_currentDay);
                    if (m1) m1->setStatus(2);
                }
                
                {
                    Mango *m2 = (Mango*)_mangoBoard->getChildByTag(day);
                    if (m2) m2->setStatus(1);
                }
                
                
                _currentDay = day;
            }
        }
        

        auto scene = GameSelectScene::createScene();
        scene->setName("GameSelectScene");
        
        auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
        fadeScene->setName("(TransitionFade GameSelectScene)");
        Director::getInstance()->pushScene(fadeScene);
    }
    
    LogManager::getInstance()->logEvent("DailyScene", "select_day", _levelID, day);

}

void DailyScene::dismiss()
{

    ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
//    
//    auto t = 0.3;
//    auto winSize = getContentSize();
//    auto cover = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
//    
//    _parentView->addChild(cover);
//    cover->runAction(Sequence::create(FadeIn::create(t), CallFunc::create([this](){
//        if (onDismiss) this->onDismiss();
//        removeFromParent();
//    }), FadeOut::create(t), CallFunc::create([cover](){
//        cover->removeFromParent();
//    }), nullptr));
}

