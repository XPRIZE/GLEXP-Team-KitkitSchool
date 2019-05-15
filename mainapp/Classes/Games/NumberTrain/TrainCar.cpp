//
//  TrainCar.cpp
//  todomath
//
//  Created by Gunho Lee on 3/21/17.
//
//

#include "TrainCar.hpp"

#include "NumberTrainHeader.h"

#include "ui/UIScale9Sprite.h"

bool TrainCar::init() {
    if (!Node::init()) return false;
    
    
    _hasSlot = false;
    _cardInSlot = nullptr;
    _animalType = NONE;
    _playing = false;
    
    if (LanguageManager::getInstance()->getCurrentLanguageTag() == "en-US") _isEN = true; else _isEN = false;

    return true;
}

void TrainCar::setType(TrainCar::carType type)
{
    _type = type;
    
    int numConnector = 0;
    
    switch (type) {
        case FRONT:
            _body = Sprite::create("NumberTrain/smallest_largest_train_2_front.png");
            numConnector = 2;
            break;
        case MIDDLE:
            numConnector = 1;
        case BACK:
            _body = Sprite::create("NumberTrain/smallest_largest_train_2_back.png");
            _hasSlot = true;
            _slotPos = Vec2(156, 231);
            break;
        case ONE:
            _body = Sprite::create("NumberTrain/smallest_largest_train_1.png");
            _hasSlot = true;
            _slotPos = Vec2(175, 239);
            break;
    }
    
    auto size = _body->getContentSize();
    setContentSize(size);
    
    for (int i=0; i<numConnector; i++) {
        
        
        auto c = cocos2d::ui::Scale9Sprite::create("NumberTrain/smallest_largest_train_2_connector.png");
        c->setPreferredSize(Size(size.width, 22));
        c->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        c->setPosition(Vec2(size.width/2, 70+i*40));
        addChild(c);
        
    }
    
    _body->setPosition(size/2);
    addChild(_body);
    
    
}

void TrainCar::soundButtonSpeech() {
    
    if (_playing) return;
    _playing = true;
    float delay;
    
    switch(_soundType) {
        case ORDERASC:
        {
            auto file = LanguageManager::getInstance()->findLocalizedResource("NumberTrain/Sounds/arrange_from_smallest_to_largest.m4a");
            GameSoundManager::getInstance()->playEffectSound(file);
            if (_isEN) {
//                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/arrange_from_smallest_to_largest.m4a");
                delay = 6.5;
            } else {
//                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/OrderAsc.m4a");
                delay = 5.5;
            }
        }
            break;
        case LARGEST:
        {
            auto file = LanguageManager::getInstance()->findLocalizedResource("NumberTrain/Sounds/largest_number.m4a");
            GameSoundManager::getInstance()->playEffectSound(file);
            if (_isEN) {
//                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/largest_number.wav");
                delay = 3.5;
            } else {
//                GameSoundManager::getInstance()->playEffectSound("NumberTrain/Sounds/Largest.wav");
                delay = 2.5;

                
            }
        }

            break;
        default: break;
    }
    
    runAction(Sequence::create(
        CallFunc::create([this](){
            _soundButtonActive->setVisible(true);
            _soundButtonNormal->setVisible(false);
        }),
        DelayTime::create(delay),
        CallFunc::create([this](){
            _soundButtonActive->setVisible(false);
            _soundButtonNormal->setVisible(true);
            _playing = false;
        }),
        nullptr
    ));

}

