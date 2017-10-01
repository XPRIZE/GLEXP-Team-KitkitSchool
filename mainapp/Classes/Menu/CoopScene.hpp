//
//  CoopScene.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#ifndef CoopScene_hpp
#define CoopScene_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
USING_NS_CC;

class Bird;
class LevelCurriculum;

namespace CoopSceneSpace {
    class Room;
};


class CoopScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    virtual void onEnterTransitionDidFinish() override;
    
    void confirmReset();
    void setupCoop();
    
    CREATE_FUNC(CoopScene);
    


    

private:
    //std::vector<Bird*> birds;
    std::vector<CoopSceneSpace::Room*> _rooms;
    

    void zoomIn(int level);
    
    void hatchEgg(CoopSceneSpace::Room *room);
    void showDailyScene(std::string levelID);
    
    void checkLight();
    
    
    Node *_roofCover;
    Node *_coopView;
    Node *_debugView;
    
    int _currentZ;
    int _zoomLevel;
    

    
    
    
    
    
};



#endif /* CoopScene_hpp */
