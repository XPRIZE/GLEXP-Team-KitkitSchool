//
//  TappingScene.hpp
//
//  Created by Gunho Lee on 12/10/16.
//
//

#ifndef TappingScene_hpp
#define TappingScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <functional>

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

class TappingBalloon;

class TappingScene : public Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;
    CREATE_FUNC(TappingScene);


    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void onStart();


    
    
    void setLevel(int level);

    
private:

    void placeBalloon(int tapCount, Rect placeRect,  bool isBubble = false, float delay = 0.f);
    void balloonFlown(TappingBalloon *balloon, bool touched = true);

private:
    Node *_bgNode;
    Node *_gameNode;

    int _currentLevel;
    ProgressIndicator *_progressBar;
    
    std::vector<TappingBalloon*> _balloons;
    int _popCount;
    int _accBallonCount;
    
    int _zOrder = 10;
 
};


class TappingBalloon : public Node
{
public:
    CREATE_FUNC(TappingBalloon);
    
    virtual bool init() override {
        if (!Node::init()) return false;
        
        OnTouched = nullptr;
        OnPoped = nullptr;
        _tapCount = 0;
        _maxTap = 0;
        setCascadeOpacityEnabled(true);
        
        return true;
        
    }
    
    
    void setupBubble();
    void setupBalloon(int maxTap, Color3B color);

    void runFlyAnimation();
    
    std::function<void()> OnTouched;
    std::function<void()> OnPoped;
    
    Vec2 getCenter();
    
    
    bool _touchEnabled;
    
    int _balloonId;
    int _tapCount;
    int _maxTap;

    
protected:
    
    void inflateBalloon(int count);
    
    bool _isBalloon;
    
    Rect _touchRect;
    Sprite *_body, *_shade, *_knot;
    
    
};


#endif /* TappingScene_hpp */
