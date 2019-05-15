//
//  ShapeMatchingCard.hpp
//  KitkitSchool
//
//  Created by timewalker on 6/23/16.
//
//

#ifndef ShapeMatchingCard_hpp
#define ShapeMatchingCard_hpp

//#include <Common/Basic/SoundEffect.h>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ShapeMatchingCard : public Widget
{
public:
    CREATE_FUNC(ShapeMatchingCard);
    virtual bool init() override;
    
    
    void setImage(int level, int type, int number,
                  const std::string& cardImageName,
                  const std::string& localizedName,
                  int rotation, float scale);
    bool isTouchedIn(Point pt);
    
    // NB(xenosoz, 2016): Relationships between two cards.
    friend bool shouldBecomePair(ShapeMatchingCard* cardA,
                                 ShapeMatchingCard* cardB);
    friend bool shouldKeepAsPair(ShapeMatchingCard* cardA,
                                 ShapeMatchingCard* cardB);
    
    
    void createShiningParticle();
    void startParticle();
    void stopParticle();
    void setLink(ShapeMatchingCard *card);
    void setShapeScale(float scaleFactor);
    
public:
    static Size defaultSize();
    
public:
    int id;
    bool isMatchDone;
    std::string matchSound;
    //    SoundEffect matchSound;
    
    bool isLinked, isTouched;
    ShapeMatchingCard *linkedTarget;
    Node* _shiningParticleNode;
    
    float defaultScale;
    
    int _type;
    
    bool hasTouchSound, hasMatchingSound;
    
};

#endif /* ShapeMatchingCard_hpp */
