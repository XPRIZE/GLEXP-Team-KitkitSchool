//
//  CoopScene2.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 3/25/18.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/UIButton.h"
USING_NS_CC;

class Bird;
class LevelCurriculum;

namespace CoopScene2Space {
    class Room;
};


class CoopScene2: public Layer {
public:
    enum CoopType {
        CT_LITERACY,
        CT_MATH
    } _coopType;
    
    
    static cocos2d::Scene* createScene(CoopType type);
    
    virtual bool init() override;
    void initWithType(CoopType type);
    
    
    virtual void onEnter() override;
    
    virtual void onEnterTransitionDidFinish() override;
    
    void confirmReset();
    void setupCoop();
    void setupDebug();
    
    
    CREATE_FUNC(CoopScene2);
    


    

private:
    //std::vector<Bird*> birds;
    std::vector<CoopScene2Space::Room*> _rooms;
    

    
    void hatchEgg(CoopScene2Space::Room *room);
    void showDailyScene(std::string levelID);
    
    void checkLight();
    bool _holdCheckLight = false;
    
    Node *_roofCover;
    Node *_coopView;
    Node *_debugView;
    
    int _currentZ;
    
    

    
    
    
    
    
};


