//
//  GameSelectScene.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 09/12/17.
//
//

#include <functional>

#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"

#include "CustomDirector.h"

#include "GameSelectScene.hpp"
#include "CoopScene.hpp"
#include "DailyScene.hpp"


#include "GradeSelector.hpp"

#include "Bird.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/CompletePopup.hpp"

#include "Managers/LanguageManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "DailyData.hpp"


#include "Common/Effects/FireworksEffect.hpp"
#include "Common/Controls/CoinTab.hpp"

namespace GameSelectSceneSpace {
    const Size designSize = Size(2560,1800);
    static Size visibleSize;
    static Size frameSize;
    
    const Vec2 birdPos = Vec2(designSize.width/2.f,600);
    const Vec2 coopPos = Vec2(designSize.width-220-241, designSize.height-660-172);
    
    static Vec2 panelPos;
    
    const Size panelNodeSize = Size(670, 162);
    
    const int coopBackZ = -100;
    const int coopFrontZ = 100;
    
    const string fontName = "fonts/TodoMainCurly.ttf";
    
};

using namespace GameSelectSceneSpace;


static bool __firstEnter = true;




GameSelectScene::GameSelectScene():
_bird(nullptr),
_birdShadow(nullptr),
_day(0)
{
    
}


Scene* GameSelectScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = GameSelectScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}


void GameSelectScene::clearGameButton()
{
    if (_debugView) {
        _debugView->removeFromParent();
        _debugView = nullptr;
    }
    
    for (auto i : gameIcons) {
        i->removeFromParent();
    }
    gameIcons.clear();
}

void GameSelectScene::addDebugGameButton(Node *parent, std::string gameName, Vector<ui::Button*>& iconVector)
{
    /*
    std::string iconFilename = StringUtils::format("icons/main_gameicon_%s.png",gameName.c_str());

    bool noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
    if (noIcon) {
        iconFilename = "icons/main_gameicon_frame_shadow.png";
    }
    */
    
    std::string iconFilename = StringUtils::format("icons/game_icon_%s.png", gameName.c_str());
    
    
    bool noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
    if (noIcon) {
        iconFilename = StringUtils::format("icons/main_gameicon_%s.png", gameName.c_str());
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

    btn->addTouchEventListener([gameName, this](Ref *pSender, ui::Widget::TouchEventType eEventType) {
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

                cover->setVisible(false);
                CCAppController::sharedAppController()->startGameWithLevelChoice(gameName);
                SoundEffect::buttonEffect().play();
                
                
                break;
                
            default:
                break;
        }
        
    });
/*
    Sprite* btnShadow = Sprite::create("icons/main_gameicon_frame_shadow.png");
    btnShadow->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnShadow,-1);
    Sprite* btnFrame = Sprite::create("icons/main_gameicon_frame.png");
    btnFrame->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnFrame);
    */
    
    Sprite* btnShadow = Sprite::create("icons/game_icon_frame_shadow.png");
    btnShadow->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnShadow,-1);
    Sprite* btnFrame = Sprite::create("icons/game_icon_frame.png");
    if (gameName=="EggQuiz") {
        btnFrame = Sprite::create("icons/game_icon_frame_EggQuiz.png");
    }
    btnFrame->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnFrame);
    
    Sprite *cover = Sprite::create("icons/game_icon_frame_shadow.png");
    cover->setPosition(btn->getContentSize()/2);
    cover->setColor(Color3B::BLACK);
    cover->setOpacity(128);
    cover->setName("gameIconCover");
    cover->setVisible(false);
    btn->addChild(cover);
    

 
    

    
    
    parent->addChild(btn);
    iconVector.pushBack(btn);
}

