//
//  AnswerPadBase.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/27/16.
//
//

#ifndef AnswerPadBase_hpp
#define AnswerPadBase_hpp

#include <stdio.h>
#include <functional>

#include "cocos2d.h"

USING_NS_CC;

class AnswerPadBase : public Node
{
public:

    virtual bool init() override;
    virtual void clearAnswer() = 0;

    void setAnswerCallback(const std::function<void(int)> &func) { _answerCallback = func; }
    
    void setTouchEnabled(bool enabled) { _touchEnabled = enabled; }
    
protected:
    std::function<void(int)> _answerCallback;
    bool _touchEnabled;
    
};

#endif /* AnswerPadBase_hpp */
