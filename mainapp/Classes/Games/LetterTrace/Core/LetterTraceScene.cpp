//
//  LetterTraceScene.cpp
//  TodoSchool on May 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "LetterTraceScene.h"
#include "Managers/LanguageManager.hpp"
#include "../Utils/LetterTraceMainDepot.h"
#include "Common/Controls/BonusVideoPlayer.h"
#include "Common/Components/TargetDragBody.h"
#include <Managers/StrictLogManager.h>
#include "Common/Controls/SignLanguageVideoPlayer.hpp"

#include "CCAppController.hpp"

BEGIN_NS_LETTERTRACE

using namespace todoschool::tracefield;
using namespace cocos2d;
using namespace std;



namespace
{
    bool showDebugLines() {
        // NB(xenosoz, 2016): It's dangerous to go alone! Take this.
        return false;
    }

    Vec2 traceFieldOffset() { return Vec2(0.f, 900.f - (1535.f + 534.f) / 2.f); }
    Point topLineP() {
        return Point(MainDepot().gameSize().width/2.f, 1535.f) + traceFieldOffset();
    }
    Point midLineP() {
        return Point(MainDepot().gameSize().width/2.f, 1200.f) + traceFieldOffset();
    }
    Point botLineP() {
        return Point(MainDepot().gameSize().width/2.f, 534.f) + traceFieldOffset();
    }
    
    string solidLineSkin() {
        return MainDepot().assetPrefix() + "/lettertracing_topbottom_line.png";
    }
    string dashedLineSkin() {
        return MainDepot().assetPrefix() + "/lettertracing_middle_line.png";
    }
    string solidDebugSkin() {
        return MainDepot().assetPrefix() + "/lettertracing_topbottom_line_debug.png";
    }
    string dashedDebugSkin() {
        return MainDepot().assetPrefix() + "/lettertracing_middle_line_debug.png";
    }
}  // unnamed namespace


LetterTraceScene::LetterTraceScene()
: TopGuide(nullptr)
, MidGuide(nullptr)
, BotGuide(nullptr)
, Background(nullptr)
, TheProgressBar(nullptr)
, TheGameNode(nullptr)
, TheTraceField(nullptr)
, TheBonusNode(nullptr)
, TheTrailerNode(nullptr)
{
}

bool LetterTraceScene::init() {
    if (!Super::init()) { return false; }

    TraceWorkMaxIndex.update(0);

    MainDepot().preloadSoundEffects();
    clearInternals();
    refreshChildNodes();

    return true;
}

void LetterTraceScene::clearInternals() {
    using std::placeholders::_1;
    using This = LetterTraceScene;
    
    TraceWorkMaxIndex.OnValueUpdate = bind(&This::handleTraceWorkMaxIndexValueUpdate, this, _1);
    TraceWorkIndex.OnValueUpdate = bind(&This::handleTraceWorkIndexValueUpdate, this, _1);
    TheTraceWork.OnValueUpdate = bind(&This::handleTheTraceWorkValueUpdate, this, _1);
    NextTraceWork.OnValueUpdate = bind(&This::handleNextTraceWorkValueUpdate, this, _1);
}

