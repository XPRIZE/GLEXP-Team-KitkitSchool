//
//  GreenDottedRect.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 1/6/17.
//
//

#ifndef GreenDottedRect_hpp
#define GreenDottedRect_hpp

#include <stdio.h>

#include "cocos2d.h"

using namespace cocos2d;

class GreenDottedRect : public Node
{
public:
    static GreenDottedRect* create(Size prefferedSize);
    virtual bool init(Size prefferedSize);
    
    

    
private:
    
    Node *_face;
    
    
    
};


#endif /* GreenDottedRect_hpp */
