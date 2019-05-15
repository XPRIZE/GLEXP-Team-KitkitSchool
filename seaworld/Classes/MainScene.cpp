//
//  MainScene.cpp
//  KitKit_FB
//
//  Created by jesun kim on 12/09/2018.
//

#include "MainScene.hpp"

#include "Games/FishBowl/FishBowlScene.hpp"
#include "DecoTestScene.hpp"

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Node *node = static_cast<Node *>(CSLoader::createNode("images/MainNode.csb"));
    node->setPosition(visibleSize/2);
    this->addChild(node);
    
    Button *buttonNormal = static_cast<Button *>(node->getChildByName("buttonNormal"));
    buttonNormal->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(FishBowlScene::createScene(0));
    });
    
    Button *buttonWhole = static_cast<Button *>(node->getChildByName("buttonWhole"));
    buttonWhole->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(FishBowlScene::createScene(1));
    });
    
    Button *button6 = static_cast<Button *>(node->getChildByName("button6"));
    button6->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(FishBowlScene::createScene(6));
    });
    
    Button *button7 = static_cast<Button *>(node->getChildByName("button7"));
    button7->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(FishBowlScene::createScene(7));
    });

    Button *buttonDeco = static_cast<Button *>(node->getChildByName("buttonDeco"));
    buttonDeco->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(DecoTestScene::createScene());
    });
    
    Button *buttonReset = static_cast<Button *>(node->getChildByName("buttonReset"));
    buttonReset->addClickEventListener([this](Ref*){
        for(int i=0 ; i<10000 ; i++){
            UserDefault::getInstance()->deleteValueForKey(StringUtils::format("%d", i).c_str());
        }
    });
    
    return true;
}
