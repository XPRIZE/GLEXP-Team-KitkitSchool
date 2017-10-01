//
//  BirdPhonicsScene.cpp
//
//  Created by Gunho Lee on 6/07/17.
//
//



#include "BirdPhonicsScene.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"
#include "Common/Basic/SoundEffect.h"
#include "Managers/LanguageManager.hpp"
#include "Utils/TodoUtil.h"


#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace BirdPhonicsSceneSpace {
  //  const char* solveEffect = "Counting/UI_Star_Collected.m4a";
  //  const char* missEffect = "Counting/Help.m4a";
  //  const char* touchEffect = "Counting/tokenappear.m4a";
  //  const char* dropEffect = "Counting/panelput.m4a";
    
    
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    const Size gameSize = Size(2560, 1800);
    
    const float floorY = 878;
    

    
    

    SoundEffect solveEffect() { return SoundEffect("BirdPhonics/Effect/Success.m4a"); }
    
    SoundEffect lightEffect() { return SoundEffect("BirdPhonics/Effect/Light_on.m4a"); }
    

    
}

using namespace BirdPhonicsSceneSpace;


/*************
 Bread
 *************/

void Bread::setWord(string word, char phonic)
{
    _word = word;
    _phonic = phonic;
    _touchEnabled = false;
    
    
    auto sprite = Sprite::create("BirdPhonics/phonics_bread_answers.png");
    auto size = sprite->getContentSize();
    
    sprite->setAnchorPoint(Vec2::ZERO);
    addChild(sprite);
    
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    {
        auto w1 = word.substr(0, 1);
        auto w2 = word.substr(1);
        
        auto l1 = TodoUtil::createLabel(w1, 150, Size::ZERO, defaultFont, Color4B(120, 43, 0, 255));
        l1->setAnchorPoint(Vec2::ZERO);
        auto s1 = l1->getContentSize();
        
        
        auto ss = Size(30, 150);
        
        auto l2 = TodoUtil::createLabel(w2, 150, Size::ZERO, defaultFont, Color4B(69, 31, 9, 255));
        l2->setAnchorPoint(Vec2::ZERO);
        auto s2 = l2->getContentSize();
        
        auto l = Node::create();
        l->setContentSize(Size(s1.width+s2.width+ss.width/2, MAX(s1.height, s2.height)));
        l2->setPosition(Vec2(s1.width+ss.width/2, 0));
        l->addChild(l1);
        l->addChild(l2);
        
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(size/2);
        addChild(l);
        
    }
    
    auto path = StringUtils::format("BirdPhonics/Word/%s.m4a", word.c_str());
    _wordEffect.setPath(path);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        auto box = this->getBoundingBox();
        
        if (box.containsPoint(pos)) {
            
            _wordEffect.stop();
            _wordEffect.play();
            
            return true;
        }
        return false;
    };
    
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            
            _wordEffect.stop();
            _wordEffect.play();
            
            this->retain();
            this->removeFromParent();
            P->addChild(this);
            this->release();
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        
        auto breadBox = this->getBoundingBox();
        auto birdBox = this->_targetBird->getBoundingBox();
        if (birdBox.intersectsRect(breadBox)) {
            this->_targetBird->readyToEat(true);
        } else {
            this->_targetBird->readyToEat(false);
        }
        //        auto tp = _targetPos;
        //        auto cp = getPosition();
        //
        //
        //
        //        if (_targetPos.distance(getPosition())<snapRadiusOnMoved) {
        //            this->snapTarget();
        //
        //        }
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        auto breadBox = this->getBoundingBox();
        auto birdBox = this->_targetBird->getBoundingBox();
        if (birdBox.intersectsRect(breadBox)) {
            this->_targetBird->eatBread(this);
        } else {
            this->_targetBird->readyToEat(false);
            this->runAction(EaseOut::create(MoveTo::create(0.2, this->_orgPos), 2.0));
        }
        
        //        if (_snapped) return;
        //        setPicked(false);
        //
        //        if (_targetPos.distance(getPosition())<snapRadiusOnEnded) {
        //            this->snapTarget();
        //
        //        }
        
        
        
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
}



/*************
 Bird
 *************/


