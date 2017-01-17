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
#include "ui/uiButton.h"
USING_NS_CC;

class Bird;
class CoopScene: public Layer {
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void createBirds();
    
    CREATE_FUNC(CoopScene);
    


    

private:
    std::vector<Bird*> birds;


    
    void hatchEgg(Bird *bird);
    void showDaySelectPopup(std::string levelID);
    
    void checkLevelClear();
    
    
    Node *_coopView;
    Node *_debugView;
    
    int _currentZ;
    
    
    
};



#endif /* CoopScene_hpp */
