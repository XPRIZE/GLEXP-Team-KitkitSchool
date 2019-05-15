//
//  TouchEventLogger.cpp on Apr 3, 2018
//  TodoSchool
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "TouchEventLogger.h"
#include <Managers/StrictLogManager.h>

using namespace cocos2d;
using namespace std;


void TouchEventLogger::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();

    if (_listener) {
        this->getEventDispatcher()->removeEventListener(_listener);
        _listener = nullptr;
    }
    
    _listener = [&] {
        auto it = EventListenerTouchOneByOne::create();
        it->setSwallowTouches(false);
        
        it->onTouchBegan = [this](Touch* T, Event* E) -> bool {
            auto P = getParent();
            if (!P) { return false; }

            auto pos = P->convertToNodeSpace(T->getLocation());
            StrictLogManager::shared()->touchEvent_Begin(_parentName, pos.x, pos.y);
            return true;
        };
        it->onTouchMoved = [this](Touch* T, Event* E) {
            auto P = getParent();
            if (!P) { return; }
            
            auto pos = P->convertToNodeSpace(T->getLocation());
            StrictLogManager::shared()->touchEvent_Move(_parentName, pos.x, pos.y);
        };
        it->onTouchEnded = [this](Touch* T, Event* E) {
            auto P = getParent();
            if (!P) { return; }
            
            auto pos = P->convertToNodeSpace(T->getLocation());
            StrictLogManager::shared()->touchEvent_End(_parentName, pos.x, pos.y);
        };
        it->onTouchCancelled = [this](Touch* T, Event* E) {
            auto P = getParent();
            if (!P) { return; }
            
            auto pos = P->convertToNodeSpace(T->getLocation());
            StrictLogManager::shared()->touchEvent_Cancel(_parentName, pos.x, pos.y);
        };

        return it;
    }();
    
    this->getEventDispatcher()->addEventListenerWithFixedPriority(_listener, -10);
}

void TouchEventLogger::onExitTransitionDidStart()
{
    Super::onExitTransitionDidStart();
    this->getEventDispatcher()->removeEventListener(_listener);
    _listener = nullptr;
}

TouchEventLogger::TouchEventLogger(std::string parentName) :
_parentName(parentName),
_listener(nullptr)
{
}

TouchEventLogger* TouchEventLogger::create(std::string parentName) {
    auto* it = new(std::nothrow) TouchEventLogger(parentName);
    if (it && it->init()) {
        return it;
    }
    
    delete it;
    return nullptr;
}

Scene* TouchEventLogger::wrapScene(Scene* scene) {
    if (!scene) { return nullptr; }
    
    auto it = Scene::createWithSize(scene->getContentSize());
    it->setName(scene->getName());

    it->addChild(scene);
    it->addChild(TouchEventLogger::create(scene->getName()));
    return it;
}

//EOF
