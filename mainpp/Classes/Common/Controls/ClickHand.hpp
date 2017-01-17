//
//  ClickHand.hpp
//  todomath
//
//  Created by Gunho Lee on 2/15/16.
//
//

#ifndef ClickHand_hpp
#define ClickHand_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class ClickHand : public Node
{
public:
    CREATE_FUNC(ClickHand)
    virtual bool init() override;
    

    void startAnimation();
    void stopAnimation();
    void pressAnimation(bool animate);
    void releaseAnimation(bool animate);
    
private:
    Sprite *_handView;
    
    
    
};


#endif /* ClickHand_hpp */
