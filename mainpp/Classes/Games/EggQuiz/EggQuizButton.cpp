//
//  EggQuizButton.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 1/8/17.
//
//

#include "EggQuizButton.hpp"

#include "Utils/TodoUtil.h"
#include "Common/Basic/SoundEffect.h"
#include "Managers/GameSoundManager.h"

using namespace std;


const string fontName = "fonts/Aileron-Regular.otf";

bool EggQuizButton::init()
{
    if (!Button::init()) return false;

    
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label = nullptr;
    _labelScale = 1.0;
    
    
    return true;
}




void EggQuizButton::setNumber(int num)
{
    loadTextures("EggQuiz/Common/pretest_english_format1a_answer_normal.png", "EggQuiz/Common/pretest_english_format1a_answer_active.png");
    setScale9Enabled(true);
    auto btnSize = Size(438, 458);
    auto depth = 40;
    auto border = 10;
    setContentSize(btnSize);
    
    auto innerSize = Size(btnSize.width-2*border, btnSize.height-2*border-depth);
    auto center = Vec2(btnSize.width/2, (btnSize.height-depth)/2+depth);
    
    
    _label = TodoUtil::createLabel(TodoUtil::itos(num), 150, Size::ZERO, fontName, Color4B::WHITE);
    if (_label->getContentSize().width > innerSize.width) {
        setLabelScale(innerSize.width / _label->getContentSize().width);
    }
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label->setPosition(center);
    addChild(_label);
    
    
    addTouchEventListener([this](Ref* r,Widget::TouchEventType e) {
        if (this->isHighlighted()) {
            _label->setTextColor(Color4B(70, 70, 70, 255));
        } else {
            _label->setTextColor(Color4B::WHITE);
        }
    });
}

void EggQuizButton::setImage(std::string path)
{
    loadTextures("EggQuiz/Common/pretest_english_format1a_answer_normal.png", "EggQuiz/Common/pretest_english_format1a_answer_active.png");
    
    auto btnSize = Size(562, 446);
    auto depth = 40;
    auto border = 10;
    
    auto innerSize = Size(btnSize.width-2*border, btnSize.height-2*border-depth);
    auto center = Vec2(btnSize.width/2, (btnSize.height-depth)/2+depth);
    
    auto sp = Sprite::create(path);
    if (sp) {
        auto spSize = sp->getContentSize();
        sp->setScale(MIN(innerSize.width/spSize.width, innerSize.height/spSize.height));
        sp->setPosition(center);
        addChild(sp);
    } else {
        auto l = TodoUtil::createLabel(path, 50, Size::ZERO, fontName, Color4B::BLACK);
        l->setPosition(center);
        addChild(l);
    }
    
    
}


void EggQuizButton::setWord(std::string word, Size btnSize)
{
    
    loadTextures("EggQuiz/Common/pretest_english_format1a_answer_normal.png", "EggQuiz/Common/pretest_english_format1a_answer_active.png");

    setScale9Enabled(true);
    auto depth = 40;
    auto border = 40;
    setContentSize(btnSize);
    
    auto innerSize = Size(btnSize.width-2*border, btnSize.height-2*border-depth);
    auto center = Vec2(btnSize.width/2, (btnSize.height-depth)/2+depth);
    
    
    if (word.find("+")!=word.npos) {
        auto w = TodoUtil::split(word, '+');
        
        auto phonic = [&](string s) {
            auto sp = Sprite::create("EggQuiz/Common/egg_quiz_cards_blank.png");
            auto spSize = sp->getContentSize();
            
            auto l = TodoUtil::createLabel(s, 100, Size::ZERO, fontName, Color4B(70, 70, 70, 255));
            l->setPosition(spSize/2);
            sp->addChild(l);
            this->addChild(sp);
            return sp;
        };
        
        auto w1 = phonic(w[0]);
        w1->setPosition(center - Vec2(80, 0));

        auto w2 = phonic(w[1]);
        w2->setPosition(center + Vec2(80, 0));
        _label = nullptr;
        
        
    } else {
        _label = TodoUtil::createLabel(word, 100, Size::ZERO, fontName, Color4B::WHITE);
        if (_label->getContentSize().width > innerSize.width) {
            setLabelScale(innerSize.width / _label->getContentSize().width);
        }
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _label->setPosition(center);
        addChild(_label);
    }
    
    addTouchEventListener([this](Ref* r,Widget::TouchEventType e) {
        if (this->isHighlighted()) {
            if (_label) _label->setTextColor(Color4B(70, 70, 70, 255));
        } else {
            if (_label) _label->setTextColor(Color4B::WHITE);
        }
    });
    
}

void EggQuizButton::setSentence(std::string sentence, Size btnSize)
{
    loadTextures("EggQuiz/Common/pretest_english_format1b_answer_normal.png", "EggQuiz/Common/pretest_english_format1b_answer_active.png");
    
    setScale9Enabled(true);

    auto depth = 30;
    auto border = 20;
    setContentSize(btnSize);
    
    auto innerSize = Size(btnSize.width-2*border, btnSize.height-2*border-depth);
    auto center = Vec2(btnSize.width/2, (btnSize.height-depth)/2+depth);
    
    
    _label = TodoUtil::createLabel(sentence, 60, Size(innerSize.width, 0), fontName, Color4B::WHITE, TextHAlignment::CENTER);
    if (_label->getContentSize().height > innerSize.height) {
        setLabelScale(innerSize.height / _label->getContentSize().height);
    }
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label->setPosition(center);
    addChild(_label);
    
    
    addTouchEventListener([this](Ref* r,Widget::TouchEventType e) {
        if (this->isHighlighted()) {
            _label->setTextColor(Color4B(70, 70, 70, 255));
        } else {
            _label->setTextColor(Color4B::WHITE);
        }
    });
}

void EggQuizButton::setParagraph(std::string paragraph, Size btnSize)
{
    loadTextures("EggQuiz/Common/pretest_english_format1b_answer_normal.png", "EggQuiz/Common/pretest_english_format1b_answer_active.png");
    
    setScale9Enabled(true);
    
    auto depth = 30;
    auto border = 10;
    setContentSize(btnSize);
    
    auto extra = (btnSize.width > btnSize.height*8) ? 100 : 0;
    
    auto innerSize = Size(btnSize.width-2*border - extra, btnSize.height-2*border-depth);
    auto center = Vec2(btnSize.width/2, (btnSize.height-depth)/2+depth);

    
    _label = TodoUtil::createLabelMultiline(paragraph, 60, innerSize, fontName, Color4B::WHITE);
    if (_label->getContentSize().height > innerSize.height) {
        setLabelScale(innerSize.height / _label->getContentSize().height);
    }
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label->setPosition(center);
    addChild(_label);
    
    
    addTouchEventListener([this](Ref* r,Widget::TouchEventType e) {
        if (this->isHighlighted()) {
            _label->setTextColor(Color4B(70, 70, 70, 255));
        } else {
            _label->setTextColor(Color4B::WHITE);
        }
    });
}

void EggQuizButton::setSound(std::string soundPath)
{
    loadTextures("EggQuiz/Common/pretest_speaker_normal.png",
                "EggQuiz/Common/pretest_speaker_active.png");
    


    
    addClickEventListener([soundPath, this](Ref*){
        GameSoundManager::getInstance()->stopAllEffects();
        SoundEffect(soundPath).play();
    });
    
    runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([soundPath](){
        GameSoundManager::getInstance()->stopAllEffects();
        SoundEffect(soundPath).play();
    }), nullptr));
              
}