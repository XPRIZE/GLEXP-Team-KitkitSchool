//
//  LightBall.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 9/10/17.
//
//

#include "cocos2d.h"

USING_NS_CC;


class LightBall : public Node
{
public:
    CREATE_FUNC(LightBall);
    
    void setup(int type);
    
    Sprite *_ball;
    int _type;
    
    
    
};
