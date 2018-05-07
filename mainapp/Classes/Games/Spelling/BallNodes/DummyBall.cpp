//
//  DummyBall.h -- A dummy ball for representing the correct answer / candidate
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DummyBall.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_SPELLING

namespace {
    string contentSkinSmall(BallKind BK) {
        string P = MainDepot().assetPrefix();
        
        switch (BK) {
            case BallKind::CorrectAnswer:
                return P + "/Balls/Spelling_crystalball_yellow_xsmall.png";
            case BallKind::ActiveAnswer:
                return P + "/Balls/Spelling_crystalball_blue_small.png";
            case BallKind::PassiveAnswer:
                return P + "/Balls/Spelling_crystalball_blue_small.png";
        }
        CCLOGERROR("Unsupported BallKind in %s", __PRETTY_FUNCTION__);
        return "";
    }

    string contentSkinLarge(BallKind BK) {
        string P = MainDepot().assetPrefix();
     
        switch (BK) {
            case BallKind::CorrectAnswer:
                return P + "/Balls/Spelling_crystalball_yellow.png";
            case BallKind::ActiveAnswer:
                return P + "/Balls/Spelling_crystalball_blue_large.png";
            case BallKind::PassiveAnswer:
                return P + "/Balls/Spelling_crystalball_blue_small.png";
        }
        CCLOGERROR("Unsupported BallKind in %s", __PRETTY_FUNCTION__);
        return "";
    }

    string contentSkin(BallSize BS, BallKind BK) {
        switch (BS) {
            case BallSize::Small:
                return contentSkinSmall(BK);
            case BallSize::Large:
                return contentSkinLarge(BK);
        }
        CCLOGERROR("Unsupported BallSize in %s", __PRETTY_FUNCTION__);
        return "";
    }
    
    float fontSize(BallSize BS, BallKind BK) {
        if (BS == BallSize::Small) {
            if (BK == BallKind::CorrectAnswer)
                return 160.f;
            if (BK == BallKind::ActiveAnswer)
                return 200.f;
            if (BK == BallKind::PassiveAnswer)
                return 200.f;
        }
        else if (BS == BallSize::Large) {
            if (BK == BallKind::CorrectAnswer || BK == BallKind::ActiveAnswer)
                return 210.f;
            if (BK == BallKind::PassiveAnswer)
                return 160.f;
        }

        assert(false);
        return 200.f;
    }
    
    Color4B textColor(BallSize _BS, BallKind BK) {
        if (BK == BallKind::CorrectAnswer) {
            return Color4B(128, 63, 25, 255);
        }
        if (BK == BallKind::ActiveAnswer || BK == BallKind::PassiveAnswer) {
            return Color4B(0, 73, 107, 255);
        }

        assert(false);
        return Color4B::BLACK;
    }
}  // unnamed namespace


bool DummyBall::init(BallSize BS, BallKind BK) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;
    TheBallKind = BK;
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}


void DummyBall::clearInternals() {
    setCascadeOpacityEnabled(true);

    Text.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void DummyBall::refreshChildNodes() {
    removeAllChildren();
    MainDepot Depot;
    
    [&] {
        auto It = Sprite::create(contentSkin(TheBallSize, TheBallKind));
        Size CS = It->getContentSize();
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        setContentSize(CS);
        addChild(It);
        return It;
    }();
    

    Size CS = getContentSize();
    
    [&] {
        auto It = Label::createWithTTF(Text(),
                                       Depot.defaultFont(), fontSize(TheBallSize, TheBallKind));
        It->setTextColor(textColor(TheBallSize, TheBallKind));

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS.width / 2.f, CS.height / 2.f - 10.f + (TheBallSize == BallSize::Large ? 25 : 20)));
        
        addChild(It);
        return It;
    }();
}


END_NS_SPELLING
