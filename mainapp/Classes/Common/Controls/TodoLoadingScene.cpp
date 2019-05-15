//
//  TodoLoadingScene.cpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 6/28/16.
//
//

#include "TodoLoadingScene.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"
//#include "Managers/UserManager.hpp"
//#include "Managers/CurriculumManager.hpp"
#include <string>

using namespace std;


Scene* TodoLoadingScene::createScene(std::function<Scene*(void)> creator, float loadingTime, float fadeOutTime)
//Scene* TodoLoadingScene::createScene(Scene* nextScene, float loadingTime, float fadeOutTime)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = TodoLoadingScene::create(creator, loadingTime, fadeOutTime);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}

TodoLoadingScene* TodoLoadingScene::create(std::function<Scene*(void)> &creator, float loadingTime, float fadeOutTime)
//TodoLoadingScene* TodoLoadingScene::create(Scene* &nextScene, float loadingTime, float fadeOutTime)
{
    TodoLoadingScene* pRet = new (std::nothrow) TodoLoadingScene();
    
    if(pRet && pRet->init(creator, loadingTime, fadeOutTime))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
    
}


void TodoLoadingScene::update(float delta)
{
    Layer::update(delta);
    _currentTimer += delta;
    
    if (_nextSceneLoaded && _transitionFinished && _currentTimer>_loadingTime) {

        CCLOG("_nextSceneLoaded");
        GameSoundManager::getInstance()->unloadAllEffect();
        
        Director::getInstance()->replaceScene(TransitionFade::create(_fadeOutTime, _nextScene));
        CC_SAFE_RELEASE(_nextScene);
        
        unscheduleUpdate();
    } else {
        return;
    }
}

bool TodoLoadingScene::init(std::function<Scene*(void)> &creator, float loadingTime, float fadeOutTime)
//bool TodoLoadingScene::init(Scene* &nextScene, float loadingTime, float fadeOutTime)
{
    CCLOG("TodoLoadingScene::init");

    
    if (!Layer::init()) {
        return false;
    }
    
//    _nextScene = nextScene;
//    _nextScene->retain();
    
    _sceneCreator = creator;
    _nextSceneLoaded = false;
    _transitionFinished = false;
    
//    auto t = std::thread([this, creator]()

//    );
//    t.detach();
    
    
    _currentTimer = 0.0;
    scheduleUpdate();
    
    _loadingTime = loadingTime;
    _fadeOutTime = fadeOutTime;
    
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
//    auto eng = LanguageManager::getInstance()->isEnglish();
//    Sprite* loadingImage = Sprite::create(eng ? "System/todoschool_BI.png" : "System/todoshule_BI.png");
    
    
//    auto levelID = UserManager::getInstance()->getCurrentLevelID();
//    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    
    string loadingFile;

    loadingFile = "System/xprize_image_loading.png";
    
    
    Sprite *loadingImage = Sprite::create(loadingFile);
    
    loadingImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loadingImage->setPosition(winSize/2.f + Size(0, 50));
    addChild(loadingImage);
    
    Size imageSize = loadingImage->getContentSize();
    
    std::string loadingText = LanguageManager::getInstance()->isEnglish() ? "Loading..." : "Tafadhali subiri...";
    
    Label *loadingLabel = Label::createWithTTF(loadingText, "fonts/TodoSchoolV2.ttf", 150);
    loadingLabel->setTextColor(Color4B(255, 240, 222, 255));
    loadingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    loadingLabel->setPosition(Vec2(winSize.width/2, winSize.height/2 - imageSize.height/2 - 50));
    addChild(loadingLabel);
    
    
    return true;
    
}


void TodoLoadingScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    CCLOG("TodoLoadingScene::onEnterTransitionDidFinish");

    
    _transitionFinished = true;
    
    {
        _nextScene = _sceneCreator();
        _nextScene->retain();
        
        _nextSceneLoaded = true;
        
    }
    
//
//    auto seq = Sequence::create(DelayTime::create(_loadingTime), CallFunc::create([this]() {
//        Director::getInstance()->replaceScene(TransitionFade::create(_fadeOutTime, _nextScene));
//        CC_SAFE_RELEASE(_nextScene);
//    }), NULL);
//    
//    runAction(seq);
}

