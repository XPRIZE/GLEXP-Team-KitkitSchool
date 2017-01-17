//
//  MainScene2.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/3/16.
//
//

#include "MainScene2.hpp"

#include "CCAppController.hpp"

#include <functional>

#include "CoopScene.hpp"


#include "GradeSelector.hpp"

#include "Bird.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/CompletePopup.hpp"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"

#include "TestPopup.hpp"
#include "DailyData.hpp"
#include "Utils/TodoUtil.h"


const Size designSize = Size(2560,1800);
const Vec2 birdPos = Vec2(designSize.width/2.f,900);


static bool ___firstEnter = true;

using namespace std;


MainScene2::MainScene2():
_bird(nullptr),
_birdShadow(nullptr)
{
    
}


Scene* MainScene2::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = MainScene2::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}

//template <typename T>
//void addGameButton(Node *parent, std::string gameName, Vector<ui::Button*>& iconVector, T creator)

void MainScene2::addGameButton(Node *parent, std::string gameName, int level, Vector<ui::Button*>& iconVector)
{
    std::string iconFilename = StringUtils::format("icons/game_icon_%s.png",gameName.c_str());
    
    
    bool noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
    if (noIcon) {
        iconFilename = StringUtils::format("icons/main_gameicon_%s.png",gameName.c_str());
        noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
        if (noIcon) {
            iconFilename = "icons/main_gameicon_frame_shadow.png";
        }
    }
    auto btn = ui::Button::create(iconFilename);
    
    btn->setZoomScale(0);
    
    if (noIcon) {
        btn->setTitleFontSize(30);
        btn->setTitleText(gameName);
        
    }
    //    btn->setTitleText(gameName);
    //    btn->setTitleFontSize(70);
    // btn->setPosition(Vec2(500+500*iconVector.size(),150));
    btn->addTouchEventListener([gameName,level,this](Ref *pSender, ui::Widget::TouchEventType eEventType) {
        if (_transitionBegins) {
            return;
        }
        auto btn = (ui::Button*)pSender;
        auto cover = btn->getChildByName("gameIconCover");
        switch (eEventType) {
            case ui::Widget::TouchEventType::BEGAN:
                cover->setVisible(true);
                break;
            case ui::Widget::TouchEventType::MOVED:
                cover->setVisible(btn->isHighlighted());
                break;
            case ui::Widget::TouchEventType::ENDED:
                //                CCLOG("touch");
                cover->setVisible(false);
                _transitionBegins = true;

                CCAppController::sharedAppController()->startGame(gameName,level);
                break;
                
            default:
                break;
        }
        
        ////        Scene *scene;
        ////        creator(&scene);
        //        auto scene = creator();
    });
    btn->setVisible(false);
    
    
    
    Sprite* btnShadow = Sprite::create("icons/game_icon_frame_shadow.png");
    btnShadow->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnShadow,-1);
    Sprite* btnFrame = Sprite::create("icons/game_icon_frame.png");
    btnFrame->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnFrame);

    Sprite* levelBG = Sprite::create("icons/game_level_circle.png");
    levelBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    levelBG->setPosition(btnFrame->getContentSize().width, 15);
    btnFrame->addChild(levelBG);
    
    Sprite* levelNumber = Sprite::create(StringUtils::format("icons/game_level_circle_%d.png",level));
    levelNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    levelNumber->setPosition(levelBG->getContentSize()/2.f);
    levelBG->addChild(levelNumber);
    levelBG->setVisible(!UserManager::getInstance()->isCompletedGame(gameName, level));

    
    Sprite* cover = Sprite::create("icons/main_gameicon_activated.png");
    cover->setPosition(btn->getContentSize()/2.f);
    cover->setName("gameIconCover");
    cover->setVisible(false);
    btn->addChild(cover);
    
    Sprite* complete = Sprite::create("icons/game_icon_frame_completed.png");
    complete->setPosition(btn->getContentSize()/2.f);
    complete->setName("gameCompleteCover");
    complete->setVisible(UserManager::getInstance()->isCompletedGame(gameName, level));
    btn->addChild(complete);
    
    parent->addChild(btn);
    iconVector.pushBack(btn);
}