void GameSelectScene::addAllDebugGameButtons(char category, Vector<ui::Button*>& iconVector)
{

    clearGameButton();
    
    if (category=='M') {
        addDebugGameButton(_rootNode, "NumberTracing", iconVector);
        addDebugGameButton(_rootNode, "NumberTracingExt", iconVector);
        addDebugGameButton(_rootNode, "NumberPuzzle", iconVector);
        addDebugGameButton(_rootNode, "FindTheMatch", iconVector);
        addDebugGameButton(_rootNode, "NumberMatching", iconVector);
        addDebugGameButton(_rootNode, "Counting", iconVector);
        addDebugGameButton(_rootNode, "MovingInsects", iconVector);
        addDebugGameButton(_rootNode, "FishTank", iconVector);
        addDebugGameButton(_rootNode, "DoubleDigit", iconVector);
        addDebugGameButton(_rootNode, "EquationMaker", iconVector);
        addDebugGameButton(_rootNode, "Tapping", iconVector);
        addDebugGameButton(_rootNode, "TutorialTrace", iconVector);
        addDebugGameButton(_rootNode, "HundredPuzzle", iconVector);
        addDebugGameButton(_rootNode, "NumberTrain", iconVector);
        addDebugGameButton(_rootNode, "SoundTrain", iconVector);
        addDebugGameButton(_rootNode, "PatternTrain", iconVector);
        addDebugGameButton(_rootNode, "AnimalPuzzle", iconVector);

        
       
    } else if (category=='L') {
        addDebugGameButton(_rootNode, "Book", iconVector);
        addDebugGameButton(_rootNode, "AlphabetPuzzle", iconVector);
        addDebugGameButton(_rootNode, "WordMachine", iconVector);
        addDebugGameButton(_rootNode, "LetterTracing", iconVector);
        addDebugGameButton(_rootNode, "LetterTracingCard", iconVector);
        addDebugGameButton(_rootNode, "WordTracing", iconVector);
        addDebugGameButton(_rootNode, "Comprehension", iconVector);
        addDebugGameButton(_rootNode, "StarFall", iconVector);
        addDebugGameButton(_rootNode, "Spelling", iconVector);
        addDebugGameButton(_rootNode, "LetterMatching", iconVector);
    //    addDebugGameButton(_rootNode, "DigitalQuiz", iconVector );
        addDebugGameButton(_rootNode, "AnimalPuzzle", iconVector);
    //    addDebugGameButton(_rootNode, "CompMatching", iconVector);
        addDebugGameButton(_rootNode, "SentenceMaker", iconVector);
        addDebugGameButton(_rootNode, "EggQuiz", iconVector);
        addDebugGameButton(_rootNode, "BirdPhonics", iconVector);
        addDebugGameButton(_rootNode, "ShapeMatching", iconVector);

        

    }
    
    
    layoutGameButton(_rootNode, gameIcons);
}
        
        

