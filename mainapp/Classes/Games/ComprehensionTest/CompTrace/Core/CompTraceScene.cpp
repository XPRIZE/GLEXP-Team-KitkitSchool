//
//  CompTraceScene.cpp -- A game scene for CompTrace
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "CompTraceScene.h"
#include "../Utils/MainDepot.h"
#include "Common/Components/TargetDragBody.h"
#include "Common/Basic/NodeScopeGuard.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
#include "CCAppController.hpp"
#include "Managers/GameSoundManager.h"


using namespace todoschool::tracefield;
using namespace cocos2d;
using namespace std;


namespace {
 
    
    //Vec2 traceFieldOffset() { return Vec2(0.f, -490.f); }
    float kImageCorrectionY = -170.f;
    float kGuideCorrectionY = -120.f;
    float kTraceFieldCorrectionY = -120.f;
    
    Size traceSize() {
        auto gameSize = todoschool::comptrace::MainDepot().gameSize();
        return Size(gameSize.width*0.8, gameSize.height * 0.38);
    }
    
    //Point imageP() {
    //    return Point(MainDepot().gameSize().width/2.f, MainDepot().gameSize().height*0.75-490*0.25);
    //}
    
    Size imageS() {
        return Size(800, 600);
    }
    

}  // unnamed namespace


CompTraceScene::CompTraceScene()
: TheGameNode(nullptr)
, TheTraceField(nullptr)
, _comprehensionScene(nullptr)
{
}

bool CompTraceScene::init() {
    if (!Super::init()) { return false; }

    todoschool::comptrace::MainDepot().preloadSoundEffects();

    clearInternals();
    refreshChildNodes();
    
    return true;
}

void CompTraceScene::clearInternals() {
    
    TheTraceWork.OnValueUpdate = [this](Problem&) {

        
        Problem W = TheTraceWork();
        
        TheTraceField->Placeholder.update(W.Text);
        
        ImageNode->removeAllChildren();
        
        
        todoschool::comptrace::MainDepot Depot;
        
        if (W.Type == ProblemType::IMAGE_AND_SOUND)
        {
            //TodoUtil::replace(W.QuestionSound, "m4a", "wav");
            string audioPath = _comprehensionScene->getBookFolder()+"/quiz/"+W.QuestionSound;
            bool audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            if (!audioExist) {
                audioPath = _comprehensionScene->getBookFolder()+"/page/"+W.QuestionSound;
                audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            }
            if (audioExist)
            {
                auto soundButton = _comprehensionScene->drawSoundButton(TheGameNode);
                soundButton->addTouchEventListener([this, audioPath](Ref*,Widget::TouchEventType e) {
                    if (e == Widget::TouchEventType::ENDED) {
                        GameSoundManager::getInstance()->stopAllEffects();
                        GameSoundManager::getInstance()->playEffectSoundVoiceOnly(audioPath);
                    }
                });
            }
        }
        
        
        auto image = Sprite::create(_comprehensionScene->getBookFolder()+ "/quiz/" +TheTraceWork().TeaserFileName);
        if (!image) image = Sprite::create(_comprehensionScene->getBookFolder()+ "/page/" +TheTraceWork().TeaserFileName);
        if (image) {
            _traceFieldOffset = Vec2(0.f, -440.f + kTraceFieldCorrectionY);
            
            auto imageSize = image->getContentSize();
            auto nodeSize = ImageNode->getContentSize();
            auto imageScale = MIN(nodeSize.width/imageSize.width, nodeSize.height/imageSize.height);
            image->setScale(imageScale);
            image->setPosition(nodeSize/2);
            ImageNode->addChild(image);
        } else {
            _traceFieldOffset = Vec2(0.f, -440.f + kTraceFieldCorrectionY);
        }
        
        TheTraceField->setPosition(Point(Depot.gameSize() / 2.f) + _traceFieldOffset);
    };
}