void Bird::setType(int type, std::function<void(void)> func)
{
    if (type==1) {
        _silhouetteName = "BirdPhonics/phonics_birdtypeB_silhouette.png";
        _normalName = "BirdPhonics/phonics_birdtypeB_option1_normal.png";
        _activeName = "BirdPhonics/phonics_birdtypeB_option1_active.png";
        _correctName = "BirdPhonics/phonics_birdtypeB_option1_correct.png";

        
    } else if (type==2) {
        _silhouetteName = "BirdPhonics/phonics_birdtypeA_silhouette.png";
        _normalName = "BirdPhonics/phonics_birdtypeA_option1_normal.png";
        _activeName = "BirdPhonics/phonics_birdtypeA_option1_active.png";
        _correctName = "BirdPhonics/phonics_birdtypeA_option1_correct.png";
    }
    
    _onEat = func;
    _isChirping = false;
    
    _body = Sprite::create(_silhouetteName);
    auto size = _body->getContentSize();
    setContentSize(size);
    _body->setAnchorPoint(Vec2::ZERO);
    addChild(_body);
    
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
 
    _type = type;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {

        if (!_touchEnabled) return false;
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        auto box = this->getBoundingBox();
        
        if (box.containsPoint(pos)) {
            
            this->chirp();
            return true;
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    _touchEnabled = false;
    
}

void Bird::setPhonic(char phonic)
{
    _phonic = phonic;
    
    auto path = StringUtils::format("BirdPhonics/Phonic/%c_sound_f.wav", phonic);
    _phonicEffect.setPath(path);
    
    
}

void Bird::turnLight(bool isOn)
{
    
    if (isOn) {
        _touchEnabled = true;
        _light->setVisible(true);
        calm();
        
    } else {
        _touchEnabled = false;
        _light->setVisible(false);
        _body->setTexture(_silhouetteName);
    
    }
}


void Bird::chirp(bool autocalm)
{
    if (!_isChirping || autocalm) {
        _body->setTexture(_activeName);
        _phonicEffect.stop();
        _phonicEffect.play();
        _isChirping = true;
    }
    
    if (autocalm) {
        auto seq = Sequence::create(DelayTime::create(0.6),
                                    CallFunc::create([this](){
            calm();
        }), NULL);
        this->runAction(seq);
    }

}

void Bird::calm()
{
    _isChirping = false;
    _body->setTexture(_normalName);
}

void Bird::readyToEat(bool ready)
{
    if (ready) {
        chirp(false);
    } else {
        calm();
    }
}


void Bird::eatBread(Bread *bread)
{
    bread->_touchEnabled = false;
    _touchEnabled = false;
    

    auto birdHeight = this->getContentSize().height;
    auto birdCenter = this->getPosition() + Vec2(0, birdHeight/2);
    
    auto breadSeq = Spawn::create(MoveTo::create(0.5, birdCenter ),
                                  ScaleTo::create(0.5, 0.0),
                                  nullptr);
    bread->runAction(breadSeq);
    
    bread->_wordEffect.stop();
    bread->_wordEffect.play();

    calm();

    auto birdSeq = Sequence::create(DelayTime::create(0.2),
                                    CallFunc::create([this]() { _body->setTexture(_activeName); }),
                                    DelayTime::create(0.2),
                                    CallFunc::create([this]() { _body->setTexture(_normalName); }),
                                    DelayTime::create(0.2),
                                    CallFunc::create([this]() { _body->setTexture(_activeName); }),
                                    DelayTime::create(0.2),
                                    CallFunc::create([this]() { _body->setTexture(_normalName); }),
                                    DelayTime::create(0.2),
                                    CallFunc::create([this, bread](){
        bread->setVisible(false);
        solveEffect().play();
        this->_body->setTexture(_correctName);
    }),
                                    
                                    JumpBy::create(1.0, Vec2::ZERO, 100, 3),
                                    CallFunc::create([this, bread](){
        calm();
        _touchEnabled = true;
        this->_onEat();
    }),

                                    nullptr);
    
    this->runAction(birdSeq);
                                
}

/*************
 BirdPhonicsScene
 *************/



Scene* BirdPhonicsScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BirdPhonicsScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool BirdPhonicsScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchEnabled = false;
    
    solveEffect().preload();
    lightEffect().preload();
    
    auto winSize = getContentSize();
    
    {
        auto bg = Sprite::create("BirdPhonics/phonics_bg.jpg");
        auto bgSize = bg->getContentSize();
        float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
        bg->setScale(bgScale);
        bg->setPosition(winSize/2);
        addChild(bg);
    }
    
    {
        auto bg = Sprite::create("BirdPhonics/phonics_bottom_bg.png");
        auto bgSize = bg->getContentSize();
        float bgScale = MAX(1.0, winSize.width/bgSize.width);
        bg->setScale(bgScale);
        
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        bg->setPosition(winSize.width/2, floorY);
        
        addChild(bg);
    }

    
    _gameNode = Node::create();
    _gameNode->setContentSize(winSize);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    addChild(_gameNode);

    
    for (int i=0; i<2; i++)
    {
        auto light = Sprite::create("BirdPhonics/phonics_spotlight_normal.png");
        light->setVisible(false);
        
        auto bird = Bird::create();
        bird->setType(i+1, [this](){
            _numEatenBreads++;
            if (_numEatenBreads == _breads.size()) {
                this->onStageComplete();
            }
        });
        bird->setLight(light);
        
        
        float x = winSize.width*(i==0 ? 0.3 : 0.7);
        light->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        light->setPosition(Vec2(x, winSize.height));
        bird->setPosition(Vec2(x, floorY));
        

        _gameNode->addChild(light);
        _gameNode->addChild(bird);
        
        _birds.push_back(bird);
    }
   
    
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    return true;
}

void BirdPhonicsScene::onEnter()
{
    Layer::onEnter();


   // _progressBar->setMax((int)sheet.size());

}

void BirdPhonicsScene::onEnterTransitionDidFinish()
{
    onStart();
}

void BirdPhonicsScene::onStart()
{
    
    {
        Problem p;
        p.phonics.push_back('h');
        p.phonics.push_back('c');
        p.words.push_back(WordPair('h', "high"));
        p.words.push_back(WordPair('h', "heat"));
        p.words.push_back(WordPair('c', "care"));
        p.words.push_back(WordPair('c', "call"));
        
        _problems.push_back(p);
    }

    {
        Problem p;
        p.phonics.push_back('t');
        p.phonics.push_back('b');
        p.words.push_back(WordPair('t', "truck"));
        p.words.push_back(WordPair('t', "train"));
        p.words.push_back(WordPair('b', "bird"));
        p.words.push_back(WordPair('b', "bus"));
        
        _problems.push_back(p);
    }

    
    {
        Problem p;
        p.phonics.push_back('g');
        p.phonics.push_back('j');
        p.words.push_back(WordPair('g', "grass"));
        p.words.push_back(WordPair('g', "give"));
        p.words.push_back(WordPair('j', "giraffe"));
        p.words.push_back(WordPair('j', "jungle"));
        
        _problems.push_back(p);
    }

    
    
    _progressBar->setMax(_problems.size());
    

    showProblem(0);
  
    
}

void BirdPhonicsScene::onStageComplete()
{
    _progressBar->setCurrent(_currentProblemIndex+1, true);
    
    for (auto bird : _birds) {
        bird->setTouchEnabled(false);
    }

    
    if (_currentProblemIndex+1 == _problems.size()) {
        auto CP = CompletePopup::create();
        CP->show(1.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    } else {
        
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([this](){
            lightEffect().play();
            for (auto b : _birds) {
                b->turnLight(false);
            }
        }),
                                         DelayTime::create(1.0),
                                         CallFunc::create([this](){
            showProblem(_currentProblemIndex+1);

        }),
                                         nullptr));
                        
        
    }
}


