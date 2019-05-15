//
//  Tray.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#ifndef Tray_hpp
#define Tray_hpp

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;
using namespace std;

namespace MangoShop
{
    class Tray : public Node
    {
    private:
    public:
        CREATE_FUNC(Tray);
        bool init() override;
        Tray();
        
    private:
    public:
    };
    
} // end namespace


#endif /* Tray_hpp */
