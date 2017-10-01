//
//  Card.cpp
//  enumaXprize
//
//  Created by HyeonGyu Yu on 9/5/17.
//
//

#include "Card.h"
#include "../Utils/MainDepot.h"
#include "Managers/GameSoundManager.h"
#include <Managers/LanguageManager.hpp>

BEGIN_NS_LETTERTRACINGCARD

using namespace todoschool::tracefield;

namespace
{
    const float kCardInnerMarginForLabelWidth = 100.f;
    
    const string kSmallFrontFaceImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-small_front.png";
    const string kSmallShadowImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-small_shadow.png";
    const string kSmallBackFaceImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-small_back.png";
    const string kSmallStencilImagePath = MainDepot().assetPrefix() + "/tracing-3_image_mask_small.png";
    const float kSmallHeightCorrection = -400.f;
    
    const string kLargeFrontFaceImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-large_front.png";
    const string kLargeShadowImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-large_shadow.png";
    const string kLargeBackFaceImagePath = MainDepot().assetPrefix() + "/tracing-3_image_card-large_back.png";
    const string kLargeStencilImagePath = MainDepot().assetPrefix() + "/tracing-3_image_mask_large.png";
    const float kLargeHeightCorrection = -480.f;
}

Card::Card():
onComplete(nullptr),
isComplete(false)
{
    
}

void Card::drawCardWithProblem(Problem* problem)
{
    word = problem->word;
    
    string frontFaceImagePath;
    string shadowImagePath;
    string backFaceImagePath;
    string stencilImagePath;
    float heightCorrection;
    
    if (problem->characters.size() > 1)
    {
        frontFaceImagePath = kLargeFrontFaceImagePath;
        shadowImagePath = kLargeShadowImagePath;
        backFaceImagePath = kLargeBackFaceImagePath;
        stencilImagePath = kSmallStencilImagePath;
        heightCorrection = kLargeHeightCorrection;
    }
    else
    {
        frontFaceImagePath = kSmallFrontFaceImagePath;
        shadowImagePath = kSmallShadowImagePath;
        backFaceImagePath = kSmallBackFaceImagePath;
        stencilImagePath = kSmallStencilImagePath;
        heightCorrection = kSmallHeightCorrection;
    }
    
    {
        frontFace = ImageView::create(frontFaceImagePath);
        frontFace->setVisible(true);
        frontFace->setTouchEnabled(true);
        setContentSize(frontFace->getContentSize());
        
        auto shadow = Sprite::create(shadowImagePath);
        shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        shadow->setPosition(Vec2::ZERO);
        addChild(shadow);
        
        frontFace->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        frontFace->setPosition(Vec2::ZERO);
        addChild(frontFace);
        
        traceWord = ([&] {
            TraceField* It = TraceField::createWithCustomCursor("ComprehensionTest/CompTrace/comprehension_tracing_icon.png");
            
            It->cursorScaleNormal = 1.0;
            It->cursorScalePicked = 1.2;
            
            float traceFieldScaleFactor = problem->characters.size() > 1 ? 0.8f : 0.8f;
            It->setContentSize(frontFace->getContentSize() * traceFieldScaleFactor);
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(frontFace->getContentSize().width / 2, frontFace->getContentSize().height / 2 - 65.f);
            It->EnableDefaultBackground.update(false);
            It->ClippingEnabled.update(false);
            
            It->TheStyle.update([&] {
                float MintLineT = 12.f;
                Color4B MintLineC(250, 243, 218, 255);
                
                float WhiteLineT = 100.f;
                Color4B WhiteLineC(81, 53, 24, 50);
                
                float StrokeLineT = 100.f;
                Color4B StrokeLineC(81, 53, 24, 255);
                
                using LT = TraceLineStyle::Type;
                TraceFieldStyle Style;
                
                Style.ScrollLetterByLetter = false;
                Style.MainLineThickness = 130.f;
                
                Style.LineStyles.emplace_back(LT::PassiveGuide, WhiteLineT, WhiteLineC);
                Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeLineT, StrokeLineC);
                
                Style.LineStyles.emplace_back(LT::ActiveGuide, WhiteLineT, WhiteLineC);
                Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeLineT, StrokeLineC);
                
                Style.LineStyles.emplace_back(LT::ActiveGuide, MintLineT, MintLineC);
                
                return Style;
            }());
            
            It->OnTraceWorkDidEnd = [this](TraceField*) {
                auto Guard = NodeScopeGuard(this);
                FlipToBack();
            };
            
            It->Placeholder.update(problem->characters);
            frontFace->addChild(It);
            return It;
        }());
    }
    
    {
        backFace = ImageView::create(backFaceImagePath);
        backFace->setVisible(false);
        backFace->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        backFace->setPosition(Vec2::ZERO);
        addChild(backFace);
        
        auto stencil = Sprite::create(stencilImagePath);
        auto clipper = ClippingNode::create(stencil);
        clipper->setInverted(false);
        clipper->setAlphaThreshold(0.5f);
        clipper->setScaleX(-1.f);
        clipper->setScaleY(1.f);
        clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        clipper->setPosition(backFace->getContentSize().width / 2, backFace->getContentSize().height / 2 + backFace->getContentSize().height / 2 + heightCorrection);
        backFace->addChild(clipper);
        
        string imagePath = LanguageManager::getInstance()->isEnglish() ? "/en/" : "/sw/";
//        string imagePath = LanguageManager::getInstance()->isEnglish() ? "/images.en_US/" : "/images.sw_TZ/";
        string word = problem->word;
        TodoUtil::replaceAll(word, "'", "1");
        string fullPath = MainDepot().assetPrefix() + imagePath + word + ".png";
        CCLOG("fullPath : %s", fullPath.c_str());
        Sprite* image = Sprite::create(fullPath.c_str());
//        Sprite* image = Sprite::create("LetterTracingCard/sw/anga.png");
        if (image == nullptr)
        {
            CCLOG("The image is nullptr.");
            image = Sprite::create(MainDepot().assetPrefix() + "/emptyImage.png");
        }
        
        Size imageSize = image->getContentSize();
        Size stencilMarginSize = Size(50.f , 50.f);
        Size stencilSize = stencil->getContentSize() + stencilMarginSize;
        
        float scaleFactor = MAX(stencilSize.width / imageSize.width, stencilSize.height / imageSize.height);
        image->setScale(scaleFactor);
        image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        image->setPosition(clipper->getContentSize() / 2);
        clipper->addChild(image);

        label = TodoUtil::createLabel(problem->word, 200.f, Size::ZERO, "fonts/TodoSchoolV2.ttf", Color4B(81, 53, 24, 255 * 0.9f));
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(backFace->getContentSize().width / 2, backFace->getContentSize().height / 2 - label->getContentSize().height / 2 - 300.f);
        if (label->getContentSize().width > backFace->getContentSize().width - 100.f)
        {
            float factor = (backFace->getContentSize().width - kCardInnerMarginForLabelWidth) / label->getContentSize().width;
            label->setScaleX(-factor);
            label->setScaleY(factor);
        }
        else
        {
            label->setScaleX(-1.f);
        }
        backFace->addChild(label);
    }
}

