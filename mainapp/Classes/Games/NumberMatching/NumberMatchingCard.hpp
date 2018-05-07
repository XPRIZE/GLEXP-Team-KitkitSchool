//
//  NumberMatchingCard.hpp
//  KitkitSchool
//
//  Created by timewalker on 6/23/16.
//
//

#ifndef NumberMatchingCard_hpp
#define NumberMatchingCard_hpp

#include <Common/Basic/SoundEffect.h>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class NumberMatchingCard : public Widget
{
public:
    CREATE_FUNC(NumberMatchingCard);
    virtual bool init() override;

    
    void setImage(int level, int type, int number, const std::string& cardImageName);
    bool isTouchedIn(Point pt);
    
    // NB(xenosoz, 2016): Relationships between two cards.
    friend bool shouldBecomePair(NumberMatchingCard* cardA,
                                 NumberMatchingCard* cardB);
    friend bool shouldKeepAsPair(NumberMatchingCard* cardA,
                                 NumberMatchingCard* cardB);


    void createShiningParticle();
    void startParticle();
    void stopParticle();
    void setLink(NumberMatchingCard *card);
    

    
public:
    static Size defaultSize();
    
public:
    int id;
    bool isMatchDone;
    SoundEffect matchSound;
    
    bool isLinked, isTouched;
    NumberMatchingCard *linkedTarget;
    Node* _shiningParticleNode;
    
    float defaultScale;
    
    
};

#endif /* NumberMatchingCard_hpp */