void GameSelectScene::addGameButton(Node *parent, std::string levelID, int day, int gameIndex, Vector<ui::Button*>& iconVector)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    auto dayCur = cur->getDayCurriculum(day);
    auto gameInfo = dayCur->games.at(gameIndex);
    
    std::string iconFilename = StringUtils::format("icons/game_icon_%s.png",gameInfo.gameName.c_str());
    
    
    bool noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
    if (noIcon) {
        iconFilename = StringUtils::format("icons/main_gameicon_%s.png",gameInfo.gameName.c_str());
        noIcon = !FileUtils::getInstance()->isFileExist(iconFilename);
        if (noIcon) {
            iconFilename = "icons/main_gameicon_frame_shadow.png";
        }
    }
    auto btn = ui::Button::create(iconFilename);
    
    btn->setZoomScale(0);
    
    if (noIcon) {
        btn->setTitleFontSize(30);
        btn->setTitleText(gameInfo.gameName);
        
    }
    
    btn->addTouchEventListener([levelID, day, gameIndex, this](Ref *pSender, ui::Widget::TouchEventType eEventType) {
        if (_transitionBegins) {
            return;
        }
        if (!_backButton->isTouchEnabled()) return;
        
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
                StrictLogManager::shared()->dailyGameChoice_ChooseDailyGame(levelID, day, gameIndex);
                
                cover->setVisible(false);
                _transitionBegins = true;
                SoundEffect::buttonEffect().play();
                CCAppController::sharedAppController()->startCurriculumGame(levelID, day, gameIndex);
                break;
                
            default:
                break;
        }
        
    });
    

    
    Sprite* btnShadow = Sprite::create("icons/game_icon_frame_shadow.png");
    btnShadow->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnShadow,-1);
    Sprite* btnFrame = Sprite::create("icons/game_icon_frame.png");
    if (gameInfo.gameName=="EggQuiz") {
        btnFrame = Sprite::create("icons/game_icon_frame_EggQuiz.png");
    }
    btnFrame->setPosition(btn->getContentSize()/2.f);
    btn->addChild(btnFrame);

    
    Sprite *cover = Sprite::create("icons/game_icon_frame_shadow.png");
    cover->setPosition(btn->getContentSize()/2);
    cover->setColor(Color3B::BLACK);
    cover->setOpacity(128);
    cover->setName("gameIconCover");
    cover->setVisible(false);
    btn->addChild(cover);
    
    
    if (gameInfo.gameName!="EggQuiz" && gameInfo.gameName!="Book" && gameInfo.gameName!="Comprehension" && gameInfo.gameName!="Video") {
        
        Sprite* levelBG = Sprite::create("icons/game_level_circle.png");
        levelBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        levelBG->setPosition(btnFrame->getContentSize().width, 15);
        btnFrame->addChild(levelBG);
        
        Node* levelNumber = Sprite::create(StringUtils::format("icons/game_level_circle_%d.png",gameInfo.gameLevel));
        if (!levelNumber) {
            levelNumber = TodoUtil::createLabel(TodoUtil::itos(gameInfo.gameLevel), 62, Size::ZERO, "fonts/TodoMainCurly.ttf", Color4B::WHITE);
            levelNumber->setAnchorPoint(Vec2(0.5, 0.6));
        } else {
            levelNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        }
        levelNumber->setPosition(levelBG->getContentSize()/2.f);
        levelBG->addChild(levelNumber);
        levelBG->setVisible(!UserManager::getInstance()->isGameCleared(levelID, day, gameIndex));
        
        {
            Sprite* levelBG = Sprite::create("icons/game_level_circle.png");
            levelBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            levelBG->setPosition(btnFrame->getContentSize().width, 15);
            cover->addChild(levelBG);
            cover->setCascadeColorEnabled(true);
            cover->setCascadeOpacityEnabled(true);
        }
        
    }
    
    
    /*
    Sprite* cover = Sprite::create("icons/main_gameicon_activated.png");
    cover->setPosition(btn->getContentSize()/2.f);
    cover->setName("gameIconCover");
    cover->setVisible(false);
    btn->addChild(cover);
    */
    
    
    
    
    
    
    Sprite* complete = Sprite::create("icons/game_icon_frame_completed.png");
    complete->setPosition(btn->getContentSize()/2.f);
    complete->setName("gameCompleteCover");
    complete->setVisible(UserManager::getInstance()->isGameCleared(levelID, day, gameIndex));
    btn->addChild(complete);
    
    parent->addChild(btn);
    iconVector.pushBack(btn);
}

void GameSelectScene::addDayGameButtons(std::string levelID, int day, Vector<ui::Button*>& iconVector)
{
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    auto dayCur = cur->getDayCurriculum(day);
    
    clearGameButton();
    
    for (int i=0; i<dayCur->numGames; i++) {
        addGameButton(_rootNode, levelID, day, i, gameIcons);
    }
    layoutGameButton(_rootNode, gameIcons);
    

    if (UserManager::getInstance()->isDebugMode()) {
        
        _debugView = Node::create();
        _debugView->setContentSize(Size(designSize.width, 400));
        _debugView->setPosition(Vec2(0, designSize.height-400));
        addChild(_debugView);
        
        auto sk1 = ui::Button::create();
        sk1->setTitleText("Clear One Game");
        sk1->setTitleFontSize(50);
        sk1->setPosition(Vec2(designSize.width*0.75, 300));
        _debugView->addChild(sk1);
        sk1->addClickEventListener([dayCur, levelID, day, this](Ref*) {
            if (dayCur) {
                for (int i=0; i<dayCur->numGames; i++) {
                    if (!UserManager::getInstance()->isGameCleared(levelID, day, i)) {
                        UserManager::getInstance()->setGameCleared(levelID, day, i);
                        break;
                    }
                }
                addDayGameButtons(levelID, day, gameIcons);
                
                checkDayClear();
            }
        });
        
        
        auto sk2 = ui::Button::create();
        sk2->setTitleText("Clear All Game");
        sk2->setTitleFontSize(50);
        sk2->setPosition(Vec2(designSize.width*0.75, 100));
        _debugView->addChild(sk2);
        sk2->addClickEventListener([dayCur, levelID, day, this](Ref*) {
            if (dayCur) {
                for (int i=0; i<dayCur->numGames; i++) {
                    UserManager::getInstance()->setGameCleared(levelID, day, i);
                }
                addDayGameButtons(levelID, day, gameIcons);
                
                checkDayClear();
            }
        });
        
    };
    

}

