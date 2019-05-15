//
//  SoundToNumberStage.cpp -- Listen to the sound, choose the right number
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SoundToNumberStage.h"
#include "../Utils/DigitalQuizMainDepot.h"
#include "../ChildNodes/AnswerTextButton.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    float defaultFontSize() { return 90.f; }
    Color3B defaultFontColor() { return Color3B(23, 163, 232); }
}  // unnamed namespace


SoundToNumberStage::SoundToNumberStage()
: TheSoundButton(nullptr)
{
}

bool SoundToNumberStage::init() {
    if (!Super::init()) { return false; }
    return true;
}

void SoundToNumberStage::clearInternals() {
    Super::clearInternals();
}

void SoundToNumberStage::refreshChildNodes() {
    Super::refreshChildNodes();
    MainDepot Depot;
    
    Size CS = getContentSize();
    
    
    // NB(xenosoz, 2016): Choices are from the excel sheet.
    //   Some people are happy when the base index starts from 1.
    //   And it's the case we should take care of them.

    // NB(xenosoz, 2016): We don't shuffle the number choices.
    OneBasedVector<string> Choices = TheProblem().Choices;

    if (Choices.empty()) {
        // NB(xenosoz, 2016): The default setting is [0, 10).
        for (int I = 0; I < 10; ++I)
            Choices.push_back(itos(I));
    }

    int ButtonCount = (int)Choices.size();
    for (size_t I = Choices.begin_index(), E = Choices.end_index(); I < E; ++I)
    {
        int ButtonID = (int)(I - Choices.begin_index());
        auto& Text = Choices[I];

        AnswerTextButton::SizeKind SK;
        Size ButtonMargin;

        if (ButtonCount <= 3) {
            // NB(xenosoz, 2016): Small population -> large size button.
            SK = AnswerTextButton::SizeKind::Large;
            ButtonMargin = Size(104.f, 0.f);
        }
        else if (ButtonCount <= 5) {
            SK = AnswerTextButton::SizeKind::Medium;
            ButtonMargin = Size(70.f, 0.f);
        }
        else {
            SK = AnswerTextButton::SizeKind::Small;
            ButtonMargin = Size(40.f, 0.f);
        }
        
        auto It = AnswerTextButton::create(SK);
        It->Text.update(Text);
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


        float FullWidth = (ButtonCount * It->getContentSize().width +
                           (ButtonCount - 1) * ButtonMargin.width);
        float FullMarginLeft = (CS.width - FullWidth) / 2.f;
        float X = FullMarginLeft + (It->getContentSize().width + ButtonMargin.width) * ButtonID;
        float Y = 113.f;
        
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(X, Y));

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

void SoundToNumberStage::onEnter() {
    Super::onEnter();
}

void SoundToNumberStage::handleAnswer(const string& Answer) {
    auto Guard = NodeScopeGuard(this);
    
    if (OnAnswer)
        OnAnswer(Answer);
}


END_NS_DIGITALQUIZ
