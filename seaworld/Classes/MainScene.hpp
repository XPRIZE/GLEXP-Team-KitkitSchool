//
//  MainScene.hpp
//  KitKit_FB
//
//  Created by jesun kim on 12/09/2018.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace ui;
using namespace std;

class MainScene : public Layer {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
private:

};

#endif /* MainScene_hpp */
