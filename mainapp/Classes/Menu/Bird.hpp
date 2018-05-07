//
//  Bird.hpp
//  KitkitSchool
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
    
    static int getBirdID(char category, int level);
    static Vec2 getBirdAnchor(int id);
    
    
    
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
    
    void setupAnim(std::string animFilePrefix, int birdIndex, int numAni1Frames, int numAni2Frames, int numAniTouchFrames, float fps, float scale);
    
    int getBirdID() { return _birdID; }
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
    
    int _birdID;
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
