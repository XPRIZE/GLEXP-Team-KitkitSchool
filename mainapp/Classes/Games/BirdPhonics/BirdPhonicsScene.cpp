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
#include "Managers/StrictLogManager.h"
#include "Managers/UserManager.hpp"
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
    
    
    const char* defaultFont = "fonts/mukta-bold.ttf";
    const Size gameSize = Size(2560, 1800);
    
    const float floorY = 878;
    bool singleTouch = true;
    double chirpTime;

    
    

    SoundEffect solveEffect() { return SoundEffect("BirdPhonics/Effect/Success.m4a"); }
    
    SoundEffect lightEffect() { return SoundEffect("BirdPhonics/Effect/Light_on.m4a"); }
    
    double getCurrentTimeOfDouble() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return static_cast<double>(time.tv_sec + static_cast<double>(time.tv_usec) / 1000000.f);
    }
}

using namespace BirdPhonicsSceneSpace;


/*************
 Bread
 *************/

void Bread::setWord(string word, string phonic_owner, string word_sound, string phonic_position)
{
    _word = word;
    _phonic_owner = phonic_owner;
    _touchEnabled = false;
    _isEaten = false;
    auto sprite = Sprite::create("BirdPhonics/phonics_bread_answers.png");
    auto size = sprite->getContentSize();
    
    sprite->setAnchorPoint(Vec2::ZERO);
    addChild(sprite);
    
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    {
        auto split = TodoUtil::split(phonic_position, ',');
        auto l = Node::create();
        float width = 0;
        float maxHeight = 0;
        for (int i = 0; i < word.length(); ++i)
        {
            Label* label;
            string str;
            if (std::find(split.begin(), split.end(), TodoUtil::itos(i + 1)) != split.end())
            {
                label = TodoUtil::createLabel(str + word.at(i), 150, Size::ZERO, defaultFont,
                                              Color4B(120, 43, 0, 255));
            }
            else
            {
                label = TodoUtil::createLabel(str + word.at(i), 150, Size::ZERO, defaultFont,
                                              Color4B(69, 31, 9, 255));
            }
            
            label->setAnchorPoint(Vec2::ZERO);
            label->setPosition(Vec2(width, 0));

            width += label->getContentSize().width;
            if (label->getContentSize().height > maxHeight)
            {
                maxHeight = label->getContentSize().height;
            }
            
            l->addChild(label);
        }
        
        l->setContentSize(Size(width, maxHeight));
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(size.width/2, size.height/2+20);
        addChild(l);

    }
    
    auto path = StringUtils::format("Games/BirdPhonics/sounds/%s", word_sound.c_str());
    _wordEffect.setPath(path);
    
    singleTouch = true;
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        
        if (!_touchEnabled) return false;
        if (_isEaten) return false;
        
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
        
        if (!singleTouch) return false;
        if (_isEaten) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            singleTouch = false;
            _wordEffect.stop();
            _wordEffect.play();
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
//            this->retain();
//            this->removeFromParent();
//            P->addChild(this);
//            this->release();
            
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
        singleTouch = true;
        
        if (birdBox.intersectsRect(breadBox)) {
            this->_targetBird->eatBread(this);
            
            if (onGoodDown) {
                onGoodDown();
            }
        } else {
            this->_targetBird->readyToEat(false);
            this->runAction(EaseOut::create(MoveTo::create(0.2, this->_orgPos), 2.0));
            
            if (onBadDown) {
                onBadDown();
            }
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

void Bird::setPhonic(string phonic_owner, string phonic_sound)
{
    _phonic_owner = phonic_owner;
    
    string path;
    path = StringUtils::format("Games/BirdPhonics/sounds/%s", phonic_sound.c_str());
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
        chirpTime = getCurrentTimeOfDouble();
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
    bread->_isEaten = true;
    bread->_touchEnabled = false;
    _touchEnabled = false;
    

    auto birdHeight = this->getContentSize().height;
    auto birdCenter = this->getPosition() + Vec2(0, birdHeight/2);
    
    auto breadSeq = Spawn::create(MoveTo::create(0.5, birdCenter ),
                                  ScaleTo::create(0.5, 0.0),
                                  nullptr);
    bread->runAction(breadSeq);
    
    bread->_wordEffect.stop();
    double now = getCurrentTimeOfDouble();

    if (now - chirpTime > 0.5f) {
        bread->_wordEffect.play();

    } else {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([this, bread]() {
                                            bread->_wordEffect.play();
        }), nullptr));
    }

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
	}), nullptr);
	auto birdSeq2 = Sequence::create(JumpBy::create(1.0, Vec2::ZERO, 100, 3),
                                    CallFunc::create([this, bread](){
        calm();
        _touchEnabled = true;
        this->_onEat();
    }),

                                    nullptr);
    
    this->runAction(Sequence::create( birdSeq, birdSeq2, nullptr));
                                
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
    string p = LanguageManager::getInstance()->findLocalizedResource("Games/BirdPhonics/BirdPhonics_level.tsv");
    string s = FileUtils::getInstance()->getStringFromFile(p);
    auto data = TodoUtil::readTSV(s);
    
    // data[0][2] : Worksheet or WorksheetAll
    bool bWorkSheetAll = UserManager::getInstance()->isWorksheetTestMode();
    
    int maxWorkSheet = 0;
    for (vector<string> row : data)
    {
        if (row.size() < 1)
            continue;
        
        if (TodoUtil::trim(row[0]).size() <= 0)
            continue;
        
        if (row[0][0] == '#')
            continue;

        if (TodoUtil::stoi(row[1]) == _currentLevel)
        {
            Problem p;
            p.workSheet = TodoUtil::stoi(row[2]);
            
            if (p.workSheet > maxWorkSheet)
            {
                maxWorkSheet = p.workSheet;
            }
            
            p.phonics_owner.push_back("blue");
            p.phonics_sound.push_back(row[4]);
            
            p.phonics_owner.push_back("red");
            p.phonics_sound.push_back(row[5]);

            p.words.push_back(WordPair(row[8], row[6]));
            p.words_sound.push_back(row[7]);
            p.phonic_positions.push_back(row[9]);
            
            p.words.push_back(WordPair(row[12], row[10]));
            p.words_sound.push_back(row[11]);
            p.phonic_positions.push_back(row[13]);
            
            p.words.push_back(WordPair(row[16], row[14]));
            p.words_sound.push_back(row[15]);
            p.phonic_positions.push_back(row[17]);
            
            p.words.push_back(WordPair(row[20], row[18]));
            p.words_sound.push_back(row[19]);
            p.phonic_positions.push_back(row[21]);
            
            _problems.push_back(p);
        }
    }
    
    if (bWorkSheetAll == false)
    {
        int curWorksheet = RandomHelper::random_int(1, maxWorkSheet);
        
        for (int i = _problems.size() - 1; i >= 0; --i)
        {
            if (_problems.at(i).workSheet != curWorksheet)
            {
                _problems.erase(_problems.begin() + i);
            }
        }
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
    for (int i = 0; i < problem.phonics_owner.size(); ++i) {
        auto bird = _birds[birdIndex];
        
        bird->setPhonic(problem.phonics_owner.at(i), problem.phonics_sound.at(i));
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
    
    string workPath = [this] {
        stringstream ss;
        ss << "/" << "BirdPhonics";
        ss << "/" << "level-" << _currentLevel << "-" << _problems[_currentProblemIndex].workSheet;
        ss << "/" << "work-" << _currentProblemIndex;
        return ss.str();
    }();

    for (int i = 0; i < problem.words.size(); ++i)
    {
        auto b = Bread::create();
        b->setWord(problem.words.at(i).second, problem.words.at(i).first, problem.words_sound.at(i), problem.phonic_positions.at(i));
        b->onGoodDown = [this, b, workPath] {
            StrictLogManager::shared()->game_Peek_Answer("BirdPhonics", workPath, b->_word, b->_word);
        };
        b->onBadDown = [this, b, workPath] {
            StrictLogManager::shared()->game_Peek_Answer("BirdPhonics", workPath, b->_word, "None");
        };
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
            if (bird->_phonic_owner == bread->_phonic_owner) {
                bread->_targetBird = bird;
            }
        }
        
        
    }
}

std::vector<int> BirdPhonicsScene::getLevelIDs()
{
    std::vector<int> result;
    
    string p = LanguageManager::getInstance()->findLocalizedResource("Games/BirdPhonics/BirdPhonics_level.tsv");
    string s = FileUtils::getInstance()->getStringFromFile(p);
    auto data = TodoUtil::readTSV(s);
    int level;
    for (auto row : data)
    {
        if (row.size() < 1)
            continue;
        
        if (TodoUtil::trim(row[0]).size() <= 0)
            continue;
        
        if (row[0][0] == '#')
            continue;
        
        level = TodoUtil::stoi(row[1]);
        if(std::find(result.begin(), result.end(), level) == result.end())
        {
            result.push_back(level);
        }
    }

    return result;
}
