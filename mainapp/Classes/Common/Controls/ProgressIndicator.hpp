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
#include <string>


USING_NS_CC;

class ProgressIndicator : public cocos2d::ui::Widget
{
protected:
    std::vector<Sprite*> _balls;

public:
    CREATE_FUNC(ProgressIndicator);
    virtual bool init() override;
    
    void setMax(int max, bool forEgg = false);
    void setCurrent(int current, bool complete = false);
    void setStatus(std::string status);
    
protected:
    
    std::string _completedBall, _currentBall, _missedBall, _futureBall;
    
    
};



#endif /* defined(__todomath__ProgressIndicator__) */
