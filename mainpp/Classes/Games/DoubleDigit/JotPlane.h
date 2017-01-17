//
//  JotPlane.h
//  todomath
//
//  Created by Sunmi Seol on 12/16/15.
//
//

#ifndef __todomath__JotPlane__
#define __todomath__JotPlane__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


class JotPlane : public Layer
{


public:
    
    CREATE_FUNC(JotPlane);
    
    virtual bool init() override;
    void initWithNormal();
    void initwithMonsterQuiz();
    virtual ~JotPlane();

  
    void clear();

    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
    virtual void onTouchEnded(Touch* touch, Event* event) override;
    virtual void onTouchCancelled(Touch* touch, Event* event) override;
    
    
private:
    

    
    bool soundPlaying;
    unsigned int soundID;
    
    DrawNode *drawNode2;
    
    struct jot_segment {
        DrawNode* dnode;
        Point begin;
        Point end;
    };
    
    std::list<jot_segment> segments;
   
    long long sketchStartTime;
    
    void addSegment(Point begin, Point end);
    void removeSegmentAcross(Point begin, Point end);
    
    bool onSegment (Point p, Point q, Point r);
    int  orientation (Point p, Point q, Point r);
    bool doIntersect(Point p, Point q, Point p2, Point q2);
    
private:
    
    void soundStart();
    void soundStop();
    void soundUpdate(float dt);
    
};

#define BTN_TOP_GAP    35
#define JOTPLANE_DEBUG 0

#endif /* defined(__todomath__JotPlane__) */
