//
//  NumberTraceExtScene.cpp -- A game scene for NumberTraceExt
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "NumberTraceExtScene.h"
#include "../Utils/NumberTraceExtMainDepot.h"
#include "Common/Components/TargetDragBody.h"

#include "CCAppController.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "Common/Controls/SignLanguageVideoPlayer.hpp"

BEGIN_NS_NUMBERTRACEEXT

using namespace todoschool::tracefield;
using namespace cocos2d;
using namespace std;



namespace
{
    const float kNumberListMarginTop = 25.f;
    const float kGuideNodeMarginBottom = 50.f;
    
    const float kNumbersFontSize = 78.f;
    const float kNumbersWrapperTopMargin = 40.f;
    const Vec2 kTraceFieldPositionCorrection = Vec2(0.f, -190.f);
    
    const string kStick1 = MainDepot().assetPrefix() + "/tracing-2_image_stick-01.png";
    const string kStick2 = MainDepot().assetPrefix() + "/tracing-2_image_stick-02.png";
    const string kStick3 = MainDepot().assetPrefix() + "/tracing-2_image_stick-03.png";
    const string kStick4 = MainDepot().assetPrefix() + "/tracing-2_image_stick-04.png";
    const string kStick5 = MainDepot().assetPrefix() + "/tracing-2_image_stick-05.png";
    const string kStickArray[5] = { kStick1, kStick2, kStick3, kStick4, kStick5 };
    
    const string k1x1LargeBlock = MainDepot().assetPrefix() + "/tracing-2_image_block-large.png";
    
    const string k1x1Block = MainDepot().assetPrefix() + "/tracing-2_image_block-small1.png";
    const string k1x10Block = MainDepot().assetPrefix() + "/tracing-2_image_block-small10.png";
    const string k10x10Block = MainDepot().assetPrefix() + "/tracing-2_image_block-small100.png";
    
    const float kHorizontalSpacing1x1LargeBlock = 20.f;
    const float kVerticalSpacing1x1LargeBlock = 20.f;
    
    const float kHorizontalSpacing5x2Blocks = 240.f;
    const float kVerticalSpacing5x2Blocks = 100.f;
    
    const float kHorizontalSpacingEachGroup = 20.f;
    
    std::vector<std::tuple<int, string>> levelData = {
        make_tuple(1, "Stick"),    // 1
        make_tuple(1, "Block"),    // 2
        make_tuple(11, "Stick"),   // 3
        make_tuple(11, "Block"),   // 4
        make_tuple(21, "Stick"),   // 5
        make_tuple(21, "Block"),   // 6
        make_tuple(31, "Block"),   // 7
        make_tuple(41, "Block"),   // 8
        make_tuple(51, "Block"),   // 9
        make_tuple(61, "Block"),   // 10
        make_tuple(71, "Block"),   // 11
        make_tuple(81, "Block"),   // 12
        make_tuple(91, "Block"),   // 13
        make_tuple(101, "Block"),  // 14
    };
}

NumberTraceExtScene::NumberTraceExtScene()
: TheGameNode(nullptr)
, TheTraceField(nullptr)
,_currentProblemIndex(0)
{
    
}

