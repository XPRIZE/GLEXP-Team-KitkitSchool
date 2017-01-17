//
//  SentenceMakerScene.cpp
//  enumaXprize
//
//  Created by timewalker on 30/12/2016.
//
//

#include "SentenceMakerScene.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Utils/TodoUtil.h"
#include "CCAppController.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "3rdParty/CCNativeAlert.h"

namespace SentenceMakerSpace
{
    const bool bDebug = false;
    
    Size winSize;

    const string kPrefixPath = "SentenceMaker/";
    const string kImagePrefixPath = kPrefixPath + "images/";
    const string kSoundPrefixPath = kPrefixPath + "sounds/";
    const char* kDataFile = "SentenceMaker/SentenceMaker - Sheet1.tsv";
    
    const char* kSolveEffectSound = "Counting/UI_Star_Collected.m4a";
    const char* kPickEffectSound = "Common/Sounds/Effect/SFX_Wood_SlideOut.m4a";
    const char* kSnapEffectSound = "Common/Sounds/Effect/SFX_Wood_Correct.m4a";
    
    const Size kGameSize = Size(2560, 1800);
    const Size kImageSize = Size(1000, 640);
    
    const float kPageCorrectionY = -200.f;
    const float kSpaceBetweenEachSlotX = 20.f;
    const float kSpaceBetweenBottomItemY = 10.f;
    const float kBottomAreaLayerCorrectionY = -820.f;
    const float kSketchbookCorrectionY = 0.f;
    const float kQuestionImageCorrectionY = 100.f;
    const float kSketchbokPageCorrectionWidth = 280.f;
//    const float kSentenceDelayTime = 0.3f; // 맞춰지는 효과음 때문에 딜레이 시간을 늘림
    const float kSentenceDelayTime = 0.6f;
}

using namespace SentenceMakerSpace;

SentenceMakerScene::SentenceMakerScene():
_currentProblem(0)
{
    
}

Scene* SentenceMakerScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SentenceMakerScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SentenceMakerScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

void SentenceMakerScene::onEnter()
{
    Layer::onEnter();
    CCLOG("[SentenceMakerScene::onEnter]");
    
    initData();
    
    winSize = getContentSize();
    
    auto bg = Sprite::create(kPrefixPath + "sentencemaker_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    _gameNode = Node::create();
    _gameNode->setContentSize(kGameSize);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(getContentSize() / 2);
    addChild(_gameNode);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    auto blockerView = Node::create();
    blockerView->setContentSize(winSize);
    _blocker = EventListenerTouchOneByOne::create();
    _blocker->setSwallowTouches(true);
    _blocker->onTouchBegan = [](Touch* T, Event* E) {
        return true;
    };
    blockerView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_blocker, blockerView);
    addChild(blockerView);
    _blocker->setEnabled(false);

    onStart();
}

void SentenceMakerScene::onStart()
{
    _currentProblem = 1;
    _progressBar->setMax(_progressBarSize);
    
    parseSoundDurationJson();
    createGame(_currentProblem);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                onSolve();
            }
        });
    }
}

void SentenceMakerScene::parseSoundDurationJson()
{
    Json::Reader reader;
    string jsonFileName;
    string jsonFile;
    bool bSuccess = false;
    
    jsonFileName = kPrefixPath + "word_sound_duraion_" + LanguageManager::getInstance()->getCurrentLanguageCode() + ".json";
    jsonFile = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    bSuccess = reader.parse(jsonFile, _wordSoundDurationJson, false);
    if (!bSuccess)
    {
        NativeAlert::show("Error", "Cannot be Parsed : " + jsonFileName, "OK");
        return;
    }
    
    jsonFileName = kPrefixPath + "sentence_sound_duraion.json";
    jsonFile = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    bSuccess = reader.parse(jsonFile, _sentenceSoundDurationJson, false);
    if (!bSuccess)
    {
        NativeAlert::show("Error", "Cannot be Parsed : " + jsonFileName, "OK");
        return;
    }
}

void SentenceMakerScene::createGame(int problemId)
{
    _blocker->setEnabled(false);
                         
    _gameNode->removeAllChildren();
    
    _slots.clear();
    _bottomItemVector.clear();
    
    _progressBar->setCurrent(problemId, false);
    
    auto key = ProblemSetKey(LanguageManager::getInstance()->getCurrentLanguageTag(), _currentLevel, problemId);
    _problemData = _problemDataMap[key];
    
    // Sketchbook
    _sketchbookPage = Sprite::create(kPrefixPath + "sentencemaker_sketchbook.png");
    _sketchbookPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sketchbookPage->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height + kSketchbookCorrectionY);
    _gameNode->addChild(_sketchbookPage);
    
