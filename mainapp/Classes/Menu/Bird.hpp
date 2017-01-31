//
//  Bird.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/23/16.
//
//

#ifndef Bird_hpp
#define Bird_hpp

#include "cocos2d.h"
#include <functional>
#include "Common/Basic/SoundEffect.h"


USING_NS_CC;


class Bird : public Node {
public:
    enum BirdType {
        BIRD_L0 = 1,
        BIRD_M0,
        BIRD_L1,
        BIRD_L2,
        BIRD_L3,
        BIRD_L4,
        BIRD_L5,
        BIRD_M1,
        BIRD_M2,
        BIRD_M3,
        BIRD_M4,
        BIRD_M5,
        BIRD_MAX
    };
    
    enum BirdStatus {
        EGG = 0,
        EGG_HATCHED,
    };
    
    //static Bird* create(BirdType type, BirdStatus status);
    //bool init(BirdType type, BirdStatus status);
    
    static Bird* create(char category, int level, std::string levelID);
    bool init(char category, int level, std::string levelID);
    

    void setBirdProgress(float progress);
    void refreshSize();
   
    void setStatus(BirdStatus status);
    BirdStatus getStatus() {return _status;}
  
    
    void loadAnimation();
    
    void setupAnim(std::string animFilePrefix, int birdIndex, int numSheets, int numAni1Frames, int numAni2Frames, int numAniTouchFrames, float fps, float scale);
    
    BirdType getType() { return _type; }
    char getCategory() { return _category; }
    int getCategoryLevel() { return _categoryLevel; }
    std::string getLevelID() { return _levelID; }
    

    std::function<void()> onTouchBegan;
    std::function<void()> onTouchEnded;
    
    
    void runIdleAnimation();
    void runTouchAnimation();
    
    void runCrackAnimation();
    
    

    
    
protected:
    
    std::string _loadedAnimationSub;
    
    BirdType _type;
    BirdStatus _status;
    float _scale;
    float _progressScale;
    float _minProgressScale;
    char _category;
    int _categoryLevel;
    std::string _levelID;
    

    SoundEffect _sound;
    
    
    


    Vector<Animate*> animateArray;
    std::vector<Vector<SpriteFrame*>> aniFramesArray;
    

    Sprite* _aniSprite;
    Sprite* _eggSprite;


    
};
#endif /* Bird_hpp */
