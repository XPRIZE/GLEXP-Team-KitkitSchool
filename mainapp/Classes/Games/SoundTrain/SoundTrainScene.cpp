//
//  SoundTrainScene.cpp
//  KitkitSchool
//
//  Created by JungJaehun on 30/08/2017.
//
//

#include "SoundTrainScene.hpp"

#include "CCAppController.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Effects/FireworksEffect.hpp"


BEGIN_NS_SOUNDTRAIN;

const float gameNodeHeight = 1800; // 1536, 1800

const string soundSlotIn = "Common/Sounds/Effect/BlockSlotIn.m4a";
const string soundTouch = "Common/Sounds/Effect/BlockTouch.m4a";
const string soundMiss = "Common/Sounds/Effect/BlockMiss.m4a";
const string soundCombine = "SoundTrain/Sound/TrainCombine.m4a";
const string soundCome = "NumberTrain/Sounds/Train2.m4a";
const string soundMove = "NumberTrain/Sounds/Train1.m4a";
const string soundWhistle1 = "SoundTrain/Sound/Pattern_Train_1.m4a";
const string soundWhistle2 = "SoundTrain/Sound/Pattern_Train_2.m4a";
const string soundWhistle3 = "SoundTrain/Sound/Pattern_Train_3.m4a";
const string fontName = "fonts/Andika-R.ttf";
bool isSW = false;
bool isEN = false;

bool SoundTrainScene::init() {
    LOGFN();
    if (!Layer::init()) return false;
    if (LanguageManager::getInstance()->getCurrentLanguageTag() == "en-US") isEN = true; else isSW = true;
    
    this->initUI();
    
    return true;
}

Scene* SoundTrainScene::createScene(int levelID)
{
    LOGFN();
    auto scene = Scene::create();
    
    auto layer = SoundTrainScene::create();
    layer->setLevel(levelID);

    scene->addChild(layer);
    
    return scene;
}

void SoundTrainScene::setLevel(int level)
{
    LOGFN();
    _currentLevel = level;
    SoundTrainProblemBank *problemBank = new SoundTrainProblemBank();
    _problems = problemBank->loadData(_currentLevel);
    _progressBar->setMax(_problems.size());
    _totalProblemCount = _problems.size();
}


void SoundTrainScene::onEnter(){
    LOGFN();
    Layer::onEnter();
    
    _currentProblemIndex = 0;
    SoundTrainScene::setProblem();
}

