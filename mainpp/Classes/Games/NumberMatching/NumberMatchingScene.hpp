//
//  GameScene.hpp
//  Matching
//
//  Created by sugar on 5/18/16.
//
//

#ifndef NumberMatchingScene_hpp
#define NumberMatchingScene_hpp

#include <stdio.h>
#include "NumberMatchingCard.hpp"
#include "Models/LevelData.hpp"
#include "Models/Worksheet.hpp"
#include "Models/Problem.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

class NumberMatchingScene : Layer
{
public:
    static Scene* createScene(int levelID);
    virtual bool init() override;
    //virtual void update(float) override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    void setLevel(int levelID);
    
    CREATE_FUNC(NumberMatchingScene);
    
private:
    float _defaultScaleFactor;
    float _upScaleFactor;

    int _currentLevelID;
    todoschool::numbermatching::Worksheet _currentWorksheet;
    int _currentProblemID;
    todoschool::numbermatching::Problem _currentProblem;

    ProgressIndicator *_progressBar;
    Node* _gameNode;
    //bool bActiveParticle = false;
    //bool bLinked = false;
    //NumberMatchingCard* linkedSourceCard = nullptr;
    //NumberMatchingCard* linkedTargetCard = nullptr;
    int _currentLevelMaxCardCount = 0;
    int _matchCount = 0;
    int _zOrder = 10;
    ParticleSystemQuad* _particleEffect;
    //Node* _shiningParticleArray[2];
    cocos2d::ui::Button* _nextButton;
    
    
    NumberMatchingCard* createMatchingCard(int zOrder, Point pos,
                                           int level, int id, int type,
                                           const std::string& cardImageName);
    void bindingEvents(NumberMatchingCard* card);
    void initCardList();
    /*
    void preloadShiningParticle();
    void startParticle();
    void stopParticle();
    Node* createShiningParticle();
    */
    void addStarParticle(Node* targetNode);
    
    bool _nextTransition;
    
public:
    std::vector<NumberMatchingCard*> matchingCardList;
    
    void onStart();
    void onSolve();
    void advanceToNextProblem();
    bool touchBeganHandler(Touch* touch, Event* event);
    void touchMovedHandler(Touch* touch, Event* event);
    void touchEndedHandler(Touch* touch, Event* event);
};
#endif /* NumberMatchingScene_hpp */
