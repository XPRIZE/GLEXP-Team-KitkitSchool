//
//  SplashScene.hpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/28/16.
//
//

#ifndef SplashScene_hpp
#define SplashScene_hpp

#include <stdio.h>

#include "cocos2d.h"

USING_NS_CC;

class SplashScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onEnter();
    
    void finishSplash(float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
    
    static std::string getLaunchString();
    static void clearLaunchString();
};

#endif /* SplashScene_hpp */