//    {
//        auto tl = LayerColor::create(Color4B(255, 0, 0, 100));
//        tl->setContentSize(_sketchbookPage->getContentSize() - Size(280.f, 0));
//        tl->ignoreAnchorPointForPosition(false);
//        tl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//        tl->setPosition(_sketchbookPage->getPosition());
//        _gameNode->addChild(tl);
//    }
    
    // Binder
    auto binder = Sprite::create(kPrefixPath + "sentencemaker_binding.png");
    binder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    binder->setPosition(_sketchbookPage->getContentSize() / 2);
    _sketchbookPage->addChild(binder);
    
    // Tray
    auto tray = Sprite::create(kPrefixPath + "sentencemaker_wordtray.png");
    tray->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    tray->setPosition(_gameNode->getContentSize().width / 2, 400.f);
    _gameNode->addChild(tray);
    
    // Sound Button
    if (FileUtils::getInstance()->isFileExist(kSoundPrefixPath + _problemData->soundPath))
    {
        _sketchbookPage->addChild(createSoundButton());
    }
    
    // Back Button
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    drawPage();
    drawQuestionImage();
    drawBottomItems();
}

bool SentenceMakerScene::isSolved()
{
    for (auto slot : _slots) {
        if (slot->_pair == nullptr)
            return false;
    }
    return true;
}

void SentenceMakerScene::onSolve()
{
    _blocker->setEnabled(true);
    
    auto delayTime = getSentenceDuration(_problemData->soundPath);
    auto seq = Sequence::create(DelayTime::create(kSentenceDelayTime),
                                CallFunc::create([this](){ GameSoundManager::getInstance()->playEffectSound(kSoundPrefixPath + _problemData->soundPath); }),
                                DelayTime::create(kSentenceDelayTime + delayTime),
                                CallFunc::create([this](){ onSolvePostProcess(); }),
                                nullptr);
    this->runAction(seq);
}

void SentenceMakerScene::onSolvePostProcess()
{
    GameSoundManager::getInstance()->playEffectSound(kSolveEffectSound);
    
    _progressBar->setCurrent(_currentProblem, true);
    
    if (_currentProblem >= _progressBarSize)
    {
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
    else
    {
        _currentProblem++;
        auto seq = Sequence::create(DelayTime::create(1.0), CallFunc::create([this](){
            createGame(_currentProblem);
        }), nullptr);
        this->runAction(seq);
    }
}

void SentenceMakerScene::setLevel(int level)
{
    _currentLevel = level;
}

void SentenceMakerScene::initData()
{
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile(kDataFile);
    auto data = TodoUtil::readTSV(rawString);
    
    _progressBarSize = 0;
    _problemDataMap.clear();
    _bottomItemVector.clear();
    _slots.clear();
    
    for (auto row : data)
    {
        if (row.size() < 1)
            continue;
        
        if (TodoUtil::trim(row[0]).size() <= 0)
            continue;
        
        if (row[0][0] == '#')
            continue;

        _problemDataMap.insert({ProblemSetKey(row[0], TodoUtil::stoi(row[1]), TodoUtil::stoi(row[2])), ProblemData::parse(row)});
        
        if (row[0] == LanguageManager::getInstance()->getCurrentLanguageTag() && TodoUtil::stoi(row[1]) == _currentLevel)
            _progressBarSize++;
    }
}

void SentenceMakerScene::drawPage()
{
    auto page = Node::create();
    
    Point pos = Point::ZERO;
    Size pageSize = Size::ZERO;
    
    for (auto each : _problemData->wordSet)
    {
        if (each->bBlank)
        {
            WordItem* slot = WordItem::create();
            slot->initBlank(each->value);
            slot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            slot->setPosition(pos);
            page->addChild(slot);
            pos.x += slot->getContentSize().width + kSpaceBetweenEachSlotX;
            pageSize.width += slot->getContentSize().width + kSpaceBetweenEachSlotX;
            pageSize.height = slot->getContentSize().height;
            _slots.push_back(slot);
        }
    }
    
    auto sf = (_sketchbookPage->getContentSize().width - kSketchbokPageCorrectionWidth) / pageSize.width;
    _blockScaleFactor = (sf < 1.f ? sf : 1.f);
    
    pageSize.height *= -1;
    page->setContentSize(pageSize);
    page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    page->setScale(_blockScaleFactor);
    page->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height / 2 + kPageCorrectionY);
    _gameNode->addChild(page);
}