void SoundTrainScene::onExit() {
    LOGFN();

    Layer::onExit();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void SoundTrainScene::initUI() {
    LOGFN();
    
    _winSize = getContentSize();
    _gameSize = Size(_winSize.width/_winSize.height*gameNodeHeight, gameNodeHeight);
    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    float gameScale = _winSize.height/gameNodeHeight;
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    addChild(_gameNode);
    
    auto bg = Sprite::create("SoundTrain/_train_sound_bg.png");
    auto bgSize = bg->getContentSize();
    //auto bgScale = MAX(_gameSize.width / bgSize.width, _gameSize.height / bgSize.height);
    auto bgScaleX = _gameSize.width / bgSize.width;
    auto bgScaleY = _gameSize.height / bgSize.height;
    bg->setScale(bgScaleX, bgScaleY);
    bg->setPosition(_gameSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->addChild(bg);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _gameSize.height-25));
    _gameNode->addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_gameSize.width/2, _gameSize.height - _progressBar->getContentSize().height));
    _gameNode->addChild(_progressBar);

    GameSoundManager::getInstance()->preloadEffect(soundSlotIn);
    GameSoundManager::getInstance()->preloadEffect(soundTouch);
    GameSoundManager::getInstance()->preloadEffect(soundMiss);
    GameSoundManager::getInstance()->preloadEffect(soundCombine);
    GameSoundManager::getInstance()->preloadEffect(soundMove);
    GameSoundManager::getInstance()->preloadEffect(soundWhistle1);
    GameSoundManager::getInstance()->preloadEffect(soundWhistle2);
    GameSoundManager::getInstance()->preloadEffect(soundWhistle3);
    
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    string item;
    for (char ch: s) {
        if (ch == delim) {
            if (!item.empty())
                elems.push_back(item);
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty())
        elems.push_back(item);
    return elems;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to){
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void SoundTrainScene::setProblem(){
    LOGFN();
    _progressBar->setCurrent(_currentProblemIndex+1, false);
    if (_currentProblemIndex == _totalProblemCount) {
        onComplete();
        return;
    }
    
    auto problem = _problems[_currentProblemIndex];

    _trainName = ReplaceAll(problem.answer, string(","), string(""));
    _questions = TodoUtil::split(problem.answer, ',');
    _answers = TodoUtil::split(problem.answer, ',');
    random_shuffle(_answers.begin(), _answers.end(), [](int n) { return rand() % n; });

    runAction(Sequence::create(
        CallFunc::create([this](){
            SoundTrainScene::trainCome();
        }),
        DelayTime::create(1.7),
        CallFunc::create([this](){
            SoundTrainScene::cardCome();
        }),
        nullptr
    ));
    
    
}


void SoundTrainScene::checkReadyToSubmit() {
    LOGFN();
    checkAnswer();
    /*
    bool isReadyToSubmit = true;
    for (auto v : _freightTrains) {
        if (!v->_cardInTrain) isReadyToSubmit = false;
    }
    if (isReadyToSubmit) checkAnswer();
     */
}

void SoundTrainScene::checkAnswer() {
    LOGFN();
    bool hasIncorrectAnswer = false;
    bool finished = true;
    stringstream userAnswer;
    stringstream correctAnswer;

    for (auto v : _freightTrains) {
        userAnswer << (v->_cardInTrain ? v->_cardInTrain->_letter : "_");
        correctAnswer << (v->_correctAnswer);
        
        if (!v->_cardInTrain) {
            finished = false;
            continue;
        }
        
        if (v->_correctAnswer.compare(v->_cardInTrain->_letter) != 0) {
            auto card = v->_cardInTrain;
            card->unload(_gameNode);
            card->_targetTrain = nullptr;
            card->runAction(MoveTo::create(0.2, card->_originalPos));
            GameSoundManager::getInstance()->playEffectSound(soundMiss);
            hasIncorrectAnswer = true;
        }
    }

    // NB(xenosoz, 2018): Log for future analysis
    auto workPath = [this] {
        stringstream ss;
        ss << "/" << "SoundTrain";
        ss << "/" << "level-" << _currentLevel << "-0";
        ss << "/" << "work-" << _currentProblemIndex;
        return ss.str();
    }();

    StrictLogManager::shared()->game_Peek_Answer("SoundTrain", workPath,
                                                 userAnswer.str(), correctAnswer.str());


    if (hasIncorrectAnswer) FireworksEffect::miss();
    if (finished && !hasIncorrectAnswer) onCorrect();

}

void SoundTrainScene::removeObjects() {
    for (auto v : _cards) v->removeFromParent();
    _cards.clear();
    
    for (auto v : _freightTrains) v->removeFromParent();
    _freightTrains.clear();
    
    _locomotive->setVisible(false);
    _locomotive->removeAllChildren();
    
}

void SoundTrainScene::playSound(string letter){
    CCLOG("playSound");
    {
        string filepath = LanguageManager::getInstance()->findLocalizedResource("SoundTrain/Sounds/"+letter+".m4a");
        if (FileUtils::getInstance()->isFileExist(filepath)) { GameSoundManager::getInstance()->playEffectSound(filepath); return; }
        
        filepath = LanguageManager::getInstance()->findLocalizedResource("LetterVoice/"+letter+".m4a");
        if (FileUtils::getInstance()->isFileExist(filepath)) { GameSoundManager::getInstance()->playEffectSound(filepath); return; }

        filepath = LanguageManager::getInstance()->findLocalizedResource("WordVoice/"+letter+".m4a");
        if (FileUtils::getInstance()->isFileExist(filepath)) { GameSoundManager::getInstance()->playEffectSound(filepath); return; }

    }
    
//    
//    
//    if (isSW) {
//        string filePath1 = "SoundTrain/VoiceSW/"+letter+".m4a";
//        string filePath2 = "Common/Sounds/Imma.sw_TZ/LetterNames/"+letter+".wav";
//        string filePath3 = "Common/Sounds/Imma.sw_TZ/Words/"+letter+".wav";
//        
//        if (FileUtils::getInstance()->isFileExist(filePath1)) {
//            GameSoundManager::getInstance()->playEffectSound(filePath1); return;
//        } else if (FileUtils::getInstance()->isFileExist(filePath2)) {
//            GameSoundManager::getInstance()->playEffectSound(filePath2); return;
//        } else if (FileUtils::getInstance()->isFileExist(filePath3)) {
//            GameSoundManager::getInstance()->playEffectSound(filePath3); return;
//        }
//    } else if (isEN) {
//        string filePath = "SoundTrainVoice/"+letter+".wav";
//        GameSoundManager::getInstance()->playEffectSound(filePath); return;
//    }
}

void SoundTrainScene::cardCome() {
    
    float cardWidth = 210;
    float margin = 120;
    int cardCount = (int)_answers.size();
    
    
    float left = 105 + _gameSize.width/2 - cardWidth*cardCount/2 - margin*(cardCount-1)/2;

    for (int i=0; i<_answers.size(); i++) {
        auto card = SoundCard::create();
        card->setType(_answers[i]);
        auto pos = Vec2(left + i*margin + i*cardWidth, 215);
        card->_originalPos = pos;
        
        card->setPosition(pos.x, pos.y-500);
        
        card->onTouchBegan = [card, this]() {
            playSound(card->_letter);
            card->unload(this->_gameNode);
        };
        
        card->onTouchEnded = [card,this]() {
            SoundTrain *targetTrain = nullptr;
            for (auto train : _freightTrains) {
                auto slotPos = train->convertToWorldSpace(train->_slotPos);
                auto cardPos = card->getPosition();
                //CCLOG("slotpos %f, %f", slotPos.x, slotPos.y);
                //CCLOG("cardpos %f, %f", cardPos.x, cardPos.y);
                if (slotPos.distance(cardPos) <= 200) targetTrain = train;
            }
            
            if (targetTrain) {
                auto existingCard = targetTrain->_cardInTrain;
                if (existingCard) {
                    existingCard->unload(this->_gameNode);
                    
                    if (card->_targetTrain != nullptr) {
                        existingCard->loadOnTrain(card->_targetTrain);
                    } else {
                        existingCard->_targetTrain = nullptr;
                        existingCard->runAction(MoveTo::create(0.2,existingCard->_originalPos));
                    }
                }
                card->loadOnTrain(targetTrain);
                GameSoundManager::getInstance()->playEffectSound(soundSlotIn);
            } else {
                card->_targetTrain = nullptr;
                card->runAction(MoveTo::create(0.2, card->_originalPos));

            }
            checkReadyToSubmit();
        };
        
        card->_targetTrain = nullptr;
        _gameNode->addChild(card);
        _cards.push_back(card);
        
        card->runAction(Sequence::create(DelayTime::create(1.3+0.1*i), EaseOut::create(MoveTo::create(0.3, pos), 1.2), nullptr));
    }
    
}


const float locomotiveY = 500;
const float trainY = locomotiveY + 228;
const float locomotiveWidth = 677;
const float locomotiveMargin = 152;
const float trainWidth = 372;
const float trainLongWidth = 450;
const float blank = 0;
const float purgeMargin = 50; // 50
const float trainSpeed = 2500; // px/sec
float totalWidth = 0;

float SoundTrainScene::getLocomotiveStartX() {
    return _gameSize.width + locomotiveWidth/2;
}

float SoundTrainScene::getLocomotiveArriveX() {
    totalWidth = locomotiveWidth + blank*(_questions.size()-1) + _questions.size() * purgeMargin;
    for (auto question : _questions) {
        if (question.size() == 1) totalWidth += trainWidth;
        else totalWidth += trainLongWidth;
    }
    return _gameSize.width/2 - totalWidth/2 + locomotiveWidth/2;
}

float SoundTrainScene::getMoveTime(float px) {
    return px / trainSpeed;
}

float SoundTrainScene::getTrainStartX(int index) {
    return getLocomotiveStartX() + locomotiveMargin + getTrainSeq(index) + (blank*index) - 20;
}

float SoundTrainScene::getTrainArriveX(int index) {
    return getLocomotiveArriveX() + locomotiveMargin + getTrainSeq(index) + (blank*index) + (purgeMargin*(index+1));
}

float SoundTrainScene::getTrainSeq(int index){
    float trainSeq = trainWidth/2;
    for (int i=0; i<=index; i++) {
        if (_questions[i].size() == 1) trainSeq += trainWidth/(i==index?2:1);
        else trainSeq += trainLongWidth/(i==index?2:1);
    }

    return trainSeq;
}

void SoundTrainScene::lionMoveToggle() {
    LOGFN();
    if (_lionFront->isVisible()) {
        _lionFront->setVisible(false);
        _lionBack->setVisible(true);
    } else {
        _lionFront->setVisible(true);
        _lionBack->setVisible(false);
    }
}


void SoundTrainScene::trainCome() {
    
    _locomotive = Sprite::create("SoundTrain/train_compartment_front.png");
    _locomotive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _locomotive->setPosition(getLocomotiveStartX(), locomotiveY);
    _gameNode->addChild(_locomotive);
    
    auto locomotiveLabel = TodoUtil::createLabel(_trainName, 120 - _trainName.size() * 5, Size::ZERO, fontName, Color4B(255, 255, 255,255), TextHAlignment::CENTER);
    locomotiveLabel->setPosition(360, 270);
    locomotiveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _locomotive->addChild(locomotiveLabel);
    
    auto lionFront = Sprite::create("SoundTrain/train_lion_2.png");
    lionFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lionFront->setPosition(490,504);
    _locomotive->addChild(lionFront);
    _lionFront = lionFront;
    lionFront->setVisible(false);
    
    auto lionBack = Sprite::create("SoundTrain/train_lion.png");
    lionBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lionBack->setPosition(510,504);
    _locomotive->addChild(lionBack);
    _lionBack = lionBack;
    
    auto soundBubble = Sprite::create("SoundTrain/train_soundbubble.png");
    soundBubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundBubble->setPosition(630,870);
    _soundBubble = soundBubble;
    _locomotive->addChild(soundBubble);
    
    auto bubbleSize = soundBubble->getContentSize();

    auto soundButtonNormal = Sprite::create("SoundTrain/train_sound_normal.png");
    soundButtonNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButtonNormal->setPosition(Vec2(bubbleSize.width/2, bubbleSize.height/2+10));
    soundBubble->addChild(soundButtonNormal);

    auto soundButtonActive = Sprite::create("SoundTrain/train_sound_active.png");
    soundButtonActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundButtonActive->setPosition(Vec2(bubbleSize.width/2, bubbleSize.height/2+10));
    soundBubble->addChild(soundButtonActive);
    
    soundButtonActive->setOpacity(0);
    
    _soundButtonNormal = soundButtonNormal;
    _soundButtonActive = soundButtonActive;
    _soundButtonNormal->setVisible(true);
    _soundButtonActive->setVisible(true);
    soundButtonFadeOut();
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        auto P = _soundBubble->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (_soundBubble->getBoundingBox().containsPoint(pos)) {
            soundButtonSpeech();
            return true;
        }
        return false;
    };
    
    soundBubble->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    runAction(Sequence::create(
        DelayTime::create(0.5),
        CallFunc::create([this](){
            GameSoundManager::getInstance()->playEffectSound(soundCome);
            _locomotive->runAction(EaseOut::create(MoveTo::create(getMoveTime(getLocomotiveStartX()-getLocomotiveArriveX()), Vec2(getLocomotiveArriveX(),locomotiveY)), 2));
        }),
        DelayTime::create(1.7),
        CallFunc::create([this](){
            soundButtonSpeech();
        }),
        nullptr
    ));
    
    for (int i=0; i<_questions.size(); i++) {
        auto train = SoundTrain::create();
        train->setType(_questions[i]);
        train->setPosition(Vec2(getTrainStartX(i), trainY));
        _gameNode->addChild(train);
        _freightTrains.push_back(train);
        
        runAction(Sequence::create(
            DelayTime::create(0.5),
            CallFunc::create([this, i, train](){
                train->runAction(EaseOut::create(MoveTo::create(getMoveTime(getTrainStartX(i)-getTrainArriveX(i)), Vec2(getTrainArriveX(i),trainY)), 2));
            }),
            DelayTime::create(1.2),
            CallFunc::create([this, train](){
                soundButtonFadeIn();
                train->toggleConnector();
            }),
            DelayTime::create(0.5),
            CallFunc::create([this, train](){
                train->toggleConnector();
            }),
            nullptr
        ));
        
    }
    
    return;
    
}

