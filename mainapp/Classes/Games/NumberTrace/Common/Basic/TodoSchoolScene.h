//
//  TodoScene.h on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_BASIC_TODOSCENE_H
#define TODOSCHOOL_BASIC_TODOSCENE_H

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/Basic/Cocos2dFwd.h>
#include <cocos/2d/CCScene.h>
#include <memory>

namespace todoschool {

template<typename Derived>
class __attribute__((deprecated)) TodoSchoolScene : public cocos2d::Scene {
public:
    // NB(xenosoz, 2016): Consult cocos2d::Node::onEnter family.
    Chain<bool> IsInStage;
    Chain<bool> HasFocus;

public:
    static Derived* create() {
        std::unique_ptr<Derived> Ret(new(std::nothrow) Derived());
        if (Ret && Ret->init())
            Ret->autorelease();
        else
            Ret.reset(nullptr);
        return Ret.release();
    }

    static Derived* createWithSize(const cocos2d::Size &size) {
        std::unique_ptr<Derived> Ret(new(std::nothrow) Derived());
        if (Ret && Ret->initWithSize(size))
            Ret->autorelease();
        else
            Ret.reset(nullptr);
        return Ret.release();
    }

    bool init() override {
        return cocos2d::Scene::init();
    }

    virtual bool initWithSize(const cocos2d::Size& S) {
        return cocos2d::Scene::initWithSize(S);
    }
    
    void onEnter() override {
        cocos2d::Scene::onEnter();
        IsInStage.update(true);
    }

    void onEnterTransitionDidFinish() override {
        cocos2d::Scene::onEnterTransitionDidFinish();
        HasFocus.update(true);
    }

    void onExit() override {
        cocos2d::Scene::onExit();
        IsInStage.update(false);
    }

    void onExitTransitionDidStart() override {
        cocos2d::Scene::onExitTransitionDidStart();
        HasFocus.update(false);
    }
};

}

#endif //!defined(TODOSCHOOL_BASIC_TODOSCENE_H)