void SentenceMakerScene::drawQuestionImage()
{
    auto questionImage = Sprite::create(kImagePrefixPath + _problemData->imagePath);
    if (questionImage == nullptr) { NativeAlert::show("Image does not exist.", kImagePrefixPath + _problemData->imagePath, "OK"); return; }
    
    float scaleFactor = MIN(kImageSize.width / questionImage->getContentSize().width, kImageSize.height / questionImage->getContentSize().height);
    questionImage->setScale(scaleFactor);
    questionImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionImage->setPosition(_sketchbookPage->getContentSize().width / 2, _sketchbookPage->getContentSize().height / 2 + kQuestionImageCorrectionY);
    _sketchbookPage->addChild(questionImage);
}

void SentenceMakerScene::drawBottomItems()
{
    const float _kGapBetweenBottomItems = 20.f;
    _bottomItemVector.clear();
    for (auto each : _problemData->wordSet)
    {
        if (each->bBlank)
            _bottomItemVector.push_back(each->value);
    }
    for (auto each : _problemData->wrongWordSet)
    {
        _bottomItemVector.push_back(each);
    }
    random_shuffle(_bottomItemVector.begin(), _bottomItemVector.end());
    
    _bottomAreaLayer = LayerColor::create(Color4B(100, 100, 100, bDebug ? 255 : 0));
    _bottomAreaLayer->setContentSize(Size(1600, 400));
    _bottomAreaLayer->ignoreAnchorPointForPosition(false);
    _bottomAreaLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bottomAreaLayer->setPosition(getContentSize().width / 2 + 50.f, getContentSize().height / 2 + kBottomAreaLayerCorrectionY);
    _gameNode->addChild(_bottomAreaLayer);
    
    //TODO: 잘 이해가 되지 않는 부분
    Point pos = _gameNode->convertToNodeSpace(_gameNode->convertToWorldSpace(_bottomAreaLayer->getPosition()));
//    Point pos = _gameNode->convertToWorldSpace(_bottomAreaLayer->getPosition());
    
    float originPosX = pos.x - _bottomAreaLayer->getContentSize().width / 2;
    pos = Vec2(originPosX, pos.y + _bottomAreaLayer->getContentSize().height / 2);
    
    if (bDebug)
    {
        auto tLayer = LayerColor::create(Color4B::BLUE);
        tLayer->setContentSize(_bottomAreaLayer->getContentSize());
        tLayer->ignoreAnchorPointForPosition(false);
        tLayer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        tLayer->setPosition(pos);
        _gameNode->addChild(tLayer);
    }
    
    for (auto each : _bottomItemVector)
    {
        auto item = createWordItem(each);
        item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        item->setScale(_blockScaleFactor);
        item->setPosition(pos);
        _gameNode->addChild(item);
        
        if (pos.x + (item->getContentSize().width + _kGapBetweenBottomItems) * _blockScaleFactor > _bottomAreaLayer->getContentSize().width)
        {
            pos.x = originPosX;
            pos.y -= item->getContentSize().height * _blockScaleFactor + kSpaceBetweenBottomItemY;
        }
        else
        {
            pos.x += (item->getContentSize().width + _kGapBetweenBottomItems) * _blockScaleFactor;
        }
    }
}

void SentenceMakerScene::playWordSound(string word)
{
    double delayTime = 0;
    auto wordSoundNameVector = getRefinedWordSoundName(word);
    for (auto wordSoundName : wordSoundNameVector)
    {
        auto effectName = LanguageManager::getInstance()->soundPathForWordFile(wordSoundName);
        auto seq = Sequence::create(DelayTime::create(delayTime),
                                    CallFunc::create([this, effectName, wordSoundName](){ GameSoundManager::getInstance()->playEffectSound(effectName); }),
                                    nullptr);
        
        this->runAction(seq);
        delayTime += getWordDuration(wordSoundName);
    }
}

Node* SentenceMakerScene::createWordItem(string word)
{
    WordItem* item = WordItem::create();
    item->initWord(word);
    
    // (s) touch callback
    item->onCheckTargetBegan = [this, word](){
        GameSoundManager::getInstance()->playEffectSound(kPickEffectSound);
        // playWordSound(word);
    };
    
    item->onCheckTargetEnded = [this, item](){
        // 들어갈 수 있는 빈칸이 있나 찾아봅니다.
        WordItem *newSlot = nullptr;
        float minDist = -1;
        
        for (auto slot : _slots) {
            if (slot->_pair==nullptr) {
                // 슬롯과 블록의 부모노드가 다르기 때문에 원점을 world좌표계로 변환한 후 비교합니다.
                auto sp = slot->convertToWorldSpace(Vec2::ZERO);
                auto bp = item->convertToWorldSpace(Vec2::ZERO);
                auto dist = sp.distance(bp);
                if (dist < 100) { // 들어갈 수 있는 슬롯 중에서 가장 가까운 것을 고릅니다.
                    if (minDist<0 || minDist>dist) {
                        minDist = dist;
                        newSlot = slot;
                    }
                }
            }
        }
        
        // 들어갈 수 있으면 끼워넣습니다. (슬롯이 비어있고, newSlot의 word 값과 block의 word 값이 같을 경우)
        if (newSlot && newSlot->_pair == nullptr && newSlot->_word == item->_word) {
            // TODO: 위의 if 조건절 변화로 이 부분 동작하고 있지 않음, 처리해야 함
            if (item->_pair) { // 원래 자리가 있었으면 비웁니다.
                item->_pair->_pair = nullptr;
            }
            
            newSlot->_pair = item;
            item->_pair = newSlot;
            
            // position은 부모좌표계 기준이므로, slot의 위치를 block의 부모좌표계로 변환해서 세팅합니다.
            auto newBP = _gameNode->convertToNodeSpace(newSlot->getParent()->convertToWorldSpace(newSlot->getPosition()));
            item->setPosition(newBP);
            item->setSnapped(true);
            GameSoundManager::getInstance()->playEffectSound(kSnapEffectSound);
            
        } else {
            item->returnToOrigin();
        }
        
        if (isSolved())
            onSolve();
    };
    // (e) touch callback
    
    return item;
}

