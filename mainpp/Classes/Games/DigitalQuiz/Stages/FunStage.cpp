//
//  FunStage.cpp -- A refresher for children
//  TodoSchool on Nov 3, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FunStage.h"
#include "../Utils/MainDepot.h"
#include "../ChildNodes/AnswerTextButton.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    float defaultFontSize() { return 90.f; }
    Color3B defaultFontColor() { return Color3B(23, 163, 232); }
}  // unnamed namespace


FunStage::FunStage()
{
}

bool FunStage::init() {
    if (!Super::init()) { return false; }
    return true;
}

void FunStage::clearInternals() {
    Super::clearInternals();
}

void FunStage::refreshChildNodes() {
    Super::refreshChildNodes();
    MainDepot Depot;
    
    Size CS = getContentSize();

    [&]() -> Sprite* {
        if (TheProblem().Choices.size() <= 0) { return nullptr; }

        // NB(xenosoz, 2016): Choices are from the excel sheet.
        //   Some people are happy when the base index starts from 1.
        //   And it's the case we should take care of them.
        OneBasedVector<string>& Choices = TheProblem().Choices;
        auto ImageFilename = Depot.assetPrefix() + "/QuizImages/" + Choices[1];

        auto It = Sprite::create(ImageFilename);
        It->setScale(1.20f);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 3.f / 5.f));

        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The next button.
        auto It = AnswerTextButton::create(AnswerTextButton::SizeKind::Large);
        It->Text.update(">>");

        It->OnAnswer = [this](const string& _Answer) {
            // NB(xenosoz, 2016): Override the answer.
            string Answer = TheProblem().Answer;
            handleAnswer(Answer);
        };

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(CS.width / 2.f, 113.f));
        addChild(It);
        return It;
    }();
    
    if (!TheProblem().ButtonSound.empty() && TheProblem().ButtonSound != "__NO_SOUND__") {
        auto SoundPath = Depot.assetPrefix() + "/QuizSounds/" + TheProblem().ButtonSound;
        SoundToPlay = SoundEffect(SoundPath).preloaded();
    }
}

void FunStage::onEnter() {
    Super::onEnter();
}

void FunStage::handleAnswer(const string& Answer) {
    auto Guard = NodeScopeGuard(this);
    
    if (OnAnswer)
        OnAnswer(Answer);
}


END_NS_DIGITALQUIZ