Node* NumberTraceExtScene::getNumberObjectNode(int count, NumberObjectType type)
{
    Node* wrapper = Node::create();
    if (type == NumberObjectType::Stick)
    {
        int cntBundle5 = count / 5;
        float sumWidth = 0.f;
        
        Size stickSize = Sprite::create(kStick1)->getContentSize();
        
        for (int i = 0; i < cntBundle5; i++)
        {
            auto bundle = Sprite::create(kStick5);
            bundle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            bundle->setPosition(stickSize.width * i, 0.f);
            wrapper->addChild(bundle);
            sumWidth += stickSize.width;
        }
        
        if (count % 5 > 0)
        {
            auto piece = Sprite::create(kStickArray[count % 5 - 1]);
            piece->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            piece->setPosition(sumWidth, 0.f);
            wrapper->addChild(piece);
            sumWidth += stickSize.width;
        }
        
        if (count > 20)
        {
            wrapper->setScale(0.7f);
        }
        
        wrapper->setContentSize(Size(sumWidth, stickSize.height));
    }
    else
    {
        float curX = 0.f;
        float curY = 0.f;
        auto innerWrapper = Node::create();
        
        if (count > 40)
        {
            const Size k1x1BlockSize = Sprite::create(k1x1Block)->getContentSize();
            const Size k1x10BlockSize = Sprite::create(k1x10Block)->getContentSize();
            const Size k10x10BlockSize = Sprite::create(k10x10Block)->getContentSize();
            
            int count10x10Blocks = 0;
            int count1x10Blocks = 0;
            int count1x1Blocks = 0;
            
            if (count >= 100 && count / 100 > 0 && count % 100 == 0)
            {
                count10x10Blocks = (count / 100) - 1;
                count1x10Blocks = 10;
            }
            else
            {
                count10x10Blocks = count / 100;
                count1x10Blocks = (count % 100) / 10;
                count1x1Blocks = (count % 100) % 10;
            }
            
            for (int i = 0; i < count10x10Blocks; i++)
            {
                auto block = Sprite::create(k10x10Block);
                block->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                block->setPosition(curX, curY);
                innerWrapper->addChild(block);
                
                curX += k10x10BlockSize.width;
            }
            
            if (count10x10Blocks > 0 && count1x10Blocks > 0) curX += kHorizontalSpacingEachGroup;
            for (int i = 0; i < count1x10Blocks; i++)
            {
                auto block = Sprite::create(k1x10Block);
                block->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                block->setPosition(curX, curY);
                innerWrapper->addChild(block);
                
                curX += k1x10BlockSize.width;
            }
            if (count1x1Blocks == 0) curX -= k1x10BlockSize.width;
            
            for (int i = 0; i < count1x1Blocks; i++)
            {
                auto block = Sprite::create(k1x1Block);
                block->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                block->setPosition(curX, curY);
                innerWrapper->addChild(block);
                
                curY -= (k1x1BlockSize.height - 21.f);
            }
            innerWrapper->setContentSize(Size(curX, -k1x10BlockSize.height));
            innerWrapper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            //wrapper->addChild(innerWrapper);
        }
        else
        {
            const Size k1x1LargeBlockSize = Sprite::create(k1x1LargeBlock)->getContentSize();
            const Size k5x2LargeBlocksSize = Size(k1x1LargeBlockSize.width * 5.f, k1x1LargeBlockSize.width * 2.f);
            
            int count5x2Blocks = count / 10;
            
            for (int i = 0; i < count5x2Blocks; i++)
            {
                auto blocks = get5x2LargeBlocksObject();
                blocks->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                blocks->setPosition(curX, curY);
                innerWrapper->addChild(blocks);
                
                if ((i + 1) % 2 == 0)
                {
                    curX = 0.f;
                    curY -= k5x2LargeBlocksSize.height + kVerticalSpacing5x2Blocks;
                }
                else
                {
                    curX += k5x2LargeBlocksSize.width + kHorizontalSpacing5x2Blocks;
                }
            }
            
            
            auto localCurX = curX;
            for (int i = 0; i < count % 10; i++)
            {
                auto block = Sprite::create(k1x1LargeBlock);
                block->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                block->setPosition(localCurX, curY);
                innerWrapper->addChild(block);
                
                if ((i + 1) % 5 == 0)
                {
                    localCurX = curX;
                    curY -= k1x1LargeBlockSize.height + kVerticalSpacing1x1LargeBlock;
                }
                else
                {
                    localCurX += k1x1LargeBlockSize.width + kHorizontalSpacing1x1LargeBlock;
                }
            }
            
            Size innerWrapperSize;
            if (count5x2Blocks == 0)
            {
                innerWrapperSize = Size(k5x2LargeBlocksSize);
            }
            else if (count5x2Blocks == 1 || count == 20)
            {
                innerWrapperSize = Size(k5x2LargeBlocksSize.width * 2, k5x2LargeBlocksSize.height) + Size(kHorizontalSpacing5x2Blocks, 0.f);
            }
            else
            {
                innerWrapperSize = Size(k5x2LargeBlocksSize.width * 2, k5x2LargeBlocksSize.height * 2) + Size(kHorizontalSpacing5x2Blocks, kVerticalSpacing5x2Blocks);
            }
            
            innerWrapperSize.height = innerWrapperSize.height * -1.f;
            innerWrapper->setContentSize(innerWrapperSize);
            innerWrapper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
        }
        auto absInnerWrapperHeight = abs(innerWrapper->getContentSize().height);
        auto gameNodeHeight = TheGameNode->getContentSize().height;
        if (absInnerWrapperHeight + 150.f > gameNodeHeight)
        {
            float scaleFactor = gameNodeHeight / (absInnerWrapperHeight + 150.f);
            innerWrapper->setScale(scaleFactor);
        }
        wrapper->addChild(innerWrapper);
    }
    return wrapper;
}

