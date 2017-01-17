//
//  WordMatchingCard.hpp
//  enumaXprize
//
//  Created by timewalker on 6/30/16.
//
//

#ifndef LetterMatchingCard_hpp
#define LetterMatchingCard_hpp

#include <Common/Basic/SoundEffect.h>
#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class LetterMatchingCard : public Widget
{
public:
    CREATE_FUNC(LetterMatchingCard);
    virtual bool init() override;

    void setImage(int level, int type, int number,
                  const std::string& cardImageName);
    bool isTouchedIn(Point pt);

    // NB(xenosoz, 2016): Relationships between two cards.
    friend bool shouldBecomePair(LetterMatchingCard* cardA,
                                 LetterMatchingCard* cardB);
    friend bool shouldKeepAsPair(LetterMatchingCard* cardA,
                                 LetterMatchingCard* cardB);

    void createShiningParticle();
    void startParticle();
    void stopParticle();
    void setLink(LetterMatchingCard *card);

    
public:
    static Size defaultSize();
    
public:
    int id;
    bool isMatchDone;
    SoundEffect matchSound;
    
    bool isLinked, isTouched;
    LetterMatchingCard *linkedTarget;
    Node* _shiningParticleNode;
    
    float defaultScale;

    
};

#endif /* WordMatchingCard_hpp */
