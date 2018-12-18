//
//  Egg.cpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 03/29/18
//

#include "Egg.hpp"

#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"

namespace MissingNumber
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    const std::string ROOT_RESOURCE_PATH = "Games/MissingNumber";
#else
    const std::string ROOT_RESOURCE_PATH = "MissingNumber";
#endif
    const int ID_EXPLOSTION_ROOT = 100;
    const int ID_BIRD = 101;
    const int ID_EGG_TOP = 102;
    const int ID_CLOUD_0 = 103;
    const int ID_CLOUD_1 = 104;
    const int ID_CLOUD_2 = 105;
    const int ID_EGG = 106;
    
    bool Egg::init()
    {
        if (!Node::init()) { return false; }
        return true;
    }
    
    void Egg::initWithNumberString(int resourceIndex, string numberString, string answerString, bool isLast)
    {
        mNumberString = TodoUtil::trim(numberString);
        mAnswer = TodoUtil::trim(answerString);
        
        auto nestShadow = Sprite::create(ROOT_RESOURCE_PATH + "/nest_shadow.png");
        auto nestBack = Sprite::create(ROOT_RESOURCE_PATH + "/nest_back.png");
        auto egg = Sprite::create(ROOT_RESOURCE_PATH + "/" + StringUtils::format("egg_%02d.png", (resourceIndex + 1)));
        auto nest = Sprite::create(ROOT_RESOURCE_PATH + "/nest.png");
        
        float width = nestShadow->getContentSize().width;
        float height = 480;
        this->setContentSize(Size(width, height));
        
        nestShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        nestShadow->setPosition(Vec2(width / 2, -50));
        
        nestBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        nestBack->setPosition(Vec2(width / 2, 0));
        
        mEggRoot = Node::create();
        mEggRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        mEggRoot->setContentSize(egg->getContentSize());
        mEggRoot->setPosition(Vec2(width / 2, height));
        
        egg->setPosition(mEggRoot->getContentSize()/2);
        egg->setTag(ID_EGG);
        mEggRoot->addChild(egg);
        
        nest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        nest->setPosition(Vec2(width / 2, 0));
        
        if (isLast == true)
        {
            auto explosionRoot = Node::create();
            auto eggBottom = Sprite::create(ROOT_RESOURCE_PATH + "/" + StringUtils::format("egg_%02d_bottom.png", (resourceIndex + 1)));
            auto eggTop = Sprite::create(ROOT_RESOURCE_PATH + "/" + StringUtils::format("egg_%02d_top.png", (resourceIndex + 1)));
            auto bird = Sprite::create(ROOT_RESOURCE_PATH + "/animal_bird.png");
            
            explosionRoot->setContentSize(mEggRoot->getContentSize());
            explosionRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            explosionRoot->setPosition(explosionRoot->getContentSize()/2);
            explosionRoot->setTag(ID_EXPLOSTION_ROOT);

            bird->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bird->setPosition(explosionRoot->getContentSize().width / 2, 230);
            bird->setTag(ID_BIRD);
            bird->setScale(0.85f, 0.85f);
            bird->setVisible(false);
            explosionRoot->addChild(bird);
            
            eggBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            eggBottom->setPosition(explosionRoot->getContentSize().width / 2, 0);
            explosionRoot->addChild(eggBottom);
            
            eggTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            eggTop->setPosition(explosionRoot->getContentSize().width / 2, explosionRoot->getContentSize().height);
            eggTop->setTag(ID_EGG_TOP);
            explosionRoot->addChild(eggTop);

            auto cloud_0 = Sprite::create(ROOT_RESOURCE_PATH + "/cloud.png");
            cloud_0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            cloud_0->setPosition(explosionRoot->getContentSize().width / 2, explosionRoot->getContentSize().height / 2 + 100);
            cloud_0->setTag(ID_CLOUD_0);
            cloud_0->setVisible(false);

            auto cloud_1 = Sprite::create(ROOT_RESOURCE_PATH + "/cloud.png");
            cloud_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            cloud_1->setPosition(cloud_0->getPosition());
            cloud_1->setTag(ID_CLOUD_1);
            cloud_1->setVisible(false);

            auto cloud_2 = Sprite::create(ROOT_RESOURCE_PATH + "/cloud.png");
            cloud_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            cloud_2->setPosition(cloud_0->getPosition());
            cloud_2->setTag(ID_CLOUD_2);
            cloud_2->setVisible(false);
            
            explosionRoot->addChild(cloud_0);
            explosionRoot->addChild(cloud_1);
            explosionRoot->addChild(cloud_2);
            
            explosionRoot->setVisible(false);
            mEggRoot->addChild(explosionRoot);
        }
        this->addChild(nestShadow);
        this->addChild(nestBack);
        this->addChild(mEggRoot);
        this->addChild(nest);
        
        setNumberString(mNumberString != "?" ? mNumberString : mAnswer);
        
        if (mNumberString != "?")
        {
            mbQuestion = false;
        }
        else
        {
            mbQuestion = true;
            mNumberRoot->setVisible(false);
            mQuestionRoot = Node::create();
            if (mAnswer.length() == 2)
            {
                mQuestion0 = Sprite::create(ROOT_RESOURCE_PATH + "/card-slot_empty.png");
                
                mQuestion0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestion0->setPosition(mQuestion0->getContentSize()/2);
                
                mQuestionMark0 = Sprite::create(ROOT_RESOURCE_PATH + "/card-slot-question-mark_normal.png");
                mQuestionMark0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestionMark0->setPosition(mQuestion0->getContentSize()/2);
                mQuestion0->addChild(mQuestionMark0);
                
                std::string emptyString = "";
                
                mNumber0 = NumberCard::create();
                mNumber0->initWithNumberString(emptyString + mAnswer.at(0), true, false, 0.85f);
                mNumber0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mNumber0->setPosition(mQuestion0->getContentSize()/2);
                mNumber0->setVisible(false);
                mQuestion0->addChild(mNumber0);

                float width = mQuestion0->getContentSize().width;
                mQuestion1 = Sprite::create(ROOT_RESOURCE_PATH + "/card-slot_empty.png");
                
                mQuestion1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestion1->setPosition(Size(width + width / 2 - 17, mQuestion1->getContentSize().height/2));
                
                mQuestionMark1 = Sprite::create(ROOT_RESOURCE_PATH + "/card-slot-question-mark_normal.png");
                mQuestionMark1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestionMark1->setPosition(mQuestion1->getContentSize()/2);
                mQuestion1->addChild(mQuestionMark1);

                mNumber1 = NumberCard::create();
                mNumber1->initWithNumberString(emptyString + mAnswer.at(1), true, false, 0.85f);
                mNumber1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mNumber1->setPosition(mQuestion0->getContentSize()/2);
                mNumber1->setVisible(false);
                mQuestion1->addChild(mNumber1);

                mQuestionRoot->setContentSize(Size(width * 2 - 17, mQuestion0->getContentSize().height));
                mQuestionRoot->addChild(mQuestion0);
                mQuestionRoot->addChild(mQuestion1);
            }
            else
            {
                mQuestion0 = Sprite::create(ROOT_RESOURCE_PATH + (mAnswer.length() == 1 ? "/card-slot_empty.png" : "/card-slot_empty_forThreeDigit.png"));
                
                mQuestion0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestion0->setPosition(mQuestion0->getContentSize()/2);
                
                mQuestionMark0 = Sprite::create(ROOT_RESOURCE_PATH + "/card-slot-question-mark_normal.png");
                mQuestionMark0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mQuestionMark0->setPosition(mQuestion0->getContentSize()/2);
                mQuestion0->addChild(mQuestionMark0);
                
                mNumber0 = NumberCard::create();
                mNumber0->initWithNumberString(mAnswer, mAnswer.length() == 1 ? true : false, false, 0.85f);
                mNumber0->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                mNumber0->setPosition(mQuestion0->getContentSize()/2);
                mNumber0->setVisible(false);
                mQuestion0->addChild(mNumber0);

                mQuestionRoot->setContentSize(mQuestion0->getContentSize());
                mQuestionRoot->addChild(mQuestion0);
            }
            
            mQuestionRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            mQuestionRoot->setPosition(mEggRoot->getContentSize() / 2);
            mEggRoot->addChild(mQuestionRoot);
            
            createShiningParticle();
        }
    }
    
    void Egg::setNumberString(string numberString)
    {
        if (mNumberRoot != nullptr)
        {
            mEggRoot->removeChild(mNumberRoot);
        }
        
        mNumberRoot = Node::create();
        
        const Rect RESOURCE_NUM_POSTION[] =
        {
            Rect(0, 0, 99, 112),
            Rect(99, 0, 48, 112),
            Rect(147, 0, 95, 112),
            Rect(242, 0, 82, 112),
            Rect(324, 0, 91, 112),
            Rect(415, 0, 80, 112),
            Rect(495, 0, 85, 112),
            Rect(580, 0, 79, 112),
            Rect(659, 0, 90, 112),
            Rect(749, 0, 79, 112),
        };
        
        float width = 0;
        for (int i = 0; i < numberString.size(); ++i)
        {
            int num = numberString[i] - '0';
            
            auto number = Sprite::create();
            number->setTexture(ROOT_RESOURCE_PATH + "/egg_number_image.png");
            number->setTextureRect(RESOURCE_NUM_POSTION[num]);
            number->setContentSize(RESOURCE_NUM_POSTION[num].size);
            number->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            number->setPosition(Vec2(width, 0));
            mNumberRoot->addChild(number);
            width += number->getContentSize().width;
        }
        
        mNumberRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        mNumberRoot->setContentSize(Size(width, 112));
        mNumberRoot->setPosition(mEggRoot->getContentSize() / 2);
        mEggRoot->addChild(mNumberRoot);
    }
    
    void Egg::selectedQuestion(int index, bool bSelected)
    {
        if (index == 0 && mQuestion0 != nullptr)
        {
            if (mAnswer.length() == 1 || mAnswer.length() == 2)
            {
                mQuestion0->setTexture(ROOT_RESOURCE_PATH + (bSelected == true ? "/card-slot_selected.png" : "/card-slot_empty.png"));
            }
            else
            {
                mQuestion0->setTexture(ROOT_RESOURCE_PATH + (bSelected == true ? "/card-slot_selected_forThreeDigit.png" : "/card-slot_empty_forThreeDigit.png"));
            }
            
            mQuestionMark0->setTexture(ROOT_RESOURCE_PATH + (bSelected == true ? "/card-slot-question-mark_selected.png" : "/card-slot-question-mark_normal.png"));
            
        }
        else if (index == 1 && mQuestion1 != nullptr)
        {
            mQuestion1->setTexture(ROOT_RESOURCE_PATH + (bSelected == true ? "/card-slot_selected.png" : "/card-slot_empty.png"));
            mQuestionMark1->setTexture(ROOT_RESOURCE_PATH + (bSelected == true ? "/card-slot-question-mark_selected.png" : "/card-slot-question-mark_normal.png"));
        }
    }
    
    void Egg::showNumber(int index)
    {
        if (index == 0 && mNumber0 != nullptr)
        {
            mNumber0->setVisible(true);
        }
        else if (index == 1 && mNumber1 != nullptr)
        {
            mNumber1->setVisible(true);
        }
    }
    
    string Egg::getNumberString(int index)
    {
        if (index == 0 && mNumber0 != nullptr)
        {
            return mNumber0->getNumberString();
        }
        else if (index == 1 && mNumber1 != nullptr)
        {
            return mNumber1->getNumberString();
        }
        
        return "";
    }
    
    string Egg::getNumberShowing(int index)
    {
        // NB(xenosoz, 2018): number string (w/ correct answer), "_" * answerStringLen (w/ no answer).
        NumberCard* card = (index == 0 ? mNumber0 : (index == 1 ? mNumber1 : nullptr));
        if (!card) { return ""; }
        
        if (card->isVisible()) { return card->getNumberString(); }
        return string(card->getNumberString().size(), '_');
    }
    
    int Egg::checkCollisionWithQuestion(Node* node)
    {
        int result = -1;
        
        float nodeWidth = node->getScale() * node->getContentSize().width;
        float nodeHeight = node->getScale() * node->getContentSize().height;
        Vec2 nodePos = node->convertToWorldSpace(Vec2(0, 0));
        Rect nodeRect = Rect(nodePos.x, nodePos.y, nodeWidth, nodeHeight);
        const float OFFSET = 20.0f;
        
        Vec2 questionPos0;
        Rect questionRect0;
        
        if (mQuestion0 != nullptr && mNumber0->isVisible() == false)
        {
            questionPos0 = mQuestion0->convertToWorldSpace(Vec2(0,0));
            questionRect0 = Rect(questionPos0.x + OFFSET, questionPos0.y + OFFSET, mQuestion0->getContentSize().width - OFFSET * 2, mQuestion0->getContentSize().height - OFFSET * 2);
            
            if (nodeRect.intersectsRect(questionRect0) == true)
            {
                result = 0;
            }
        }
        
        if (mQuestion1 != nullptr && mNumber1->isVisible() == false)
        {
            Vec2 questionPos1 = mQuestion1->convertToWorldSpace(Vec2(0,0));
            Rect questionRect1 = Rect(questionPos1.x + OFFSET, questionPos1.y + OFFSET, mQuestion1->getContentSize().width - OFFSET * 2, mQuestion1->getContentSize().height - OFFSET * 2);
            
            if (nodeRect.intersectsRect(questionRect1) == true)
            {
                if (result == 0)
                {
                    Vec2 center0 = Vec2(questionRect0.getMidX(), questionRect0.getMidY());
                    Vec2 center1 = Vec2(questionRect1.getMidX(), questionRect1.getMidY());
                    Vec2 centerNode = Vec2(nodeRect.getMidX(), nodeRect.getMidY());
                    
                    return center0.distance(centerNode) <= center1.distance(centerNode) ? 0 : 1;
                }
                else
                {
                    return 1;
                }
            }
        }
        
        return result;
    }
    
    bool Egg::isSolved()
    {
        if (mbQuestion == false)
        {
            return false;
        }
        
        if (mNumber0->isVisible() == false)
        {
            return false;
        }
        
        if (mNumber1 != nullptr && mNumber1->isVisible() == false)
        {
            return false;
        }
        
        return true;
    }
    
    void Egg::createShiningParticle()
    {
        mShiningParticleNode = Node::create();
        
        ParticleSystemQuad* particleEffect = nullptr;
        
        auto createParticleEffect = [&](std::string name, std::string plist) {
            particleEffect = ParticleSystemQuad::create(plist);
            particleEffect->setName(name);
            particleEffect->setScale(4.f);
            particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            particleEffect->setPosition(Vec2::ZERO);
            particleEffect->stopSystem();
            mShiningParticleNode->addChild(particleEffect);
        };
        
        createParticleEffect("particle1", "common/effects/particle/star_particle.plist");
        
        mShiningParticleNode->setPosition(mEggRoot->getContentSize() / 2);
        mEggRoot->addChild(mShiningParticleNode);
    }
    
    void Egg::startParticle()
    {
        if (mShiningParticleNode != nullptr)
        {
            mShiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
        }
    }
    
    void Egg::stopParticle()
    {
        if (mShiningParticleNode != nullptr)
        {
            mShiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
        }
    }
    
    void Egg::showNumberString()
    {
        if (mQuestionRoot != nullptr)
        {
            mQuestionRoot->setVisible(false);
        }
        
        mNumberRoot->setVisible(true);
    }
    
    bool Egg::hasQuestion2()
    {
        return mQuestion1 == nullptr ? false : true;
    }
    
    bool Egg::hasCorrectQuestion()
    {
        if (mNumber0->isVisible() == true)
        {
            return true;
        }
        
        if (mNumber1 != nullptr && mNumber1->isVisible() == true)
        {
            return true;
        }
        
        return false;
    }
    
    void Egg::startJumpAnimation(int audioType, int index, float duration, std::function<void()> callback)
    {
        auto action = JumpBy::create(0.1f, Vec2::ZERO, 50, 1);
        mEggRoot->runAction(Sequence::create(DelayTime::create(duration),
                                             CallFunc::create([=](){
                if (audioType == 0)
                {
                    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/" + StringUtils::format("Pattern_Doll_%d.m4a", (index + 1)));
                }
                else if (audioType == 1)
                {
                    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/" + StringUtils::format("Pattern_Flag_%d.m4a", (index + 1)));
                }
                else if (audioType == 2)
                {
                    if (mbQuestion == true)
                    {
                        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/tokenappear.m4a");
                    }
                    else
                    {
                        GameSoundManager::getInstance()->playEffectSound("NumberVoice/" + StringUtils::format("D_%s.m4a", mNumberString.c_str()));
                    }
                }

            }),
                                             action, CallFunc::create([=](){
                if (callback != nullptr)
                {
                    callback();
                }
            }),
                                             nullptr));
    }
    
    void Egg::startExplosionAnimation(std::function<void()> callback)
    {
        auto explosionRoot = mEggRoot->getChildByTag(ID_EXPLOSTION_ROOT);
        if (explosionRoot == nullptr)
        {
            return;
        }
     
        auto egg = mEggRoot->getChildByTag(ID_EGG);
        auto eggTop = explosionRoot->getChildByTag(ID_EGG_TOP);
        auto bird = explosionRoot->getChildByTag(ID_BIRD);
        auto cloud_0 = explosionRoot->getChildByTag(ID_CLOUD_0);
        auto cloud_1 = explosionRoot->getChildByTag(ID_CLOUD_1);
        auto cloud_2 = explosionRoot->getChildByTag(ID_CLOUD_2);
        
        egg->setVisible(false);
        explosionRoot->setVisible(true);
        
        auto spawnAction_0 = Spawn::create(MoveBy::create(0.3f, Point(0, 400)), FadeOut::create(0.5f), nullptr);
        eggTop->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=](){
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Drop.m4a");
            bird->setVisible(true);
            bird->runAction(ScaleBy::create(0.5f, 1.15f));
            
            auto croud_action_0 = Spawn::create(MoveBy::create(0.3f, Point(260, 80)), FadeOut::create(0.3f), nullptr);
            auto croud_action_1 = Spawn::create(MoveBy::create(0.3f, Point(280, -140)), FadeOut::create(0.3f), nullptr);
            auto croud_action_2 = Spawn::create(MoveBy::create(0.3f, Point(-260, -80)), FadeOut::create(0.3f), nullptr);
            
            cloud_0->setVisible(true);
            cloud_1->setVisible(true);
            cloud_2->setVisible(true);
            
            cloud_0->runAction(croud_action_0);
            cloud_1->runAction(croud_action_1);
            cloud_2->runAction(croud_action_2);
                                           }),
                                           spawnAction_0,
                                           DelayTime::create(0.2f),
                                           CallFunc::create([=](){
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
}