Node* NumberTraceExtScene::get5x2LargeBlocksObject()
{
    Size blockSize = Sprite::create(k1x1LargeBlock)->getContentSize();
    auto wrapper = Node::create();
    for (int i = 0; i < 5; i++)
    {
        float horizontalSpacing = i > 0 ? kHorizontalSpacing1x1LargeBlock : 0.f;
        
        Sprite* block;
        block = Sprite::create(k1x1LargeBlock);
        block->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        block->setPosition((blockSize.width + horizontalSpacing) * i, 0.f);
        wrapper->addChild(block);
        
        block = Sprite::create(k1x1LargeBlock);
        block->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        block->setPosition((blockSize.width + horizontalSpacing) * i, blockSize.height + kVerticalSpacing1x1LargeBlock);
        wrapper->addChild(block);
    }
    wrapper->setContentSize(Size(blockSize.width * 5, blockSize.height * 2) + Size(kHorizontalSpacing1x1LargeBlock * 4, kVerticalSpacing1x1LargeBlock));
    return wrapper;
}

bool NumberTraceExtScene::init()
{
    if (!Super::init()) { return false; }
    return true;
}

void NumberTraceExtScene::refreshGuide()
{
    if (GuideNode != nullptr)
        this->removeChild(GuideNode);
    
    string guideSpriteName;
    if (std::get<0>(levelData[_currentLevel - 1]) + _currentProblemIndex >= 100)
    {
        guideSpriteName = "tracing-2_image_paper-until200.png";
    }
    else
    {
        guideSpriteName = "tracing-2_image_paper-until40.png";
    }
    GuideNode = Sprite::create(MainDepot().assetPrefix() + "/" + guideSpriteName);
    GuideNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    GuideNode->setPosition(MainDepot().windowSize().width / 2, kGuideNodeMarginBottom);
    this->addChild(GuideNode, 0);
}

void NumberTraceExtScene::refreshNumberList()
{
    NumberListNode->removeAllChildren();
    
    auto barBackground = Sprite::create(MainDepot().assetPrefix() + "/tracing-2_image_progress-bar_bg.png");
    barBackground->setName("BarBackground");
    barBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    barBackground->setPosition(0.f, kNumberListMarginTop);
    NumberListNode->addChild(barBackground);
    
    Color4B color;
    float fontSize;
    
    float margin = 120.f;
    float eachGap = (barBackground->getContentSize().width - (margin * 2)) / 9;
    
    for (int i = 0; i < 10; i++)
    {
        auto number = std::get<0>(levelData[_currentLevel - 1]) + i;
        std::ostringstream os;
        os << number;
        color = _currentProblemIndex == i ? Color4B(227.f, 214.f, 191.f, 255.f) : Color4B(207.f, 190.f, 158.f, 255.f * 0.5f);
        fontSize = _currentProblemIndex == i ? kNumbersFontSize + 15.f : kNumbersFontSize;
        auto numberLabel = TodoUtil::createLabel(os.str(), fontSize, Size::ZERO, MainDepot().defaultFont(), color);
        numberLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        numberLabel->setPosition(margin + (eachGap * i), barBackground->getContentSize().height / 2 - 10.f);
        barBackground->addChild(numberLabel);
    }
}