void SoundTrainScene::soundButtonFadeIn() {
    _soundBubble->runAction(FadeIn::create(0.5));
    _soundButtonNormal->runAction(FadeIn::create(0.5));
    _soundButtonActive->runAction(FadeIn::create(0.5));
}

void SoundTrainScene::soundButtonFadeOut() {
    _soundBubble->runAction(FadeOut::create(0.5));
    _soundButtonNormal->runAction(FadeOut::create(0.5));
    _soundButtonActive->runAction(FadeOut::create(0.5));
}

void SoundTrainScene::soundButtonSpeech() {
    
    playSound(_trainName);
    
    runAction(Sequence::create(
        CallFunc::create([this](){
            _soundButtonActive->setVisible(true);
            _soundButtonNormal->setVisible(false);
        }),
        DelayTime::create(1.0),
        CallFunc::create([this](){
            _soundButtonActive->setVisible(false);
            _soundButtonNormal->setVisible(true);
        }),
        nullptr
    ));

}

void SoundTrainScene::onComplete() {
    auto CP = CompletePopup::create();
    CP->show(0, [this] {
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

void SoundTrainScene::onCorrect() {

    FireworksEffect::fire();

    float delayStart = 0.7;
    float delayJump = 0.4*_freightTrains.size() + 0.4;
    //float delayToggleConnector = 0; // 0.3
    float delayCombine = 0.2*(_freightTrains.size()) + 0.2;
    float delayGo = 2.0;
    
    for (auto card : _cards) {
        card->_enableTouch = false;
    }
    
    soundButtonFadeOut();
    
    runAction(Sequence::create(
        DelayTime::create(delayStart),
        //CallFunc::create([this](){ trainJump(); }),
        //DelayTime::create(delayJump),
        CallFunc::create([this](){ trainCombine(); }),
        DelayTime::create(delayCombine),
        CallFunc::create([this](){ playSound(_trainName); }),
        DelayTime::create(0.8),
        CallFunc::create([this](){ trainGo(); }),
        DelayTime::create(delayGo),
        CallFunc::create([this](){
            removeObjects();
            _currentProblemIndex++;
            setProblem();
        }),
        nullptr
    ));
    
}

void SoundTrainScene::trainGo(){
    lionMoveToggle();
    _soundButtonNormal->setVisible(false);
    _soundButtonActive->setVisible(false);
    /*
    runAction(Repeat::create(Sequence::create(
                                              CallFunc::create([this](){ soundButtonSpeech(); }),
                                              DelayTime::create(1.0),
                                              nullptr

                                              ), 3));
     */
    GameSoundManager::getInstance()->playEffectSound(soundMove);
    _locomotive->runAction(EaseIn::create(MoveBy::create(1.5, Vec2(_gameNode->getContentSize().width*-1,0)), 1.5));
}


void SoundTrainScene::trainCombine() {
    runAction(Sequence::create(
        CallFunc::create([this](){
            _locomotive->runAction(EaseOut::create(MoveBy::create(0.2, Vec2(purgeMargin, 0)), 2));
        }),
        CallFunc::create([this](){
            for (int i=0; i<_freightTrains.size(); i++) {
                
                runAction(Sequence::create(
                    DelayTime::create(0.21*(i+1)),
                    CallFunc::create([this, i](){
                        auto train = _freightTrains[i];
                        // train->getParent()->reorderChild(train, train->getLocalZOrder());
                    
                        train->retain();
                        train->removeFromParent();
                        _locomotive->addChild(train);
                        auto trainWorldSpace = _gameNode->convertToWorldSpace(train->getPosition());
                        auto trainNewNodeSpace = _locomotive->convertToNodeSpace(trainWorldSpace);
                        train->setPosition(trainNewNodeSpace);
                        train->release();
                        if (i < _freightTrains.size()-1) _locomotive->runAction(EaseOut::create(MoveBy::create(0.2, Vec2(purgeMargin, 0)),2.0));
                    }),
                    //DelayTime::create(0.6),
                    CallFunc::create([this](){
                        GameSoundManager::getInstance()->playEffectSound(soundCombine);
                    }),
                    nullptr
                ));
            }
        }),
        nullptr
    ));
    
}

void SoundTrainScene::toggleConnectors() {
    return;
    
    for (auto train : _freightTrains) {
        runAction(Sequence::create(
            CallFunc::create([this, train](){
                train->toggleConnector();
            }),
            nullptr
        ));
    }
}

void SoundTrainScene::trainJump() { // 0.2*_freightTrains.size()+0.5
    
    int SoundNumber = 1;
    //GameSoundManager::getInstance()->playEffectSound("SoundTrain/Sound/Pattern_Train_"+to_string(SoundNumber)+".m4a");
    //_locomotive->runAction(JumpBy::create(0.5, Point(0,0), 50, 1));
    
    for (int i=0; i<_freightTrains.size(); i++) {
        auto train = _freightTrains[i];
        SoundNumber++;
        if (SoundNumber==4) SoundNumber = 1;
        runAction(Sequence::create(
            DelayTime::create(0.4*i),
            CallFunc::create([this, train, SoundNumber](){
                train->runAction(JumpBy::create(0.5, Point(0,0), 50, 1));
                //playSound(train->_cardInTrain->_letter);
            }),
            nullptr
        ));
    }
}

END_NS_SOUNDTRAIN;