bool Card::init()
{
    return true;
}

void Card::FlipToBack()
{
    auto actionfloat = Sequence::create(DelayTime::create(0.3f), EaseInOut::create(ActionFloat::create(0.6f, 0.f, 180.f, [&, this](float value) {
        setRotation3D(Vec3(0.f, value, 0.f));
        if (value > 90.f)
        {
            frontFace->setVisible(false);
            backFace->setVisible(true);
        }
    }), 2.f), DelayTime::create(0.6f), CallFunc::create([this]() {
        auto scaleX = label->getScaleX();
        auto scaleY = label->getScaleY();
        
        label->setTextColor(Color4B(255, 0, 0, 255 * 0.9f));
        label->runAction(Sequence::create(ScaleTo::create(0.2f, scaleX * 1.1f, scaleY * 1.1f),
                                          DelayTime::create(0.2f),
                                          ScaleTo::create(0.1f, scaleX * 1.f, scaleY * 1.f), nullptr));
        label->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this](){
            label->setTextColor(Color4B(81, 53, 24, 255 * 0.9f));
        }), nullptr));
                                          
        if (LanguageManager::getInstance()->isEnglish())
        {
            GameSoundManager::getInstance()->playEffectSound(MainDepot().assetPrefix() + "/Sounds.en_US/" + word + ".wav");
        }
        else
        {
            TodoUtil::replaceAll(word, "'", "");
            GameSoundManager::getInstance()->playEffectSound(MainDepot().assetPrefix() + "/Sounds.sw_TZ/" + word + ".m4a");
        }
        
        isComplete = true;
        if (onComplete != nullptr)
        {
            onComplete();
        }
    }), nullptr);
    this->runAction(actionfloat);
}

END_NS_LETTERTRACINGCARD
