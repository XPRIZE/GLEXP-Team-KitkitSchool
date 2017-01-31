//
//  LittleButterfly.cpp on May 18, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "_LittleButterfly.h"
#include "../Utils/CountFieldDepot.h"
#include <cocos/cocos2d.h>
#include <functional>

using namespace cocos2d;
using namespace std;

namespace todoschool {
namespace countfield {

bool LittleButterfly::init() {
    if (!Super::init()) { return false; }

    setContentSize(Size(256.f, 256.f));

    clear();
    return true;
}

void LittleButterfly::clear() {
    //SpriteFrameCache* Cache = SpriteFrameCache::getInstance();
    //Cache->addSpriteFramesWithFile("assetfactory.plist");
    //Cache->getSpriteFrameByName(getAssetPrefix() + "/Objects/Butterfly.0.png");

    CountFieldDepot CFD;

    Rect R(0.f, 0.f, 256.f, 256.f);
    Vector<SpriteFrame*> Frames;
    Frames.pushBack(SpriteFrame::create(CFD.assetPrefix() + "/Objects/Little_Butterfly/LittleButterfly.0.png", R));
    Frames.pushBack(SpriteFrame::create(CFD.assetPrefix() + "/Objects/Little_Butterfly/LittleButterfly.1.png", R));

    if (TheSprite)
        TheSprite->stopAction(TheAnimation);

    TheAnimation = ([&]{
        float Duration = 2.f;
        float FlipPerSeconds = 3.f;
        float Frequency = 1.f / (Frames.size() * FlipPerSeconds);

        Spawn* All = ([&]{
            Repeat* Flap = Repeat::create(Animate::create(Animation::createWithSpriteFrames(Frames, Frequency)), 6);
            MoveBy* Move = MoveBy::create(Duration, Vec2(200.f, 100.f));
            FadeOut* Fade = FadeOut::create(Duration);

            return Spawn::create(Flap, Move, Fade, nullptr);
        }());

        CallFunc* Callback = CallFunc::create([&]{
            if (OnAnimationDidEnd)
                OnAnimationDidEnd(this);
        });

        return Sequence::create(All, Callback, nullptr);
    }());

    if (TheSprite)
        TheSprite->removeFromParent();

    TheSprite = ([&]{
        Sprite* It = Sprite::createWithSpriteFrame(Frames.front());
        It->setPosition(Point(0, 0));
        It->setScale(2.f);
        It->setBlendFunc((BlendFunc){GL_ZERO, GL_ONE_MINUS_SRC_ALPHA});
        addChild(It);
        return It;
    }());

    // NB(xenosoz, 2016): Active.update(false) may cause an infinite loop!
    Active.OnValueUpdate = bind(&LittleButterfly::handleActiveValueUpdated, this, placeholders::_1);
    Active() = false;
}

void LittleButterfly::handleActiveValueUpdated(bool &) {
    if (Active.value()) {
        TheSprite->setBlendFunc((BlendFunc){GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        TheSprite->runAction(TheAnimation);
    }
    else {
        TheSprite->stopAction(TheAnimation);
        clear();
    }
}

}  // namespace countfield
}  // namespace todoschool