void GameSelectScene::layoutGameButton(Node *parent, Vector<ui::Button*>& iconVector) {
    
    
    auto numIcon = iconVector.size();
    float step = parent->getContentSize().width / (numIcon+1);
    
    float y = UserManager::getInstance()->isGameTestingMode() ? 180 : 150;
    
    for (int i=0; i<numIcon; i++) {
        auto btn = iconVector.at(i);
        btn->setPosition(Vec2(step*(i+1), y));
    }
}

bool GameSelectScene::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    {
        auto levelID = UserManager::getInstance()->getCurrentLevelID();
        auto day = UserManager::getInstance()->getCurrentDay(levelID);
        StrictLogManager::shared()->dailyGameChoice_Begin(levelID, day);
    }

    
    visibleSize = Director::getInstance()->getVisibleSize();
    Size winSize = Director::getInstance()->getWinSize();

    
    _debugView = nullptr;
    
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

    /*
    {
        ui::Button* test = ui::Button::create("icons/game_icon_frame_completed.png");
        
        test->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        test->setPosition(Vec2(visibleSize.width*0.3, visibleSize.height-300));
        test->addClickEventListener([this](Ref*) {
            
            auto effect = FireworksEffect::create();
            effect->setPosition(Vec2(1024, 1000));
            this->addChild(effect);
            effect->start();
            
        });
        addChild(test);
    }
    
    {
        ui::Button* test = ui::Button::create("icons/game_icon_frame_completed.png");
        
        test->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        test->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height-300));
        test->addClickEventListener([this](Ref*) {
            auto cl = LayerColor::create(Color4B(255, 0, 0, 64));
            Director::getInstance()->getRunningScene()->addChild(cl);
            auto shake = Sequence::create(EaseOut::create(MoveBy::create(0.06, Vec2(50, 0)), 2.0),
                                          EaseInOut::create(MoveBy::create(0.12, Vec2(-100, 0)), 2.0),
                                          EaseOut::create(MoveBy::create(0.06, Vec2(50, 0)), 2.0),
                                          CallFunc::create([cl](){
                cl->removeFromParent();
            }),
                                          nullptr);
            Director::getInstance()->getRunningScene()->runAction(shake);
            
        });
        addChild(test);
    }
    */
    
    
    
    Sprite* mountain = Sprite::create("MainScene/right_mountain.png");
    mountain->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    mountain->setPosition(designSize.width, 730);
    _groundNode->addChild(mountain);
    
    
    Sprite* grass = Sprite::create("MainScene/day_grass_ground.png");
    grass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    grass->setPosition(Vec2(0,0));
    _groundNode->addChild(grass);
    
    

    
    _rootScale = 1.f;
    if (visibleSize.width > designSize.width) {
        _rootScale = (visibleSize.width/designSize.width);
        _rootNode->setScale(_rootScale);
        
        frameSize = visibleSize/_rootScale;
        
        
    } else {
        frameSize = visibleSize;
    }
    
    _frameNode = Node::create();
    _frameNode->setContentSize(frameSize);
    _frameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frameNode->setPosition(Vec2(designSize.width/2, 0));
    _rootNode->addChild(_frameNode);
    
    
    _leavesLeft = Sprite::create("MainScene/main_leaves_left.png");
    _leavesLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _leavesLeft->setPosition(Vec2(0, frameSize.height-474));
    _frameNode->addChild(_leavesLeft);
    
    _leavesRight = Sprite::create("MainScene/main_leaves_right.png");
    _leavesRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _leavesRight->setPosition(Vec2(frameSize.width, frameSize.height-382));
    _frameNode->addChild(_leavesRight);
    
    


    
    panelPos = Vec2(frameSize.width/2, frameSize.height);
    _panelNode = Node::create();
    _panelNode->setContentSize(panelNodeSize);
    _panelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _panelNode->setPosition(panelPos);
    _frameNode->addChild(_panelNode);
    
 
    _dayNode = Sprite::create("MainScene/panel_day.png");
    _dayNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _dayNode->setPosition(Vec2(panelNodeSize.width-44, 60));
    _panelNode->addChild(_dayNode);
 

    _backButton = TodoSchoolBackButton::create();
    _backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _backButton->setPosition(Vec2(25, winSize.height-25));
    _backButton->onBack = [this] {
        StrictLogManager::shared()->dailyGameChoice_End_Quit(_levelID, _day);
    };
    addChild(_backButton);
