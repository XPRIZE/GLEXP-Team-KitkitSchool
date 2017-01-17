//
//  ProgressIndicator.hpp
//
//  Created by Gunho Lee on 6/28/16.
//
//

#ifndef __todomath__ProgressIndicator__
#define __todomath__ProgressIndicator__

#include "cocos2d.h"
#include <cocos/ui/CocosGUI.h>
#include <vector>

USING_NS_CC;

class ProgressIndicator : public cocos2d::ui::Widget
{
protected:
    std::vector<Sprite*> _balls;

public:
    CREATE_FUNC(ProgressIndicator);
    virtual bool init() override;
    
    void setMax(int max);
    void setCurrent(int current, bool complete = false);
    
    
};



#endif /* defined(__todomath__ProgressIndicator__) */
