//
//  ShapeMatchingCard.cpp
//  KitkitSchool
//
//  Created by timewalker on 6/23/16.
//
//


#include "ShapeMatchingScene.hpp"
#include "ShapeMatchingCard.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
//#include "Managers/LocalizationManager.hpp"
//#include "Managers/TodoFontManager.hpp"
#include "Utils/TodoUtil.h"
//#include "Controls/TodoLabel.hpp"

#include <string>
#include <vector>

#define GROUP_MAX_COUNT 10
#define SCALE_FACTOR 0.7f

using namespace std;
using namespace ShapeMatching;


namespace {
}  // unnamed namespace


bool ShapeMatchingCard::init()
{
    if (!Widget::init()) { return false; }
    
    isMatchDone = false;
    isTouched = false;
    isLinked = false;
    matchSound = "";
    
    return true;
}




void ShapeMatchingCard::setImage(int level, int type, int number,
                                 const std::string& cardImageName,
                                 const std::string& localizedName,
                                 int rotation, float scale)
{
    auto spriteName = std::string("ShapeMatching/Images/") + cardImageName;
    std::string bgImageName = "ShapeMatching/Images/matching_shape_cardbg.png";
    
    Sprite* background = Sprite::create(bgImageName);
    background->setScale(1.2f);
    
    auto contentSize = background->getContentSize();
    
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(contentSize / 2);
    addChild(background);
    
    Node* shape = Sprite::create(spriteName);
    Color4B fontColor = Color4B(105,65,27,255);
    Vec2 offset = Vec2::ZERO;
    
    const string fontName = "fonts/OpenSans-Bold.ttf";
    
    if (!shape) {
        std::string localizedText = "";
        vector<std::string> elms;
        int number = -1;
        string key = "";
        bool face = false;
        bool side = false;
        if (cardImageName.find("Face") != std::string::npos) {
            
            face = true;
        }
        else if (cardImageName.find("Side") != std::string::npos) {
            side = true;
        }
        
        // number image and label on the card
        if (face || side) {
            elms = TodoUtil::split(cardImageName, ' ');
            number = TodoUtil::stoi(elms[0]);
            if (face) {
                //* LocalizationManager::getInstance()->isPlural(number) ? key = "faces" : key = "face";
                key = (number > 1 || number == 0) ? "faces" : "face";
            }
            else if(side) {
                //* LocalizationManager::getInstance()->isPlural(number) ? key = "sides" : key = "side";
                key = (number > 1 || number == 0) ? "sides" : "side";
            }
            
            shape = Sprite::create(StringUtils::format("ShapeMatching/Images/matchinggame_numbers_type1_%d.png",number) );
            
            string attrText = LanguageManager::getInstance()->getLocalizedString(key);
            
            auto label = TodoUtil::createLabel(attrText, 70, Size(contentSize.width- 10, 100), fontName, fontColor, TextHAlignment::CENTER);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            label->setPosition(Vec2(contentSize.width/2.f, 100));
            addChild(label);
            offset.y = 35;
            
        }
        // only label
        else {
            string localizedText = localizedName;
            TodoUtil::replaceAll(localizedText, "_", " ");
            TodoUtil::replaceAll(localizedText, " ", "\n");
            
            shape = TodoUtil::createLabel(localizedText, 90, Size::ZERO, fontName, fontColor, TextHAlignment::CENTER);
            shape->setName("shape");
        }
        
    }
    else {
        shape->setRotation((float)rotation);
        shape->setScale(scale);
    }
    
    shape->setPosition(Vec2(background->getContentSize()/2.f) + offset);
    shape->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(shape);
    
    id = number;
    _type = type;
    setContentSize(contentSize);
    setCascadeOpacityEnabled(true);
    
    createShiningParticle();
    
    
    
    
}

void ShapeMatchingCard::setShapeScale(float scaleFactor) {
    this->getChildByName("shape")->setScale(scaleFactor);
}


bool ShapeMatchingCard::isTouchedIn(Point pt)
{
    Point pos = getPosition();
    
    Rect bBox = this->getBoundingBox();
    bBox.origin.x = pos.x -(bBox.size.width / 2.0f);
    bBox.origin.y = pos.y -(bBox.size.height / 2.0f);
    
    //    CCLOG("scale : %f, pos : (%f,%f) bBox : (%f,%f)",getScale(),pos.x,pos.y,bBox.origin.x,bBox.origin.y);
    
    return bBox.containsPoint(pt);
}

bool shouldBecomePair(ShapeMatchingCard* cardA, ShapeMatchingCard* cardB) {
    if (!cardA || !cardB) { return false; }
    if (cardA->isMatchDone || cardB->isMatchDone) { return false; }
    
    auto convertToWorldSpace = [](Node* node, Rect rect) {
        auto origin = node->convertToWorldSpace(rect.origin);
        auto diagonal = node->convertToWorldSpace(rect.origin + rect.size);
        
        return Rect(origin, Size(diagonal - origin));
    };
    
    Rect rectA = convertToWorldSpace(cardA, Rect(Point::ZERO, cardA->getContentSize()));
    Rect rectB = convertToWorldSpace(cardB, Rect(Point::ZERO, cardB->getContentSize()));
    
    return rectA.intersectsRect(rectB);
}

bool shouldKeepAsPair(ShapeMatchingCard* cardA, ShapeMatchingCard* cardB) {
    return shouldBecomePair(cardA, cardB);
}

Size ShapeMatchingCard::defaultSize() {
    return Size(450, 570);
}

void ShapeMatchingCard::createShiningParticle()
{
    _shiningParticleNode = Node::create();
    
    ParticleSystemQuad* particleEffect = nullptr;
    
    
    auto createParticleEffect = [&](std::string name, std::string plist) {
        particleEffect = ParticleSystemQuad::create(plist);
        particleEffect->setName(name);
        particleEffect->setScale(1.8f);
        particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particleEffect->setPosition(Vec2::ZERO);
        particleEffect->stopSystem();
        _shiningParticleNode->addChild(particleEffect);
    };
    
    createParticleEffect("particle1", "Common/Effects/Particle/shining_particle_blur.plist");
    createParticleEffect("particle2", "Common/Effects/Particle/shining_particle_circle.plist");
    createParticleEffect("particle3", "Common/Effects/Particle/shining_particle_star.plist");
    
    _shiningParticleNode->setPosition(this->getContentSize()/2);
    addChild(_shiningParticleNode);
    
}

void ShapeMatchingCard::startParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
}

void ShapeMatchingCard::stopParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
}

void ShapeMatchingCard::setLink(ShapeMatchingCard *card)
{
    if (card) {
        isLinked = true;
        linkedTarget = card;
        startParticle();
    } else {
        isLinked = false;
        linkedTarget = nullptr;
        stopParticle();
        
        if (!isTouched) runAction(EaseElasticOut::create(ScaleTo::create(0.3f, defaultScale)));
    }
    
}

