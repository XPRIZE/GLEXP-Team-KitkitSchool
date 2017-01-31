//
//  TodoSchoolNode.h on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_BASIC_TODOSCHOOLNODE_H
#define TODOSCHOOL_BASIC_TODOSCHOOLNODE_H

#include <cocos/2d/CCNode.h>

namespace todoschool {

// NB(xenosoz, 2016): If you hit the multiple inheritance problem,
//   consider using TodoSchoolNodeMixin instead.

template<typename Derived>
class __attribute__((deprecated)) TodoSchoolNode : public cocos2d::Node {
public:
    static Derived* create() {
        std::unique_ptr<Derived> Ret(new(std::nothrow) Derived());
        if (Ret && Ret->init())
            Ret->autorelease();
        else
            Ret.reset(nullptr);
        return Ret.release();
    }

    bool init() override {
        return cocos2d::Node::init();
    }
};

}

#endif //!defined(TODOSCHOOL_BASIC_TODOSCHOOLNODE_H)
