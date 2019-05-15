//
//  DailyScene2.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 03/26/18.
//
//

#include "CoopScene.hpp"
#include "DailyScene2.hpp"
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

namespace DailyScene2Space {
    
    const int LEVEL_SPECIAL_COURSE = CoopScene::LEVEL_SPECIAL_COURSE;
    const int ACTION_ID_SPECIAL_COURSE = 100;
    
    const Size viewSize = Size(2560, 1800);
    
    const string defaultFont = "fonts/TodoMainCurly.ttf";
    const string folder = "MainScene/DailyScene/";
    
    const Color4B whiteColor = Color4B(255, 252, 236, 255);
    //const Color4B darkColor = Color4B(120, 16, 0, 255);
    const Size boardSize = Size(1200, 1200);
    
//    const Size startBtnSize = Size(846, 582);
//    const Size startBtnLabelSize = Size(700, 250);
//    const Vec2 startBtnLabelPos = Vec2(423, 385);
    
//    const int dayLabelTag = 200;
    
    Vec2 tabPos;
  
    
    
//    const float dayLabelY = 62;
//    const float dayLabelYComp = 56;
    
    
    
    
}

using namespace DailyScene2Space;
using namespace std;

Scene* DailyScene2::createScene(string levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = DailyScene2::create(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



DailyScene2* DailyScene2::create(string levelID)
{
    DailyScene2 *popup = new (std::nothrow) DailyScene2();
    if (popup && popup->init(levelID))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool DailyScene2::init(string levelID)
{

    
    if (!Node::init()) {
        return false;
    }
    
    GameSoundManager::getInstance()->stopBGM();

    UserManager::getInstance()->setPlayingDay(0);
    StrictLogManager::shared()->dayChoice_Begin(levelID);
    _levelID = levelID;
    _isSpecialCourse = TodoUtil::endsWith(_levelID, TodoUtil::itos(LEVEL_SPECIAL_COURSE));
    
    _currentDay = 0;
    _doneClearedDay = false;
    
    _touchEnabled = true;
    
    _cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    _freechoiceGames.clear();
    
    if(_isSpecialCourse) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(folder + "badge/special_course/sc_badge_small-money-bag.plist", folder + "badge/special_course/sc_badge_small-money-bag.png");
        
        if (_cur->category == 'L') {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(folder + "badge/special_course/sc_badge_large-money-bag.plist", folder + "badge/special_course/sc_badge_large-money-bag.png");

        } else {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(folder + "badge/special_course/sc_badge_large-money-bag_math.plist", folder + "badge/special_course/sc_badge_large-money-bag_math.png");

        }

        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(folder + "badge/special_course/sc_badge_large-money-bag_effect.plist", folder + "badge/special_course/sc_badge_large-money-bag_effect.png");
    }

    auto winSize = Director::getInstance()->getWinSize();
    setContentSize(winSize);
    
    _choiceTabJumpEnabled = false;
    _choiceUp = false;
    
    
//    _backView = Node::create();
//    _backView->setContentSize(winSize);
//    auto blocker = EventListenerTouchOneByOne::create();
//    blocker->setSwallowTouches(true);
//    blocker->onTouchBegan = [](Touch* T, Event* E) {
//        return true;
//    };
//    _backView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(blocker, _backView);
//    addChild(_backView);
    
    _bgView = Sprite::create(folder+"daily_bg_large.png");
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
    

    {
        const Size stageSize = Size(960, winSize.height);
        
        auto stage = Node::create();
        
        stage->setContentSize(stageSize);
        stage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        stage->setPosition(Vec2(viewSize.width/2, viewSize.height));
        _mainView->addChild(stage);
        
        
        
        
        for (int i=0; i<3; i++) {
            
            auto l = LightBall::create();
            l->setup(1);
            stage->addChild(l);
            lightFall(l, true);
            
        }
        for (int i=0; i<2; i++) {
            
            auto l = LightBall::create();
            l->setup(2);
            stage->addChild(l);
            lightFall(l, true);
            
        }
        
        auto beam = Sprite::create(folder+"daily_beam_large.png");
        beam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        beam->setPosition(Vec2(stageSize.width/2, stageSize.height));
        stage->addChild(beam);
        
        
        auto treetop = Sprite::create(folder+"daily_treetop_right.png");
        treetop->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        treetop->setPosition(winSize);
        this->addChild(treetop);
        
        
        
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
    _mangoBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _mangoBoard->setPosition(Vec2(_mainView->getContentSize().width/2, _mainView->getContentSize().height-254-boardSize.height));
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


    
    
    
    
    _choiceTab = Node::create();
    _choiceTab->setContentSize(Size(winSize.width, 1789));
    tabPos = Vec2(0, -1789+135 );
    _choiceTab->setPosition(tabPos);
    addChild(_choiceTab);
    
    if (_isSpecialCourse) {
        _choiceTab->setVisible(false);
    }
    
  
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


void DailyScene2::refreshData()
{
    if (_doneClearedDay) return;
    
    std::map<std::string, int> lastAvailable;
    for (auto game : _freechoiceGames) {
        lastAvailable[game.first] = game.second.maxAvailable;
    }
    
    _freechoiceGames.clear();
    
    _currentDay = UserManager::getInstance()->getCurrentDay(_levelID);

    if (_currentDay<=0) _currentDay = 1;
    if (_currentDay>_cur->numDays) _currentDay = _cur->numDays;
    
    for (int i=0; i<_cur->numDays; i++) {
        
        auto day = i+1;
        
        auto dayCur = _cur->getDayCurriculum(day);
        bool cleared = UserManager::getInstance()->isDayCleared(_levelID, day);
        
        if ( (day == _currentDay) && cleared) {
            if (_currentDay < _cur->numDays) {
                _currentDay = day + 1;
            }
        }
        
        for (auto game : dayCur->games) {
            if (game.gameLevel<=0) continue;
            if (game.gameName == "EggQuizLiteracy") continue;
            if (game.gameName == "EggQuizMath") continue;
            if (game.gameName == "Comprehension") continue;
            if (game.gameName == "EggQuizMath") continue;
            
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
                if (isNew) _choiceTabJumpEnabled = true;
                _freechoiceGames[game.gameName] = v;
            }
            

        }

    }
    
    
    
    if (UserManager::getInstance()->getCurrentDay(_levelID)!=_currentDay) {
        UserManager::getInstance()->setCurrentDay(_levelID, _currentDay);
    }
    
    
    _maxAvailableDay = _currentDay;
    
    if (!_isSpecialCourse) {
        if (UserManager::getInstance()->checkIfNextDayIsAvailable(_levelID, _currentDay)) {
            _maxAvailableDay = _currentDay+1;
        }
    }
}


bool DailyScene2::Mango::init()
{
    if (!Node::init()) return false;
    
    
    return true;
}

void DailyScene2::Mango::setupShape(int birdID, string levelID, int day, bool crown, bool quiz)
{
    _birdID = birdID;
    _levelID = levelID;
    _day = day;
    _isCrown = crown;
    _isQuiz = quiz;
    _isSpecialCourse = TodoUtil::endsWith(levelID, TodoUtil::itos(LEVEL_SPECIAL_COURSE));
    if (_isSpecialCourse) {
        loadSpriteFrameForSC(false);
        _body = Sprite::createWithSpriteFrame(_spriteFrameForSC.front());
        
    } else {
        if (crown) {
            _body = Sprite::create(folder+"badge/daily_badge_common_future_crown.png");
        } else {
            _body = Sprite::create(folder+"badge/daily_badge_common_future.png");
        }
    }
    
    auto size = _body->getContentSize();
    this->setContentSize(size);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _body->setPosition(size/2 + Size(0, 20));
    if (_isSpecialCourse) {
        _topGlow = Sprite::createWithSpriteFrame(_spriteFrameTopGlowForGC.front());
        _topGlow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _topGlow->setPosition(Vec2(_body->getContentSize().width / 2, _body->getContentSize().height));
        _body->addChild(_topGlow);
        _topGlow->setVisible(false);
    }
    
    
    if (_isSpecialCourse) {
        _backGlow = Sprite::create(folder+"badge/special_course/sc_badge_large-money-bag_bg-effect.png");
        _backGlow->setPosition(size/2 + Size(0, 20));
        
    } else {
        _backGlow = Sprite::create(folder+"badge/daily_badge_common_current_effect.png");
        _backGlow->setPosition(size/2 + Size(0, 20));
    }
    
    _backGlow->setVisible(false);
    this->addChild(_backGlow);
    
    this->addChild(_body);
    
//    _labelGlow = Sprite::create(folder+"daily_mango_active_number_glow.png");
//    _labelGlow->setVisible(false);
//    this->addChild(_labelGlow);
    
    
    _label = TodoUtil::createLabel(TodoUtil::itos(day), 65, Size::ZERO, defaultFont, Color4B(49, 16, 0, 255));
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_label);
    
    _label->setPosition(size.width/2, size.height/2);
    
    
    
    _cover = Sprite::createWithTexture(_body->getTexture());
    _cover->setColor(Color3B::BLACK);
    _cover->setOpacity(64);
    _cover->setVisible(false);
    _cover->setAnchorPoint(Vec2::ZERO);
    this->addChild(_cover);
    
    _isFirstSetup = true;
}

void DailyScene2::Mango::setTouched(bool touched)
{
    if (_isSpecialCourse) {
        return;
    }
    _cover->setVisible(!touched);
}


void DailyScene2::Mango::refresh(bool animate)
{
    string btnFile;
    Color4B btnColor;
    Vec2 labelOffset = Vec2::ZERO;
    
    Vec2 bodyCenter = _body->getContentSize()/2;
    
    int currentDay = UserManager::getInstance()->getCurrentDay(_levelID);
    if (currentDay<1) currentDay = 1;
    int time = _day - currentDay;
    bool isAvailable = _isAvailable;
    bool isLevelCleared = UserManager::getInstance()->isLevelCleared(_levelID);
    bool isDayCleared = UserManager::getInstance()->isDayCleared(_levelID, _day);
        
    stopActionByTag(ACTION_ID_SPECIAL_COURSE);
    
    btnColor = Color4B(49, 16, 0, 128);
    
    if (time>0) {
        if (isAvailable) {
            labelOffset = Vec2(0, 45);
            if (_isCrown) btnFile = "daily_badge_common_front_crown.png";
            //else (_isQuiz) btnFile = "daily_badge_common_future_withCrown.png";
            else {
                btnFile = "daily_badge_common_front.png";
                labelOffset = Vec2(0, 15);
            }
        } else {
            if (_isSpecialCourse) {
                btnFile = "special_course/sc_badge_future.png";
                
            } else {
                labelOffset = Vec2(0, 15);
                if (_isCrown) btnFile = "daily_badge_common_future_crown.png";
                else if (_isQuiz) btnFile = "daily_badge_common_future_withCrown.png";
                else btnFile = "daily_badge_common_future.png";
            }
        }
    } else {
        const string prefix = "daily_badge";
        string birdInFix = StringUtils::format("_bird%d", _birdID);
        string timeInFix = time<0 ? "_past" : "_current";
        string clearPostFix = isDayCleared ? "-gold" : "-silver";
        string withCrownPostFix = (isLevelCleared  && (time==0)) ? "_withCrown" : "";
        
        if (_isSpecialCourse) {
            loadSpriteFrameForSC(time == 0);
            labelOffset = Vec2(0, 0);
            
        } else {
            btnFile = prefix + birdInFix + timeInFix + clearPostFix + withCrownPostFix + ".png";
            labelOffset = Vec2(0, 60);
        }

        if (time==0) btnColor = Color4B(0, 0, 0, 0);
    }
    
    if (_isSpecialCourse && time <= 0) {
        _body->stopAllActions();
        _body->setSpriteFrame(_spriteFrameForSC.front());

        if (time == 0) {
            _topGlow->setVisible(true);
            
            auto action = Sequence::create(DelayTime::create(3.0f), CallFunc::create([this, animate]() {
                if (random(0, 1)) {
                    auto animation = Animation::createWithSpriteFrames(_spriteFrameForSC, 0.15);
                    animation->setRestoreOriginalFrame(true);
                    auto animate = Animate::create(animation);
                    _body->runAction(animate);
                }
            }), NULL);
            
            auto repeat = RepeatForever::create(action);
            repeat->setTag(ACTION_ID_SPECIAL_COURSE);
            runAction(repeat);
            
            {
                auto animation = Animation::createWithSpriteFrames(_spriteFrameTopGlowForGC, 0.15);
                auto animate = Animate::create(animation);
                _topGlow->runAction(RepeatForever::create(animate));
            }

        } else {
            _topGlow->setVisible(false);
            auto action = Sequence::create(DelayTime::create(random(2.0f, 5.0f)), CallFunc::create([this, animate]() {
                if (random(0, 1)) {
                    auto animation = Animation::createWithSpriteFrames(_spriteFrameForSC, 0.15);
                    animation->setRestoreOriginalFrame(true);
                    auto animate = Animate::create(animation);
                    _body->runAction(animate);
                }
            }), NULL);
            
            auto repeat = RepeatForever::create(action);
            repeat->setTag(ACTION_ID_SPECIAL_COURSE);
            runAction(repeat);
        }
        
        _label->setTextColor(btnColor);
        _label->setPosition(bodyCenter + labelOffset);

    } else {
        if (animate) {
            _body->setTexture(folder+"badge/"+btnFile);
            _label->setTextColor(btnColor);
            _label->setPosition(bodyCenter + labelOffset);
        } else {
            _body->setTexture(folder+"badge/"+btnFile);
            if (!_isSpecialCourse && _isCrown && time > 0) {
                auto size = _body->getContentSize();
                _body->setPosition(size/2 + Size(0, 35));
            }
            _label->setTextColor(btnColor);
            _label->setPosition(bodyCenter + labelOffset);
        }
    }

    if (_isSpecialCourse) {
        _backGlow->setVisible(time==0);
        _backGlow->stopAllActions();

    } else {
        if (isDayCleared && _isQuiz) {
            auto addCrown = Sprite::create(folder+"badge/daily_badge_common_past_crown.png");
            addCrown->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            _body->addChild(addCrown);
        }

        if (time==0) {
            _backGlow->setVisible(true);
            _backGlow->runAction(RepeatForever::create(Sequence::create(
                                                                        EaseOut::create(ScaleTo::create(0.8, 1.2), 2.0),
                                                                        EaseIn::create(ScaleTo::create(0.8, 0.7), 2.0), NULL)));
        } else {
            _backGlow->setVisible(false);
            _backGlow->stopAllActions();

        }
    }
 
    _isFirstSetup = false;
}

void DailyScene2::Mango::loadSpriteFrameForSC(bool isCrown) {
    _spriteFrameForSC.clear();
    _spriteFrameTopGlowForGC.clear();
    char category = 'L';
    
    vector<string> split = TodoUtil::split(_levelID, '_');
    if (split.size() >= 2) {
        category = split[1] == "L" ? 'L' : 'M';
    }
    
    auto stringFormat = category == 'L' ? "sc_badge_large-money-bag_%02d.png" : "sc_badge_large-money-bag_math_%02d.png";
    if (isCrown) {
        for (int i = 1; i < 7; i++) {
            auto framename = StringUtils::format(stringFormat, i);
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
            _spriteFrameForSC.pushBack(frame);
        }

    } else {
        for (int i = 1; i <= 8; i++) {
            auto framename = StringUtils::format("sc_badge_small-money-bag_%02d.png", i);
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
            _spriteFrameForSC.pushBack(frame);
        }
    }
    
    for (int i = 1; i <= 9; i++) {
        auto framename = StringUtils::format("sc_badge_large-money-bag_effect_%02d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
        _spriteFrameTopGlowForGC.pushBack(frame);
    }
}

void DailyScene2::setupMangoBoard()
{
    //auto cur = CurriculumManager::getInstance()->findCurriculum(_levelID);
    
    _mangoBoard->removeAllChildren();
    
    Size btnSize = Size(288, 239);
    Size btnMargin = Size(0, 0);
    int numBtnX = 7;
    
    
    float leftX = (boardSize.width - (numBtnX*btnSize.width + (numBtnX-1)*btnMargin.width))/2;
    float topY = boardSize.height; // 4*btnSize.height+3*btnMargin.height;
    
    
    
    int x = leftX;
    int y = topY;
    
    
    bool isLevelCleared = UserManager::getInstance()->isLevelCleared(_levelID);
    
    
    for (int i=0; i<_cur->numDays; i++) {
        
        auto day = i+1;
        auto dayInfo = _cur->days[i];
        
        bool crown = (day == _cur->numDays); //(dayCur->isEggQuiz);
        auto birdID = Bird::getBirdID(_cur->category, _cur->categoryLevel);
        bool isAvailable = (day <= _maxAvailableDay);
        bool quiz = (dayInfo.games.size() && dayInfo.games[0].gameParameter.find("MiniTest") != string::npos) ? true : false;
        
        Mango *mango = Mango::create();
        mango->setupShape(birdID, _levelID, day, crown, quiz);
        mango->setAvailable(isAvailable);
        mango->refresh();
        
        
        mango->setPosition(x+btnSize.width/2, y-btnSize.height/2);

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
            if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
            
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

void DailyScene2::setupFreechoiceTab()
{
    showChoiceTab(_choiceUp);
    _choiceTab->removeAllChildren();
    
    
    auto cs = _choiceTab->getContentSize();
    auto t = Sprite::create(folder+"freechoice_popup_bg.png");
    t->setPosition(cs/2);
    _choiceTab->addChild(t);
    
    _choiceTabArrow = Sprite::create(folder+(_choiceUp?"freechoice_popup_button_close.png" : "freechoice_popup_button_open.png"));
    _choiceTabArrow->setPosition(Vec2(cs.width/2, cs.height - 55));
    _choiceTab->addChild(_choiceTabArrow);
    
    
    Rect box = Rect(0, 250, cs.width, cs.height-250);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, t, box](Touch* T, Event* E) {
        if (!_touchEnabled) return false;
        
        auto P = t->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (box.containsPoint(pos))
        {
            if (_choiceUp) {
                GameSoundManager::getInstance()->playEffectSound("FishTank/Sounds/HB_turn off.m4a");
                StrictLogManager::shared()->dayChoice_CloseFreeChoiceMenu();
            }
            else {
                GameSoundManager::getInstance()->playEffectSound("FishTank/Sounds/HB_turn on.m4a");
                StrictLogManager::shared()->dayChoice_OpenFreeChoiceMenu();
            }

            this->showChoiceTab(!_choiceUp);
            _choiceTabJumpEnabled = false;
            return true;
        }
        return false;
    };
    
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, t);
    
    {
        
        
        
        
        auto iconSize = Size(574, 408);
        auto btnSize = iconSize + Size(0, 50);
        
        
        auto xMargin = (cs.width-btnSize.width*4)/5.0;
        auto yMargin = (cs.height - 105 - btnSize.height*3)/4.0;
        float xOrigin = xMargin + btnSize.width/2;
        float x = xOrigin;
        float y = cs.height - 105 - yMargin - btnSize.height/2;
        float xStep = btnSize.width+xMargin;
        float yStep = btnSize.height+yMargin;
        int index = 0;
        for (auto it : _freechoiceGames) {
            
            bool avaliable = it.second.maxAvailable>0 ? true : false;
            auto gameBtn = Node::create();
            
            gameBtn->setContentSize(btnSize);
            gameBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            gameBtn->setPosition(Vec2(x, y));
            
            auto gameIcon = Node::create();
            gameIcon->setContentSize(iconSize);
            gameIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            gameIcon->setPosition(Vec2(btnSize.width/2, btnSize.height-iconSize.height/2));
            gameBtn->addChild(gameIcon);
            
            auto l = TodoUtil::createLabelMultilineToFit(LanguageManager::getInstance()->getLocalizedString(it.first.c_str()), 50, Size(btnSize.width, 70), defaultFont, Color4B(255, 210, 74, 255*(avaliable ? 1 : 0.1)));
            l->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            l->setPosition(Vec2(btnSize.width/2, 30));
            gameBtn->addChild(l);
            
            index++;
            if (index%4==0) {
                x = xOrigin;
                y -= yStep;
            } else {
                x += xStep;
            }
            
            _choiceTab->addChild(gameBtn);
            
            
            std::string iconFilename = StringUtils::format("Main/MainScene/FreeChoiceThumbnail/freechoice_game_%s.png",it.first.c_str());
            Node *ic = Sprite::create(iconFilename);
            if (ic==nullptr) {
                ic = Node::create();
                ic->setContentSize(Size(800, 510));
            }
            ic->setScale(0.6);
            ic->setPosition(iconSize/2);
            
            
            if (it.second.isNew) {
                auto effect1 = Sprite::create(folder+"freechoice_icon_new_effect.png");
                effect1->setPosition(iconSize/2);
                gameIcon->addChild(effect1);
                effect1->runAction(RepeatForever::create(Sequence::create(
                                                                          FadeTo::create(0.7, 128),
                                                                          FadeTo::create(0.7, 255),
                                                                          nullptr)));
                
                auto bg = Sprite::create(folder+"freechoice_icon_new_bg.png");
                bg->setPosition(iconSize/2);
                gameIcon->addChild(bg);
                
                gameIcon->addChild(ic);
                
                auto frame = Sprite::create(folder+"freechoice_icon_new.png");
                frame->setPosition(iconSize/2);
                gameIcon->addChild(frame);
                
            } else {
                auto bg = Sprite::create(folder+"freechoice_icon_normal_bg.png");
                bg->setPosition(iconSize/2);
                gameIcon->addChild(bg);
                
                gameIcon->addChild(ic);
                
                auto frame = Sprite::create(folder+"freechoice_icon_normal.png");
                frame->setPosition(iconSize/2);
                gameIcon->addChild(frame);
            }
            
            if (avaliable) {
                
                
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
                auto cover = Sprite::create(folder+"freechoice_icon_disabled.png");
                cover->setPosition(iconSize/2);
                gameIcon->addChild(cover);
            }
            
            
        }
    }

}

void DailyScene2::onEnter()
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
        if (_isSpecialCourse) {
            _coinTab->setCoinLabel(coin);
            
        } else {
            _coinTab->addCoin(coin-showingCoin, Vec2(viewSize.width/2, 250));
        }
        
    }
    

}

void DailyScene2::refreshScene()
{
    
    
    bool isLevelCleared = UserManager::getInstance()->isLevelCleared(_levelID);
    
    if (_doneClearedDay) {
        
        setupMangoBoard();
        setupFreechoiceTab();
        
        // if (isLevelCleared) showChoiceTab(true);
        _doneClearedDay = false;
        
        return;
    }
    
    //auto oldCurrentDay = _currentDay;
    
    auto playingDay = UserManager::getInstance()->getPlayingDay();
    refreshData();

    
    if ((playingDay>0) && UserManager::getInstance()->isDayCleared(_levelID, playingDay)) {
        setupFreechoiceTab();
        showClear(playingDay);
    } else {
        
        
        setupMangoBoard();
        setupFreechoiceTab();
        
        if (isLevelCleared) {
            showChoiceTab(true);
        }
        
    }
    
}

void DailyScene2::onExit()
{
    Node::onExit();
    _touchEnabled = true;
    
}


void DailyScene2::lightFall(LightBall *l, bool first)
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


void DailyScene2::showFreechoicePopup(std::string gameName, int maxPlayable, int maxAvailable)
{
    GameSoundManager::getInstance()->playEffectSoundForAutoStart("WordVoice/GameName/"+gameName+".m4a");

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
        _freeChoiceStartEnabled = true;

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
            
            b->addClickEventListener([this, gameName, gameLevel, popup](Ref*) {
                if (!_freeChoiceStartEnabled) return;
                _freeChoiceStartEnabled = false;
                if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
                
                CCLOG("start game %s, %d", gameName.c_str(), gameLevel);
                StrictLogManager::shared()->dayChoice_ChooseFreeChoiceGame(gameName, gameLevel);
                CCAppController::sharedAppController()->startFreeChoiceGame(gameName, gameLevel);
                popup->dismiss(true);
            });
            
        }
    }
    
    
    
    popup->show(this, true);


}



