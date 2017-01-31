//
//  SplashScene.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/28/16.
//
//

#include "SplashScene.hpp"
#include "Menu/MainScene.hpp"
#include "Menu/MainScene2.hpp"

using namespace cocos2d;

cocos2d::Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool SplashScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto sprite = Sprite::create("System/enuma.png");
    auto spriteSize = sprite->getContentSize();
    auto scale = MAX(visibleSize.width/spriteSize.width, visibleSize.height/spriteSize.height);
    sprite->setScale(scale);
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 ));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void SplashScene::onEnter() {
    Layer::onEnter();
    
    // Wait for 0.5 seconds to load main scene
    this->scheduleOnce(schedule_selector(SplashScene::finishSplash), 0.2f);
}

void SplashScene::finishSplash(float dt) {
    // ... do whatever other initializations here
    // Show the actual main scene

    
    Director::getInstance()->replaceScene(MainScene::createScene());
}