void TrainCar::setAnimal(TrainCar::animalType animalType, bool isNumber, bool oneBack)
{

    _animalType = animalType;
    
    auto size = getContentSize();
    Vec2 bPos;
    
    const string fontName = "fonts/OpenSans-Bold.ttf";

    if (animalType==MOUSE) {
        _animal = Sprite::create("NumberTrain/smallet_largest_mice_normal.png");
        _animal->setAnchorPoint(Vec2(106.f/177.f, 0.f));
        
        //_bubble = Sprite::create("NumberTrain/smallet_largest_mice_bubble_2.png"); // 633*387
        _bubble = Scale9Sprite::create("NumberTrain/smallet_largest_mice_bubble_2.png", Rect(0,0,633,387), Rect(52,139,530,226));
        _bubble->setContentSize(Size(633+300,487));
        bPos = Vec2(60, size.height + 220);

        _soundButtonActive = Sprite::create("NumberTrain/train_sound_active.png");
        _soundButtonActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _soundButtonActive->setPosition(160,315);
        _soundButtonActive->setScale(0.9);
        _bubble->addChild(_soundButtonActive);
        _soundButtonActive->setVisible(false);
        
        _soundButtonNormal = Sprite::create("NumberTrain/train_sound_normal.png");
        _soundButtonNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _soundButtonNormal->setPosition(160,315);
        _soundButtonNormal->setScale(0.9);
        _bubble->addChild(_soundButtonNormal);
        _soundType = ORDERASC;
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            auto P = _soundButtonNormal->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (_soundButtonNormal->getBoundingBox().containsPoint(pos)) {
                soundButtonSpeech();
                return true;
            }
            return false;
        };
        
        _soundButtonNormal->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        
        string labelKey = LanguageManager::getInstance()->getLocalizedString("Arrange the numbers in order from smallest to largest");
        
        _label = TodoUtil::createLabel(labelKey, 45, Size(500, 190), fontName, Color4B(54, 54, 54, 255), TextHAlignment::LEFT);
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _label->setPosition(565, 320);
        
    } else if (animalType==ELEPHANT) {
        _animal = Sprite::create("NumberTrain/smallet_largest_elephant_normal.png");
        _animal->setAnchorPoint(Vec2(310.f/770.f, 0.f));
        
        if (_type==ONE) {
            _bubble = Scale9Sprite::create("NumberTrain/smallet_largest_elephant_bubble_2.png", Rect(0,0,1140,208), Rect(166, 26, 900, 146));
            _bubble->setContentSize(Size(1140-250,208+100));
            
            bPos = Vec2(700, size.height + 320);
            if (oneBack) {
                bPos = Vec2(-470, size.height + 50);
            }
            
            string labelKey = LanguageManager::getInstance()->getLocalizedString("Largest number");
            _label = TodoUtil::createLabel(labelKey, 45, Size(430, 120), fontName, Color4B(54, 54, 54, 255), TextHAlignment::LEFT);
            _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _label->setPosition(610, 160);
            
            _soundButtonActive = Sprite::create("NumberTrain/train_sound_active.png");
            _soundButtonActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _soundButtonActive->setPosition(250,155);
            _soundButtonActive->setScale(0.9);
            _bubble->addChild(_soundButtonActive);
            _soundButtonActive->setVisible(false);
            
            _soundButtonNormal = Sprite::create("NumberTrain/train_sound_normal.png");
            _soundButtonNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _soundButtonNormal->setPosition(250,155);
            _soundButtonNormal->setScale(0.9);
            _bubble->addChild(_soundButtonNormal);
            _soundType = LARGEST;
            
            auto *listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](Touch* T, Event* E) {
                auto P = _soundButtonNormal->getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                if (_soundButtonNormal->getBoundingBox().containsPoint(pos)) {
                    soundButtonSpeech();
                    return true;
                }
                return false;
            };
            
            _soundButtonNormal->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

            
        } else {
            /*
            _bubble = Sprite::create("NumberTrain/smallet_largest_elephant_bubble_2.png");
            bPos = Vec2(600, size.height + 320);
            
            auto labelKey = isNumber ? "Arrange in order from largest to smallest" : "Arrange in order from largest to smallest";
            if (isSW) labelKey = "panga kwa mpangilio kutoka kubwa zaidi kwenda ndogo zaidi";
            _label = TodoUtil::createLabel(labelKey, 45, Size(920, 170), fontName, Color4B(54, 54, 54, 255), TextHAlignment::CENTER);
            _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _label->setPosition(630, 110);
             */
        }
        
    }
    
    
    _animal->setPosition(Vec2(size.width/2, size.height));
    if (oneBack) _animal->setScaleX(-1.0);
    
    addChild(_animal);
    
    auto bSize = _bubble->getContentSize();
    
    _bubble->setPosition(bPos + bSize/2);
    _bubble->addChild(_label);
    addChild(_bubble);
    
    _bubble->setVisible(false);
}

