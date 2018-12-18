//
//  BookWithQuiz.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 24/07/2018.
//

#include "BookWithQuiz.hpp"
#include "CCAppController.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Utils/TodoUtil.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Games/Books/BookView.hpp"
#include "Games/ComprehensionTest/ComprehensionScene.hpp"
#include "Menu/TutorialVideoScene.hpp"

BookWithQuiz::SceneState BookWithQuiz::_currentState = BookWithQuiz::SceneState::StandBy;

bool BookWithQuiz::init()
{
    if (!Layer::init()) return false;
    
    _currentState = BookWithQuiz::SceneState::StandBy;
    this->setContentSize(Director::getInstance()->getWinSize());
    
    return true;
}

Scene* BookWithQuiz::createScene(string bookFolder, int comprehensionTestLevel)
{
    auto scene = Scene::create();
    scene->setName("BookWithQuiz");
    
    // 'layer' is an autorelease object
    auto layer = BookWithQuiz::create();
    layer->_bookFolder = bookFolder;
    layer->_comprehensionTestLevel = comprehensionTestLevel;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void BookWithQuiz::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (_currentState == SceneState::Book && BookView::getReadAll() == false)
    {
        _currentState = SceneState::NeedToPopThisScene;
    }
    else if (_currentState == SceneState::ComprehensionTest && ComprehensionScene::getSolveAll() == false)
    {
        _currentState = SceneState::NeedToPopThisScene;
    }
    
    switch (_currentState)
    {
        case SceneState::StandBy:
            nextState();
            startBook();
            break;
        case SceneState::Book:
            nextState();
            DELAYED_CALLFUNC(0.01f, { // TODO: this is not a solution. need to fix (hyeon-gyu)
                startComprehensionTest();
            });
            break;
        case SceneState::NeedToPopThisScene:
            DELAYED_CALLFUNC(0.01f, { // TODO: this is not a solution. need to fix (hyeon-gyu)
                Director::getInstance()->popScene();
            });
            break;
        default:
            DELAYED_CALLFUNC(0.01f, { // TODO: this is not a solution. need to fix (hyeon-gyu)
                //Director::getInstance()->popScene();
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
            break;
    }
}

void BookWithQuiz::onEnter()
{
    Layer::onEnter();
}

void BookWithQuiz::onExit()
{
    Layer::onExit();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void BookWithQuiz::nextState()
{
    _currentState = (SceneState)((int)_currentState + 1);
}

void BookWithQuiz::startBook()
{
    CCAppController::sharedAppController()->startBookScene(_bookFolder+"/", false, false);
}

void BookWithQuiz::startComprehensionTest()
{
    CCAppController::sharedAppController()->startComprehensionScene(_bookFolder, _comprehensionTestLevel, false, false);
}

void BookWithQuiz::setSceneState(SceneState toState)
{
    _currentState = toState;
}
