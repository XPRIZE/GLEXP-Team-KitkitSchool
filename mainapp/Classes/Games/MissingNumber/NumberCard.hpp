//
//  NumberCard.hpp
//  KitkitSchool-mobile
//
//  Created by YongSoo Hong on 03/29/18
//

#ifndef MissingNumber_NumberCard_hpp
#define MissingNumber_NumberCard_hpp

#include "cocos2d.h"
#include <stdio.h>
#include <string>

USING_NS_CC;
using namespace std;

namespace MissingNumber
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
        static bool isActive;
        function<void()> onTouchBegan;
        function<void()> onTouchMoved;
        function<void()> onTouchEnded;
        void initWithNumberString(string numberString, bool bSmall, bool bMove = false, float scale = 1.0f);
        void setNumberString(string numberString);
        string getNumberString();
    };

} // end namespace

#endif /* MissingNumber_NumberCard_hpp */
