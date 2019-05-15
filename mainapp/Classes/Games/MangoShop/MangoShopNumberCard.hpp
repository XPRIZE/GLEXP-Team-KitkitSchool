//
//  NumberCard.hpp
//  KitkitSchool-mobile
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#ifndef NumberCard_hpp
#define NumberCard_hpp

#include "cocos2d.h"
#include <stdio.h>
#include <string>

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class NumberCard : public Node
    {
    private:
    public:
        CREATE_FUNC(NumberCard);
        bool init() override;
        NumberCard();
        
    private:
        string _numberString;
        Label* _label;
    public:
        bool isActive;
        static bool _isOtherMovingCard;
        function<void()> onTouchBegan;
        function<void()> onTouchMoved;
        function<void()> onTouchEnded;
        void initWithNumberString(string numberString, float scale);
        void setNumberString(string numberString);
        string getNumberString();
    };

} // end namespace

#endif /* NumberCard_hpp */
