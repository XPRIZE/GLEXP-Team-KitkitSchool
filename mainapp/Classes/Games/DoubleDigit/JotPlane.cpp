//
//  JotPlane.cpp
//  todomath
//
//  Created by Sunmi Seol on 12/16/15.
//
//

#include "JotPlane.h"
#include <stdio.h>
#include <algorithm>
#include "Managers/GameSoundManager.h"


using namespace std;
USING_NS_CC;



bool JotPlane::init()
{
    
    initWithNormal();
    
    return true;
}

JotPlane::~JotPlane()
{
   
    
}

//static const Vec2 sketchAreaOrigin(0.0f, 0.0f);
//static const Size notepadSize(1024, 1440);
//static const Size sketchAreaSize(2048/2, 1536 - 48.f - 179.f);
static const Size notepadSize(1228, 1612);
//static const Size sketchAreaSize(1228, 1612);
static const Rect sketchAreaRect(50, 150, 1128, 1230);


void JotPlane::initWithNormal()
{
    
    //if (JOTPLANE_DEBUG) CCLOG("JotPlane::initWithNormal");
    
    this->setContentSize(notepadSize);
    
    drawNode2 = DrawNode::create();
    drawNode2->setAnchorPoint(Vec2(0.0f, 0.0f));
    drawNode2->setPosition(0.0f, 0.0f);
    this->addChild(drawNode2,1);
    
    setTouchEnabled(true);

    GameSoundManager::getInstance()->preloadEffect("Common/Controls/TraceField/Sounds/SFX_PENCILLOOP.m4a");
    
    
}


// related to sound effect
void JotPlane::soundStart()
{
    
    if (JOTPLANE_DEBUG) CCLOG("soundStart");
    
    if (!soundPlaying)
    {
        soundID = GameSoundManager::getInstance()->playEffectSound("Common/Controls/TraceField/Sounds/SFX_PENCILLOOP.m4a", true, 1.f, 0.f, 0.5f);
        soundPlaying = true;
    }

}

void JotPlane::soundStop()
{
    if (JOTPLANE_DEBUG) CCLOG("soundStop");
    
    if (soundPlaying)
    {
        GameSoundManager::getInstance()->stopEffectSound(soundID);
        soundPlaying = false;
    }
    
}

void JotPlane::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(JotPlane::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(JotPlane::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(JotPlane::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(JotPlane::onTouchCancelled, this);
    listener->setSwallowTouches(true);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    soundPlaying = false;
    this->scheduleUpdate();
}

void JotPlane::onExit()
{
    this->unscheduleAllCallbacks();
    Layer::onExit();
    
    soundStop();
    GameSoundManager::getInstance()->unloadAllEffect();
    this->unscheduleUpdate();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void JotPlane::clear()
{
    sketchStartTime = 0;
    drawNode2->clear();
    soundStop();
    

    
    // remove all segment
    auto x = segments.begin();
    while (x!= segments.end())
    {
        // remove draw node from parent
        DrawNode* dnode = x->dnode;
        dnode->removeFromParent();
        
        // remove segment from list
        x = segments.erase(x);
    }
    
//    if (erasorCursor->isVisible())
//       erasorCursor->setVisible(false);
}

bool JotPlane::onTouchBegan(Touch* touch, Event* event)
{
    
    if (!isTouchEnabled()) return false;
    
    long long currentTime = std::time(nullptr);
    
    if (sketchStartTime == 0) {
        sketchStartTime = currentTime;
    }
    
    Point pt = this->convertToNodeSpace(touch->getLocation());
    
    return (sketchAreaRect.containsPoint(pt));
    
//    if (pt.x < 0 || pt.y < 0 || pt.x > sketchAreaSize.width || pt.y > sketchAreaSize.height) {
//        return false;
//    }
//    
//    //  Flip vertically
//    //SketchPoint sp(pt.x, sketchAreaSize.height - pt.y, currentTime - sketchStartTime);
//    
//    return true;
}

void JotPlane::addSegment(Point begin, Point end)
{
    JotPlane::jot_segment J;
    DrawNode* drawNode = DrawNode::create();
    J.dnode = drawNode;
    J.begin = begin;
    J.end = end;
    
    Color4F curr_pencolor = (Color4F::BLACK);
    drawNode->drawSegment(begin, end, 10.0f, curr_pencolor);
    
    this->addChild(drawNode,1);
    segments.push_back(J);
}


void JotPlane::onTouchMoved(Touch* touch, Event* event)
{
    
    //CCLOG("onTouchMoved: %p %p", touch, event);
    
    Point ptPrev = this->convertToNodeSpace(touch->getPreviousLocation());
    Point ptThis = this->convertToNodeSpace(touch->getLocation());
    
    ptPrev.x = clampf(ptPrev.x, sketchAreaRect.origin.x, sketchAreaRect.origin.x+sketchAreaRect.size.width);
    ptPrev.y = clampf(ptPrev.y, sketchAreaRect.origin.y, sketchAreaRect.origin.y+sketchAreaRect.size.height);
    
    ptThis.x = clampf(ptThis.x, sketchAreaRect.origin.x, sketchAreaRect.origin.x+sketchAreaRect.size.width);
    ptThis.y = clampf(ptThis.y, sketchAreaRect.origin.y, sketchAreaRect.origin.y+sketchAreaRect.size.height);
    
    //const float offMargin = 20.f;
    
    bool isOffScreen = !sketchAreaRect.containsPoint(ptThis);
    if(isOffScreen) return;
    
    soundStart();

    {
        
        addSegment(ptPrev, ptThis);
        
    }

}

void JotPlane::onTouchEnded(Touch* touch, Event *event)
{
    soundStop();
    
}

void JotPlane::onTouchCancelled(Touch* touch, Event* event)
{
    soundStop();
}

