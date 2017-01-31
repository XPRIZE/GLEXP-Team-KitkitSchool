//
//  TodoLoadingScene.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/28/16.
//
//

#include "TodoLoadingScene.hpp"

#include "MainScene.h"



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
    
    if (_transitionFinished && !_nextSceneLoading && !_nextSceneLoaded) {
        _nextSceneLoading = true;
        _nextScene = _sceneCreator();
        _nextScene->retain();
        
        _nextSceneLoaded = true;
        
    }
    
    
    
    if (_nextSceneLoaded && _transitionFinished && _currentTimer>_loadingTime) {
     
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
    
    if (!Layer::init()) {
        return false;
    }
    
//    _nextScene = nextScene;
//    _nextScene->retain();
    
    _sceneCreator = creator;
    _nextSceneLoaded = false;
    _nextSceneLoading = false;
    _transitionFinished = false;
    
//    auto t = std::thread([this, creator]()

//    );
//    t.detach();
    
    
    _currentTimer = 0.0;
    scheduleUpdate();
    
    _loadingTime = loadingTime;
    _fadeOutTime = fadeOutTime;
    
    
    CCLOG("showLoading...");
    
    auto winSize = Director::getInstance()->getVisibleSize();

    Sprite *loadingImage = Sprite::create("Common/xprize_image_loading.png");
    loadingImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loadingImage->setPosition(winSize/2.f);
    addChild(loadingImage);
    
    Size imageSize = loadingImage->getContentSize();
    

    
    std::string loadingText = IS_ENGLISH ? "Loading..." : "Tafadhali subiri...";
    
    Label *loadingLabel = Label::createWithTTF(loadingText, "fonts/TodoSchoolV2.ttf", 56);
    loadingLabel->setTextColor(Color4B(255, 240, 222, 255));
    loadingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    loadingLabel->setPosition(Vec2(winSize.width/2, winSize.height/2 - imageSize.height/2 - 50));
    addChild(loadingLabel);
    
    
    return true;
    
}


void TodoLoadingScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    _transitionFinished = true;
    _nextSceneLoading = false;
    

//
//    auto seq = Sequence::create(DelayTime::create(_loadingTime), CallFunc::create([this]() {
//        Director::getInstance()->replaceScene(TransitionFade::create(_fadeOutTime, _nextScene));
//        CC_SAFE_RELEASE(_nextScene);
//    }), NULL);
//    
//    runAction(seq);
}

