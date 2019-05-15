//
//  SpellingScene.cpp -- Spell-filling game with balls
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingScene.h"
#include "../BallNodes/AnswerBall.h"
#include "../ChildNodes/BallSlot.h"
#include "../Utils/SpellingMainDepot.h"
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Managers/StrictLogManager.h>

#include "CCAppController.hpp"

#include "Common/Controls/SignLanguageVideoPlayer.hpp"

BEGIN_NS_SPELLING

namespace {
    Node* videoNode = nullptr;
    
    string contentSkin() {
        return MainDepot().assetPrefix() + "/Background/spelling_background.jpg";
    }

    string leftLeafSkin() {
        return MainDepot().assetPrefix() + "/Background/spelling_leaves_left.png";
    }

    string rightLeafSkin() {
        return MainDepot().assetPrefix() + "/Background/spelling_leaves_right.png";
    }

    float scoreForBallAndSlot(AnswerBall* Ball, BallSlot* Slot) {
        const float Inf = numeric_limits<float>::infinity();
        if (!Ball || !Slot || !Slot->empty()) { return -Inf; }
        if (Ball->Text() != Slot->BallText()) { return -Inf; }

        auto convertToWorldSpace = [](Node* N, Rect R) {
            auto Origin = N->convertToWorldSpace(R.origin);
            auto Diagonal = N->convertToWorldSpace(R.origin + R.size);
            
            return Rect(Origin, Size(Diagonal - Origin));
        };
        
        Rect BallR = convertToWorldSpace(Ball, Rect(Point::ZERO, Ball->getContentSize()));
        Rect SlotR = convertToWorldSpace(Slot, Rect(Point::ZERO, Slot->getContentSize()));
        Rect Cap = BallR.intersection(SlotR);
        
        return max(Cap.area() / BallR.area(), Cap.area() / SlotR.area());
    }
    
    vector<string> choiceLettersForProblem(const Problem& Spec) {
        MainDepot Depot;
        auto DictC = Depot.auxiliaryConsonantsFor("");
        auto DictV = Depot.auxiliaryVowelsFor("");
        int QuotaC = Spec.ConsonantChoiceCount;
        int QuotaV = Spec.VowelChoiceCount;

        // NB(xenosoz, 2016): Fill correct answers.
        vector<string> Letters;
        for (size_t I = 0, E = Spec.Word.size(); I < E; ++I) {
            if (!Spec.letterBecomesChoice(I)) { continue; }
            auto L = string{Spec.Word[I]};

            // NB(xenosoz, 2016): add to choice whenever the mask is '*'.
            if (find(DictC.begin(), DictC.end(), L) != DictC.end()) {
                QuotaC = max(0, QuotaC - 1);
            }
            if (find(DictV.begin(), DictV.end(), L) != DictV.end()) {
                QuotaV = max(0, QuotaV - 1);
            }
            Letters.push_back(L);
        }
        
        // NB(xenosoz, 2016): Fill consonants.
        auto AuxC = Depot.auxiliaryConsonantsFor(Spec.Word);

        while (AuxC.size() < QuotaC)
            AuxC.insert(AuxC.end(), DictC.begin(), DictC.end());

        for (auto C : sample(AuxC, QuotaC))
            Letters.push_back(C);

        QuotaC = 0;

        // NB(xenosoz, 2016): Fill vowels.
        auto AuxV = Depot.auxiliaryVowelsFor(Spec.Word);

        while (AuxV.size() < QuotaV)
            AuxV.insert(AuxV.end(), DictV.begin(), DictV.end());

        for (auto V : sample(AuxV, QuotaV))
            Letters.push_back(V);

        QuotaV = 0;

        // NB(xenosoz, 2016): Shuffle and return.
        shuffle(Letters);
        return Letters;
    }
    
    BallSize ballSizeForProblem(Problem& P) {
        return (P.Word.length() <= 4 ? BallSize::Large : BallSize::Small);
    }
}  // unnamed namespace


SpellingScene::SpellingScene()
: TheGameNode(nullptr)
, TheProgressBar(nullptr)
, TheGameBoard(nullptr)
, ThePlatform(nullptr)
{
}

bool SpellingScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
};


void SpellingScene::clearInternals() {
    SoundForWord = SoundEffect::emptyEffect();

    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
        SoundForWord = (MainDepot().soundForWord(TheProblem().Word) ||
                        MainDepot().soundForCardBirth());
    };
}

