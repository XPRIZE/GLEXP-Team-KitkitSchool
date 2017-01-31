//
//  Bird.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/23/16.
//
//

#include "Bird.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"


Bird* Bird::create(char category, int level, std::string levelID)
{
    Bird* pRet = new (std::nothrow) Bird();
    
    if(pRet && pRet->init(category, level, levelID))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Bird::init(char category, int level, std::string levelID)
{
    if (!Node::init()) {
        return false;
    }
    
    onTouchBegan = nullptr;
    onTouchEnded = nullptr;
    
    setCascadeOpacityEnabled(true);
    
    _minProgressScale = 0.4;
    _progressScale = 1.0;
    
    if (level==0) {
        _minProgressScale = 0.6;
    }

    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            if (onTouchBegan) onTouchBegan();
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            if (onTouchEnded) onTouchEnded();
        }
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    _category = category;
    _categoryLevel = level;
    _levelID = levelID;

    string soundPath;
    
    if (category=='M') {
        if (level==0) _type = BIRD_M0;
        else _type = (BirdType)(BIRD_M1 + (level-1));
        soundPath = "BirdAnimation/Sound/Reptile_M"+TodoUtil::itos(level)+".m4a";
    } else if (category=='L') {
        if (level==0) _type = BIRD_L0;
        else _type = (BirdType)(BIRD_L1 + (level-1));
        soundPath = "BirdAnimation/Sound/Bird_L"+TodoUtil::itos(level)+".m4a";
    } else {
        _type = BIRD_L1;
        
    }

    _sound = SoundEffect(soundPath);
    
    

    _aniSprite = nullptr;
    _eggSprite = nullptr;
    
    _loadedAnimationSub = "";
    
    return true;
}


void Bird::setBirdProgress(float progress)
{
    _progressScale = MIN(1.0, _minProgressScale+((1-_minProgressScale)*progress));
    refreshSize();
    
}

void Bird::refreshSize()
{
    if (_status==EGG) {
        setContentSize(_eggSprite->getContentSize()*_eggSprite->getScale());
        setAnchorPoint(Vec2(0.5, 0.05));
    } else {
        auto size = aniFramesArray.front().front()->getOriginalSize()*_scale*_progressScale;
        setContentSize(size);
        if (_aniSprite) {
            _aniSprite->setScale(_scale*_progressScale);
            _aniSprite->setPosition(Vec2(getContentSize().width/2, 0));
        }
        
        switch (_type) {
            case BIRD_L0:
                setAnchorPoint(Vec2(0.5, 0.02));
                break;
            case BIRD_M0:
                setAnchorPoint(Vec2(0.5, 0.05));
                break;
            case BIRD_L1:
                setAnchorPoint(Vec2(0.5, 0.02));
                break;
            case BIRD_L2:
                setAnchorPoint(Vec2(0.5, 0.02));
                break;
            case BIRD_L3:
                setAnchorPoint(Vec2(0.5, 0.02));
                break;
            case BIRD_L4:
                setAnchorPoint(Vec2(0.45, 0.05));
                break;
            case BIRD_L5:
                setAnchorPoint(Vec2(0.45, 0.02));
                break;
            case BIRD_M1:
                setAnchorPoint(Vec2(0.45, 0.12));
                break;
            case BIRD_M2:
                setAnchorPoint(Vec2(0.45, 0.18));
                break;
            case BIRD_M3:
                setAnchorPoint(Vec2(0.45, 0.10));
                break;
            case BIRD_M4:
                setAnchorPoint(Vec2(0.55, 0.10));
                break;
            case BIRD_M5:
                setAnchorPoint(Vec2(0.50, 0.08));
                break;
            default:
                break;
        }

        
    }
}

void Bird::setStatus(BirdStatus status)
{
    for(auto child : getChildren()) {
        child->setVisible(false);
    }
    
    _status = status;
    
    switch (status) {
        case EGG: {

            
            if (!_eggSprite) {
                std::string eggName;
                if (_categoryLevel==0) {
                    eggName = "coop_egg_english_1.png";
                } else if (_category=='M') {
                    eggName = "coop_egg_math_"+TodoUtil::itos(_categoryLevel)+".png";
                } else if (_category=='L') {
                    eggName = "coop_egg_english_"+TodoUtil::itos(_categoryLevel)+".png";
                } else {
                    eggName = "coop_egg_english_1.png";
                }
                _eggSprite = Sprite::create("BirdAnimation/"+eggName);
                _eggSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                addChild(_eggSprite);
            }
            
            _eggSprite->setVisible(true);
            refreshSize();
 
            
            break;
        }
        case EGG_HATCHED:
        
        {
            
            loadAnimation();
            
            if (_eggSprite) _eggSprite->setVisible(false);
            runIdleAnimation();
            break;
        }
            
        default:
            break;
    }
    


    
    

}

