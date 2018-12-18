//
//  EraseDataConfirmPopup.cpp -- Let the user decide erase/resume the app
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "EraseDataConfirmPopup.h"
#include "../Utils/DigitalQuizMainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string contentSkin() {
        return MainDepot().assetPrefix() + "/Background/digitalquiz_background.png";
    }
}  // unnamed namespace


EraseDataConfirmPopup::EraseDataConfirmPopup() {
}

bool EraseDataConfirmPopup::init() {
    if (!Super::init()) { return false; }
    
    setContentSize(MainDepot().gameSize());
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void EraseDataConfirmPopup::clearInternals() {
    setCascadeOpacityEnabled(true);

    Classroom.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };

    StudentNumber.OnValueUpdate = [this](int&) {
        refreshChildNodes();
    };

    ProblemName.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };

    Finished.OnValueUpdate = [this](bool&) {
        refreshChildNodes();
    };
}

void EraseDataConfirmPopup::refreshChildNodes() {
    removeAllChildren();
    MainDepot Depot;
    
    auto CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): The background node.
        auto BN = Node::create();
        BN->setContentSize(CS);
        
        if (true) {
            auto It = Sprite::create(contentSkin());
            Size SpriteSize = It->getContentSize();
            float Scale = Depot.scaleToCover(CS, SpriteSize);
            
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(CS / 2.f);
            
            It->setScale(Scale);
            It->setBlendFunc(BlendFunc::DISABLE);
            BN->addChild(It);
        }
        
        addChild(BN);
    }();
    
    [&]{
        stringstream Text;
        Text << "Warning: Data exists\n";
        Text << "\n";
        Text << "Class " << Classroom() << ", ";
        Text << "Number " << StudentNumber() << "\n";
        
        if (Finished())
            Text << "Finished!";
        else
            Text << ProblemName();

        // NB(xenosoz, 2016): Status Label
        auto It = Label::createWithTTF(Text.str(),
                                       Depot.defaultFont(),
                                       Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 4 / 6));

        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): Erase and restart
        if (Finished()) { return (Button*)nullptr; }
        auto It = Button::create();

        It->setTitleFontName(Depot.defaultFont());
        It->setTitleFontSize(Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());
        It->setTitleText("[Erase and restart]");

        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            if (OnEraseAndRestart)
                OnEraseAndRestart();
        });

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 2.5 / 6));

        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): Resume
        if (Finished()) { return (Button*)nullptr; }
        auto It = Button::create();

        It->setTitleFontName(Depot.defaultFont());
        It->setTitleFontSize(Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());
        It->setTitleText("[Resume]");

        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            if (OnResume)
                OnResume();
        });

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 2 / 6));

        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): Cancel
        auto It = Button::create();

        It->setTitleFontName(Depot.defaultFont());
        It->setTitleFontSize(Depot.defaultFontSize() / 2.f);
        It->setColor(Depot.defaultFontColor());
        It->setTitleText("[Cancel]");

        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            if (OnCancel)
                OnCancel();
        });
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 1 / 6));
        
        addChild(It);
        return It;
    }();
}

END_NS_DIGITALQUIZ
