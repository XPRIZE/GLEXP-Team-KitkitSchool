//
//  TrainCar.hpp
//  todomath
//
//  Created by Gunho Lee on 3/21/17.
//
//

#ifndef TrainCar_hpp
#define TrainCar_hpp


#include "cocos2d.h"
#include <string>


USING_NS_CC;
using namespace std;
using namespace ui;


class AnswerPiece;


class TrainCar : public Node
{
public:
    
    enum carType {
        FRONT,
        MIDDLE,
        BACK,
        ONE,
    } _type;
    
    enum animalType {
        NONE,
        MOUSE,
        ELEPHANT,
    } _animalType;
    
    enum soundType {
        LARGEST,
        ORDERASC
    } _soundType;
    
    CREATE_FUNC(TrainCar);
    virtual bool init() override;
    
    void setType(carType type);
    void setAnimal(animalType animalType, bool isNumber = true, bool oneBack = false);
    
    void showBubble();
    void hideBubble();
    
    
    void animSuccess();
    void animFail();
    void animShake();
    
    void soundButtonSpeech();
    
    
    Vec2 _slotPos;
    bool _hasSlot, _playing;
    AnswerPiece *_cardInSlot;
    
    Sprite *_animal;
    Sprite *_body, *_soundButtonActive, *_soundButtonNormal;
    
    Scale9Sprite *_bubble;
    Label *_label;
    
    bool _isEN;
    
    
};

#endif /* TrainCar_hpp */
