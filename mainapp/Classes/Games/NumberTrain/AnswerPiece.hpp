//
//  AnswerPiece.hpp
//  todomath
//
//  Created by Gunho Lee on 3/21/17.
//
//

#ifndef AnswerPiece_hpp
#define AnswerPiece_hpp

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;
using namespace ui;


class TrainCar;


class AnswerPiece : public Node
{
public:
    
    CREATE_FUNC(AnswerPiece);
    virtual bool init() override;
    
    
    static Node* createFace(int number);
    
    void setNumber(int number);
    
    void setPicked(bool picked);
    
    void snapTarget(TrainCar *target);
    void unSnap(Node *root);
    
    
    
    std::function<void()> onTouchBegan;
    std::function<void()> onTouchEnded;
    
    Sprite *_face, *_body, *_shadow;
    
    TrainCar *_targetSlot;
    Vec2 _originalPos;
    
    
    bool _snapped;
    bool _enableTouch;
    int _number;

    
};

#endif /* AnswerPiece_hpp */
