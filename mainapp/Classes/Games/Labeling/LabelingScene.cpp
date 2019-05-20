//
//  LabelingScene.cpp
//
//  Created by Seungbum Son on 9/5/18.
//
//


#include "LabelingScene.hpp"
#include "Common/Basic/SoundEffect.h"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "Utils/TodoUtil.h"
#include "Utils/Random.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace LabelingSceneSpace {

    
    const char* defaultFont = "fonts/chanakya.ttf";
    
    const string resourcePathLabeling = "labeling/";
    
    const Size gameSize = Size(2560, 1800);
    const Size mainImageSize = Size(1933,1673);
    const Size labelSize = Size(592, 214);
    const Size themeSlotSize = Size(557, 186);
    const Size slotSize = Size(442, 146);
    const int slotHeadWidth = 60;
    const int themeSlotHeadWidth = 79;
    
    const Vec2 labelSpritePosition = Vec2(88, 0);
    
    const int spaceBetweenStartLabels = 195;
    const float idleOpacity = 0.6f;
    const float tearAnimationDelay = 0.7f;
    const float startAnimationDelay = 0.75f;
    const float snapAnimationDelay = 0.1f;
    const float slotAppearDelay = 0.4f;
    const float labelAppearDelay = 0.2f;
    
    const float snapBoundaryOffset = 1.3f;
    
    const int maxWordCount = 7;
    
    SoundEffect pickEffect() { return SoundEffect("Games/Labeling/Sounds/card_pick_ver1.m4a");  }
    SoundEffect snapEffect() { return SoundEffect("Common/Sounds/Effect/blockslotin.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }
    SoundEffect tearEffect() { return SoundEffect("Games/Labeling/Sounds/card_correct_ver2.m4a"); }
    SoundEffect incorrectEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_Incorrect.m4a"); }
    SoundEffect paperEffect() { return SoundEffect("Common/Sounds/Effect/card_move_right.m4a"); }
    SoundEffect slotApperEffect() { return SoundEffect("Common/Sounds/Effect/tokenappear.m4a"); }
    SoundEffect cardApperEffect() { return SoundEffect("Games/Labeling/Sounds/card_appear.m4a"); }
    SoundEffect paperTurnEffect() { return SoundEffect("Games/Labeling/Sounds/page_turn_ver2.m4a"); }
    SoundEffect highlightEffect() { return SoundEffect("Games/Labeling/Sounds/slot_highlight_ver2.m4a"); }
    
    //const string fontName = "fonts/chanakya.ttf";
    const string fontName = "fonts/andikanewbasic-b.ttf";
    const float fontSize = 90.0f;
    const float fontSizeSmall = 70.0f;
    const float fontSizeMedium = 85.0f;
    
    const string flashParticle = "Main/Common/Effects/Particle/shining_particle_star.plist";

    const vector<Color3B> colorPool = {Color3B(255,240,158),Color3B(170,190,255),Color3B(218,255,165),Color3B(255,190,255),Color3B(158,253,255),Color3B(255,194,122),Color3B(255,152,176)};
}

using namespace LabelingSceneSpace;

Color3B getRandomColor()
{
    int randnum = RandomHelper::random_int(0, (int)(colorPool.size()-1));
    
    return colorPool.at(randnum);
}

Scene* LabelingScene::createScene(string levelID)
{
    auto level = TodoUtil::stoi(levelID);
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LabelingScene::create();
    layer->setLevel(level);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LabelingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    pickEffect().preload();
    snapEffect().preload();
    solveEffect().preload();
    tearEffect().preload();
    incorrectEffect().preload();
    paperEffect().preload();
    slotApperEffect().preload();
    cardApperEffect().preload();
    highlightEffect().preload();
    
    auto winSize = getContentSize();
    
    auto bg = Sprite::create(resourcePathLabeling+"ui_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    _bgNode = Node::create();
    _bgNode->setContentSize(gameSize);
    _bgNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgNode->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(_bgNode);
    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(_gameNode);
    
    
    // Enuma default UIs
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                for (auto p : _labelingPieces) {
                    if (!p->_snapped) {
                        p->snapTarget();
                        break;
                    }
                }
            }
        });
        
    }

    loadDurationsheet();
    
    return true;
}

