//
//  KeyNode.cpp on Jul 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "KeyNode.h"
#include "../Utils/StarFallDepot.h"


BEGIN_NS_STARFALL

namespace {
    string normalSkin() {
        return StarFallDepot().assetPrefix() + "/KeyNode/keyboard_default.png";
    }
    string selectedSkin() {
        return StarFallDepot().assetPrefix() + "/KeyNode/keyboard_correct.png";
    }
    string disabledSkin() {
        return StarFallDepot().assetPrefix() + "/KeyNode/keyboard_disabled.png";
    }
    
    string fontFace() { return StarFallDepot().defaultFontFace(); }
    float fontSize() { return 88.f; }

    Color3B regularColor() { return Color3B(74, 70, 67); }
    Color3B disabledColor() { return Color3B(148, 100, 63); }
    Color3B specialColor() { return Color3B(222, 74, 0); }
}  // unnamed namespace


KeyNode::KeyNode()
    : SymbolButton(nullptr)
{
}

bool KeyNode::init() {
    if (!Super::init()) { return false; }

    clearInternals();
    refreshChildNodes();
    return true;
}

Size KeyNode::defaultSize() {
    return Size(196.f, 196.f);
}

void KeyNode::clearInternals() {
    setContentSize(defaultSize());

    // NB(xenosoz, 2016): Set default values.
    Enabled.update(true);
    TouchEnabled.update(true);

    // NB(xenosoz, 2016): Set event handlers.
    Symbol.OnValueUpdate = [this](std::string&) {
        refreshChildNodes();
    };
    Enabled.OnValueUpdate = [this](bool&) {
        if (!SymbolButton) { return; }

        SymbolButton->setEnabled(Enabled());
        updateTitleColor();
    };
}

void KeyNode::refreshChildNodes() {
    removeAllChildren();
    
    SymbolButton = ([&] {
        Button* It = Button::create(normalSkin(), selectedSkin(), disabledSkin());
        It->setContentSize(defaultSize());
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);

        It->setTitleFontName(fontFace());
        It->setTitleFontSize(fontSize());
        It->setTitleText(Symbol());

        It->addTouchEventListener([this](Ref*, Widget::TouchEventType E) {
            if (!Enabled() || !TouchEnabled()) { return; }
            if (E != Widget::TouchEventType::BEGAN) { return; }
            
            switch (E) {
                case Widget::TouchEventType::BEGAN:
                    //btn->setTitleColor(highlightColor);
                    break;
                case Widget::TouchEventType::CANCELED:  /* fallthrough */
                case Widget::TouchEventType::ENDED:
                    //btn->setTitleColor(normalColor);
                    break;
                case Widget::TouchEventType::MOVED:
                    //btn->setTitleColor(btn->isHighlighted() ? highlightColor : normalColor);
                    break;
                default:
                    break;
            }
            
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            if (OnKeyPressed)
                OnKeyPressed(*this);
        });
        
        addChild(It);
        return It;
    }());
}

bool KeyNode::special() const {
    return (Symbol() == "a" ||
            Symbol() == "e" ||
            Symbol() == "i" ||
            Symbol() == "o" ||
            Symbol() == "u");
}

void KeyNode::updateTitleColor() {
    if (!SymbolButton) { return; }

    SymbolButton->setTitleColor([&] {
        if (!Enabled()) { return disabledColor(); }
        if (!special()) { return regularColor(); }
        return specialColor();
    }());
}

END_NS_STARFALL