void NumberTraceExtScene::refreshNumberObjects()
{
    NumberObjectNode->removeAllChildren();
    
    NumberObjectType type;
    if (std::get<1>(levelData[_currentLevel - 1]) == "Stick")
    {
        type = NumberObjectType::Stick;
    }
    else
    {
        type = NumberObjectType::Block;
    }
    
    auto objects = getNumberObjectNode(std::get<0>(levelData[_currentLevel - 1]) + _currentProblemIndex, type);
    objects->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    NumberObjectNode->addChild(objects);
}

void NumberTraceExtScene::playCurrentNumberSound()
{
    int currentNumber = std::get<0>(levelData[_currentLevel - 1]) + _currentProblemIndex;
    std::string currentNumberString = TodoUtil::itos(currentNumber);
    
    if (currentNumber > 100 && currentNumber <= 199)
    {
        auto remainder = currentNumber % 100;
        
        if (LanguageManager::getInstance()->isEnglish())
        {
            this->runAction(Sequence::create(CallFunc::create([this](){ playNumberSound(100); }),
                                             DelayTime::create(0.85f),
                                             CallFunc::create([this, remainder](){ playNumberSound(remainder); }), nullptr));
        }
        else
        {
            this->runAction(Sequence::create(CallFunc::create([this](){ playNumberSound(100); }),
                                             DelayTime::create(0.90f),
                                             CallFunc::create([](){ GameSoundManager::getInstance()->playEffectSound(MainDepot().assetPrefix() + "/Sounds.sw_TZ/na.wav"); }),
                                             DelayTime::create(0.30f),
                                             CallFunc::create([this, remainder](){ playNumberSound(remainder); }), nullptr));
        }
    }
    else
    {
        playNumberSound(currentNumber);
    }
}

void NumberTraceExtScene::initProblem()
{
    std::string currentNumberString = TodoUtil::itos(std::get<0>(levelData[_currentLevel - 1]) + _currentProblemIndex);
    
    refreshGuide();
    refreshNumberList();
    refreshNumberObjects();
    
    TheTraceField->clear();
    TheTraceField->Placeholder.update(currentNumberString);
    TheTraceField->setPosition(GuideNode->getPosition() + kTraceFieldPositionCorrection);
}

