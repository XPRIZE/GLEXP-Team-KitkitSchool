//
//  SoundButton.cpp -- A button with speaker mark on it
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "SoundButton.h"
#include "../Utils/DigitalQuizMainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string normalSkin(SoundButton::SizeKind SK) {
        string P = MainDepot().assetPrefix();
        switch (SK) {
            case SoundButton::SizeKind::Small:
                return P + "/SoundButton/Digital-Quiz_button_sound_small_normal.png";
            case SoundButton::SizeKind::Large:
                return P + "/SoundButton/Digital-Quiz_button_sound_large_normal.png";
        }
        return "";
    }

    string touchSkin(SoundButton::SizeKind SK) {
        string P = MainDepot().assetPrefix();
        switch (SK) {
            case SoundButton::SizeKind::Small:
                return P + "/SoundButton/Digital-Quiz_button_sound_small_touch.png";
            case SoundButton::SizeKind::Large:
                return P + "/SoundButton/Digital-Quiz_button_sound_large_touch.png";
        }
        return "";
    }
}  // unnamed namespace


SoundButton::SoundButton()
: ButtonNode(nullptr)
{
}

SoundButton::~SoundButton() {
    stop();
}

bool SoundButton::init() {
    return init(SizeKind::Large);
}

bool SoundButton::init(SizeKind SK) {
    if (!Super::init()) { return false; }

    TheSizeKind = SK;

    clearInternals();
    refreshChildNodes();
    
    return true;
}

void SoundButton::play() {
    SoundToPlay.stop();
    SoundToPlay.play();
}

void SoundButton::stop() {
    SoundToPlay.stop();
}

void SoundButton::clearInternals() {
    setCascadeOpacityEnabled(true);

    TouchEnabled.OnValueUpdate = [this](bool&) {
        if (ButtonNode)
            ButtonNode->setEnabled(TouchEnabled());
    };
    TouchEnabled.update(true);
}

void SoundButton::refreshChildNodes() {
    removeAllChildren();
    
    ButtonNode = [&] {
        auto It = Button::create(normalSkin(TheSizeKind), touchSkin(TheSizeKind));
        auto CS = It->getContentSize();

        It->addClickEventListener([this](Ref*) {
            NodeScopeGuard Guard(this);
            play();
            
            if (OnClicked)
                OnClicked();
        });

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        setContentSize(CS);
        addChild(It);
        return It;
    }();
    
    //Size CS = getContentSize();
}

END_NS_DIGITALQUIZ
