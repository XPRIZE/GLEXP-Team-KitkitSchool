//
//  TrailerNode.cpp
//  TodoSchool on Jul 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TrailerNode.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_WORDTRACE

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


bool TrailerNode::init() {
    if (!Node::init()) { return false; }
    
    NextLabel = nullptr;
    NextButtonA = nullptr;
    NextButtonB = nullptr;
    NextButtonLabelA = nullptr;
    NextButtonLabelB = nullptr;
    
    clear();
    return true;
}

void TrailerNode::clear() {
    clearInternals();
    
    refreshNextLabel();
    refreshNextButton();
}

void TrailerNode::clearInternals() {
    NextTraceWorkA.OnValueUpdate =
    NextTraceWorkB.OnValueUpdate = [this](Problem&) {
        refreshNextButton();
    };
}

void TrailerNode::refreshNextLabel() {
    MainDepot Depot;
    Size GameSize = Depot.gameSize();
    
    if (NextLabel)
        NextLabel->removeFromParent();
    
    NextLabel = ([&] {
        Label* It = Depot.createNextLabel();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width/2.f, GameSize.height - 258.f));
        
        addChild(It);
        return It;
    }());
}

void TrailerNode::refreshNextButton() {
    MainDepot Depot;
    Size GameSize = Depot.gameSize();
    
    mbClicked = false;
    if (NextButtonA)
        NextButtonA->removeFromParent();

    NextButtonA = ([&] {
        Button* It = Depot.createNextButton(NextTraceWorkA().TeaserFileName);
        Size CS = It->getContentSize();
        
        // NB(xenosoz, 2016):
        // [---------------------------GameSizeX--------------------------]
        // [--BlankX--] [**Button**] [--BlankX--] [--Button--] [--BlankX--]
        
        float BlankWidth = (GameSize.width - CS.width * 2) / 3.f;
        float X = BlankWidth * 1 + CS.width / 2.f;
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(X, GameSize.height - 684.f));
        It->addClickEventListener([this](Ref*) {
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });

            if (mbClicked == true)
                return;
            
            if (OnNextButtonClicked)
                OnNextButtonClicked(*this, NextTraceWorkA());
            
            mbClicked = true;
        });
        
        addChild(It);
        return It;
    }());


    if (NextButtonB)
        NextButtonB->removeFromParent();
    
    NextButtonB = ([&] {
        Button* It = Depot.createNextButton(NextTraceWorkB().TeaserFileName);
        Size CS = It->getContentSize();
        
        // NB(xenosoz, 2016):
        // [---------------------------GameSizeX--------------------------]
        // [--BlankX--] [--Button--] [--BlankX--] [**Button**] [--BlankX--]
        
        float BlankWidth = (GameSize.width - CS.width * 2) / 3.f;
        float X = GameSize.width - BlankWidth - CS.width / 2.f;

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(X, GameSize.height - 684.f));
        It->addClickEventListener([this](Ref*) {
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            if (mbClicked == true)
                return;

            if (OnNextButtonClicked)
                OnNextButtonClicked(*this, NextTraceWorkB());
            
            mbClicked = true;
        });
        
        addChild(It);
        return It;
    }());
    

    if (NextButtonLabelA)
        NextButtonLabelA->removeFromParent();
    
    NextButtonLabelA = ([&] {
        Point BP = NextButtonA->getPosition();
        Size BS = NextButtonA->getContentSize();

        Label* It = Depot.createNextButtonLabel(NextTraceWorkA().Text);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(BP.x, BP.y - BS.height - 10.f));

        addChild(It);
        return It;
    }());


    if (NextButtonLabelB)
        NextButtonLabelB->removeFromParent();
    
    NextButtonLabelB = ([&] {
        Point BP = NextButtonB->getPosition();
        Size BS = NextButtonB->getContentSize();
        
        Label* It = Depot.createNextButtonLabel(NextTraceWorkB().Text);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(BP.x, BP.y - BS.height - 10.f));
        
        addChild(It);
        return It;
    }());
}

END_NS_WORDTRACE
