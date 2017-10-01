//
//  CountObject.cpp on Jun 29, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "CountObject.h"
#include "../Utils/CountFieldDepot.h"
#include "../Kinematics/WalkIn.h"
#include "../Kinematics/WalkOut.h"
#include <Games/NumberTrace/Common/Repr/AllRepr.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include <functional>

using namespace cocos2d;
using namespace std;


namespace todoschool {
namespace countfield {

CountObject::CountObject()
//    : Speed(700.f), AngularSpeed(M_PI_2)
    : Speed(1400.f), AngularSpeed(M_PI_2)
{
}

bool CountObject::init() {
    if (!Super::init()) { return false; }
    

    
    clear();
    return true;
}

void CountObject::clear() {
    clearInternals();

    Opacity.update(255);

    rest();
}

void CountObject::jumpTo(const Pose& TargetPose) {
    ThePose.update(TargetPose);
    Moving.update(false);
}

void CountObject::moveTo(const Pose& TargetPose) {
    Moving.update(true);
    
    auto Guard = NodeScopeGuard(this);

    ThePose.update(TargetPose);
    Moving.update(false);
    if (OnMovingDidEnd)
        OnMovingDidEnd(this);
}

void CountObject::moveIn(const Pose& TargetPose, float TotalDuration, float FadeDuration) {
    auto Guard = NodeScopeGuard(this);

    if (!WalkSprite) {
        Moving.update(false);
        if (OnMovingDidEnd)
            OnMovingDidEnd(this);
        return;
    }

    Moving.update(true);
    WalkSprite->stopAction(MoveInAction);
    WalkSprite->stopAction(MoveOutAction);

    Pose CurrentPose = ([&] {
        Point P = WalkSprite->getPosition();
        float R = MATH_DEG_TO_RAD(90.f - WalkSprite->getRotation());

        Pose It(P, R);
        return It;
    })();
    
    MoveInAction = ([&] {
        Sequence* FI = Sequence::create(FadeIn::create(FadeDuration),
                                      DelayTime::create(TotalDuration - FadeDuration),
                                      nullptr);
        WalkIn* WI = WalkIn::create(TotalDuration,
                                    CurrentPose, TargetPose,
                                    Scale(),
                                    Speed, AngularSpeed);

        CallFunc* CF = CallFunc::create([this] {
            Point P = WalkSprite->getPosition();
            float R = MATH_DEG_TO_RAD(90.f - WalkSprite->getRotation());
            ThePose.update(Pose(P, R));
            Opacity.update(WalkSprite->getOpacity());
            Moving.update(false);

            if (OnMovingDidEnd)
                OnMovingDidEnd(this);
        });

        Action* It = Sequence::create(Spawn::create(FI, WI, nullptr),
                                      CF,
                                      nullptr);
        return It;
    })();
        

    if (WalkSprite)
        WalkSprite->runAction(MoveInAction);
}

void CountObject::moveOut(const Pose& TargetPose, float TotalDuration, float FadeDuration) {
    auto Guard = NodeScopeGuard(this);

    if (!WalkSprite) {
        Moving.update(false);
        if (OnMovingDidEnd)
            OnMovingDidEnd(this);
        return;
    }
    
    Moving.update(true);
    WalkSprite->stopAction(MoveInAction);
    WalkSprite->stopAction(MoveOutAction);
    
    Pose CurrentPose = ([&] {
        Point P = WalkSprite->getPosition();
        float R = MATH_DEG_TO_RAD(90.f - WalkSprite->getRotation());
        
        Pose It(P, R);
        return It;
    })();
    
    MoveOutAction = ([&] {
        Sequence* FO = Sequence::create(DelayTime::create(TotalDuration - FadeDuration),
                                        FadeOut::create(FadeDuration),
                                        nullptr);
        WalkOut* WO = WalkOut::create(TotalDuration,
                                      CurrentPose, TargetPose,
                                      Scale(),
                                      Speed, AngularSpeed);
        
        CallFunc* CF = CallFunc::create([this] {
            Point P = WalkSprite->getPosition();
            float R = MATH_DEG_TO_RAD(90.f - WalkSprite->getRotation());
            ThePose.update(Pose(P, R));
            Opacity.update(WalkSprite->getOpacity());
            Moving.update(false);
            
            if (OnMovingDidEnd)
                OnMovingDidEnd(this);
        });
        
        Action* It = Sequence::create(Spawn::create(FO, WO, nullptr),
                                      CF,
                                      nullptr);
        return It;
    })();
    
    
    if (WalkSprite)
        WalkSprite->runAction(MoveOutAction);
}

void CountObject::rest() {
    Moving.update(false);
}

void CountObject::glow(bool on) {
    
    if (on) {
        if (!GlowSprite) {
            GlowSprite = Sprite::create("NumberTrace/daily_mango_active_number_glow.png");
            addChild(GlowSprite);
            GlowSprite->setPosition(ThePose->Position);
            GlowSprite->setOpacity(225);
            auto fitScale = WalkSprite->getContentSize().width / GlowSprite->getContentSize().width;
            GlowSprite->setScale(0);
            
            this->runAction(Sequence::create(DelayTime::create(random(0.1, 2.0)),
                                             CallFunc::create([this, fitScale](){
                if (!GlowSprite) return;
                auto seq = Sequence::create(
                                            EaseOut::create(ScaleTo::create(0.8, fitScale*0.9), 2.0),
                                            EaseIn::create(ScaleTo::create(0.8, 0), 2.0),
                                            DelayTime::create(random(2.5, 3.8)),
                                            nullptr);
                
                GlowSprite->runAction(RepeatForever::create(seq));
            }), NULL));
            
        }
    } else {
        if (GlowSprite) GlowSprite->removeFromParent();
        GlowSprite = nullptr;
    }
}
    
    
Sprite* CountObject::activeSprite() {
    return (Moving() ? WalkSprite.get() : RestSprite.get());
}

void CountObject::onEnter() {
    Super::onEnter();
    
    // Install event listener.
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CountObject::handleTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CountObject::handleTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CountObject::handleTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CountObject::handleTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void CountObject::onExit() {
    Super::onExit();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    unscheduleUpdate();
}

void CountObject::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);
}

bool CountObject::handleTouchBegan(cocos2d::Touch* T, cocos2d::Event* E) {
    const bool CATCH_EVENT = true;
    const bool DONT_CATCH_EVENT = false;
    
    // We don't catch any touch events when deactivated.
    if (!Enabled())
        return DONT_CATCH_EVENT;
    
    // We don't catch any touch events from outside.
    Point PointInParentSpace = activeSprite()->getParent()->convertToNodeSpace(T->getLocation());
    if (!activeSprite()->getBoundingBox().containsPoint(PointInParentSpace))
        return DONT_CATCH_EVENT;
    
    //return CATCH_EVENT;
    auto Guard = NodeScopeGuard(this);
    
    glow(false);
    
    if (OnTouchUpInside)
        OnTouchUpInside(this);

    return DONT_CATCH_EVENT;
}

void CountObject::handleTouchMoved(cocos2d::Touch* T, cocos2d::Event* E) {
    // Do nothing.
}

void CountObject::handleTouchEnded(cocos2d::Touch* T, cocos2d::Event* E) {
    Point PointInParentSpace = activeSprite()->getParent()->convertToNodeSpace(T->getLocation());
    if (!activeSprite()->getBoundingBox().containsPoint(PointInParentSpace))
        return;

//    auto Guard = NodeScopeGuard(this);
//
//    if (OnTouchUpInside)
//        OnTouchUpInside(this);
}

void CountObject::handleTouchCancelled(cocos2d::Touch* T, cocos2d::Event* E) {
    // Do nothing.
}

void CountObject::clearInternals() {
    using std::placeholders::_1;
    
    setContentSize(Size::ZERO);
    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    Enabled.update(true);
    Moving.update(false);
    Scale.update(1.f);
    
    // Value update handlers.
    ThePose.OnValueUpdate = bind(&CountObject::handleThePoseValueUpdated, this, _1);
    Opacity.OnValueUpdate = bind(&CountObject::handleOpacityValueUpdated, this, _1);
    Moving.OnValueUpdate = bind(&CountObject::handleMovingValueUpdated, this, _1);
    Scale.OnValueUpdate = bind(&CountObject::handleScaleValueUpdated, this, _1);
}

void CountObject::handleThePoseValueUpdated(Pose&) {
    Point PositionInNodeSpace = ThePose().Position;
    float AngleInNodeSpace = 90.f - MATH_RAD_TO_DEG(ThePose().Rotation);

    if (WalkSprite) {
        WalkSprite->setPosition(PositionInNodeSpace);
        WalkSprite->setRotation(AngleInNodeSpace);
    }
    
    if (RestSprite) {
        RestSprite->setPosition(PositionInNodeSpace);
        RestSprite->setRotation(AngleInNodeSpace);
    }
}
    
void CountObject::handleOpacityValueUpdated(GLubyte&) {
    if (WalkSprite)
        WalkSprite->setOpacity(Opacity());

    if (RestSprite)
        RestSprite->setOpacity(Opacity());
}

void CountObject::handleMovingValueUpdated(bool&) {
    if (WalkSprite)
        WalkSprite->setVisible(Moving());
    
    if (RestSprite)
        RestSprite->setVisible(!Moving());
}

void CountObject::handleScaleValueUpdated(float &) {
    if (WalkSprite)
        WalkSprite->setScale(Scale());
    
    if (RestSprite)
        RestSprite->setScale(Scale());
}

}  // namespace countfield
}  // namespace todoschool
