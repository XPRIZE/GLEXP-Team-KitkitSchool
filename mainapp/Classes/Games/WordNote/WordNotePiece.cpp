//
//  Piece.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 21/02/2018.
//

#include "WordNotePiece.hpp"
#include "PlateSlot.hpp"
#include "NoteSlot.hpp"

BEGIN_NS_WORDNOTE;

void randomShuffle(vector<int> *vec) {
    random_shuffle(vec->begin(), vec->end(), [](int n) { return rand() % n; });
}

bool Piece::init() {
    if (!Node::init()) return false;
    _paper = nullptr;
    _touchEnabled = true;
    _placedNoteSlot = nullptr;
    _registeredPlateSlot = nullptr;
    _isOnPlateSlot = false;
    _correctActionFlag = false;
    
    _serialNumber = ++ _serialCounter;
    _letters.clear();
    _fadeOutCount=0;
    
    onTouchBegan = nullptr;
    onTouchMoved = nullptr;
    onTouchEnded = nullptr;

    setCascadeOpacityEnabled(true);

    return true;
}

int Piece::_serialCounter = 0;

void Piece::setLetter(string letter) {

    Color4B fontColor;
    Color4B fontBlack = Color4B(65,64,62,255);
    Color4B fontWhite = Color4B(255,255,255,255);
    
    vector<string> fontPaths = {
        "fonts/Aileron-Regular.otf",
        "fonts/OpenSans-Bold.ttf",
        "fonts/Seshat.otf",
        "fonts/TodoSchoolV2.ttf",
        "fonts/Andika-R.ttf"
    };
    random_shuffle(fontPaths.begin(), fontPaths.end(), [](int n) { return rand() % n; });

    vector<int> paperTypes;
    char paperFileName[256];

    switch(letter.size()) {
        case 1:
            paperTypes = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
            WordNote::randomShuffle(&paperTypes);
            switch(paperTypes[0]) {
                case 6: case 15: case 16: case 23: case 25: fontColor = fontWhite; break;
                default: fontColor = fontBlack; break;
            }
            sprintf(paperFileName, "WordNote/Images/WordNote_image_paper_%02d.png", paperTypes[0]);
            break;
        case 2:
            paperTypes = {1,2,3,4,5,6,7};
            WordNote::randomShuffle(&paperTypes);
            switch(paperTypes[0]) {
                case 3: fontColor = fontWhite; break;
                default: fontColor = fontBlack; break;
            }
            sprintf(paperFileName, "WordNote/Images/WordNote_image_paper-dubble-slots_%02d.png", paperTypes[0]);
            break;
        case 3:
            paperTypes = {1,2,3,4};
            WordNote::randomShuffle(&paperTypes);
            switch(paperTypes[0]) {
                case 4: fontColor = fontWhite; break;
                default: fontColor = fontBlack; break;
            }
            sprintf(paperFileName, "WordNote/Images/WordNote_image_paper_triple_%02d.png", paperTypes[0]);
            break;
        case 4:
            paperTypes = {1,2,3};
            WordNote::randomShuffle(&paperTypes);
            fontColor = fontBlack;
            sprintf(paperFileName, "WordNote/Images/WordNote_image_paper_quad_%02d.png", paperTypes[0]);
            break;
    }
    
    //CCLOG("random: %d", paperTypes[0]);
        
    auto paper = Sprite::create(paperFileName);
    setContentSize(paper->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //setPosition(getContentSize()/2*-1);
    paper->setPosition(paper->getContentSize()/2);
    addChild(paper);
    
    int totalLetterLength = letter.length();
    float startXPos = 160*totalLetterLength/2*-1;
    
    for (int i=0; i<totalLetterLength; i++) {
        auto paperLetter = TodoUtil::createLabel(string(1,letter[i]), 130, Size(160,180), fontPaths[0], fontColor, TextHAlignment::CENTER);
        paperLetter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        paperLetter->setPosition(startXPos + ((float)((2*i)+1) / (totalLetterLength*2)) * 160*totalLetterLength + paper->getContentSize().width/2, 7 + paper->getContentSize().height/2);
        paper->addChild(paperLetter);
        _letters.push_back(paperLetter);
    }
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (!_touchEnabled) return false;
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos)) {
            if (onTouchBegan) onTouchBegan();
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event) {
        auto P = getParent();
        auto pl = P->convertToNodeSpace(touch->getPreviousLocation());
        auto cl = P->convertToNodeSpace(touch->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        if (onTouchMoved) onTouchMoved(touch);

    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* E) {
        if (onTouchEnded) onTouchEnded(touch);
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    //auto rectNode = DrawNode::create(); rectNode->drawRect(Vec2(this->getBoundingBox().getMinX(), this->getBoundingBox().getMinY()), Vec2(this->getBoundingBox().getMaxX(), this->getBoundingBox().getMaxY()), Color4F::GREEN); addChild(rectNode);

    _paper = paper;
    _letter = letter;

    createShiningParticle();

}

void Piece::setTouchEnable(bool enable) {
    if (enable) {
        _touchEnabled = true;
        _paper->setOpacity(255);
        for (auto letter: _letters) {
            Color3B letterColor = (Color3B)letter->getTextColor();
            letter->setTextColor(Color4B(letterColor, 255));
        }
    } else {
        _touchEnabled = false;
        _paper->setOpacity((int)(255*0.15));

        for (auto letter: _letters) {
            Color3B letterColor = (Color3B)letter->getTextColor();
            letter->setTextColor(Color4B(letterColor, (int)(255*0.15)));
        }
    }
}

void Piece::togglePicked(bool picked) {
    /*
    if (picked) {
        _paper->runAction(MoveBy::create(0.15, Vec2(0,100)));
    } else {
        _paper->runAction(MoveBy::create(0.15, Vec2(0,-100)));
    }
     */
}

void Piece::place(NoteSlot *noteSlot) {
    _placedNoteSlot = noteSlot;
    noteSlot->_placedPiece = this;
    this->runAction(EaseIn::create(MoveTo::create(0.1, noteSlot->getPosition()),3.0));
}

void Piece::createShiningParticle()
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
    
    _shiningParticleNode->setPosition(_paper->getContentSize()/2);
    _paper->addChild(_shiningParticleNode);
    
}

void Piece::startParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
}

void Piece::stopParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
}

void Piece::fadeOutLetter() {
    
    this->runAction(Repeat::create(
    Sequence::create(
        CallFunc::create([this](){
            _fadeOutCount++;
            for (auto letter: _letters) {
                Color3B letterColor = (Color3B)letter->getTextColor();
                letter->setTextColor(Color4B(letterColor, (int)255*(10-_fadeOutCount)/10));
            }
        }),
        DelayTime::create(0.1),
        nullptr
    ), 10));

    
}

string Piece::getLetter() { return _letter; }

END_NS_WORDNOTE;

