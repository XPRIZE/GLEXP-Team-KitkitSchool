//
//  Mango.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#ifndef Mango_hpp
#define Mango_hpp

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;
using namespace std;

class MangoSlot;

namespace MangoShop
{
    class Mango : public Node
    {
    private:
    public:
        CREATE_FUNC(Mango);
        bool init() override;
        Mango();
        
    private:
        int _id;
        Sprite *_body;
    public:
        bool isActive;
        static bool _isOtherMovingCard;
        MangoSlot *targetSlot;
        Vec2 origin;
        function<void()> onTouchBegan;
        function<void(Vec2 delta)> onTouchMoved;
        function<void()> onTouchEnded;
        void initWithOrientation(bool isLandscape);
    };
    
} // end namespace

#endif /* Mango_hpp */
