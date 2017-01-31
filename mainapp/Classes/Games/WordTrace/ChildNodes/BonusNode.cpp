//
//  BonusNode.cpp on Jul 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BonusNode.h"
#include "../Utils/MainDepot.h"
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>

using namespace cocos2d;
using namespace std;


namespace todoschool {
namespace wordtrace {

bool BonusNode::init() {
    if (!Super::init()) { return false; }
    
    BonusPlayer = nullptr;
    TextLabel = nullptr;
    
    clear();
    return true;
}

void BonusNode::setVisible(bool Visible) {
    Super::setVisible(Visible);
    
    // NB(xenosoz, 2016): BonusPlayer contains special video player
    //   that cannot be hidden by the default parent-child relationship.
    if (BonusPlayer)
        BonusPlayer->setVisible(Visible);
    
    // NB(xenosoz, 2016): There's no point setting visibility of other nodes manually.
}

void BonusNode::clear() {
    clearInternals();
    
    MainDepot Depot;
    Size GameSize = Depot.gameSize();
    
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

            if (OnBonusWorkDidFinish)
                OnBonusWorkDidFinish(*this);
        };
        It->setVisible(isVisible());
        
        addChild(It);
        return It;
    }());
    
    refreshTextNodes();
}

void BonusNode::startBonusWork() {
    if (!BonusPlayer) { return; }
    BonusPlayer->play();
}

void BonusNode::clearInternals() {
    Text.OnValueUpdate = [this](string&) {
        refreshTextNodes();
    };
}

void BonusNode::refreshTextNodes() {
    MainDepot Depot;
    Size GameSize = Depot.gameSize();
    
    if (TextLabel)
        TextLabel->removeFromParent();
    
    TextLabel = ([&] {
        BigLabel* It = Depot.createBonusTextLabel(Text());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 43.f));
        
        addChild(It);
        return It;
    }());
}

}  // namespace wordtrace
}  // namespace todoschool