void BirdPhonicsScene::setLevel(int level)
{
    _currentLevel = level;
}

void BirdPhonicsScene::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
    
}

void BirdPhonicsScene::showProblem(int index)
{
    _numEatenBreads = 0;
    
    _currentProblemIndex = index;
    _progressBar->setCurrent(_currentProblemIndex+1, false);
    
    auto problem = _problems[index];
    
    int birdIndex = 0;
    for (auto phonic : problem.phonics) {
        auto bird = _birds[birdIndex];
        
        bird->setPhonic(phonic);
        bird->setTouchEnabled(true);
        birdIndex++;
    }
    
    
    
    runAction(Sequence::create(DelayTime::create(1.0),
                               CallFunc::create([this](){ lightEffect().play(); _birds[0]->turnLight(true); }),
                               DelayTime::create(0.3),
                               CallFunc::create([this](){ _birds[0]->chirp(); }),
                               
                               DelayTime::create(1.0),
                               CallFunc::create([this](){ lightEffect().play(); _birds[1]->turnLight(true); }),
                               DelayTime::create(0.3),
                               CallFunc::create([this](){ _birds[1]->chirp(); }),
                               
                               DelayTime::create(1.0),
                               CallFunc::create([this](){ this->createBread(); }),
                               
                               NULL));
    

}


void BirdPhonicsScene::createBread()
{
    
    
    for (auto b : _breads) {
        b->removeFromParent();
    }
    _breads.clear();
    auto problem = _problems[_currentProblemIndex];
    
    for (auto wordPair : problem.words)
    {
        auto b = Bread::create();
        b->setWord(wordPair.second, wordPair.first);
        addChild(b);
        _breads.push_back(b);
    }
    
    random_shuffle(_breads.begin(), _breads.end());
    
    double Ps[][2] = { { 0.3, 0.3 }, {0.7, 0.3}, {0.3, 0.7}, {0.7, 0.7}};
    auto winSize = getContentSize();
    
    for (int i=0; i<4; i++) {
        auto bread = _breads[i];
        bread->_orgPos = Vec2(Ps[i][0]*winSize.width, Ps[i][1]*floorY);
        bread->setPosition(bread->_orgPos - Vec2(0, floorY));
        
        auto breadSeq = Sequence::create(EaseIn::create(MoveTo::create(0.7, bread->_orgPos), 2.0),
                                         JumpBy::create(0.3, Vec2::ZERO, 40, 1),
                                         nullptr);
        bread->runAction(breadSeq);
        
        bread->_touchEnabled = true;
        for (auto bird : _birds) {
            if (bird->_phonic == bread->_phonic) {
                bread->_targetBird = bird;
            }
        }
        
        
    }
}