void NumberTraceExtScene::initUI()
{
    MainDepot Depot;
    
    Background = ([&] {
        Sprite* It = Depot.createBackgroundSprite();
        addChild(It);
        return It;
    }());
    
    refreshGuide();
    
    TheGameNode = ([&] {
        auto It = Node::create();
        auto barHeight = Sprite::create(MainDepot().assetPrefix() + "/tracing-2_image_progress-bar_bg.png")->getContentSize().height;
        auto guideHeight = GuideNode->getContentSize().height;
        float sum = kNumberListMarginTop + kGuideNodeMarginBottom + barHeight + guideHeight;
        It->setContentSize(Size(MainDepot().windowSize().width, MainDepot().windowSize().height - sum));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(MainDepot().windowSize().width / 2, MainDepot().windowSize().height - (kNumberListMarginTop + barHeight));
        addChild(It);
        return It;
    }());
    
    NumberListNode = Node::create();
    NumberListNode->setPosition(Depot.windowSize().width / 2, Depot.windowSize().height - kNumbersWrapperTopMargin);
    addChild(NumberListNode);
    
    NumberObjectNode = Node::create();
    NumberObjectNode->setPosition(TheGameNode->getContentSize() / 2);
    TheGameNode->addChild(NumberObjectNode);
    
    [&] {
        auto It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, Depot.windowSize().height - 25.f));
        It->addClickEventListener([this](Ref*) {
            handleFail();
        });
        addChild(It);
        return It;
    }();
    
    TheTraceField = ([&] {
        TraceField* It = TraceField::createWithCustomCursor("ComprehensionTest/CompTrace/comprehension_tracing_icon.png");
        
        It->cursorScaleNormal = 1.f;
        It->cursorScalePicked = 1.2f;
        
        It->setContentSize(GuideNode->getContentSize() * 1.25f);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(GuideNode->getPosition() + kTraceFieldPositionCorrection);
        It->EnableDefaultBackground.update(false);
        It->ClippingEnabled.update(false);
        
        It->TheStyle.update([&] {
            float MintLineT = 12.f;
            Color4B MintLineC(250, 243, 218, 255);
            
            float WhiteLineT = 100.f;
            // Color4B WhiteLineC(81, 53, 24, 50);
            Color4B WhiteLineC(233, 225, 199, 255);
            
            float StrokeLineT = 100.f;
            Color4B StrokeLineC(81, 53, 24, 255);

            using LT = TraceLineStyle::Type;
            TraceFieldStyle Style;
            
            Style.ScrollLetterByLetter = false;
            Style.MainLineThickness = 130.f;
            
            // Style.LineStyles.emplace_back(LT::PassiveGuide, WhiteLineT, Color4B(233, 225, 199, 255)); // before
            Style.LineStyles.emplace_back(LT::PassiveGuide, WhiteLineT, Color4B(217, 203, 162, 255)); // before
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeLineT, StrokeLineC);
            
            // Style.LineStyles.emplace_back(LT::ActiveGuide, WhiteLineT, Color4B(217, 207, 181, 255)); // after
            Style.LineStyles.emplace_back(LT::ActiveGuide, WhiteLineT, Color4B(224, 167, 108, 255)); // after
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeLineT, StrokeLineC);
            
            Style.LineStyles.emplace_back(LT::ActiveGuide, MintLineT, MintLineC);
            
            return Style;
        }());
        
        // NB(xenosoz, 2018): Log for future analysis
        auto makeWorkPath = [this] {
            // NB(xenosoz, 2018): _currentProblemIndex is dynamic. That's why I made this as a function.
            stringstream SS;
            SS << "/" << "NumberTraceExt";
            SS << "/" << "level-" << _currentLevel << "-0";
            SS << "/" << "work-" << _currentProblemIndex;
            return SS.str();
        };

        It->OnEndEditing = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("NumberTraceExt", makeWorkPath(), "work-middle", "work-end");
        };

        It->OnTraceWorkDidEnd = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("NumberTraceExt", makeWorkPath(), "work-end", "work-end");
            auto Guard = NodeScopeGuard(this);
            handleSuccess();
        };
        
        this->addChild(It, 1);
        return It;
    }());
}

void NumberTraceExtScene::handleSuccess()
{
    if (_currentProblemIndex < 9)
    {
        MainDepot().soundForTraceEnd().play();
        
        if (LanguageManager::getInstance()->isSignLanguageMode())
        {
            SHOW_SL_VIDEO_WITH_CALLBACK_IF_ENABLED("common/temp_video_short.mp4", {
                _currentProblemIndex++;
                initProblem();
                playCurrentNumberSound();
            });
        }
        else
        {
            runAction(Sequence::create(DelayTime::create(0.9f), CallFunc::create([this]() {
                _currentProblemIndex++;
                initProblem();
                playCurrentNumberSound();
            }), nullptr));

        }
    }
    else
    {
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            auto Guard = NodeScopeGuard(this);
            
            if (OnSuccess)
                OnSuccess();
            
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
}

void NumberTraceExtScene::handleFail()
{
    auto Guard = NodeScopeGuard(this);
    if (OnFail)
        OnFail();
}

void NumberTraceExtScene::onEnter()
{
    Super::onEnter();
}

void NumberTraceExtScene::onStart()
{
    initUI();
    initProblem();
}

void NumberTraceExtScene::onEnterTransitionDidFinish()
{
    playCurrentNumberSound();
}

void NumberTraceExtScene::onExitTransitionDidStart()
{
    Super::onExitTransitionDidStart();
}

void NumberTraceExtScene::setCurrentLevel(int level)
{
    _currentLevel = level;
}

void NumberTraceExtScene::playNumberSound(int number)
{
    char filename[256] = "";
    if (LanguageManager::getInstance()->isEnglish())
    {
        
        sprintf(filename,  + "%s/Sounds/C_%03d.m4a" , MainDepot().assetPrefix().c_str(), number);
    }
    else
    {
        sprintf(filename,  + "%s/Sounds/%d.m4a" , MainDepot().assetPrefix().c_str(), number);
        
    }
    GameSoundManager::getInstance()->playEffectSound(filename);
}

END_NS_NUMBERTRACEEXT
