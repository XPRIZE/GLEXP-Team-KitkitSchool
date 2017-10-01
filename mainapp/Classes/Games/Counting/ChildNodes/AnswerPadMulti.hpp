//
//  AnswerPadMulti.hpp on Jul 29, 2016
//  enumaXprize
//
//  Extended by Taihyun Hwang 7/29/16 base on the work of
//  Created by Gunho Lee on 6/26/16.
//


#ifndef AnswerPadMulti_hpp
#define AnswerPadMulti_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "AnswerPadBase.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class AnswerPadMulti : public AnswerPadBase
{
public:
    CREATE_FUNC(AnswerPadMulti);
    virtual bool init() override;
    virtual void clearAnswer() override;
    
    void setAnswerDigitCount(int count);
    
protected:
    virtual void handleAnswerStringChanged();
    virtual void refreshAnswerLabels();

protected:
    int _answerDigitCount;
    std::string _answerString;

    Sprite *_answerPadBg;
    Label *_questionLabel;
    std::vector<Button*> _answerLabels;

    Node *_panelNode;
    Button *_zeroButton;
    Button *_deleteButton;
};

#endif /* AnswerPadMulti_hpp */