Node* SentenceMakerScene::createSoundButton()
{
    string normalImage = "ComprehensionTest/Common/comprehension_speaker_normal.png";
    string activeImage = "ComprehensionTest/Common/comprehension_speaker_active.png";
    
    auto soundButton = Button::create(normalImage, activeImage);
    soundButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    soundButton->setPosition(Vec2(130.f, _sketchbookPage->getContentSize().height - 180.f));
    soundButton->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
        if (e == Widget::TouchEventType::ENDED) {
            GameSoundManager::getInstance()->playEffectSound(kSoundPrefixPath + _problemData->soundPath);
        }
    });
    return soundButton;
}

#pragma mark - ProblemData
SentenceMakerScene::ProblemData* SentenceMakerScene::ProblemData::parse(std::vector<std::string> rawData)
{
    ProblemData* pm = new ProblemData();
    
    // wordSet
    for (std::string eachValue : TodoUtil::split(rawData[3], '/'))
    {
        if (TodoUtil::trim(eachValue).size() == 0)
            continue;
        
        eachValue = "[" + eachValue + "]";
        
        if (eachValue.find("]") != std::string::npos)
        {
            // ex) This is the [apple]. <== this value
            if (eachValue.find("].") != std::string::npos)
            {
                Word* w;
                std::vector<std::string> eachValueVector = TodoUtil::split(eachValue, ']');
                TodoUtil::replace(eachValue, "].", "");
                w = new Word();
                TodoUtil::replace(eachValueVector[0], "[", "");
                w->value = eachValueVector[0];
                w->bBlank = true;
                pm->wordSet.push_back(w);
                
                w = new Word();
                w->value = eachValueVector[1];
                w->bBlank = false;
                pm->wordSet.push_back(w);
            }
            else
            {
                Word* w = new Word();
                TodoUtil::replace(eachValue, "[", "");
                TodoUtil::replace(eachValue, "]", "");
                
                w->value = eachValue;
                w->bBlank = true;
                pm->wordSet.push_back(w);
            }
        }
        else
        {
            Word* w = new Word();
            w->value = eachValue;
            w->bBlank = false;
            pm->wordSet.push_back(w);
        }
    }
    
    // wrongWordSet
    pm->wrongWordSet = TodoUtil::splitCSV(rawData[4]);
    for (int i = 0; i < pm->wrongWordSet.size(); i++)
    {
        pm->wrongWordSet[i] = TodoUtil::trim(pm->wrongWordSet[i]);
    }
    
    pm->imagePath = TodoUtil::trim(rawData[5]);
    pm->soundPath = TodoUtil::trim(rawData[6]);
    
    return pm;
}

string SentenceMakerScene::getLowerString(string value)
{
    for (int i = 0; i < value.length(); i++)
    {
        value[i] = tolower(value[i]);
    }
    return value;
}

vector<string> SentenceMakerScene::getRefinedWordSoundName(string value)
{
    vector<string> soundVector;
    soundVector.empty();
    TodoUtil::replaceAll(value, ".", "");
    
    for (auto eachValue : TodoUtil::split(value, ' '))
    {
        if (TodoUtil::trim(eachValue).size() <= 0)
            continue;
        
        soundVector.push_back(getLowerString(eachValue) + ".wav");
    }
    
    return soundVector;
}

double SentenceMakerScene::getWordDuration(string soundName)
{
    auto durationString = _wordSoundDurationJson[soundName].asString();
    return TodoUtil::stod(durationString);
}

double SentenceMakerScene::getSentenceDuration(string soundName)
{
    auto durationString = _sentenceSoundDurationJson[soundName].asString();
    return TodoUtil::stod(durationString);
}