//    
//    {
//        auto backBtn = ui::Button::create();
//        backBtn->loadTextures("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png");
//        
//        auto keyListener = EventListenerKeyboard::create();
//        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
//            //if (!_touchEnabled) return;
//            
//            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
//                ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
//            }
//        };
//        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
//        backBtn->addClickEventListener([this](Ref*){
//            //if (!_touchEnabled) return;
//            SoundEffect::menuBackEffect().play();
//            
//            ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
//        });
//        
//        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
//        backBtn->setPosition(Vec2(25, winSize.height-25));
//        addChild(backBtn);
//    }

    
    
    {
        _coinTab = CoinTab::create();
        _coinTab->setPosition(winSize - Size(50, 50));
        addChild(_coinTab);
    }

    
    SoundEffect::coopClickEffect().preload();
    

    

    
    
    return true;
}




void GameSelectScene::onEnter()
{
    Layer::onEnter();
    
    _backButton->setTouchEnabled(true);
    
    visibleSize = Director::getInstance()->getVisibleSize();

    
    stopAllActions();
    setScale(1.0);
    setPosition(Vec2::ZERO);
    
    //_rootNode->stopAllActions();
    //_rootNode->setScale(_rootScale);
    //_rootNode->setPosition(Vec2(visibleSize.width/2, 0));
    


    _transitionBegins = false;
    
    _panelNode->setPosition(panelPos);
    //_dayNode->setPosition(dayPos);
    

    auto oldLevelID = _bird ? _bird->getLevelID() : "";
    auto oldDay = _day;
    
    
    
    
    auto levelID = UserManager::getInstance()->getCurrentLevelID();
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    _levelID = levelID;
    
    clearGameButton();
    
    if (cur) {
        
        if (_birdShadow) {
            _birdShadow->removeFromParent();
        }
        
        _birdShadow = Sprite::create("MainScene/character_shadow.png");
        _groundNode->addChild(_birdShadow);
        
        Bird* bird = Bird::create(cur->category, cur->categoryLevel, cur->levelID);
        
        bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        
        bird->onTouchBegan = [bird](){
            bird->runTouchAnimation();
        };
        
        setCurrentBird(bird);
        
        if (UserManager::getInstance()->isGameTestingMode()) {
            addAllDebugGameButtons(cur->category, gameIcons);
            

        } else {
            bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(cur->levelID));
            auto shadowScale = bird->getBoundingBox().size.width / _birdShadow->getContentSize().width;
            _birdShadow->setScale(shadowScale);
            
            _day = UserManager::getInstance()->getCurrentDay(levelID);
            if (_day>cur->days.size()) _day = cur->days.size();
            
            /*
            if (day==0 && !__firstEnter) {
             
                showDailyScene(levelID);
                
            } else
             */
            if (_day>0) {
    
                addDayGameButtons(levelID, _day, gameIcons);
            }
            
        }


        //_birdShadow->setPosition(_groundNode->convertToNodeSpace(shadowPosInWorld));
        _birdShadow->setScale(bird->getBoundingBox().size.width / _birdShadow->getContentSize().width);
        _birdShadow->setPosition(birdPos);
        


        {
            
            _dayNode->retain();
            
            _panelNode->setVisible(true);
            _panelNode->removeAllChildren();
            
            _panelNode->addChild(_dayNode);
            
            string panelFilename = cur->category=='L' ? "panel_english.png" : "panel_math.png";
//            if (cur->categoryLevel==0) panelFilename = "panel_prek.png";
            auto panel = Sprite::create("MainScene/"+panelFilename);
            panel->setPosition(panelNodeSize/2);
            _panelNode->addChild(panel);
            
            auto panelLabel = TodoUtil::createLabel(cur->levelTitle, 55, Size::ZERO, fontName, Color4B(255, 240, 184, 255));
            panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            panelLabel->setPosition(panel->getContentSize()/2 - Size(0, 25));
            panel->addChild(panelLabel);
            
            
        }
        
        {
            
            _dayNode->setVisible(true);
            _dayNode->removeAllChildren();
            
            
            auto dayLabel = TodoUtil::createLabel(TodoUtil::itos(_day), 70, Size::ZERO, fontName, Color4B(255, 205, 66, 255));
            dayLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dayLabel->setPosition(_dayNode->getContentSize()/2 + Size(20, -5));
            _dayNode->addChild(dayLabel);
            
        }
        
        

        
  
        
        
        
        

    }
    
    


    
    
    GameSoundManager::getInstance()->stopBGM();
    
    if (__firstEnter)    {
        
        
        __firstEnter = false;
        
        const float appearDelay = 0.2;
        
        auto eng = LanguageManager::getInstance()->isEnglish();
        

        
        if (_bird) {
        
            _bird->setPosition(Vec2(birdPos.x, designSize.height+_bird->getContentSize().height));
            
            _bird->runAction(Sequence::create(DelayTime::create(appearDelay),
                                            EaseOut::create(MoveTo::create(0.12, birdPos), 3.0),
                                             nullptr
                                             
                                             ));
        }
       
        
        _leavesLeft->setPosition(Vec2(-_leavesLeft->getContentSize().width, frameSize.height-474));
        _leavesLeft->runAction(Sequence::create(
                                                DelayTime::create(appearDelay+0.3),
                                                EaseOut::create(MoveTo::create(0.2, Vec2(0, frameSize.height-474)), 2.0),
                                                nullptr));
        
        _leavesRight->setPosition(Vec2(designSize.width+_leavesLeft->getContentSize().width, frameSize.height-382));
        _leavesRight->runAction(Sequence::create(
                                                 DelayTime::create(appearDelay+0.3),
                                                 EaseOut::create(MoveTo::create(0.2, Vec2(frameSize.width, frameSize.height-382)), 2.0),
                                                 nullptr));
        
        _panelNode->setPosition(panelPos + Vec2(0, panelNodeSize.height));
        _panelNode->runAction(Sequence::create(
                                               DelayTime::create(appearDelay+0.3),
                                               EaseOut::create(MoveTo::create(0.2, panelPos), 2.0),
                                               nullptr));
        
        
        
        //GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/LogoFall.m4a");
        spawnGameEggs(appearDelay);
    } else {
        
        checkDayClear();
        
        if ( _bird && _bird->getLevelID()!="" && ((oldLevelID!=_bird->getLevelID()) || (oldDay!=_day))) {
            spawnGameEggs(0.2);
        } else {
            GameSoundManager::getInstance()->playBGM("Common/Music/BGM1_TitleScreen_intro.m4a");
        }
            
        

        
    }
    

}