void MainScene2::layoutGameButton(Node *parent, Vector<ui::Button*>& iconVector) {
    
    
    auto numIcon = iconVector.size();
    float step = parent->getContentSize().width / (numIcon+1);
    
    for (int i=0; i<numIcon; i++) {
        auto btn = iconVector.at(i);
        btn->setPosition(Vec2(step*(i+1), 150));
    }
}

bool MainScene2::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    _mode = MODE_SHOW_POPUP;
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _rootNode = Node::create();
    _rootNode->setContentSize(designSize);
    _rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _rootNode->setPosition(Vec2(visibleSize.width/2, 0));
    
    addChild(_rootNode);
    
    auto skySize = Size(designSize.width, designSize.height*2);
    _skyNode = Node::create();
    _skyNode->setContentSize(skySize);
    _rootNode->addChild(_skyNode);
    
    _groundNode = Node::create();
    _groundNode->setContentSize(designSize);
    _rootNode->addChild(_groundNode);
    
    
    
    
    Sprite* sky = Sprite::create("MainScene/main_bg_sky.png");
    sky->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sky->setPosition(0,0);
    auto skySpriteSize = sky->getContentSize();
    auto skyScale = MAX(skySize.width/skySpriteSize.width, skySize.height/skySpriteSize.height);
    sky->setScale(skyScale);
    _skyNode->addChild(sky);
    
    
    
    Sprite* cloud1 = Sprite::create("MainScene/cloud_day_1.png");
    cloud1->setPosition(Vec2(visibleSize.width, random(1350, 1450)));
    cloud1->runAction(RepeatForever::create(Sequence::create(
                                                             MoveBy::create(random(40.0, 50.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud1->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud1);
    
    Sprite* cloud2 = Sprite::create("MainScene/cloud_day_2.png");
    cloud2->setPosition(Vec2(visibleSize.width, random(1450, 1550)));
    cloud2->runAction(RepeatForever::create(Sequence::create(
                                                             
                                                             MoveBy::create(random(35.0, 40.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud2->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud2);
    
    Sprite* cloud3 = Sprite::create("MainScene/cloud_day_3.png");
    cloud3->setPosition(Vec2(visibleSize.width, random(1650, 1750)));
    cloud3->runAction(RepeatForever::create(Sequence::create(
                                                             MoveBy::create(random(25.0, 35.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud3->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud3);
    
    _coopHighlight = Sprite::create("MainScene/coop_highlight.png");
    _coopHighlight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _coopHighlight->setVisible(false);
    Size highlightSpriteSize = _coopHighlight->getContentSize();
    Size winSize = Director::getInstance()->getWinSize();
    auto highlightScaleX = winSize.width/highlightSpriteSize.width;
    auto highlightScaleY = winSize.height/highlightSpriteSize.height;
    _coopHighlight->setScale(highlightScaleX, highlightScaleY);
    addChild(_coopHighlight);
    
    ui::Button* coop = ui::Button::create("MainScene/main_coop.png");
    coop->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    coop->setPosition(Vec2(designSize.width-220, designSize.height-660));
    coop->addClickEventListener([this](Ref*) {
        if (_transitionBegins) {
            return;
        }
        _transitionBegins = true;
        //        CCLOG("click");
        auto scene = CoopScene::createScene();
        Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
    });
    
    _groundNode->addChild(coop);
    _coopNode = coop;
    
    
    Sprite* grass = Sprite::create("MainScene/day_grass_ground.png");
    grass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    grass->setPosition(Vec2(0,0));
    _groundNode->addChild(grass);
    
    
    
    
    _leavesLeft = Sprite::create("MainScene/main_leaves_left.png");
    _leavesLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _leavesLeft->setPosition(Vec2(0, designSize.height-474));
    _rootNode->addChild(_leavesLeft);
    
    _leavesRight = Sprite::create("MainScene/main_leaves_right.png");
    _leavesRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _leavesRight->setPosition(Vec2(designSize.width, designSize.height-382));
    _rootNode->addChild(_leavesRight);
    
    
//    Vector<ui::Button*> mathIcons;
//    Vector<ui::Button*> literacyIcons;
//    
//
//    
//    layoutGameButton(_rootNode, literacyIcons);
//    layoutGameButton(_rootNode, mathIcons);
//    
//    gameIcons.push_back(literacyIcons);
//    gameIcons.push_back(mathIcons);
    
    
    if (visibleSize.width > designSize.width) {
        _rootNode->setScale(visibleSize.width/designSize.width);
    }
    
    
    //    auto cp = Button::create();
    //    cp->setTitleText("CompletePopup");
    //    cp->setTitleFontSize(50);
    //    cp->setPosition(Vec2(200, designSize.height-100));
    //    cp->addClickEventListener([](Ref*) {
    //        CompletePopup::create()->show(0, [](){
    //
    //        });
    //    });
    //    addChild(cp);
    
    LanguageManager::getInstance()->setCurrentLanguage(LanguageManager::SWAHILI);
    
    
    
    _infoBg = nullptr;
    
    return true;
}



void MainScene2::onEnter()
{
    Layer::onEnter();
    

    

    
    
    _transitionBegins = false;
    
    
    {
        Size visibleSize = getContentSize();
        
        const Size infoSize = Size(400, 300);
        
        
        if (_infoBg) _infoBg->removeFromParent();
        _infoBg = LayerColor::create(Color4B(0, 0, 0, 64), infoSize.width, infoSize.height);
        _infoBg->setPosition(30, visibleSize.height-infoSize.height - 30);
        addChild(_infoBg);
        
        
        Label* classTitle = Label::createWithTTF("Class : "+UserManager::getInstance()->getClassId(), "fonts/OpenSans-Bold.ttf", 50);
        classTitle->setTextColor(Color4B::WHITE);
        classTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        classTitle->setPosition(Vec2(infoSize.width/2.f, infoSize.height/4.0*(3.0)));
        _infoBg->addChild(classTitle);
        
        Label* studentTitle = Label::createWithTTF("Student : "+UserManager::getInstance()->getStudentId(), "fonts/OpenSans-Bold.ttf", 50);
        studentTitle->setTextColor(Color4B::WHITE);
        studentTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        studentTitle->setPosition(Vec2(infoSize.width/2.f, infoSize.height/4.0*(2.0)));
        _infoBg->addChild(studentTitle);
        
        Label* courseTitle = Label::createWithTTF("Course : "+UserManager::getInstance()->getCourseId(), "fonts/OpenSans-Bold.ttf", 50);
        courseTitle->setTextColor(Color4B::WHITE);
        courseTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        courseTitle->setPosition(Vec2(infoSize.width/2.f, infoSize.height/4.0*(1.0)));
        _infoBg->addChild(courseTitle);
        
        
    }

    
    
    if (___firstEnter)    {
        
        ___firstEnter = false;
        
        _infoBg->setVisible(false);
        
        auto eng = LanguageManager::getInstance()->isEnglish();
        auto logo = Sprite::create(eng ? "System/todoschool_BI.png" : "System/todoshule_BI.png");
        logo->setPosition(designSize/2);
        _rootNode->addChild(logo);
        logo->runAction(Sequence::create(DelayTime::create(1.7),
                                         EaseIn::create(MoveBy::create(0.2, Vec2(0, -100)), 2.0),
                                         EaseOut::create(MoveBy::create(1.0, Vec2(0, designSize.height)), 2.0),
                                         nullptr));
        
        
        _skyNode->setPosition(Vec2(0, -designSize.height*1.0));
        _skyNode->runAction(Sequence::create(DelayTime::create(0.2), MoveTo::create(1.8, Vec2(0, 0)), nullptr));
        
        _groundNode->setPosition(Vec2(0, -designSize.height));
        _groundNode->runAction(Sequence::create(DelayTime::create(1.0),
                                                EaseIn::create( MoveTo::create(1.0, Vec2(0, 0)), 2.0),
                                                nullptr));
        
        _coopNode->runAction(Sequence::create(DelayTime::create(2.0),
                                              EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                              EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
                                              nullptr));
        
        
//        _bird->setPosition(Vec2(birdPos.x, designSize.height+_bird->getContentSize().height));
//        
//        _bird->runAction(Sequence::create(DelayTime::create(2.2),
//                                          EaseOut::create(MoveTo::create(0.12, birdPos), 3.0),
//                                          nullptr
//                                          
//                                          ));
        
        auto winSize = _rootNode->getContentSize();
        
        _leavesLeft->setPosition(Vec2(-_leavesLeft->getContentSize().width, winSize.height-474));
        _leavesLeft->runAction(Sequence::create(
                                                DelayTime::create(2.5),
                                                EaseOut::create(MoveTo::create(0.2, Vec2(0, winSize.height-474)), 2.0),
                                                nullptr));
        
        _leavesRight->setPosition(Vec2(winSize.width+_leavesLeft->getContentSize().width, winSize.height-382));
        _leavesRight->runAction(Sequence::create(
                                                 DelayTime::create(2.5),
                                                 EaseOut::create(MoveTo::create(0.2, Vec2(winSize.width, winSize.height-382)), 2.0),
                                                 CallFunc::create([this]{
            setMode(MODE_SHOW_POPUP);
        }),
                                                 nullptr));
        
//        spawnGameEggs(2.2);
    }
    else {
        
        
        _mode = (MainSceneMode)UserDefault::getInstance()->getIntegerForKey("mainSceneMode", MainScene2::MODE_SHOW_POPUP);

        
        setMode(_mode);
    }
    

    
    
}

void MainScene2::setMode(MainScene2::MainSceneMode mode)
{
    
    _mode = mode;
    _coopHighlight->setVisible(false);
    switch (_mode) {
        case MODE_SHOW_POPUP:
        {
            _infoBg->setVisible(false);
            TestPopup* popup = TestPopup::create(this, Size(1500,1000));
            popup->_delegate = this;
            if (___firstEnter) {
                popup->show(this, true,2.5);
            }
            else {
                popup->show(this, true);
            }
        }
            break;
        case MODE_SELECT_COOP:
        {
            _infoBg->setVisible(false);
            _coopHighlight->setVisible(true);
        }
            break;
        case MODE_SELECT_GAME:
        {
            _infoBg->setVisible(true);
            Bird::BirdType currentBirdType = static_cast<Bird::BirdType>(UserDefault::getInstance()->getIntegerForKey("currentBirdType", Bird::BirdType::BIRD_L0));
            
            Bird::BirdStatus currentBirdStatus;
            
            currentBirdStatus = static_cast<Bird::BirdStatus>(UserManager::getInstance()->getBirdStatus(currentBirdType));
            
            if (_birdShadow) {
                _birdShadow->removeFromParent();
            }
            
            _birdShadow = Sprite::create("MainScene/character_shadow.png");
            _groundNode->addChild(_birdShadow);
            
            string currentCourse = UserManager::getInstance()->getCourseId();
            string category =currentBirdType == Bird::BIRD_L0 ? "L" : "M";
            int currentCourseInt = TodoUtil::stoi(currentCourse);
            
            if (currentCourseInt < 1) {
                currentCourseInt = 1;
            }
            else if(currentCourseInt > 12) {
                currentCourseInt = 12;
            }
//            else if(currentCourseInt > 2) {
//                currentCourseInt = 2;
//            }
            
            auto dailyData = DailyData::defaultData();
            
            DailyData::DailyGameArray dailyGameArray = dailyData.getDailyGameArray(DailyData::DailyDataKey(category, currentCourseInt));
            
            Vector<ui::Button*> icons;
            
            for (auto iconVector : gameIcons) {
                for (auto icon : iconVector) {
                    icon->removeFromParent();
                }
            }
            gameIcons.clear();
            
            for (auto game : dailyGameArray) {
                string gameName = game.first;
                int gameLevel = game.second;
                addGameButton(_rootNode, gameName, gameLevel, icons);
            }
            layoutGameButton(_rootNode, icons);
            
            gameIcons.push_back(icons);
            
            
            for (auto iconVector : gameIcons) {
                for (auto icon : iconVector) {
                    auto cover = icon->getChildByName("gameIconCover");
                    cover->setVisible(false);
                    
                }
            }
            
            
            //Bird* bird = Bird::create(currentBirdType, currentBirdStatus);
            Bird* bird = Bird::create(currentBirdType==Bird::BirdType::BIRD_L0 ? 'L' : 'M', 0, "");
            bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
            
            bird->onTouchEnded = [this, bird]() {
                
                int status = bird->getStatus();
                
                if (status == Bird::BirdStatus::EGG_HATCHED) return;
                
                status++;
                bird->setStatus(static_cast<Bird::BirdStatus>(status));
                
                if(status >= Bird::BirdStatus::EGG_HATCHED) {
                    
                    status = Bird::BirdStatus::EGG_HATCHED;
                    
                    //UserManager::getInstance()->setBirdStatus(bird->getType(), Bird::BirdStatus::EGG_HATCHED);
                    //UserDefault::getInstance()->flush();
                    
                    
                    
                    //        if (_bird->_type <= gameIcons.size()) {
                    for (auto icon : gameIcons[0]) {
                        icon->setVisible(true);
                    }
                    //        }
                    
                    spawnGameEggs(0.0);
                }

            };
            

            setCurrentBird(bird);
            //    _birdShadow->setPosition(birdPos.x, birdPos.y - bird->getContentSize().height/2.f);
            Vec2 shadowPosInWorld = _rootNode->convertToWorldSpace(Vec2(birdPos.x, birdPos.y - bird->getContentSize().height/2.f));
            _birdShadow->setPosition(_groundNode->convertToNodeSpace(shadowPosInWorld));
            
            spawnGameEggs(0);
        }
            
        default:
            break;
    }
    
    UserDefault::getInstance()->setIntegerForKey("mainSceneMode", _mode);
    UserDefault::getInstance()->flush();
    
}


void MainScene2::spawnGameEggs(float delay)
{
    for (auto iconVector : gameIcons) {
        for (auto icon : iconVector) {
            if (icon->isVisible()) {
                auto p = icon->getPosition();
                icon->setPosition(birdPos);
                icon->setScale(0);
                icon->runAction(Sequence::create(
                                                 DelayTime::create(delay+random(0.0, 0.3)),
                                                 Spawn::create(
                                                               ScaleTo::create(0.7, 1),
                                                               MoveBy::create(1.0, Vec2(p.x-birdPos.x, 0)),
                                                               Sequence::create(
                                                                                
                                                                                EaseOut::create(MoveBy::create(0.5, Vec2(0, 120)), 2.0),
                                                                                EaseIn::create(MoveBy::create(0.5, Vec2(0, -120+(p.y-birdPos.y))), 2.0),
                                                                                EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
                                                                                EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                                                                
                                                                                nullptr
                                                                                ),
                                                               
                                                               nullptr),
                                                 nullptr));
                
                
            }
            
        }
    }
    
}

void MainScene2::setCurrentBird(Bird *bird) {
    if (_bird) {
        _bird->removeFromParent();
        //        CC_SAFE_RELEASE_NULL(_bird);
    }
    
    _bird = bird;
    _bird->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bird->setPosition(birdPos);
    _rootNode->addChild(bird);
    
    //Bird::BirdType currentBirdType = _bird->getType();
    
    for(int i = 0; i < gameIcons.size(); i++) {
        bool visible = true;
//        if (i+1 == currentBirdType) {
            if (_bird->getStatus() != Bird::BirdStatus::EGG_HATCHED) {
                visible = false;
            }
//        }
        for (auto icon : gameIcons[i]) {
            icon->setVisible(visible);
        }
    }
    //    LayerColor* bb = LayerColor::create(Color4B::WHITE);
    //    bb->setPosition(bird->getBoundingBox().origin);
    //    bb->setContentSize(bird->getBoundingBox().size);
    //    addChild(bb);
    
    
}



void MainScene2::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
    _transitionBegins = true;
}
