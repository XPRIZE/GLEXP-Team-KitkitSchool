//
//  LargeImageButton.cpp -- A large button with an image on it
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "LargeImageButton.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string contentSkin(const string& ImageName) {
        return MainDepot().assetPrefix() + "/QuizImages/" + ImageName;
    }

    string normalSkin() {
        return (MainDepot().assetPrefix() + "/AnswerButton/" +
                "Digital-Quiz_button_answer_large_normal.png");
    }

    string touchSkin() {
        return (MainDepot().assetPrefix() + "/AnswerButton/" +
                "Digital-Quiz_button_answer_large_touch.png");
    }
}  // unnamed namespace


LargeImageButton::LargeImageButton()
: ButtonNode(nullptr)
, ImageNode(nullptr)
{
}

bool LargeImageButton::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void LargeImageButton::clearInternals() {
    setCascadeOpacityEnabled(true);

    ImageName.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };

    TouchEnabled.OnValueUpdate = [this](bool&) {
        if (ButtonNode)
            ButtonNode->setEnabled(TouchEnabled());
    };
    TouchEnabled.update(true);
}

void LargeImageButton::refreshChildNodes() {
    removeAllChildren();
    
    ButtonNode = [&] {
        auto It = Button::create(normalSkin(), touchSkin());
        auto CS = It->getContentSize();
        
        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
           
            if (OnAnswer)
                OnAnswer(ImageName());
        });
        
        It->addTouchEventListener([this](Ref*, Widget::TouchEventType E) {
            switch (E) {
                case Widget::TouchEventType::BEGAN:
                case Widget::TouchEventType::MOVED: /* Fallthrough */
                    handleButtonDown();
                    break;
                case Widget::TouchEventType::CANCELED: /* Fallthrough */
                case Widget::TouchEventType::ENDED:
                    handleButtonUp();
                    break;
            }
        });
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        setContentSize(CS);
        addChild(It);
        return It;
    }();

    // NB(xenosoz, 2016): The size was determined by the previous child node.
    auto CS = getContentSize();
    
    ImageNode = [&] {
        // NB(xenosoz, 2016): Put the sprite on the button image.
        auto It = Sprite::create(contentSkin(ImageName()));
        if (!It) { return (It = nullptr); }
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f));
        addChild(It);
        return It;
    }();
}

void LargeImageButton::handleButtonDown() {
    if (!ImageNode) { return; }
    auto CS = getContentSize();

    ImageNode->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f - 10.f));
}

void LargeImageButton::handleButtonUp() {
    if (!ImageNode) { return; }
    auto CS = getContentSize();

    ImageNode->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f));
}

END_NS_DIGITALQUIZ
