//
//  CountObject.h on Jun 29, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef ENUMAXPRIZE_COUNTFIELD_COUNTOBJECT_H
#define ENUMAXPRIZE_COUNTFIELD_COUNTOBJECT_H


#include "../Kinematics/RotateAndForward.h"
#include "../Kinematics/CircleAndStraight.h"
#include "../Models/Pose.h"
#include <Common/ADT/Chain.h>
#include <cocos/cocos2d.h>

namespace todoschool {
namespace countfield {

class CountObject: public cocos2d::Node {
    typedef cocos2d::Node Super;

protected:
    const float Speed;
    const float AngularSpeed;

    cocos2d::RefPtr<cocos2d::Sprite> GlowSprite;
    cocos2d::RefPtr<cocos2d::Sprite> WalkSprite;
    cocos2d::RefPtr<cocos2d::Sprite> RestSprite;

    cocos2d::RefPtr<cocos2d::Action> WalkAction;
    cocos2d::RefPtr<cocos2d::Action> RestAction;

    cocos2d::RefPtr<cocos2d::Action> MoveInAction;
    cocos2d::RefPtr<cocos2d::Action> MoveToAction;
    cocos2d::RefPtr<cocos2d::Action> MoveOutAction;

public:
    // NB(xenosoz, 2016): Relative position to the virtual node.
    Chain<Pose> ThePose;
    Chain<GLubyte> Opacity;

    // NB(xenosoz, 2016): The object responds to user events only when it's enabled.
    Chain<bool> Enabled;

    Chain<bool> Moving;
    Chain<float> Scale;
    
    std::function<void(CountObject*)> OnTouchDidBegin;
    std::function<void(CountObject*)> OnTouchDidEnd;
    std::function<void(CountObject*)> OnTouchDidCancel;

    std::function<void(CountObject*)> OnTouchUpInside;
    std::function<void(CountObject*)> OnMovingDidEnd;

public:
    CountObject();
    bool init() override;
    virtual void clear();
    
    void jumpTo(const Pose& TargetPose);
    void moveTo(const Pose& TargetPose);
    void moveIn(const Pose& TargetPose, float TotalDuration, float FadeDuration);
    void moveOut(const Pose& TargetPose, float TotalDuration, float FadeDuration);
    void rest();
    void glow(bool on);

    cocos2d::Sprite* activeSprite();
    
private:
    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;
    
    bool handleTouchBegan(cocos2d::Touch* T, cocos2d::Event* E);
    void handleTouchMoved(cocos2d::Touch* T, cocos2d::Event* E);
    void handleTouchEnded(cocos2d::Touch* T, cocos2d::Event* E);
    void handleTouchCancelled(cocos2d::Touch* T, cocos2d::Event* E);
    
    void clearInternals();
    void handleEngineWorkingValueUpdated(bool&);
    void handleThePoseValueUpdated(Pose&);
    void handleOpacityValueUpdated(GLubyte&);
    void handleTargetPositionValueUpdated(cocos2d::Point&);
    void handleTargetRotationValueUpdated(float&);
    void handleMovingValueUpdated(bool&);
    void handleScaleValueUpdated(float&);
};

}  // namespace countfield
}  // namespace todoschool


#endif  // !defined(ENUMAXPRIZE_COUNTFIELD_COUNTOBJECT_H)
