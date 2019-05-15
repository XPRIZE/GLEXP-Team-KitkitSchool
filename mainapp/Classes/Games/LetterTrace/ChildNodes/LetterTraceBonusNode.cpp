//
//  BonusNode.cpp on Jul 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterTraceBonusNode.h"
#include "Common/Basic/DeviceSpec.h"
#include "Common/Basic/ScopeGuard.h"

using namespace cocos2d;
using namespace std;

namespace {
    const float TextFontSize = 500.f;
    
    // NB(xenosoz, 2016): assert(FullText... < TraceText...).
    const float FullTextNodeLocalZOrder = 1;
    const float TraceTextNodeLocalZOrder = 2;
    
    const char* formalFont = "fonts/Aileron-Regular.otf";
    
}

namespace todoschool {
namespace lettertrace {
    
bool BonusNode::init() {
    if (!Node::init()) { return false; }

    BonusPlayer = nullptr;
    FullTextNode = nullptr;
    TraceTextNode = nullptr;

    clear();
    return true;
}

void BonusNode::setVisible(bool Visible) {
    Node::setVisible(Visible);
    
    // NB(xenosoz, 2016): BonusPlayer contains special video player
    //   that cannot be hidden by the default parent-child relationship.
    BonusPlayer->setVisible(Visible);
    
    // NB(xenosoz, 2016): There's no point setting visibility of other nodes manually.
}

void BonusNode::clear() {
    clearInternals();

    Size GameSize = DeviceSpec::Google_Pixel_C().FullScreenResolution;
    //Size WindowSize = DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
    
    if (BonusPlayer)
        BonusPlayer->removeFromParent();
    
    BonusPlayer = ([&] {
        BonusVideoPlayer* It = BonusVideoPlayer::create();
        Size S = Size(1788.f, 1000.f);

        It->setContentSize(S);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height - 172.f));
        
        It->VideoFilePath.follow(VideoFilePath);
        It->OnPlayDidEnd = [this](BonusVideoPlayer*) {
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            if (OnBonusWorkDidEnd)
                OnBonusWorkDidEnd(this);
        };
        It->setVisible(isVisible());

        addChild(It);
        return It;
    }());
    
    updateTextNodes();
}

void BonusNode::startPlay() {
    BonusPlayer->play();
}

void BonusNode::clearInternals() {
    FullText.OnValueUpdate = [this](string&) {
        updateTextNodes();
    };
    
    TraceText.OnValueUpdate = [this](string&) {
        updateTextNodes();
    };
}

void BonusNode::updateTextNodes() {
    Size GameSize = DeviceSpec::Google_Pixel_C().FullScreenResolution;

    if (FullTextNode)
        FullTextNode->removeFromParent();

    FullTextNode = ([&] {
        // NB(xenosoz, 2016): Label::createWithTTF is very unstable with a large font size.
        Label* It = Label::createWithSystemFont(FullText(), formalFont, ::TextFontSize);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 43.f));
        It->setColor(Color3B(105, 68, 31));
        
        addChild(It, FullTextNodeLocalZOrder);
        return It;
    }());

    if (TraceTextNode)
        TraceTextNode->removeFromParent();
    
    TraceTextNode = ([&] {
        // NB(xenosoz, 2016): Label::createWithTTF is very unstable with a large font size.
        Label* It = Label::createWithSystemFont(TraceText(), formalFont, ::TextFontSize);
        Rect FullTextBB = (FullTextNode ? FullTextNode->getBoundingBox() : Rect());
        
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(FullTextBB.origin);
        It->setColor(Color3B(69, 45, 20));
        
        addChild(It, TraceTextNodeLocalZOrder);
        return It;
    }());
}

}  // namespace lettertrace
}  // namespace todoschool
