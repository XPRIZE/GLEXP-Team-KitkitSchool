//
//  TextToNumberStage.h -- See the text, choose the right number
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TextToNumberStage.h"
#include "../Utils/DigitalQuizMainDepot.h"
#include "../ChildNodes/AnswerTextButton.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    float defaultFontSize() { return 90.f; }
    Color3B defaultFontColor() { return Color3B(23, 163, 232); }
    
    RichText* richTextForQuestion(const string& Question) {
        MainDepot Depot;

        // NB(xenosoz, 2016): Normal color and highlighted color.
        auto NC = Depot.defaultFontColor();
        auto HC = Depot.highlightFontColor();
        
        float FontSize = 300.f;
        auto FontName = Depot.defaultFont();
        GLubyte Opacity = 255;
        
        int NextTag = 0;
        auto It = RichText::create();

        // NB(xenosoz, 2016): I choose a string type here.
        //   I just couldn't find to see if stringstream is empty.
        string ResidueText;
        Color3B ResidueColor;
        auto FlushResidue = [&] {
            if (ResidueText.empty()) { return; }
            ResidueText += " ";  // XXX: 

            auto E = RichElementText::create(NextTag++,
                                             ResidueColor, Opacity,
                                             ResidueText, FontName, FontSize);
            It->pushBackElement(E);
            ResidueText.clear();
        };
        
        for (size_t I = 0, E = Question.size(); I < E; ++I) {
            auto Letter = Question[I];
            auto NewColor = (Letter == '_' ? HC : NC);
            
            if (!ResidueText.empty() && ResidueColor != NewColor)
                FlushResidue();

            ResidueText += string{Letter};
            ResidueColor = NewColor;
        }
        FlushResidue();

        return It;
    }
    
}  // unnamed namespace


TextToNumberStage::TextToNumberStage()
{
}

bool TextToNumberStage::init() {
    if (!Super::init()) { return false; }
    return true;
}

void TextToNumberStage::clearInternals() {
    Super::clearInternals();
}

void TextToNumberStage::refreshChildNodes() {
    Super::refreshChildNodes();
    MainDepot Depot;
    
    Size CS = getContentSize();
    
    [&] {
        auto It = richTextForQuestion(TheProblem().Text);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height * 2.f / 3.f));
        addChild(It);
        return It;
    }();
    
    // NB(xenosoz, 2016): Choices are from the excel sheet.
    //   Some people are happy when the base index starts from 1.
    //   And it's the case we should take care of them.

    // NB(xenosoz, 2016): We don't shuffle number choices.
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
}

void TextToNumberStage::onEnter() {
    Super::onEnter();
}

void TextToNumberStage::handleAnswer(const string& Answer) {
    auto Guard = NodeScopeGuard(this);
    
    if (OnAnswer)
        OnAnswer(Answer);
}


END_NS_DIGITALQUIZ
