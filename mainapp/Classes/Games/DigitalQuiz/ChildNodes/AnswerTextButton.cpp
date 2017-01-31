//
//  AnswerTextButton.cpp -- A large button with a text on it
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "AnswerTextButton.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string contentSkin(const string& ImageName) {
        return MainDepot().assetPrefix() + "/QuizImages/" + ImageName;
    }
    
    string normalSkin(AnswerTextButton::SizeKind SK) {
        string P = MainDepot().assetPrefix() + "/AnswerButton/";
        
        switch (SK) {
            case AnswerTextButton::SizeKind::Small:
                P += "Digital-Quiz_button_answer_small_normal.png";
                break;
            case AnswerTextButton::SizeKind::Medium:
                P += "Digital-Quiz_button_answer_medium_normal.png";
                break;
            case AnswerTextButton::SizeKind::Large:
                P += "Digital-Quiz_button_answer_large_normal.png";
                break;
        }
        return P;
    }
    
    string touchSkin(AnswerTextButton::SizeKind SK) {
        string P = MainDepot().assetPrefix() + "/AnswerButton/";
        
        switch (SK) {
            case AnswerTextButton::SizeKind::Small:
                P += "Digital-Quiz_button_answer_small_touch.png";
                break;
            case AnswerTextButton::SizeKind::Medium:
                P += "Digital-Quiz_button_answer_medium_touch.png";
                break;
            case AnswerTextButton::SizeKind::Large:
                P += "Digital-Quiz_button_answer_large_touch.png";
                break;
        }
        return P;
    }
}  // unnamed namespace


AnswerTextButton::AnswerTextButton()
: ButtonNode(nullptr)
, TextNode(nullptr)
{
}

bool AnswerTextButton::init(/* SizeKind SK = Large */) {
    return init(SizeKind::Large);
}

bool AnswerTextButton::init(SizeKind SK) {
    if (!Super::init()) { return false; }

    TheSizeKind = SK;

    clearInternals();
    refreshChildNodes();
    
    return true;
}

void AnswerTextButton::clearInternals() {
    setCascadeOpacityEnabled(true);
    
    Text.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };

    TouchEnabled.OnValueUpdate = [this](bool&) {
        if (ButtonNode)
            ButtonNode->setEnabled(TouchEnabled());
    };
    TouchEnabled.update(true);
}

void AnswerTextButton::refreshChildNodes() {
    removeAllChildren();
    MainDepot Depot;
    
    ButtonNode = [&] {
        auto It = Button::create(normalSkin(TheSizeKind), touchSkin(TheSizeKind));
        auto CS = It->getContentSize();
        
        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            
            if (OnAnswer)
                OnAnswer(Text());
        });
        
        It->addTouchEventListener([this, It, CS](Ref*, Widget::TouchEventType E) {
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
    
    TextNode = [&] {
        // NB(xenosoz, 2016): Put the text on the button image.
        auto It = BigLabel::createWithTTF(Text(),
                                          Depot.defaultFont(), 180.f);
        It->setColor(Depot.defaultFontColor());
        It->setOpacity(255 * 1.00f);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f));
        addChild(It);
        return It;
    }();
}

void AnswerTextButton::handleButtonDown() {
    if (!TextNode) { return; }
    auto CS = getContentSize();

    TextNode->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f - 10.f));
    TextNode->setColor(Color3B(204, 235, 255));
    TextNode->setOpacity(255 * 0.50f);
}

void AnswerTextButton::handleButtonUp() {
    if (!TextNode) { return; }
    auto CS = getContentSize();

    TextNode->setPosition(Point(CS.width / 2.f, CS.height / 2.f + 5.f));
    TextNode->setColor(MainDepot().defaultFontColor());
    TextNode->setOpacity(255 * 1.00f);
}

END_NS_DIGITALQUIZ
