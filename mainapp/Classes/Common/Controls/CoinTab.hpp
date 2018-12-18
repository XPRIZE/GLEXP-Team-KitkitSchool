//
//  CoinTab.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 9/18/17.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include <vector>
USING_NS_CC;

class CoinTab : public Node {
public:
    CREATE_FUNC(CoinTab);
    
    virtual bool init() override;
    void onEnter() override;
    void updateCoinLabel();
    
    void addCoinLabel(int add) { _numCoin+=add; setCoinLabel(_numCoin); }
    void setCoinLabel(int num);
    
    
    void addCoin(int numCoin, Vec2 fromPosWorld, bool modeGameSelect = false);
    void addCoinForSpecialCourse(int gameCount, int numCoin, Vec2 fromPosWorld, bool isCourseClear);

    void removeCoin(int numCoin, Vec2 toPosWorld, std::function<void()> callback = nullptr);
    
    static int _numCoin;
    Label *_coinLabel;
    
    Vector<SpriteFrame*> _frames;
    Animate *_animate;
    Animation *_animation;
    
    
};

