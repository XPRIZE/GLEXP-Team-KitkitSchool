//
//  DottedRect.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 1/6/17.
//
//

#ifndef DottedRect_hpp
#define DottedRect_hpp

#include <stdio.h>

#include "cocos2d.h"

using namespace cocos2d;

class DottedRect : public Node
{
public:
    static DottedRect* create(Size prefferedSize);
    virtual bool init(Size prefferedSize);
    
    

    
private:
    
    Node *_face;
    
    
    
};


#endif /* DottedRect_hpp */