void SpellingScene::refreshChildNodes() {
    removeAllChildren();
    
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    [&] {
        // NB(xenosoz, 2016): The background node.
        auto BN = Node::create();
        BN->setContentSize(WindowSize);
        
        if (true) {
            auto It = Sprite::create(contentSkin());
            Size SpriteSize = It->getContentSize();
            float Scale = Depot.scaleToCoverWindow(SpriteSize);
            
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(WindowSize/2.f);
            
            It->setScale(Scale);
            It->setBlendFunc(BlendFunc::DISABLE);
            BN->addChild(It);
        }
        
        if (true) {
            auto It = Sprite::create(leftLeafSkin());
            It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            It->setPosition(Point(0.f, WindowSize.height));

            BN->addChild(It);
        }
        
        if (true) {
            auto It = Sprite::create(rightLeafSkin());
            It->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            It->setPosition(Point(WindowSize));

            BN->addChild(It);
        }

        addChild(BN);
    }();
    
    TheGameNode = ([&] {
        Size WindowSize = Depot.windowSize();
        Size GameSize = Depot.gameSize();
        float Scale = Depot.scaleToBeContainedInWindow(GameSize);
        
        Node* It = Node::create();
        It->setContentSize(GameSize);
        It->setScale(Scale);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(WindowSize.width / 2.f, 0.f));
        
        addChild(It);
        return It;
    }());
    
    TheProgressBar = [&] {
        // NB(xenosoz, 2016): The progress indicator.
        auto It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));
        
        It->setMax((int)TheSheet().size());
        It->setCurrent((int)ProblemID - TheSheet().beginProblemID() + 1);
        
        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            handleFail();
        });
        
        addChild(It);
    }();


    // NB(xenosoz, 2016): Direct descendants of TheGameNode
    TheGameBoard = ([&] {
        auto It = GameBoard::create(ballSizeForProblem(TheProblem()));

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height - 80.f));

        It->TheProblem.follow(TheProblem);
        It->OnAllSlotDidFill = [this] {
            handleCorrectAnswer();
        };

        It->setOpacity(0);

        TheGameNode->addChild(It);
        return It;
    }());

    ThePlatform = ([&] {
        string CorrectAnswer = TheProblem().Word;

        auto It = Platform::create(ballSizeForProblem(TheProblem()));
        auto Letters = choiceLettersForProblem(TheProblem());

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 0.f));
        It->Letters.update(Letters);
        
        It->setOpacity(0);

        TheGameNode->addChild(It);
        return It;
    }());

    for (size_t I = 0, E = (ThePlatform ? ThePlatform->Letters().size() : 0); I < E; ++I) {
        // NB(xenosoz, 2016): Answer balls
        if (!TheGameNode || !ThePlatform) { continue; }
        Point WorldP = ThePlatform->positionForSlotInWorldSpace(I);
        Point LocalP = TheGameNode->convertToNodeSpace(WorldP);
        string Letter = ThePlatform->Letters()[I];

        auto It = AnswerBall::create(ballSizeForProblem(TheProblem()));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(LocalP);

        It->BallID.update(I);
        It->Text.update(Letter);
        It->Opacity.follow(ThePlatform->Opacity);
        It->HomePositionInWorldSpace.update(WorldP);
        It->PlatformPositionInLocalSpace.follow(ThePlatform->LocalPosition);
        
        It->PlatformPositionInLocalSpace.OnValueUpdate = [this, I, It](Point&) {
            // NB(xenosoz, 2016): Answer balls follow platform for animation
            if (!TheGameNode || !ThePlatform) { return; }
            Point WorldP = ThePlatform->positionForSlotInWorldSpace(I);
            
            It->PositionInWorldSpace.update(WorldP);
        };
        
        It->OnDragDidBegin = [this, It](Touch*, Event*) {
            if (!ThePlatform) { return; }
            ThePlatform->hideShadowForBallID(It->BallID());
        };

        It->OnDrag = [this, It](Touch*, Event*) {
            if (!TheGameBoard) { return; }
            auto& Board = *TheGameBoard;
            
            BallSlot* BestSlot = nullptr;
            float BestScore = -numeric_limits<float>::infinity();
            for (size_t I = 0, E = Board.size(); I < E; ++I) {
                auto Slot = Board.slotAtIndex(I);
                auto Ball = It;

                float Score = scoreForBallAndSlot(Ball, Slot);
                if (Score < 0.70f) { continue; }
                if (BestScore <= Score) {
                    BestScore = Score;
                    BestSlot = Slot;
                }
            }
            if (!BestSlot) { return; }
            
            
            // NB(xenosoz, 2018): Log for future analysis (#1/2)
            string workPath = [this] {
                stringstream SS;
                SS << "/" << "Spelling";
                SS << "/" << "level-" << LevelID << "-" << SheetID;
                SS << "/" << "work-" << ProblemID;
                return SS.str();
            }();
            StrictLogManager::shared()->game_Peek_Answer("Spelling", workPath, It->Text(), It->Text());

            
            //
            BestSlot->fillSlot();  // XXX
            It->playHitSound();
            It->removeFromParent();  // XXX
        };
        
        It->OnDragDidEnd = [this, It](Touch*, Event*) {
            if (!ThePlatform) { return; }
            ThePlatform->showShadowForBallID(It->BallID());
            
            // NB(xenosoz, 2018): Log for future analysis (#2/2)
            string workPath = [this] {
                stringstream SS;
                SS << "/" << "Spelling";
                SS << "/" << "level-" << LevelID << "-" << SheetID;
                SS << "/" << "work-" << ProblemID;
                return SS.str();
            }();
            StrictLogManager::shared()->game_Peek_Answer("Spelling", workPath, It->Text(), "None");
        };
        
        It->setOpacity(0);
        TheGameNode->addChild(It);
    }
}

