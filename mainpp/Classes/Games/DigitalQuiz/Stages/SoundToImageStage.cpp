//
//  SoundToImageStage.h -- Listen to the sound, choose the right image
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SoundToImageStage.h"
#include "../Utils/MainDepot.h"
#include "../ChildNodes/LargeImageButton.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    float defaultFontSize() { return 90.f; }
    Color3B defaultFontColor() { return Color3B(23, 163, 232); }
}  // unnamed namespace


SoundToImageStage::SoundToImageStage()
: TheSoundButton(nullptr)
{
}

bool SoundToImageStage::init() {
    if (!Super::init()) { return false; }
    return true;
}

void SoundToImageStage::clearInternals() {
    Super::clearInternals();
}

void SoundToImageStage::refreshChildNodes() {
    Super::refreshChildNodes();
    MainDepot Depot;
    
    Size CS = getContentSize();

    // NB(xenosoz, 2016): We shuffle the image choices.
    OneBasedVector<string> Choices = TheProblem().Choices;
    shuffle(Choices);

    for (size_t I = Choices.begin_index(), E = Choices.end_index(); I < E; ++I) {
        // NB(xenosoz, 2016): The problem is from the excel sheet.
        //   Some people are happy when the base index starts from 1.
        //   And it's the case we should take care of them.

        auto& ImageName = Choices[I];
        int ButtonID = (int)(I - Choices.begin_index());
        int ButtonCount = (int)Choices.size();

        auto CS = getContentSize();
        auto It = LargeImageButton::create();
        It->ImageName.update(ImageName);
        It->setOpacity(255 * .9f);
        
        Size ButtonMargin(104.f, 0.f);
        float FullWidth = (ButtonCount * It->getContentSize().width +
                           (ButtonCount - 1) * ButtonMargin.width);
        float FullMarginLeft = (CS.width - FullWidth) / 2.f;
        float X = FullMarginLeft + (It->getContentSize().width + ButtonMargin.width) * ButtonID;
        float Y = 113.f;
        
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(X, Y));
        
        It->OnAnswer = [this](const string& Answer) {
            handleAnswer(Answer);
        };


        // NB(xenosoz, 2016): Hide -> show
        It->TouchEnabled.update(false);
        It->setOpacity(Depot.opacityForDisabledButton());

        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(TheProblem().OpeningDuration));
        Actions.pushBack(CallFunc::create([It] { It->TouchEnabled.update(true); }));
        Actions.pushBack(FadeIn::create(Depot.fadeInDurationForQuizButton()));
        It->stopAllActions();
        It->runAction(Sequence::create(Actions));
        
        addChild(It);
    }

    TheSoundButton = [&] {
        // NB(xenosoz, 2016): The sound button
        auto It = SoundButton::create(SoundButton::SizeKind::Large);

        if (!TheProblem().ButtonSound.empty() && TheProblem().ButtonSound != "__NO_SOUND__") {
            auto SoundPath = Depot.assetPrefix() + "/QuizSounds/" + TheProblem().ButtonSound;
            It->SoundToPlay = SoundEffect(SoundPath).preloaded();
        }
        It->OnClicked = [this] {
            OpeningSound.stop();
        };

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 2.f / 3.f));

        addChild(It);
        return It;
    }();
}

void SoundToImageStage::onEnter() {
    Super::onEnter();
}

void SoundToImageStage::handleAnswer(const string& Answer) {
    auto Guard = NodeScopeGuard(this);

    if (OnAnswer)
        OnAnswer(Answer);
}


END_NS_DIGITALQUIZ