void LabelingScene::onEnter()
{
    Layer::onEnter();

}

void LabelingScene::onEnterTransitionDidFinish()
{

    onStart();

}

void LabelingScene::onStart()
{
    
    loadData(_currentLevel);
    _currentPuzzleIndex = 0;
    
    _progressBar->setMax((int)_labels.size());
    
    showPuzzle(_currentPuzzleIndex);
}


void LabelingScene::showPuzzle(int index)
{
    _progressBar->setCurrent(index+1, false);
    createPuzzle(index);
}

void LabelingScene::onPuzzleComplete()
{
    //pickEffect().play();
    solveEffect().play();
    
    auto e = _labels[_currentPuzzleIndex];
    
    
    _progressBar->setCurrent(_currentPuzzleIndex+1, true);
    _currentPuzzleIndex++;
    
    auto winSize = getContentSize();
    
    if (_currentPuzzleIndex>=_labels.size()) {
        
        auto CP = CompletePopup::create();
        CP->show(1.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    } else {
        auto seq = Sequence::create(DelayTime::create(1.0),
                                    CallFunc::create([this](){ showPuzzle(_currentPuzzleIndex); }),
                                    nullptr);
        this->runAction(seq);
        
    }
    
}

void LabelingScene::setLevel(int level)
{
    _currentLevel = level;
}



void LabelingScene::createPuzzle(int index)
{
    _gameNode->removeAllChildren();
    _labelingPieces.clear();
    _slotPieces.clear();
    _numSnappedPieces = 0;
    
    float currentDelay = 0.0f;
    
    auto info = _labels.at(index);
    auto folder = resourcePathLabeling+"levels/";

    Vec2 origin = Vec2(5, 1710);
    auto blankPage = Sprite::create(resourcePathLabeling + "image_empty.png");
    blankPage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //auto imagePos = _gameNode->convertToNodeSpace(origin);
    auto imagePos = Vec2(origin);
    blankPage->setPosition(imagePos);
    _gameNode->addChild(blankPage);
    blankPage->getParent()->reorderChild(blankPage, -1);
    
    origin = Vec2(151, 1566);
    auto mainImage = Sprite::create(folder + info.mainImageFilename);
    mainImage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //imagePos = _gameNode->convertToNodeSpace(origin);
    imagePos = Vec2(origin);
    mainImage->setPosition(imagePos);
    _gameNode->addChild(mainImage);
    
    string prevMainImagePath;
    if (index == 0)
    {
        // First page does not show page flip animation
    }
    else
    {
        prevMainImagePath = folder + _labels.at(index-1).mainImageFilename;
        
        origin = Vec2(5, 1710);
        auto blankImage = Sprite::create(resourcePathLabeling + "image_empty.png");
        blankImage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        imagePos = Vec2(origin);
        blankImage->setPosition(imagePos);
        _gameNode->addChild(blankImage);
        
        origin = Vec2(151, 1566);
        auto prevMainImage = Sprite::create(prevMainImagePath);
        prevMainImage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        imagePos = Vec2(origin);
        prevMainImage->setPosition(imagePos);
        _gameNode->addChild(prevMainImage);
        
        // page turn animation
        paperTurnEffect().play();
        
        auto seq = Sequence::create(DelayTime::create(0.05f),
                                        EaseOut::create(MoveBy::create(0.3f, Vec2(150, -30)), 2.0),
                                        DelayTime::create(0.45f),
                                        EaseIn::create(MoveBy::create(0.04f, Vec2(40, 0)), 2.0),
                                        DelayTime::create(0.04f),
                                        EaseElasticIn::create(MoveBy::create(0.2f, Vec2(-4000, 0)), 2.0),
                                        nullptr);
        blankImage->runAction(seq->clone());
        prevMainImage->runAction(seq->clone());
        currentDelay += startAnimationDelay + 0.3f;
    }
    
    origin = Vec2(-96, 1557);
    auto springImage = Sprite::create(resourcePathLabeling + "ui_note_spring.png");
    springImage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    imagePos = Vec2(origin);
    springImage->setPosition(imagePos);
    _gameNode->addChild(springImage);
    
    _pieceNode = Node::create();
    _pieceNode->setContentSize(gameSize);
    _gameNode->addChild(_pieceNode);
    _pieceNode->getParent()->reorderChild(_pieceNode, 0);
    
    auto winSize = getContentSize();
    
    vector<Vec2> adjustedPositions;
    
    for (int i = (int)info.wordTextVector.size()-1; i >= 0; i--)
    {
        string text = info.wordTextVector.at(i);
        
        Point answerPos = info.answerPosVector.at(i);
        
        Size slotSizeOffset = slotSize;
        if (i == 0)
            slotSizeOffset = themeSlotSize;
        
        Vec2 adjustedPos = Vec2(answerPos.x+slotSizeOffset.width/2, answerPos.y-slotSizeOffset.height/2);
        adjustedPositions.push_back(adjustedPos);
        
        auto *slotNode = SlotPiece::create();
        slotNode->setContentSize(slotSizeOffset);
        slotNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slotNode->setPosition(Vec2(adjustedPos.x, adjustedPos.y));
        slotNode->_slotSize = slotSizeOffset;
        if (i == 0)
            slotNode->_isTheme = true;
        slotNode->setText(text);
        _pieceNode->addChild(slotNode);
        _slotPieces.push_back(slotNode);
        slotNode->setScale(0.0f);
        
        float popDelay;
        
        if (i == 0)
            popDelay = 0.8f;
        else
            popDelay = 0.5f;
        
        slotNode->runAction(Sequence::create(DelayTime::create(currentDelay),
                                             EaseElasticInOut::create(ScaleTo::create(popDelay, 1.0f)),
                                             DelayTime::create(0.01f),
                                             CallFunc::create([this](){ slotApperEffect().play();}),
                                             nullptr));
        if (i == 1)
            currentDelay += slotAppearDelay + 0.2f;
        else
            currentDelay += slotAppearDelay;
    }
    
    currentDelay += 0.6f;
    
    vector<Vec2> startPositions;
    startPositions.clear();
    for (int i = 0; i < info.wordTextVector.size(); i++)
    {
        Vec2 startPosition = Vec2(1956, 1520 - (i*spaceBetweenStartLabels) - labelSize.height);
        startPositions.push_back(startPosition);
    }
    
    for (int i = 0; i < info.wordTextVector.size(); i++)
    {
        string text = info.wordTextVector.at(i);
        
        auto label = LabelingPiece::create();
        label->_labelId = i;
        label->_voiceFilename = info.voiceFilenameVector.at(i);
        
        label->setText(text);
        
        _slotPieces.at(_slotPieces.size()-i-1)->setTagColor(label->_labelColor);
        label->_targetSize = _slotPieces.at(_slotPieces.size()-i-1)->_slotSize;
        
        auto labelSize = label->getContentSize();
        
        label->_targetPos = adjustedPositions.at(adjustedPositions.size()-1-i);
        
        label->_touchEnabled = true;
        
        label->onTouchStart = [this, label]() {
            this->blockTouches(label->_labelId);
        };
        
        label->onSnapped = [this, label]() {
            this->sendLog(label->_labelId, label->_labelId);
            _numSnappedPieces++;
            
            for (int i = 0; i < _slotPieces.size(); i++)
                _slotPieces.at(i)->showHighlight(false);
            
            float additionalDelay = 0.4f;
            
            this->runAction(Sequence::create(DelayTime::create(snapAnimationDelay),
                                             CallFunc::create([this,label](){ this->unblockTouches(); _slotPieces.at(_slotPieces.size()-label->_labelId-1)->revealText();}),
                                             DelayTime::create(tearAnimationDelay+additionalDelay),
                                             CallFunc::create([this,label](){ playSound(label->_voiceFilename); }),
                                             nullptr));
            
            
            if (_numSnappedPieces >= _labelingPieces.size()) {
                float voiceDelay = getDuration(label->_voiceFilename);
                
                this->runAction(Sequence::create(DelayTime::create(tearAnimationDelay+additionalDelay+voiceDelay),
                                                 CallFunc::create([this](){ onPuzzleComplete(); }),
                                                 nullptr));
            }
        };
        
        label->onReturn = [this, label] () {
            if (this->isWrongAnswer(label))
            {
                //incorrectEffect().play();
            }
            
            this->unblockTouches();
            
            for (int i = 0; i < _slotPieces.size(); i++)
                _slotPieces.at(i)->showHighlight(false);
        };
        
        label->onMoved = [this, label]() {
            int index = this->isWithinBoundary(label);
            if (index >= 0)
            {
                for (int i = 0; i < _slotPieces.size(); i++)
                {
                    if (!_labelingPieces.at(index)->_snapped && i == _slotPieces.size()-_labelingPieces.at(index)->_labelId-1)
                    {
                        _slotPieces.at(i)->showHighlight(true);
                    }
                    else
                        _slotPieces.at(i)->showHighlight(false);
                }
            }
            else
            {
                for (int i = 0; i < _slotPieces.size(); i++)
                    _slotPieces.at(i)->showHighlight(false);
            }
        };
        
        _labelingPieces.push_back(label);
    }
    
    std::random_shuffle(_labelingPieces.begin(), _labelingPieces.end());
    
    for (int i = 0; i < _labelingPieces.size(); i++)
    {
        Vec2 startPosition = startPositions.at(i);
        int moveOffset = 2000;
        
        auto label = _labelingPieces.at(i);
        label->setPosition(Vec2(startPosition.x + moveOffset, startPosition.y));
        label->_startPos = startPosition;
        _pieceNode->addChild(label);
        
        label->runAction(Sequence::create(DelayTime::create(currentDelay),
                                             EaseIn::create(MoveTo::create(labelAppearDelay, startPosition),2.0f),
                                          DelayTime::create(0.004f),
                                          CallFunc::create([this](){ cardApperEffect().play();}),
                                             nullptr));
        currentDelay += labelAppearDelay;
    }
}

void LabelingScene::blockTouches(int targetId)
{
    for (int i = 0; i < _labelingPieces.size(); i++)
    {
        auto label = _labelingPieces.at(i);
        
        if (label->_labelId == targetId)
            continue;
        
        label->_touchEnabled = false;
    }
}

void LabelingScene::unblockTouches()
{
    for (int i = 0; i < _labelingPieces.size(); i++)
    {
        auto label = _labelingPieces.at(i);
        
        if (label->_snapped)
            continue;
        
        label->_touchEnabled = true;
    }
}

void LabelingScene::sendLog(int userPick, int correctPick)
{
    string workPath = [this] {
        stringstream SS;
        SS << "/" << "Labeling";
        SS << "/" << "level-" << _currentLevel << "-0";
        SS << "/" << "work-" << _currentPuzzleIndex;
        return SS.str();
    }();
    StrictLogManager::shared()->game_Peek_Answer("Labeling", workPath, TodoUtil::itos(userPick), TodoUtil::itos(correctPick));
}

Vec2 tempLabelPostion;

bool LabelingScene::isWrongAnswer(LabelingPiece* targetPiece)
{
    Size snapRectSize = labelSize * snapBoundaryOffset;
    Rect snapRect;
    LabelingPiece* labelPiece;
    
    for(int i = 0; i < _labelingPieces.size(); i++)
    {
        labelPiece = _labelingPieces.at(i);
        if (labelPiece->_labelId == targetPiece->_labelId)
            continue;
        
        snapRect = Rect(labelPiece->_targetPos.x - snapRectSize.width/2, labelPiece->_targetPos.y - snapRectSize.height/2, snapRectSize.width, snapRectSize.height);
        
        if (snapRect.containsPoint(tempLabelPostion))
        {
            sendLog(targetPiece->_labelId, labelPiece->_labelId);
            
            return true;
        }
    }
    
    return false;
}

int LabelingScene::isWithinBoundary(LabelingPiece* targetPiece)
{
    Size snapRectSize = labelSize * snapBoundaryOffset;
    Rect snapRect;
    LabelingPiece* labelPiece;
    
    for(int i = 0; i < _labelingPieces.size(); i++)
    {
        labelPiece = _labelingPieces.at(i);
        
        snapRect = Rect(labelPiece->_targetPos.x - snapRectSize.width/2, labelPiece->_targetPos.y - snapRectSize.height/2, snapRectSize.width, snapRectSize.height);
        
        if (snapRect.containsPoint(tempLabelPostion))
        {
            return i;
        }
    }
    
    return -1;
}

void LabelingScene::loadData(int level)
{
    string P = "Games/" + resourcePathLabeling + "labeling_levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    auto data = TodoUtil::readTSV(S);
   
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

    string LanguageTag;
    size_t levelID;
    size_t puzzleID;
    
    _labels.clear();
    
    LabelingLevelStruct e;
    
    int rowIndex = 0;
    while (rowIndex < data.size()) {
        auto row = data[rowIndex++];
        LanguageTag = row[0];
        
        if (LanguageTag != Lang.c_str())
            continue;
        
        levelID = TodoUtil::stoi(row[1]);
        
        if (levelID != level)
            continue;
        
        puzzleID = TodoUtil::stoi(row[2]);
        e.mainImageFilename = row[3];
        
        e.wordTextVector.clear();
        e.answerPosVector.clear();
        e.voiceFilenameVector.clear();
        
        int currentIndex = 4;
        
        for (int i=0; i<maxWordCount; i++) {
            string text = row[currentIndex++];
            
            if (text.empty())
                break;
            
            e.wordTextVector.push_back(text);
            
            string posText = row[currentIndex++];
            posText.erase(std::remove(posText.begin(),posText.end(),'"'),posText.end());
            //auto posVec = TodoUtil::split(posText, ',');
            auto posVec = TodoUtil::split(posText, '.');
            int x = TodoUtil::stoi(posVec[0]);
            int y = abs(TodoUtil::stoi(posVec[1]));
            e.answerPosVector.push_back(Point(x,y));
            
            string voiceFilename = row[currentIndex++];
            e.voiceFilenameVector.push_back(voiceFilename);
        }
        
        if (levelID==level) {
            _labels.push_back(e);
        }
        
    }
}


void LabelingScene::playSound(string name)
{
    string path = "Games/Labeling/Sound/"+name;
    
    //GameSoundManager::getInstance()->playEffectSound(path);
    GameSoundManager::getInstance()->playEffectSoundVoiceOnly(path);
}

void LabelingScene::loadDurationsheet() {

    if (_duration.size() != 0) return;
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("Labeling/Sound/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);

    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        _duration[row[0]] = rowDuration;
    }
}

float LabelingScene::getDuration(string name) {

    if (_duration.count(name))
        return _duration[name];

    return 2.5;
}


////////////////////



bool LabelingPiece::init()
{

    if (!Node::init()) return false;

    _snapped = false;
    _scaled = false;
    onSnapped = nullptr;
    onReturn = nullptr;
    _touchEnabled = false;
    
    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_snapped) return false;
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos))
        {
            this->setPicked(true);
            pickEffect().play();
            
            this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->setPosition(pos);
            snapEffect().play();
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
            
            if (onTouchStart)
                onTouchStart();
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        if (_snapped) return;
        if (!_touchEnabled) return;
        
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);

        if (cl.x < 1940 && !_scaled)
        {
            _scaled = true;
            this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->runAction(EaseElasticIn::create(ScaleTo::create(0.2f, slotSize.width/labelSize.width), 2.0));
        }
        
        if (onMoved)
        {
            tempLabelPostion = getPosition();
            onMoved();
        }
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        Size snapRectSize = labelSize * snapBoundaryOffset;
        Rect snapRect = Rect(_targetPos.x - snapRectSize.width/2, _targetPos.y - snapRectSize.height/2, snapRectSize.width, snapRectSize.height);
        
        if (snapRect.containsPoint(getPosition()))
        {
            this->_touchEnabled = false;
            // Right answer
            this->snapTarget();
        }
        else
        {
            this->_touchEnabled = false;
            tempLabelPostion = getPosition();
            
            this->returnTarget();
        }
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void LabelingPiece::setText(string text)
{
    auto winSize = getContentSize();
    
    setContentSize(labelSize);
    
    _labelColor = getRandomColor();
    
    _tagSprite = Sprite::create(resourcePathLabeling+"ui_card_head.png");
    _tagSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _tagSprite->setPosition(Vec2::ZERO);
    _tagSprite->setColor(_labelColor);
    addChild(_tagSprite);
    
    _labelSprite = Sprite::create(resourcePathLabeling+"ui_card_body.png");
    _labelSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _labelSprite->setPosition(labelSpritePosition);
    _labelSprite->setColor(_labelColor);
    addChild(_labelSprite);
    
    _textLabel = TodoUtil::createLabel(text, fontSize, Size(labelSize.width-100,labelSize.height-40), fontName, Color4B(70, 70, 70, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
    _textLabel->setOverflow(Label::Overflow::SHRINK);
    _textLabel->enableWrap(true);
    _textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textLabel->setPosition(labelSize.width/2,labelSize.height/2);
    addChild(_textLabel);
    
    _flashTagSprite = Sprite::create(resourcePathLabeling+"ui_card_head.png");
    _flashTagSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _flashTagSprite->setPosition(Vec2::ZERO);
    _flashTagSprite->setOpacity(0);
    addChild(_flashTagSprite);
    _flashTagSprite->getParent()->reorderChild(_flashTagSprite, 0);
    
    _flashBodySprite = Sprite::create(resourcePathLabeling+"ui_card_body.png");
    _flashBodySprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _flashBodySprite->setPosition(labelSpritePosition);
    _flashBodySprite->setOpacity(0);
    addChild(_flashBodySprite);
    _flashBodySprite->getParent()->reorderChild(_flashBodySprite, 0);
    
}

void LabelingPiece::setPicked(bool picked)
{
    Size size = getContentSize();
    
    if (picked) {
        
        _labelSprite->setPosition(labelSpritePosition.x, labelSpritePosition.y + 5);
        _tagSprite->setPosition(0, 5);
        _textLabel->setPosition(labelSize.width/2,labelSize.height/2+5);
        
    } else {
        _labelSprite->setPosition(labelSpritePosition);
        _tagSprite->setPosition(Vec2::ZERO);
        _textLabel->setPosition(labelSize.width/2,labelSize.height/2);
    }
}

void LabelingPiece::showFlash()
{
    float flashDelay = 0.3f;
    _flashTagSprite->setOpacity(255);
    _flashBodySprite->setOpacity(255);
    
    _flashTagSprite->runAction(EaseIn::create(FadeTo::create(flashDelay,0), 2.0f));
    _flashBodySprite->runAction(EaseIn::create(FadeTo::create(flashDelay,0), 2.0f));
   
    /*
    //auto particleSystem = ParticleSystemQuad::create(flashParticle);
    auto particleSystem = ParticleExplosion::create();
    particleSystem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    particleSystem->setPosition(labelSize.width/2, labelSize.height/2);
    particleSystem->setDuration(0.2f);
    this->addChild(particleSystem);
    particleSystem->setAutoRemoveOnFinish(true);
     */
}

void LabelingPiece::scaleTo(float scaleValue)
{
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->runAction(ScaleTo::create(0.0f, scaleValue));
}

void LabelingPiece::snapTarget()
{
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->runAction(Sequence::create(ScaleTo::create(snapAnimationDelay, _targetSize.width/labelSize.width),
                                     MoveTo::create(snapAnimationDelay, Vec2(_targetPos.x, _targetPos.y)),
                                     CallFunc::create([this](){float fadeDelayOffset = 0.03f;
                                     auto fadeAction = FadeTo::create(snapAnimationDelay-fadeDelayOffset, 0);
                                     _textLabel->runAction(fadeAction->clone());
                                     _tagSprite->runAction(fadeAction->clone());
                                     _labelSprite->runAction(fadeAction->clone());}),
                                     nullptr));
    setPicked(false);
    _snapped = true;
    _touchEnabled = false;
    
    if (onSnapped) {
        onSnapped();
    }

}

void LabelingPiece::returnTarget()
{
    float returnDelay = 0.2f;
    this->scaleTo(1.0f);
    _scaled = false;
    
    
    auto seq = Sequence::create(EaseElasticIn::create(MoveTo::create(returnDelay, Vec2(_startPos.x+labelSize.width/2, _startPos.y+labelSize.height/2)), 2.0),
                                DelayTime::create(returnDelay - 0.04f),
                                CallFunc::create([this](){ snapEffect().play(); this->showFlash(); this->_touchEnabled = true; this->scaleTo(1.0f);}),
                                nullptr);
    
    runAction(seq);
    
    if (onReturn) {
        onReturn();
    }
}

bool SlotPiece::init()
{
    if (!Node::init()) return false;
    
    _isTheme = false;
    _isHighlighted = false;
    
    return true;
}

void SlotPiece::setText(string text)
{
    string largeTag = "";
    int headWidth = slotHeadWidth;
    if (_isTheme)
    {
        largeTag = "_l";
        headWidth = themeSlotHeadWidth;
    }
    
    _headSprite = Sprite::create(resourcePathLabeling+"ui_cardslot_head"+largeTag+".png");
    _headSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _headSprite->setPosition(Vec2::ZERO);
    _headSprite->setOpacity(255);
    addChild(_headSprite);
    
    _headColorSprite = Sprite::create(resourcePathLabeling+"ui_cardslot_head"+largeTag+".png");
    _headColorSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _headColorSprite->setPosition(Vec2::ZERO);
    _headColorSprite->setOpacity(0);
    addChild(_headColorSprite);
    
    _bodySprite = Sprite::create(resourcePathLabeling+"ui_cardslot_body"+largeTag+".png");
    _bodySprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _bodySprite->setPosition(0 + headWidth, 0);
    _bodySprite->setOpacity(255 * idleOpacity);
    addChild(_bodySprite);
    
    _flashSlotSprite = Sprite::create(resourcePathLabeling+"ui_cardslot_body"+largeTag+".png");
    _flashSlotSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _flashSlotSprite->setPosition(headWidth, 0);
    _flashSlotSprite->setOpacity(0);
    addChild(_flashSlotSprite);
    
    _highlightSprite = Sprite::create(resourcePathLabeling+"ui_cardslot_highlight"+largeTag+".png");
    _highlightSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _highlightSprite->setPosition(_slotSize.width/2, _slotSize.height/2);
    _highlightSprite->setOpacity(0);
    addChild(_highlightSprite);
    
    float fontSizeAdjusted = fontSizeSmall;
    if (_isTheme)
        fontSizeAdjusted = fontSizeMedium;
    
    _textSlotLabel = TodoUtil::createLabel(text, fontSizeAdjusted, Size(_slotSize.width-headWidth-60,_slotSize.height-40), fontName, Color4B(70, 70, 70, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
    _textSlotLabel->setOverflow(Label::Overflow::SHRINK);
    _textSlotLabel->enableWrap(true);
    _textSlotLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //_textSlotLabel->setPosition(labelSize.width/2,labelSize.height/2);
    _textSlotLabel->setPosition(headWidth + (_bodySprite->getContentSize().width/2)-5, _bodySprite->getContentSize().height/2);
    _textSlotLabel->setOpacity(0);
    addChild(_textSlotLabel);
    
    _animationSprite = Sprite::create(resourcePathLabeling+"card_body_pick0.png");
    _animationSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    int animPositionOffset = -21;
    if (_isTheme)
        animPositionOffset = -28;
    _animationSprite->setPosition(headWidth, animPositionOffset);
    if (!_isTheme)
        _animationSprite->setScale(slotSize.width/themeSlotSize.width);
    _animationSprite->setOpacity(0);
    addChild(_animationSprite);
}

void SlotPiece::setTagColor(Color3B color)
{
    _headColorSprite->setColor(color);
    _animationSprite->setColor(color);
}

void SlotPiece::showHighlight(bool _show)
{
    if (_show && !_isHighlighted)
    {
        _highlightSprite->setOpacity(255);
        _isHighlighted = true;
        GameSoundManager::getInstance()->setEffectSoundVolume(0.5f);
        highlightEffect().play();
        runAction(Sequence::create(DelayTime::create(0.2f),
                                   CallFunc::create([this](){GameSoundManager::getInstance()->setEffectSoundVolume(1.0f);}),
                                   nullptr));
        
    }
    else if (!_show && _isHighlighted)
    {
        _highlightSprite->setOpacity(0);
        _isHighlighted = false;
    }
}

void SlotPiece::revealText()
{
    float flashDelay = 0.5f;
    float animationDelay = 0.4f;
    
    _textSlotLabel->setOpacity(255);
    _headColorSprite->setOpacity(255);
    _animationSprite->setOpacity(255);
    
    // Tear animation
    Vector<SpriteFrame*>animFrames(14);
    
    for (int i = 0; i <= 13; i++)
    {
        SpriteFrame *frame = SpriteFrame::create(resourcePathLabeling + "card_body_pick" + TodoUtil::itos(i) + ".png", Rect(0,0,1009,591));
        animFrames.pushBack(frame);
    }
    
    float frameDelay = 0.05f;
    auto tearAnimation = Animation::createWithSpriteFrames(animFrames, frameDelay);
    Animate *tearAnimate = Animate::create(tearAnimation);
    _animationSprite->runAction(tearAnimate);
    _animationSprite->runAction(Sequence::create(DelayTime::create(frameDelay*7),
                                                 EaseElasticIn::create(MoveBy::create(animationDelay, Vec2(4000, 0)), 2.0),
                                                 nullptr));
    //paperEffect().play();
    //tearEffect().play();
    
    runAction(Sequence::create(DelayTime::create(0.2f),
                               CallFunc::create([this](){tearEffect().play();}),
                               nullptr));
    
    auto seq = Sequence::create(DelayTime::create(tearAnimationDelay),
                                CallFunc::create([this](){this->_flashSlotSprite->setOpacity(255);
                                /*auto particleSystem = ParticleSystemQuad::create(flashParticle);
                                particleSystem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                particleSystem->setPosition(labelSize.width/2, labelSize.height/2);
                                particleSystem->setDuration(0.2f);
                                this->addChild(particleSystem);
                                particleSystem->setAutoRemoveOnFinish(true);*/}),
                                EaseIn::create(FadeTo::create(flashDelay,0), 2.0f),
                                nullptr);
    _flashSlotSprite->runAction(seq);
}

