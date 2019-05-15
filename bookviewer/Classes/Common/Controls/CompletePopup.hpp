//
//  CompletePopup.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/29/16.
//
//

#ifndef CompletePopup_hpp
#define CompletePopup_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class CompletePopup : public Node
{
public:
    CREATE_FUNC(CompletePopup);
    virtual bool init() override;
    
    void show(float delay, std::function<void(void)> callback);
    void dismiss();

    void setTitleText(const std::string& titleText);

protected:
    
    Node *_mainView;
    LayerColor *_backView;
    Label *_titleTextLabel;

    std::function<void(void)> _callback;
    
    
    
    
};
#endif /* CompletePopup_hpp */