void DailyScene2::showChoiceTab(bool show)
{
    if (_choiceTab->isVisible() == false) return;
    if (_choiceUp==show) return;
    
    _choiceTab->stopAllActions();
    
    auto arrow = show ? "freechoice_popup_button_close.png" : "freechoice_popup_button_open.png";
    _choiceTabArrow->setTexture(folder+arrow);
    
    _choiceUp = show;
    auto pos = show ? Vec2(tabPos.x, 0) : tabPos;
    _choiceTab->runAction(EaseOut::create(MoveTo::create(0.3, pos), 2.0));

}


void DailyScene2::showClear(int day)
{

    _touchEnabled = false;
    
    

    
    auto cur = CurriculumManager::getInstance()->findCurriculum(_levelID);
    
    Mango *mango = (Mango*)_mangoBoard->getChildByTag(day);
    int nextDay = _currentDay;
    if (nextDay>cur->numDays || nextDay<=0) nextDay = -1;
    
    
    Sprite *nextBtn = nextDay>=0 ? (Sprite*)_mangoBoard->getChildByTag(nextDay) : nullptr;
    if (nextDay == day) nextBtn = nullptr;
    
    Sequence *nextSeq;
    
    if (nextBtn && nextDay>0) {

        nextSeq = Sequence::create(CallFunc::create([this, nextDay](){
            auto nextMango = (Mango*)_mangoBoard->getChildByTag(nextDay);
            
            auto seqForNextBtn = Sequence::create(
                                                  DelayTime::create(0.1),
                                                  ScaleTo::create(0.3, 1.2),
                                                  CallFunc::create([this, nextDay, nextMango](){
                
                nextMango->setAvailable(true);
                nextMango->refresh();
                _touchEnabled = true;
            }),ScaleTo::create(0.3, 1.0),nullptr);
            nextMango->runAction(seqForNextBtn);
        }), nullptr);
    } else {
        nextSeq = Sequence::create(DelayTime::create(0.8),
                                   CallFunc::create([this](){
            StrictLogManager::shared()->dayChoice_End_Complete(_levelID);
            _touchEnabled = true;
            if (UserManager::getInstance()->isLevelCleared(_levelID)) {
                this->dismiss();
            }
        }),
                                   nullptr);
     

    }


    //mango->setStatus(1);

    

    auto seq = Sequence::create(DelayTime::create(0.3),
                                ScaleTo::create(0.6, 0.8),
                                ScaleTo::create(0.3, 1.2),
                                
                                CallFunc::create([mango, this]() {
        mango->refresh();
        SoundEffect::dayClearEffect().play();
        
        
        //showChoiceTab(true);
        
    }),
                                ScaleTo::create(0.2, 1.0),
                                DelayTime::create(0.1),
                                nextSeq,
                                nullptr);
    
    
    mango->runAction(seq);
    
    if (_choiceTabJumpEnabled) {
        _choiceTab->runAction(
        Sequence::create(
            DelayTime::create(0.5),
            JumpBy::create(1.5, Point(0,0), 200, 2),
            nullptr
        ));
    }
    _choiceTabJumpEnabled = false;
    UserManager::getInstance()->setPlayingDay(0);

}

void DailyScene2::dayChosen(int day)
{
    if (!_touchEnabled) return;
    _touchEnabled = false;
    
    
    {

        if (day > _currentDay) {
            UserManager::getInstance()->setCurrentDay(_levelID, day);
            {
                Mango *m1 = (Mango*)_mangoBoard->getChildByTag(_currentDay);
                if (m1) m1->refresh();
            }
            
            {
                Mango *m2 = (Mango*)_mangoBoard->getChildByTag(day);
                if (m2) m2->refresh();
            }

            _currentDay = day;
            
        }
        //UserManager::getInstance()->setCurrentDay(_levelID, day);
        UserManager::getInstance()->setPlayingDay(day);
        
        
        _doneClearedDay = UserManager::getInstance()->isDayCleared(_levelID, day);
        
        

        auto scene = GameSelectScene::createScene();
        scene->setName("GameSelectScene");
        
        auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
        fadeScene->setName("(TransitionFade GameSelectScene)");
        Director::getInstance()->pushScene(fadeScene);
    }
    
    LogManager::getInstance()->logEvent("DailyScene", "select_day", _levelID, day);

}

void DailyScene2::dismiss()
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

