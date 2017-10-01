//
//  AnswerPadSingle.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/26/16.
//
//

#ifndef AnswerPadSingle_hpp
#define AnswerPadSingle_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "AnswerPadBase.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class AnswerPadSingle : public AnswerPadBase
{
public:
    CREATE_FUNC(AnswerPadSingle);
    virtual bool init() override;
    virtual void clearAnswer() override;
    
protected:
    Label *_questionLabel;
    Label *_answerLabel;
    
    Node *_panelNode;
    Button *_enterButton;

    
    
};

#endif /* AnswerPadSingle_hpp */
