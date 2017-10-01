//
//  TodoLoadingScene.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/28/16.
//
//

#ifndef TodoLoadingScene_hpp
#define TodoLoadingScene_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
USING_NS_CC;


class TodoLoadingScene: public Layer {
public:
    static cocos2d::Scene* createScene(std::function<Scene*(void)> creator, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    //static cocos2d::Scene* createScene(Scene* nextScene, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    
    static TodoLoadingScene* create(std::function<Scene*(void)> &creator, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    //static TodoLoadingScene* create(Scene* &nextScene, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    
    virtual bool init(std::function<Scene*(void)> &creator, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    //virtual bool init(Scene* &nextScene, float loadingTime = 0.5f, float fadeOutTime = 0.5f);
    
    void onEnterTransitionDidFinish() override;
    
    virtual void update(float delta) override;
    
private:
    

    std::function<Scene*(void)> _sceneCreator;
    
    cocos2d::Scene* _nextScene;
    bool _nextSceneLoaded;
    bool _transitionFinished;
    
    
    float _loadingTime;
    float _fadeOutTime;
    
    float _currentTimer;
    
    
    
};

#endif /* TodoLoadingScene_hpp */