void GameSelectScene::checkDayClear()
{
    auto levelID = UserManager::getInstance()->getCurrentLevelID();
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    if (!cur) return;
    
    auto day = UserManager::getInstance()->getCurrentDay(levelID);
    auto dayCurr = cur->getDayCurriculum(day);
    if (!dayCurr) return;
    
    if (!UserManager::getInstance()->isDayCleared(levelID, day)) {
        int numClearedGame = 0;
        
        for (int i=0; i<dayCurr->numGames; i++) {
            if (UserManager::getInstance()->isGameCleared(levelID, day, i)) numClearedGame++;
        }
        
        if (dayCurr->numGames == numClearedGame) {
            
            _backButton->setTouchEnabled(false);
            
            
            GameSoundManager::getInstance()->stopBGM();
            
            UserManager::getInstance()->setDayCleared(levelID, day);
            
            int reward = dayCurr->numGames;
            if (dayCurr->dayOrder == cur->numDays) reward = 10;
            
            auto stars = UserManager::getInstance()->getStars();
            UserManager::getInstance()->updateStars(stars+reward);
            
            
            SoundEffect::birdJumpEffect().preload();
            SoundEffect::birdGrowEffect().preload();
            
            auto seq = Sequence::create(DelayTime::create(1.0),
                                        CallFunc::create([](){ SoundEffect::birdJumpEffect().play();}),
                                        EaseOut::create(MoveBy::create(0.3, Vec2(0, 30)), 2.0),
                                        EaseIn::create(MoveBy::create(0.3, Vec2(0, -30)), 2.0),
                                        CallFunc::create([](){ SoundEffect::birdJumpEffect().play();}),
                                        EaseOut::create(MoveBy::create(0.3, Vec2(0, 30)), 2.0),
                                        EaseIn::create(MoveBy::create(0.3, Vec2(0, -30)), 2.0),
                CallFunc::create([this, reward](){
                _bird->runTouchAnimation();
                SoundEffect::birdJumpEffect().play();
                
                auto birdPos = _bird->getPosition() + Vec2(0, 300);
                auto birdParent = _bird->getParent();
                
                _coinTab->addCoin(reward, birdParent->convertToWorldSpace(birdPos));
                
               
            }), EaseOut::create(MoveBy::create(1.6, Vec2(0, 150)), 2.0),
                CallFunc::create([this, levelID](){
                
                
                _bird->loadAnimation();
                _bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(levelID));
                _bird->runTouchAnimation();
                SoundEffect::birdGrowEffect().play();
                
                auto shadowScale = _bird->getBoundingBox().size.width / _birdShadow->getContentSize().width;
                _birdShadow->setScale(shadowScale);

                
            }), EaseIn::create(MoveBy::create(1.6, Vec2(0, -150)), 2.0),
                CallFunc::create([this, levelID, day](){

                StrictLogManager::shared()->dailyGameChoice_End_Complete(levelID, day);
                
                _backButton->setTouchEnabled(true);
                
                ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
// 
//                auto d = DailyScene::create(this, levelID);
//                d->showClear(day);
//                d->onSelectDay = [this, d](int day) {
//                    d->dismiss();
//                    UserManager::getInstance()->setCurrentDay(day);
//                    this->onEnter();
//                };
                
                
            }), nullptr);
            
            _bird->runAction(seq);
            

            
        }
    }
}


void GameSelectScene::spawnGameEggs(float delay)
{

    SoundEffect::iconSpawnEffect().preload();
    this->runAction(Sequence::create(DelayTime::create(1.0+delay),
                                     CallFunc::create([](){SoundEffect::iconSpawnEffect().play(); }),
                                     DelayTime::create(1.5),
                                     CallFunc::create([](){GameSoundManager::getInstance()->playBGM("Common/Music/BGM1_TitleScreen_intro.m4a");}),
                                     nullptr));
    
    for (auto icon : gameIcons) {
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

void GameSelectScene::setCurrentBird(Bird *bird) {
    if (_bird) {
        _bird->removeFromParent();
    }
    
    _bird = bird;
    _bird->setPosition(birdPos);
    _rootNode->addChild(bird);

    
    
}

void GameSelectScene::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
    _transitionBegins = true;
}


