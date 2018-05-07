//
//  TodoSchoolBackButton.cpp
//  KitkitSchool
//
//  Created by 강승우 on 6/27/16.
//
//

#include "TodoSchoolBackButton.hpp"
#include "CCAppController.hpp"
#include "Common/Basic/SoundEffect.h"
#include "CompletePopup.hpp"

bool TodoSchoolBackButton::_didFinish = false;

bool TodoSchoolBackButton::init()
{
    if(!ui::Button::init("Common/Controls/back_arrow_inactive.png", "Common/Controls/back_arrow_active.png")) {
        return false;
    }
    
    onBack = nullptr;
    
    return true;
    
}

void TodoSchoolBackButton::onEnter() {
    ui::Button::onEnter();
    TodoSchoolBackButton::_didFinish = false;
    
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(TodoSchoolBackButton::onKeyReleased, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    addTouchEventListener(CC_CALLBACK_2(TodoSchoolBackButton::onTouched, this));
    
}

void TodoSchoolBackButton::popGameScene()
{
    if (_didFinish) return;
    _didFinish = true;
    
    SoundEffect::menuBackEffect().play();
    CCAppController::sharedAppController()->handleGameQuit();
}

void TodoSchoolBackButton::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
    if (!_touchEnabled) return;
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        auto scene = Director::getInstance()->getRunningScene();
        if (scene->getChildByTag(CompletePopup::TAG) == nullptr) {
            if (onBack) {
                onBack();
            }
            
            popGameScene();
        } else {
            CCLOG("Complete Popup is Showing");
        }
    }
    
}


void TodoSchoolBackButton::onTouched(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType != ui::Widget::TouchEventType::ENDED) {
        return;
    }

    if (!_touchEnabled) return;
    
    if (onBack) {
        onBack();
    }
    popGameScene();
}
