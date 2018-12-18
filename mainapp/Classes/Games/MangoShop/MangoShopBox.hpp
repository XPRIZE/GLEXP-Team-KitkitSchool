//
//  Box.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#ifndef Box_hpp
#define Box_hpp

#include "cocos2d.h"
#include <stdio.h>
#include "Mango.hpp"
#include "MangoSlot.hpp"

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class Box : public Node
    {
    private:
    public:
        CREATE_FUNC(Box);
        bool init() override;
        Box();
        
    private:
        bool _isLandscape;
        bool _isShowCover;
        int _id;
        Sprite* _box;
        Sprite* _completeBox;
        Sprite* _cover;
        Sprite* _sticker;
        vector<MangoSlot*> _slots;
        Node* _shiningParticleNode;
        Sprite* _closeButton;
    public:
        bool isSlot;
        bool isSlotIsFull;
        bool isOnTray;
        bool isActive;
        bool isBoxMovable;
        void initMinusLayoutWithOrientation(bool isLandscape, bool isCompleteBox);
        void initWithOrientation(bool isLandscape);
        void isTransparency(bool isEnable);
        vector<MangoSlot*> getSlots();
        bool isShowCover();
        void showCover(bool isShow, bool animate);
        void initMangoSlots(int slotCount, bool visible);
        bool isFull();
        bool isEmpty();
        Vec2 origin;
        
        function<void()> onTouchBegan;
        function<void()> onTouchMoved;
        function<void()> onTouchEnded;
        function<void()> onCloseButtonTouchBegan;
        function<void()> onCloseButtonTouchEnded;
        function<void(Vec2 pos)> onStartParticle;
        function<void()> onStopParticle;
        
        void startParticle();
        void stopParticle();
        void bindingBoxEventListener();
        void bindingCloseButtonEventListener();
        
        void animateCloseButton(bool isClick);
    };

} // end namespace

#endif /* Box_hpp */