void LetterTraceScene::refreshChildNodes() {
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    removeAllChildren();
    
    // NB(xenosoz, 2016): Direct descendants of this.
    
    Background = ([&] {
        Sprite* It = Depot.createBackgroundSprite();
        addChild(It);
        return It;
    }());
    
    TheGameNode = ([&] {
        Node* It = Depot.createGameNode();
        addChild(It);
        return It;
    }());
    
    TheProgressBar = ([&] {
        ProgressIndicator* It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));
        
        It->setMax((int)TraceWorkMaxIndex());
        
        addChild(It);
        return It;
    }());
    
    [&] {
        auto It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            // NB(xenosoz, 2016): Since releasing VideoPlayer takes time, just hide it first.
            TheBonusNode->setVisible(false);
            handleFail();
        });
        
        addChild(It);
        return It;
    }();
    
    
    // NB(xenosoz, 2016): Descendants of the game node.
    
    using std::placeholders::_1;


    // NB(xenosoz, 2016): Guide posistion settings are very sensitive.
    //   Font face, font size, stroke thickness ...

    TopGuide = [&] {
        auto It = Sprite::create(solidLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(topLineP());
        TheGameNode->addChild(It);
        return It;
    }();
    
    MidGuide = [&] {
        auto It = Sprite::create(dashedLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(midLineP());
        TheGameNode->addChild(It);
        return It;
    }();
    
    BotGuide = [&] {
        auto It = Sprite::create(solidLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(botLineP());
        TheGameNode->addChild(It);
        return It;
    }();


    TheTraceField = ([&] {
        TraceField* It = TraceField::create();
        It->setContentSize(GameSize * .85f);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize.width / 2.f, 900.f) + traceFieldOffset());
        It->EnableDefaultBackground.update(false);
        
        It->TheStyle.update([&] {
            float GuideShadeT = 80.f;
            float GuideFocusT = 10.f;
            Color4B GuideShadeC(255, 255, 255, 255 * .6f);
            Color4B GuideFocusC(135, 88, 40, 255);
            
            float StrokeShadeT = 110.f;
            float StrokeMainT = 80.f;
            float StrokeFocusT = 50.f;
            Color4B StrokeShadeC(255, 255, 255, 255 * .6f);
            Color4B StrokeMainC(86, 57, 29, 255);
            Color4B StrokeFocusC(69, 45, 20, 255);
            
            using LT = TraceLineStyle::Type;
            TraceFieldStyle Style;
            
            Style.MainLineThickness = 130.f;
            
            // NB(xenosoz, 2016): White shadows.
            Style.LineStyles.emplace_back(LT::PassiveGuide, GuideShadeT, GuideShadeC);
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeShadeT, StrokeShadeC);
            Style.LineStyles.emplace_back(LT::ActiveGuide, GuideShadeT, GuideShadeC);
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeShadeT, StrokeShadeC);
            
            // NB(xenosoz, 2016): Strokes.
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeMainT, StrokeMainC);
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeMainT, StrokeMainC);
            
            // NB(xenosoz, 2016): Highlights.
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeFocusT, StrokeFocusC);
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeFocusT, StrokeFocusC);
            
            // NB(xenosoz, 2016): Active guides.
            Style.LineStyles.emplace_back(LT::ActiveGuide, GuideFocusT, GuideFocusC);
            
            return Style;
        }());
        
        // NB(xenosoz, 2018): Log for future analysis
        auto makeWorkPath = [this] {
            // NB(xenosoz, 2018): TraceWorkIndex is dynamic. That's why I made this as a function.
            stringstream SS;
            SS << "/" << "LetterTrace";
            SS << "/" << "level-" << LevelID << "-" << SheetID;
            SS << "/" << "work-" << TraceWorkIndex();
            return SS.str();
        };

        It->OnEndEditing = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("LetterTrace", makeWorkPath(), "work-middle", "work-end");
        };
        It->OnTraceWorkDidEnd = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("LetterTrace", makeWorkPath(), "work-end", "work-end");
            auto Guard = NodeScopeGuard(this);
            handleTraceWorkDidEnd();
        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());

    
    if (showDebugLines())
    {
        [&] {
            auto It = Sprite::create(solidDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(topLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
        
        [&] {
            auto It = Sprite::create(dashedDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(midLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
        
        [&] {
            auto It = Sprite::create(solidDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(botLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
    }

    
    TheBonusNode = ([&] {
        BonusNode* It = BonusNode::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize / 2.f));
        
        It->OnBonusWorkDidEnd = [this](BonusNode*) {
            auto Guard = NodeScopeGuard(this);
            
            auto seq = Sequence::create(DelayTime::create(1.0),
                             CallFunc::create([this](){ handleBonusWorkDidEnd();}),
                             nullptr);
            this->runAction(seq);
        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
    
    TheTrailerNode = ([&] {
        TrailerNode* It = TrailerNode::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);
        
        It->OnNextButtonClicked = [this](TrailerNode*) {
            MainDepot().soundForGoodAssetTouch().play();
            
            auto Guard = NodeScopeGuard(this);
            handleTrailerWorkDidEnd();
        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
}

string LetterTraceScene::getVideoFile(string letter)
{
    auto filename = StringUtils::format("asl_%c_hand", std::tolower(letter.c_str()[0]));
    auto path = StringUtils::format("Videos/%s.mp4", filename.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path = filename;
#endif
    return path;
}

void LetterTraceScene::drawSignLanguageVideo()
{
    if (LevelID != 1 && LevelID != 2)
        return;

    if (LanguageManager::getInstance()->isSignLanguageMode() == false)
        return;
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this](){
        //SHOW_SL_VIDEO_IF_ENABLED("common/temp_video_short.mp4");
        SHOW_SL_VIDEO_IF_ENABLED(getVideoFile(TheTraceWork().TraceText.c_str()));
    }), nullptr));
}

void LetterTraceScene::prepareFirstWork() {
    TraceWorkMaxIndex.update(TheWorksheet().endProblemID() - 1);
    TraceWorkIndex.update(TheWorksheet().beginProblemID());

    auto Work = TheWorksheet().problemByID(TraceWorkIndex());
    TheTraceWork.update(Work);

    if (TraceWorkIndex() + 1 < TheWorksheet().endProblemID()) {
        // NB(xenosoz, 2016): Next problem is valid.
        auto Next = TheWorksheet().problemByID(TraceWorkIndex() + 1);
        NextTraceWork.update(Next);
    }
}

bool LetterTraceScene::prepareNextWork() {
    TraceWorkMaxIndex.update(TheWorksheet().endProblemID() - 1);
    TraceWorkIndex.update(TraceWorkIndex() + 1);
    if (TraceWorkIndex() >= TheWorksheet().endProblemID()) {
        return false;
    }
    
    auto Work = TheWorksheet().problemByID(TraceWorkIndex());
    TheTraceWork.update(Work);
    
    if (TraceWorkIndex() + 1 < TheWorksheet().endProblemID()) {
        // NB(xenosoz, 2016): Next problem is valid.
        auto Next = TheWorksheet().problemByID(TraceWorkIndex() + 1);
        NextTraceWork.update(Next);
    }

    return true;
}

void LetterTraceScene::beginTheWork() {
    TheTraceField->setVisible(true);
    TopGuide->setVisible(true);
    MidGuide->setVisible(true);
    BotGuide->setVisible(true);

    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(false);

    // NB(xenosoz, 2016): The Sound.
    float Delay = 0.f;
    const float DelayUnit = 1.2f;

    if (LevelID > 4)
    {
        Delay += DelayUnit / 4.f;//(TraceWorkIndex() > 0 ? DelayUnit : DelayUnit / 2.f);
    }
    else
    {
        Delay += DelayUnit / 4.f;
    }
    
    TheTraceField->setOpacity(0);
    TheTraceField->runAction(Sequence::create(FadeIn::create(Delay), CallFunc::create([this](){
        MainDepot().soundForLetter(TheTraceWork().TraceText).play();
    }), nullptr));
    
//    scheduleOnce([this](float) {
//        MainDepot().soundForLetter(TheTraceWork().TraceText).play();
//    }, Delay, "beginTheWork_Delay_1");
}

void LetterTraceScene::playBonusVideo() {
    TheTraceField->setVisible(false);
    TopGuide->setVisible(false);
    MidGuide->setVisible(false);
    BotGuide->setVisible(false);

    TheBonusNode->setVisible(true);
    TheTrailerNode->setVisible(false);

    TheBonusNode->startPlay();

    // NB(xenosoz, 2016): The Sound.
    MainDepot Depot;
    float Delay = 0.f;
    const float DelayUnit = 1.2f;
    const bool PutSoundBetweenWords = !Depot.soundForLetterBetweenWords().empty();
    
    Depot.soundForTraceEnd().play();

    Delay += DelayUnit;
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
        MainDepot().soundForLetter(TheTraceWork().TraceText).play();
    }, Delay, "playBonusVideo_Delay_1");


    if (PutSoundBetweenWords) {
        Delay += DelayUnit;
        scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
            MainDepot().soundForLetterBetweenWords().play();
        }, Delay, "playBonusVideo_Delay_2");
    }


    Delay += DelayUnit;
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
        MainDepot().soundForWord(TheTraceWork().FullText).play();
    }, Delay, "playBonusVideo_Delay_3");
}

void LetterTraceScene::showTrailer() {
    TheTraceField->setVisible(false);
    TopGuide->setVisible(false);
    MidGuide->setVisible(false);
    BotGuide->setVisible(false);

    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(true);
    
    TheProgressBar->setCurrent((int)TraceWorkIndex(), true);

    MainDepot().soundForWorkComplete().play();
}

void LetterTraceScene::handleTraceWorkMaxIndexValueUpdate(size_t&) {
    TheProgressBar->setMax((int)TraceWorkMaxIndex());
}

void LetterTraceScene::handleTraceWorkIndexValueUpdate(size_t&) {
    TheProgressBar->setCurrent((int)TraceWorkIndex(), false);
}

void LetterTraceScene::handleTheTraceWorkValueUpdate(Problem&) {
    MainDepot Depot;

    auto W = TheTraceWork();
    string VideoFilePath = Depot.videoFilePathByFileName(W.VideoFileName);

    TheTraceField->Placeholder.update(W.TraceText);
    TheBonusNode->VideoFilePath.update(VideoFilePath);
    TheBonusNode->TraceText.update(W.TraceText);
    TheBonusNode->FullText.update(W.FullText);

    drawSignLanguageVideo();
}

void LetterTraceScene::handleNextTraceWorkValueUpdate(Problem&) {
    auto NW = NextTraceWork();

    TheTrailerNode->NextButtonText.update(NW.TraceText);
}

void LetterTraceScene::handleTraceWorkDidEnd() {
    if (TheTraceWork().BonusVideoEnabled) {
        playBonusVideo();
        return;
    }

    // NB(xenosoz, 2016): I found some cases where bonus video doesn't exist in level data.
    handleBonusWorkDidEnd();
    
    // HyeonGyu(2018.05): For Sign-Language Build
    //removeSignLanguageVideo();
}

void LetterTraceScene::handleBonusWorkDidEnd() {
    if (TraceWorkIndex() + 1 >= TheWorksheet().endProblemID()) {
        // NB(xenosoz, 2016): There's no more remaining problem -> success!
        handleSuccess();
        return;
    }

    if (LevelID > 5)
    {
        showTrailer();
    }
    else
    {
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this]() {
            auto Guard = NodeScopeGuard(this);
            handleTrailerWorkDidEnd();
        }), nullptr));
    }
}

void LetterTraceScene::handleTrailerWorkDidEnd() {
    prepareNextWork();
    beginTheWork();
}

void LetterTraceScene::handleSuccess() {
    TheTraceField->setVisible(false);
    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(false);

    auto CP = CompletePopup::create();
    CP->show(1.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

void LetterTraceScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}

void LetterTraceScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    prepareFirstWork();
    beginTheWork();
}

void LetterTraceScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}


END_NS_LETTERTRACE
