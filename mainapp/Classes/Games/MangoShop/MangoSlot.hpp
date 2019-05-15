//
//  MangoSlot.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#ifndef MangoSlot_hpp
#define MangoSlot_hpp

#include "cocos2d.h"
#include <stdio.h>
#include "Mango.hpp"

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class Box;
    
    class MangoSlot : public Node
    {
    private:
    public:
        CREATE_FUNC(MangoSlot);
        bool init() override;
        MangoSlot();
        
    private:
        int _id;
        Sprite *_body;
    public:
        bool isDraggable;
        Box *parentBox;
        Mango *targetMango;
        void initWithOrientation(bool isLandscape);
    };
    
} // end namespace

#endif /* MangoSlot_hpp */
