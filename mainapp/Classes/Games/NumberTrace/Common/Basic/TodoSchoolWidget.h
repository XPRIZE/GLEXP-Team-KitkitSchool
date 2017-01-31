//
//  TodoSchoolWidget.h on Jun 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_BASIC_TODOSCHOOLWIDGET_H
#define TODOSCHOOL_BASIC_TODOSCHOOLWIDGET_H

#include <cocos/ui/UIWidget.h>

namespace todoschool {

template<typename Derived>
class TodoSchoolWidget : public cocos2d::ui::Widget {
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
        if (!cocos2d::ui::Widget::init()) { return false; }

        setTouchEnabled(true);
        return true;
    }

    void releaseUpEvent() override {
        cocos2d::ui::Widget::releaseUpEvent();
    }

    void cancelUpEvent() override {
        cocos2d::ui::Widget::cancelUpEvent();
    }
};

}

#endif  // !defined(TODOSCHOOL_BASIC_TODOSCHOOLWIDGET_H)
