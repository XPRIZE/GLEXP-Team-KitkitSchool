//
//  CoopScene.hpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 8/16/18
//
//

#pragma once

#include "cocos2d.h"
#include "ui/UIButton.h"
USING_NS_CC;

class Bird;
class LevelCurriculum;
class CoinTab;

namespace CoopSceneSpace {
    class Room;
};


class CoopScene: public Layer {
public:
    enum CoopType {
        CT_LITERACY,
        CT_MATH
    } _coopType;
    
    static const int LEVEL_SPECIAL_COURSE = 10;
    static const int LEVEL_FISH_PRESENT = 11;
    static const int LEVEL_COUNT_REGULAR_EGG = 10;

    static cocos2d::Scene* createScene(CoopType type);
    
    virtual bool init() override;
    void initWithType(CoopType type);
    
    
    virtual void onEnter() override;
    
    virtual void onEnterTransitionDidFinish() override;
    
    void confirmReset();
    void setupCoop();
    void setupDebug();
    
    
    CREATE_FUNC(CoopScene);
    
    
    
    
    
private:
    //std::vector<Bird*> birds;
    std::vector<CoopSceneSpace::Room*> _rooms;
    
    void hatchEgg(CoopSceneSpace::Room *room);
    void showDailyScene(std::string levelID);
    
    void checkLight();
    bool _holdCheckLight = false;
    bool _checkFishPresentLight = true;

    Node *_roofCover;
    Node *_coopView;
    Node *_debugView;
    Bird *_touchBird;
    
    int _currentZ;
    
    void zoomIn(int level, float duration=0);
    int _zoomLevel;
    bool _bFirstEnter;
    
    CoinTab *_coinTab;
};