void SpellingScene::prepareFirstWork() {
    ProblemID = TheSheet().beginProblemID();
    
    auto Problem = TheSheet().problemByID(ProblemID);
    TheProblem.update(Problem);
}

bool SpellingScene::prepareNextWork() {
    ProblemID += 1;

    if (ProblemID >= TheSheet().endProblemID()) {
        return false;
    }
    
    auto Problem = TheSheet().problemByID(ProblemID);
    TheProblem.update(Problem);
    
    return true;
}

void SpellingScene::beginTheWork() {
    auto Next = [this] { openingGameBoard(); };
    Next();
}

void SpellingScene::openingGameBoard() {
    auto Next = [this] { openingWordSound(); };
    if (!TheGameBoard) { Next(); return; }

    auto Action = TheGameBoard->actionForEnter(Next);
    TheGameBoard->runAction(Action);
}

void SpellingScene::openingWordSound() {
    auto Next = [this] { openingEjectBalls(); };
    auto Delay = TheProblem().SoundDuration + .5f;

    Vector<FiniteTimeAction*> Actions;
    Actions.pushBack(CallFunc::create([this] { SoundForWord.play(); }));
    Actions.pushBack(DelayTime::create(Delay));
    Actions.pushBack(CallFunc::create(Next));
    runAction(Sequence::create(Actions));
}

void SpellingScene::openingEjectBalls() {
    auto Next = [this] { openingPlatform(); };
    if (!TheGameBoard) { Next(); return; }

    auto Action = TheGameBoard->actionForEjectBalls(Next);
    TheGameBoard->runAction(Action);
}

void SpellingScene::openingPlatform() {
    auto Next = [this] {};
    if (!ThePlatform) { Next(); return; }

    auto Action = Sequence::create(DelayTime::create(.2f),
                                   ThePlatform->actionForEnter2(),
                                   nullptr);
    ThePlatform->runAction(Action);
}

void SpellingScene::closingWordSound() {
    auto Next = [this] { closingGameBoard(); };
    auto Delay = TheProblem().SoundDuration + .5f;
    
    Vector<FiniteTimeAction*> Actions;
    Actions.pushBack(DelayTime::create(.5f));
    Actions.pushBack(CallFunc::create([this] {
        SHOW_SL_VIDEO_IF_ENABLED("common/temp_video_short.mp4");
        SoundForWord.play();
    }));
    Actions.pushBack(DelayTime::create(Delay));
    Actions.pushBack(CallFunc::create(Next));
    runAction(Sequence::create(Actions));
}

void SpellingScene::closingGameBoard() {
    function<void()> CB = [this] {
        if (prepareNextWork()) {
            beginTheWork();
            return;
        }
        
        handleSuccess();
    };
    
    if (TheProgressBar)
        TheProgressBar->setCurrent((int)ProblemID - TheSheet().beginProblemID() + 1, true);
    
    if (TheGameBoard) {
        auto Action = TheGameBoard->actionForExit(CB);
        TheGameBoard->stopAllActions();
        TheGameBoard->runAction(Action);
        CB = nullptr;
    }
    
    if (ThePlatform) {
        auto Action = ThePlatform->actionForExit(CB);
        ThePlatform->stopAllActions();
        ThePlatform->runAction(Action);
        CB = nullptr;
    }
    
    if (CB)
        CB();
}

void SpellingScene::handleCorrectAnswer() {
    closingWordSound();
}

void SpellingScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(0.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void SpellingScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}

void SpellingScene::onEnterTransitionDidFinish() {
    prepareFirstWork();
    beginTheWork();
}

void SpellingScene::onExitTransitionDidStart() {
    
}


END_NS_SPELLING
