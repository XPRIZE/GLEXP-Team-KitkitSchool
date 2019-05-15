//
//  BlankSlot.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/9/18.
//

#ifndef BlankSlot_hpp
#define BlankSlot_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class BlankSlot : public Node
    {
    private:
    public:
        CREATE_FUNC(BlankSlot);
        bool init() override;
        BlankSlot();
        
    private:
        Sprite* _body;
    public:
        string numberString;
        bool isDone;
        void setSelectEffect(bool selected);
    };
    
} // end namespace

#endif /* BlankSlot_hpp */