void CompTraceScene::refreshChildNodes() {
    todoschool::comptrace::MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    removeAllChildren();
    

    
    TheGameNode = ([&] {
        Node* It = Depot.createGameNode();
        addChild(It);
        return It;
    }());
    
    ImageNode = Node::create();
    ImageNode->setContentSize(imageS());
    ImageNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto imageP = Point(todoschool::comptrace::MainDepot().gameSize().width/2.f, todoschool::comptrace::MainDepot().gameSize().height - 550 + kImageCorrectionY);
    ImageNode->setPosition(imageP);
    TheGameNode->addChild(ImageNode);
    
    // NB(xenosoz, 2016): Descendants of the game node.
    
    using std::placeholders::_1;

    _guide = Sprite::create("ComprehensionTest/CompTrace/comprehension_tracing_lines.png");
    _guide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _guide->setPosition(Vec2(160, 284 + kGuideCorrectionY));
    TheGameNode->addChild(_guide);
    string directionContent = LanguageManager::getInstance()->isEnglish() ? "Trace the word." : "Fuatilia neno.";
    _comprehensionScene->drawQuestionTitle(directionContent, TheGameNode);

    TheTraceField = ([&] {
        TraceField* It = TraceField::createWithCustomCursor("ComprehensionTest/CompTrace/comprehension_tracing_icon.png");
        //TraceField* It = TraceField::create();
        
        It->cursorScaleNormal = 1.0;
        It->cursorScalePicked = 1.2;
        
        
        It->setContentSize(traceSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize / 2.f) + _traceFieldOffset);
        It->EnableDefaultBackground.update(false);
        It->ClippingEnabled.update(false);
        
        
        
//        auto l = LayerColor::create(Color4B::GRAY, traceSize.width, traceSize.height);
//        It->addChild(l);
        
        It->TheStyle.update([&] {
            float MintLineT = 10.f;
            Color4B MintLineC(156, 206, 167, 255);
            
            float WhiteLineT = 60.f;
            //Color4B WhiteLineC(255, 251, 176, GLubyte(255 * .50f));
            Color4B WhiteLineC(196, 193, 186, 230);
            
            float StrokeLineT = 60.f;
            Color4B StrokeLineC(56, 56, 56, 255);


            using LT = TraceLineStyle::Type;
            TraceFieldStyle Style;
            
            Style.ScrollLetterByLetter = true;
            Style.MainLineThickness = 130.f;
            
            // NB(xenosoz, 2016): Passive guide and stroke.
            Style.LineStyles.emplace_back(LT::PassiveGuide, WhiteLineT, WhiteLineC);
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeLineT, StrokeLineC);
            
            // NB(xenosoz, 2016): Active guide and stroke.
            Style.LineStyles.emplace_back(LT::ActiveGuide, WhiteLineT, WhiteLineC);
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeLineT, StrokeLineC);
            
            // NB(xenosoz, 2016): Active guide highlight.
            Style.LineStyles.emplace_back(LT::ActiveGuide, MintLineT, MintLineC);
            
            return Style;
        }());

        It->OnTraceWorkDidEnd = [this](TraceField*) {
            auto Guard = NodeScopeGuard(this);
            handleTraceWorkDidFinish();
        };

        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
}

void CompTraceScene::beginTraceWork() {
    // NB(xenosoz, 2016): Visual work.
    TheTraceField->setVisible(true);
    _guide->setVisible(true);
    
    
    
    if (TheTraceWork().Type == ProblemType::IMAGE)
    {
        // NB(xenosoz, 2016): The sound.
        float Delay = 0.f;
        const float DelayUnit = 1.2f;
        
        Delay += DelayUnit / 2.f;
        scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
            todoschool::comptrace::MainDepot().soundForWord(TheTraceWork().Text).play();
        }, Delay, "CompTraceScene::beginTraceWork::soundForWord");
    }
    else
    {
        float Delay = 0.f;
        const float DelayUnit = 1.2f;
        
        Delay += DelayUnit / 2.f;
        scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
            auto p = TheTraceWork();
            //TodoUtil::replace(p.QuestionSound, "m4a", "wav");
            string audioPath = _comprehensionScene->getBookFolder()+"/quiz/"+p.QuestionSound;
            bool audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            if (!audioExist) {
                audioPath = _comprehensionScene->getBookFolder()+"/page/"+p.QuestionSound;
                audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            }
            if (audioExist)
            {
                GameSoundManager::getInstance()->playEffectSoundVoiceOnly(audioPath);
            }
        }, Delay, "CompTraceScene::beginTraceWork::soundForWord");
    }
}

void CompTraceScene::handleTraceWorkDidFinish() {
    handleSuccess();
}

void CompTraceScene::handleSuccess() {

    TheTraceField->Enabled.update(false);
    
    // NB(xenosoz, 2016): The sound.
    float Delay = 0.f;
    const float DelayUnit = 1.2f;
    Delay += DelayUnit / 2.f;

    writePlayLog();

    if (TheTraceWork().Type == ProblemType::IMAGE)
    {
        if (_comprehensionScene)
        {
            _comprehensionScene->onSolve();
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
    else
    {
        this->runAction(Sequence::create(DelayTime::create(Delay), CallFunc::create([this](){
            
            string audioPath = _comprehensionScene->getBookFolder()+"/quiz/"+TheTraceWork().AnswerSound;
            bool audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            if (!audioExist) {
                audioPath = _comprehensionScene->getBookFolder()+"/page/"+TheTraceWork().AnswerSound;
                audioExist = FileUtils::getInstance()->isFileExist(audioPath);
            }
            if (audioExist)
            {
                GameSoundManager::getInstance()->playEffectSoundVoiceOnly(audioPath);
            }
            else
            {
                GameSoundManager::getInstance()->playEffectSoundVoiceOnly(_comprehensionScene->getBookFolder()+"/../Common/word/"+TheTraceWork().AnswerSound);
            }
            
        }), DelayTime::create(2.f), CallFunc::create([this](){
            if (_comprehensionScene)
            {
                _comprehensionScene->onSolve();
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
        }), nullptr));
    }
}


void CompTraceScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}

void CompTraceScene::onEnter() {
    Super::onEnter();
    beginTraceWork();
}

void CompTraceScene::onExit() {
    Super::onExit();
}

void CompTraceScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

void CompTraceScene::writePlayLog() {
    string answer = TheTraceWork().Text;
    StrictLogManager::shared()->game_Peek_Answer("ComprehensionTest", makeWorkPath(), answer, answer);
}

std::string CompTraceScene::makeWorkPath() {
    stringstream ss;
    ss << "ComprehensionTest";
    ss << "/" << _comprehensionScene->getBookName();
    ss << "/" << "comptrace";
    ss << "-" << _comprehensionScene->getCurrentProblem();

    return ss.str();
}



