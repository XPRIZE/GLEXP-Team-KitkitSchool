//
//  GradeSelector.cpp -- Temporary grade selector for debugging
//  TodoSchool on Oct 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "GradeSelector.hpp"
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoLoadingScene.hpp>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include <Utils/TodoUtil.h>
#include <cocos/ui/UIButton.h>

#include "CCAppController.hpp"

using namespace cocos2d;
using namespace std;
using todoschool::NodeScopeGuard;



namespace {
    const string defaultFont("fonts/TodoSchoolV2.ttf");
    const float defaultFontSize(100.f);

    Size windowSize() {
        return todoschool::DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
    }
    
    Size gameSize() {
        return todoschool::DeviceSpec::Google_Pixel_C().FullScreenResolution;
    }

}  // unnamed namespace


GradeSelector::GradeSelector() {
}

bool GradeSelector::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void GradeSelector::setGameName(std::string gameName) {
    gameName_ = gameName;
}

void GradeSelector::setChoices(const Choices& choices) {
    choices_ = choices;
    refreshChildNodes();
}

//void GradeSelector::setCreator(Creator creator) {
//    creator_ = creator;
//}

cocos2d::Scene* GradeSelector::minimalSceneByWrapping() {
    auto scene = Scene::create();
    
    [&] {
        auto it = Sprite::create("FindTheMatch/Background/picturematching.jpg");
        auto spriteSize = it->getContentSize();
        float scale = max(windowSize().width / spriteSize.width,
                          windowSize().height / spriteSize.height);
        
        it->setScale(scale);
        it->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        it->setPosition(windowSize() / 2.f);
        
        it->setBlendFunc(BlendFunc::DISABLE);
        scene->addChild(it);
        return it;
    }();
    
    auto gameNode = [&] {
        float scale = min(windowSize().width / gameSize().width,
                          windowSize().height / gameSize().height);
        
        auto it = Node::create();
        it->setContentSize(gameSize());
        it->setScale(scale);
        it->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        it->setPosition(windowSize() / 2.f);
        
        scene->addChild(it);
        return it;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, windowSize().height - 25.f));


        
        scene->addChild(It);
        return It;
    }();

    
    [&] {
        auto it = this;
        it->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        it->setPosition(gameSize() / 2.f);
        
        gameNode->addChild(it);
        return it;
    }();
    
    return scene;
}

cocos2d::Scene* GradeSelector::createScene() {
    auto it = GradeSelector::create();
    return it->minimalSceneByWrapping();
}

void GradeSelector::clearInternals() {
    
}

void GradeSelector::refreshChildNodes() {
    removeAllChildren();

    const Size buttonSize(200.f, 200.f);
    const Size buttonMargin(20.f, 20.f);
    size_t rowCount = (choices_.size() + 9) / 10;
    size_t colCount = (rowCount > 1 ? 10 : choices_.size());
    
    setContentSize([&] {
        float width = buttonSize.width * colCount + buttonMargin.width * (colCount + 1);
        float height = buttonSize.height * rowCount + buttonMargin.height * (rowCount + 1);

        height += 200.f;
        return Size(width, height);
    }());
    
    auto CS = getContentSize();

    auto TitleLabel = [&] {
        auto It = Label::createWithTTF("Choose a level to play",
                                       defaultFont, defaultFontSize);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(CS.width / 2.f, CS.height));

        addChild(It);
        return It;
    }();


    for (size_t i = 0, e = choices_.size(); i < e; ++i) {
        auto choice = choices_[i];
        auto it = ui::Button::create();
        size_t row = (i / 10);
        size_t col = (i % 10);

        it->setTitleFontName(defaultFont);
        it->setTitleFontSize(200.f);
        it->setTitleText(TodoUtil::itos((int)choice));
        it->setContentSize(buttonSize);
        it->addClickEventListener([this, choice](Ref*) {
            auto guard = NodeScopeGuard(this);
            handleSelection(choice);
        });

        it->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        it->setPosition([&] {
            float x = buttonSize.width * col + buttonMargin.width * (col + 1);
            float y = (CS.height - TitleLabel->getContentSize().height) - (buttonSize.height * row + buttonMargin.height * (row + 1));
            return Point(x, y);
        }());
        
        addChild(it);
    }
}

void GradeSelector::handleSelection(int choice) {
    CCLOG("gameName %s", gameName_.c_str());
    CCAppController::sharedAppController()->startGame(gameName_, choice);
}