void TrainCar::showBubble() {
    _bubble->setVisible(true);
    
    _bubble->setScale(0.01);
    _bubble->runAction(Sequence::create(
                                        DelayTime::create(3.0),
                                        EaseBounceOut::create(ScaleTo::create(0.5, 1.0)),
                                        nullptr));
    
}

void TrainCar::hideBubble() {
    
    _bubble->runAction(Sequence::create(
                                        EaseIn::create(ScaleTo::create(0.3, 0.0), 2.0),
                                        CallFunc::create([this](){_bubble->setVisible(false); }),
                                        nullptr));
    
    
    
}

void TrainCar::animSuccess()
{
    if (!_animal) return;
    
    if (_animalType==MOUSE) {
        _animal->setTexture("NumberTrain/smallet_largest_mice_correct.png");
        _animal->runAction(MoveBy::create(0,Vec2(0,-30)));
        auto jumpSeq =  JumpBy::create(1.2, Point(0,0), 80, 2);
        
        /* auto jumpSeq = Sequence::create(EaseOut::create(MoveBy::create(0.3, Vec2(0, 80)), 3.0),
                                        EaseIn::create(MoveBy::create(0.3, Vec2(0, -80)), 3.0),
                                        EaseOut::create(MoveBy::create(0.3, Vec2(0, 80)), 3.0),
                                        EaseIn::create(MoveBy::create(0.3, Vec2(0, -80)), 3.0),
                                        DelayTime::create(0.31), nullptr
                                        );*/
        _animal->runAction(RepeatForever::create(jumpSeq));
        
    } else if (_animalType==ELEPHANT) {
        _animal->setTexture("NumberTrain/smallet_largest_elephant_correct.png");
        
        
        _animal->setRotation(-3);
        auto danceSeq = Sequence::create(DelayTime::create(0.3),
                                         RotateBy::create(0.01, 6),
                                         DelayTime::create(0.3),
                                         RotateBy::create(0.01, -6),
                                         nullptr);
        _animal->runAction(RepeatForever::create(danceSeq));
        
    }
    
}

void TrainCar::animFail()
{
    
    
    if (!_animal) return;
    
    if (_animalType==MOUSE) {
        _animal->setTexture("NumberTrain/smallet_largest_mice_wrong.png");
        scheduleOnce([this](float){ _animal->setTexture("NumberTrain/smallet_largest_mice_normal.png"); }, 1.2, "failAnimation");
        
    } else if (_animalType==ELEPHANT) {
        _animal->setTexture("NumberTrain/smallet_largest_elephant_wrong.png");
        scheduleOnce([this](float){ _animal->setTexture("NumberTrain/smallet_largest_elephant_normal.png"); }, 1.2, "failAnimation");
    }
}

void TrainCar::animShake()
{
    
    const auto shakeTime = 0.08;
    const auto shakeDelta = Vec2(12, 0);
    
    
    auto shakeSeq = Sequence::create(
                                     MoveBy::create(shakeTime/2, shakeDelta),
                                     MoveBy::create(shakeTime, -2*shakeDelta),
                                     MoveBy::create(shakeTime, 2*shakeDelta),
                                     MoveBy::create(shakeTime, -2*shakeDelta),
                                     MoveBy::create(shakeTime, 2*shakeDelta),
                                     MoveBy::create(shakeTime/2, -shakeDelta),
                                     nullptr);
    
    _body->runAction(shakeSeq);
    
}
