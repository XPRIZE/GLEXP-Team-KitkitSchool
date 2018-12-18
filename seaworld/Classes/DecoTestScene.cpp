//
//  DecoTestScene.cpp
//  KitKit_FB
//
//  Created by jesun kim on 12/09/2018.
//

#include "DecoTestScene.hpp"

#include "MainScene.hpp"

Scene* DecoTestScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DecoTestScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool DecoTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Node *node = static_cast<Node *>(CSLoader::createNode("images/DecoTestNode.csb"));
    node->setPosition(visibleSize/2);
    this->addChild(node);
    
    for(int i=0 ; i<node->getChildrenCount() ; i++){
        Node *child = node->getChildren().at(i);
        if(strcmp(child->getName().c_str(), "ignore") == 0){
            continue;
        }
        ActionTimeline *action = (ActionTimeline *)child->getActionByTag(child->getTag());
        action->play("idle", true);
        
        Button *button = static_cast<Button *>(child->getChildByName("button"));
        button->addTouchEventListener(CC_CALLBACK_2(DecoTestScene::select, this));
        
    }
    
    Button *buttonBack = Button::create("images/back_arrow_inactive.png", "images/back_arrow_active.png");
    buttonBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    buttonBack->setPosition(Vec2(25, visibleSize.height-25));
    buttonBack->addClickEventListener([this](Ref*){
        Director::getInstance()->replaceScene(MainScene::createScene());
    });
    this->addChild(buttonBack);
    
    EventListenerKeyboard *touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
        {
            Director::getInstance()->replaceScene(MainScene::createScene());
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    return true;
}

void DecoTestScene::select(Ref *pSender, Widget::TouchEventType type)
{
    Button *button = static_cast<Button *>(pSender);
    Node *node = button->getParent();
    ActionTimeline *action = (ActionTimeline *)node->getActionByTag(node->getTag());
    switch (type) {
        case Widget::TouchEventType::CANCELED:
        case Widget::TouchEventType::BEGAN:
        case Widget::TouchEventType::MOVED:
            break;
        case Widget::TouchEventType::ENDED:
            action->play("move", false);
            action->setLastFrameCallFunc([=](){
                action->play("idle", true);
            });
            break;
    }
}

