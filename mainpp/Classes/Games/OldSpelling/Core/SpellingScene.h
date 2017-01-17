//
//  SpellingScene.h on Aug 10, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/Work.h"
#include "../BallNodes/SpellBall.h"
#include "../ChildNodes/BallSlot.h"
#include "../ChildNodes/ImageCard.h"
#include "../ChildNodes/AnswerPad.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class SpellingScene : public Scene {
    typedef Scene Super;

    Chain<Work> TheWork;
    size_t TheWorkIndex;

    Node* TheGameNode;
    ImageCard* TheImageCard;
    AnswerPad* TheAnswerPad;

public:
    // NB(xenosoz, 2016): Create the scene *with* the logic object. (cf. create());
    static SpellingScene* createScene();

    CREATE_FUNC(SpellingScene);
    SpellingScene();
    virtual ~SpellingScene();

    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    void onEnter() override;
    
    void replacePhysicsWorld();
    void revertPhysicsWorld();
    
    void beginFirstWork();
    void beginNextWork();
    void prepareCurrentWork();

    
    // NB(xenosoz, 2016): Destructor of cocos2d::PhysicsWorld is protected, and cocos2d::Scene is a friend of
    //   cocos2d::PhysicsWorld. So we just save the object, replace it back when we destruct 'this'.
    PhysicsWorld* ReplacedPhysicsWorldFromParent;
};

END_NS_SPELLING;