void Bird::loadAnimation()
{
    switch (_type) {
        case BIRD_L0:
            setupAnim("Bird1", 1, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M0:
            setupAnim("Bird2", 2, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_L1:
            setupAnim("Bird5", 5, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_L2:
            setupAnim("Bird4", 4, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_L3:
            setupAnim("Bird3", 3, 2, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_L4:
            setupAnim("Bird6", 6, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_L5:
            setupAnim("Bird7", 7, 2, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M1:
            setupAnim("Bird8", 8, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M2:
            setupAnim("Bird9", 9, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M3:
            setupAnim("Bird11", 11, 2, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M4:
            setupAnim("Bird10", 10, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        case BIRD_M5:
            setupAnim("Bird12", 12, 1, 40, 31, 31, 24, 1.0/0.35);
            break;
        default:
            break;
    }
}

void Bird::setupAnim(std::string animFilePrefix, int birdIndex, int numSheets, int numAni1Frames, int numAni2Frames, int numAniTouchFrames, float fps, float scale)
{
    
    
    Vector<SpriteFrame*> ani1Frames;
    Vector<SpriteFrame*> ani2Frames;
    Vector<SpriteFrame*> aniNormalFrames;
    
    float delayPerFrame = 1/fps;
    
    _scale = scale;
    

    std::string folder = StringUtils::format("BirdAnimation/%s/",animFilePrefix.c_str());
    
    std::string sub;
    std::string sheetPrefix;
    
    if (UserManager::getInstance()->isLevelCleared(_levelID)) {
        sub = "crown";
        sheetPrefix = animFilePrefix + "-crown";
        
    } else {
        sub = "default";
        sheetPrefix = animFilePrefix;
    }
    
    if (_loadedAnimationSub==sub) return;
    _loadedAnimationSub = sub;
    
    aniFramesArray.clear();
    animateArray.clear();
    
    
    
    auto cache = SpriteFrameCache::getInstance();
    
    for (int i=0; true; i++) {
        
        auto sheetPath = folder+StringUtils::format("%s-%d", sheetPrefix.c_str(), i);
        
        if (!FileUtils::getInstance()->isFileExist(sheetPath+".plist")) break;
        
        cache->addSpriteFramesWithFile(sheetPath+".plist", sheetPath+".png");
    }
    
    
    
    for (int i = 0; i < numAni1Frames; i++) {
        auto framename = StringUtils::format("%s/%s/%02d_%04d.png",animFilePrefix.c_str(), sub.c_str(), birdIndex, i+1);
        auto frame = cache->getSpriteFrameByName(framename);
        ani1Frames.pushBack(frame);
    }
    aniFramesArray.push_back(ani1Frames);
    
    auto ani1Animation = Animation::createWithSpriteFrames(ani1Frames, delayPerFrame);
    auto ani1Animate = Animate::create(ani1Animation);
    animateArray.pushBack(ani1Animate);
    
    for (int i = numAni1Frames; i < numAni1Frames+numAni2Frames; i++) {
        auto frame = cache->getSpriteFrameByName(StringUtils::format("%s/%s/%02d_%04d.png", animFilePrefix.c_str(), sub.c_str(), birdIndex, i+1));
        ani2Frames.pushBack(frame);
    }
    aniFramesArray.push_back(ani2Frames);
    
    auto ani2Animation = Animation::createWithSpriteFrames(ani2Frames,delayPerFrame);
    auto ani2Animate = Animate::create(ani2Animation);
    animateArray.pushBack(ani2Animate);
    
    
    for (int i = numAni1Frames+numAni2Frames; i < numAni1Frames+numAni2Frames+numAniTouchFrames; i++) {
        auto frame = cache->getSpriteFrameByName(StringUtils::format("%s/%s/%02d_%04d.png", animFilePrefix.c_str(), sub.c_str(), birdIndex, i+1));
        aniNormalFrames.pushBack(frame);
    }
    aniFramesArray.push_back(aniNormalFrames);
    
    auto aniNormalAnimation = Animation::createWithSpriteFrames(aniNormalFrames,delayPerFrame);
    auto aniNormalAnimate = Animate::create(aniNormalAnimation);
    animateArray.pushBack(aniNormalAnimate);
    
}

void Bird::runIdleAnimation()
{
    int pick = random(0, (int)animateArray.size()-2);
    auto animate = animateArray.at(pick);
    if (_aniSprite) {
        _aniSprite->removeFromParent();
    }
    _aniSprite = Sprite::createWithSpriteFrame(aniFramesArray.at(pick).front());
    refreshSize();
    _aniSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _aniSprite->setPosition(Vec2(getContentSize().width/2, 0));
    

    
    addChild(_aniSprite);
    auto seq = Sequence::create(animate->clone(), DelayTime::create(random(1.0, 2.0)), CallFunc::create([this]() {
        runIdleAnimation();
    }), NULL);
    _aniSprite->runAction(seq);
}


void Bird::runTouchAnimation()
{

    if (_status<EGG_HATCHED) {
        SoundEffect::eggClickEffect().play();
        return;
    }
    
    _sound.stop();
    _sound.play();
    
    auto animate = animateArray.back();
    if (_aniSprite) {
        _aniSprite->removeFromParent();
    }
    _aniSprite = Sprite::createWithSpriteFrame(aniFramesArray.back().front());
    refreshSize();
    _aniSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _aniSprite->setPosition(Vec2(getContentSize().width/2, 0));
    

    
    addChild(_aniSprite);
    auto seq = Sequence::create(animate->clone(), DelayTime::create(random(1.5, 2.5)), CallFunc::create([this]() {
        runIdleAnimation();
    }), NULL);
    _aniSprite->runAction(seq);
}


void Bird::runCrackAnimation()
{
    if (_status!=EGG) return;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BirdAnimation/egg_crack.plist", "BirdAnimation/egg_crack.png");
    

    Vector<SpriteFrame*> frames;
    for (int i = 0; i < 5; i++) {
        auto framename = StringUtils::format("egg_crack_stage_%d.png",i+1);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
        frames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(frames, 0.15);
    auto animate = Animate::create(animation);
    
    auto crackSprite = Sprite::createWithSpriteFrame(frames.front());
    crackSprite->setPosition(_eggSprite->getContentSize()/2);
    _eggSprite->addChild(crackSprite);
    _eggSprite->setCascadeOpacityEnabled(true);
    crackSprite->setOpacity(128);
    crackSprite->runAction(animate);
    
  
    
    
}
