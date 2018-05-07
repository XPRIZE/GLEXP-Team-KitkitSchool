//
//  FillTheBlanksScene.cpp
//  KitkitSchool
//
//  Created by timewalker on 13/12/2016.
//
//

#include "FillTheBlanksScene.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"
#include "Managers/LanguageManager.hpp"

using namespace std;

namespace FillTheBlanksSpace
{
    const bool bDebug = false;
    
    const Size gameSize = Size(2560, 1800);
    const Size maxPageSize = Size(1300, 1000);
//    const float pageBottom = 800.f;
    
    const Size trayInnerSize = Size(1800, 300);
    const Size trayOuterSize = Size(2100, 400);
    const Size kTrayArrangeSize = Size(1400.f, 400.f);
    const Rect trayInnerRect = Rect((gameSize.width - trayInnerSize.width) / 2, 0, trayInnerSize.width, trayInnerSize.height);
    const Rect trayOuterRect = Rect((gameSize.width - trayOuterSize.width) / 2, 0, trayOuterSize.width, trayOuterSize.height);
    const Rect kTrayArrangeRect = Rect((gameSize.width-kTrayArrangeSize.width) / 2, 0.f, kTrayArrangeSize.width, kTrayArrangeSize.height);
    
    const string kPrefixPath = "ComprehensionTest/FillTheBlanks/";
    //const string kPrefixCommonImagePath = "ComprehensionTest/Image/";
    const string fontName = "fonts/TodoSchoolV2.ttf";
    
    const char* kPickEffectSound = "Common/Sounds/Effect/SFX_Wood_SlideOut.m4a";
    const char* kSnapEffectSound = "Common/Sounds/Effect/SFX_Wood_Correct.m4a";
    
    const float fontSize = 60.f;
    const float lineSpacing = 80.f;
    const float wordSpacing = 60.f;
    const float labelPadding = 100.f;

    const float kPagePositionCorrectionX = 300.f;
    const float kQuestionImageCorrectionX = -400.f;
    const float kQuestionImageCorrectionY = 50.f;
    
    int randomAreasIndex = 0;
}

using namespace FillTheBlanksSpace;

namespace ComprehensionTest
{
    namespace FillTheBlanks
    {
        ProblemData* _problemData;
        
        FillTheBlanksScene::FillTheBlanksScene()
        {
            
        }
        
        bool FillTheBlanksScene::init()
        {
            if (!Layer::init()) return false;
            return true;
        }
        
        
        Scene* FillTheBlanksScene::createScene()
        {
            // 'scene' is an autorelease object
            auto scene = Scene::create();
            Size visibleSize = Director::getInstance()->getVisibleSize();
            scene->setContentSize(visibleSize);
            
            // 'layer' is an autorelease object
            auto layer = FillTheBlanksScene::create();
            
            // add layer as a child to scene
            scene->addChild(layer);
            
            // return the scene
            return scene;
        }
        
        Layer* FillTheBlanksScene::createLayer(ComprehensionScene *parent)
        {
            auto layer = FillTheBlanksScene::create();
            //layer->_parent = parent;
            layer->_comprehensionScene = parent;
            return layer;
        }
        
        void FillTheBlanksScene::onEnter()
        {
            Layer::onEnter();
            
            initData();
            randomAreasIndex = 0;
            createRandomAreas();
            
            auto winSize = getContentSize();
            
            drawFixedResources(); // 고정 리소스
            
            // DesignResolution에서 FixedHeight로 세팅해뒀기 때문에 height는 언제나 1800입니다만, width는 바뀔 수 있기 때문에
            // 화면 해상도 변화에 상관없이 고정 해상도를 사용하기 위해 _gameNode를 만들고 그 안에서만 작업합니다.
            _gameNode = Node::create();
            _gameNode->setContentSize(gameSize);
            _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _gameNode->setPosition(Vec2(winSize.width/2, 0));
            addChild(_gameNode);
            
            
            // 먼저 가장 긴 단어의 크기를 찾습니다.
            _maxWordSize = calculateMaxWordSize();
            
            // 단어 블록의 길이를 제일 긴것 (+ 패딩)을 기준으로 설정합니다.
            WordBlock::setWordBlockWidth(_maxWordSize.width+labelPadding);
            
            // 페이지를 만듭니다.
            auto page = createPage(maxPageSize.width);
            _pageScale = 1.f;
            
            // height가 제한을 넘어가는 경우, 넘어가는 비율에 맞춰서 width를 늘려 다시 그리고, 그만큼 scale해줍니다.
            if (page->getContentSize().height > maxPageSize.height) {
                _pageScale = maxPageSize.height / page->getContentSize().height;
                page = createPage(maxPageSize.width/_pageScale);
                page->setScale(_pageScale);
                
            }
            page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            page->setPosition(Vec2(gameSize.width/2 + kPagePositionCorrectionX, gameSize.height/2));
            _gameNode->addChild(page);
            
            if (bDebug)
            {
                LayerColor* lc;
                
                lc = LayerColor::create(Color4B::RED);
                lc->setOpacity(128);
                lc->setContentSize(page->getContentSize());
                lc->setIgnoreAnchorPointForPosition(false);
                lc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lc->setScale(_pageScale);
                lc->setPosition(Vec2(gameSize.width/2 + kPagePositionCorrectionX, gameSize.height/2));
                _gameNode->addChild(lc);

                lc = LayerColor::create(Color4B(0, 0, 255, 100));
                lc->setContentSize(kTrayArrangeSize);
                lc->setPosition(kTrayArrangeRect.origin.x, kTrayArrangeRect.origin.y);
                _gameNode->addChild(lc);
            }
            
            // 단어카드들을 만듭니다.
            for (auto w : _problemData->wordSet) {
                if (w->bBlank) _gameNode->addChild(createWordBlock(w->value));
            }
            for (auto w : _problemData->wrongWordSet) {
                _gameNode->addChild(createWordBlock(w));
            }
            
//            _comprehensionScene->drawQuestionTitle(_problemData->questionText, _gameNode);
            string directionContent = LanguageManager::getInstance()->isEnglish() ? "Fill in the blank(s)." : "Jaza nafasi.";
            _comprehensionScene->drawQuestionTitle(directionContent, _gameNode, 50.f);
            if (!_problemData->soundPath.empty()) {
                string audioPath = _comprehensionScene->getBookFolder()+"/quiz/"+_problemData->soundPath;
                bool audioExist = FileUtils::getInstance()->isFileExist(audioPath);
                if (!audioExist) {
                    audioPath = _comprehensionScene->getBookFolder()+"/page/"+_problemData->soundPath;
                    audioExist = FileUtils::getInstance()->isFileExist(audioPath);
                }
                if (audioExist)
                {
                    auto soundButton = _comprehensionScene->drawSoundButton(_gameNode);
                    soundButton->addTouchEventListener([this, audioPath](Ref*,Widget::TouchEventType e) {
                        if (e == Widget::TouchEventType::ENDED) {
                            GameSoundManager::getInstance()->playEffectSoundVoiceOnly(audioPath);
                        }
                    });
                }
            }
            
            
            {
                float padding = 16.0f;
                
                auto questionImageShadow = Sprite::create(kPrefixPath + "comprehension_illustration_shadow.png");
                questionImageShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                questionImageShadow->setPosition(_gameNode->getContentSize().width / 2 + kQuestionImageCorrectionX, _gameNode->getContentSize().height / 2 + kQuestionImageCorrectionY);
                questionImageShadow->setScale(0.7f);
                _gameNode->addChild(questionImageShadow);
                
                auto questionImage = Sprite::create(_comprehensionScene->getBookFolder()+"/quiz/" + _problemData->questionImage);
                if (!questionImage) questionImage =Sprite::create(_comprehensionScene->getBookFolder()+"/page/" + _problemData->questionImage);
                
                if (questionImage == nullptr) { NativeAlert::show("Image does not exist.", _problemData->questionImage, "OK"); return; }
                auto calcWidth = (questionImageShadow->getContentSize().width - padding * 2) / questionImage->getContentSize().width;
                auto calcHeight = (questionImageShadow->getContentSize().height - padding * 2) / questionImage->getContentSize().height;
                float scaleFactor = MIN(calcWidth, calcHeight);
                questionImage->setScale(scaleFactor);
                questionImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                questionImage->setPosition(questionImageShadow->getContentSize().width / 2, questionImageShadow->getContentSize().height - padding);
                questionImageShadow->addChild(questionImage);
                
                auto questionImageFrame = Sprite::create(kPrefixPath + "comprehension_illustration_frame.png");
                questionImageFrame->setPosition(questionImageShadow->getContentSize() / 2);
                questionImageShadow->addChild(questionImageFrame);
            }
        }
        
        void FillTheBlanksScene::onExit() {
            Layer::onExit();
        }
        
        Size FillTheBlanksScene::calculateMaxWordSize()
        {
            Size maxSize = Size::ZERO;
            
            for (auto word : _problemData->wordSet)
            {
                if (word->bBlank == false) continue;
                auto tempLabel = TodoUtil::createLabel(word->value, fontSize, Size::ZERO, fontName, Color4B::BLACK);
                maxSize.width = MAX(maxSize.width, tempLabel->getContentSize().width);
                maxSize.height = MAX(maxSize.height, tempLabel->getContentSize().height);
                
            }
            
            for (auto word : _problemData->wrongWordSet)
            {
                auto tempLabel = TodoUtil::createLabel(word, fontSize, Size::ZERO, fontName, Color4B::BLACK);
                maxSize.width = MAX(maxSize.width, tempLabel->getContentSize().width);
                maxSize.height = MAX(maxSize.height, tempLabel->getContentSize().height);
            }
            
            return maxSize;
        }
        
        Node* FillTheBlanksScene::createPage(float pageWidth)
        {
            
            // 페이지 node를 만들고, pageWidth 맞춰서 단어들을 출력합니다.
            // openGL 좌표계는 bottomleft가 원점이기 때문에, innerPage노드를 하나 만들어서 여기에서 위로 올라가면서 단어들을 찍어주고
            // 다 찍은 다음에 innerPage의 위치를 조정하여 page 안에 잘 표시되도록 합니다.
            
            Node *page = Node::create();
            Node *innerPage = Node::create();
    
            
            Point pos = Point::ZERO;
            Size pageSize = Size::ZERO;
            
            // 맨 윗줄에 빈칸이 있는 경우 글자 label보다 위로 올라오게 되기 때문에 이것을 보정합니다.
            float pageTopMargin = 0;
            
            _slots.clear();

            for (auto word : _problemData->wordSet) {
                if (word->bBlank) {
                    auto slot = WordBlock::create();
                    slot->initBlank(word->value);
                    _slots.push_back(slot);
                    
                    auto slotSize = slot->getContentSize();
                    auto slotOffset = Size(0, (slotSize.height-_maxWordSize.height)/2);
                    
                    
                    if (pos.x + slotSize.width > pageWidth) {
                        pos.x = 0;
                        pos.y -= _maxWordSize.height + lineSpacing;
                    }
                
              
                    slot->setPosition(pos + slotOffset);
                    
                    pageSize.width = MAX(pageSize.width, pos.x+slotSize.width);
                    pageSize.height = MIN(pageSize.height, pos.y-slotSize.height+slotOffset.height);
                    
                    pageTopMargin = MAX(pageTopMargin, pos.y+slotOffset.height);
                    
                    pos.x += wordSpacing + slotSize.width;
                    
                    innerPage->addChild(slot);
                } else {
                    auto label = TodoUtil::createLabel(word->value, fontSize, Size::ZERO, fontName, Color4B::BLACK);
                    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                    auto labelSize = label->getContentSize();
                
                    if (word->value==".") {
                        pos.x -= wordSpacing;
                    } else {
                        if (pos.x + labelSize.width > pageWidth) {
                            pos.x = 0;
                            pos.y -= _maxWordSize.height + lineSpacing;
                        }
                    }
                
                    label->setPosition(pos);
                    
                    pageSize.width = MAX(pageSize.width, pos.x+labelSize.width);
                    pageSize.height = MIN(pageSize.height, pos.y-labelSize.height);
                    
                    pos.x += wordSpacing + labelSize.width;
                
                    innerPage->addChild(label);
                }
            }
            
            // 다 찍었으니 page사이즈 및 innerpage의 위치를 설정합니다.
        
            pageSize.height *= -1.0;
            pageSize.height += pageTopMargin;
            
            page->setContentSize(pageSize);
            innerPage->setPosition(Vec2(0, pageSize.height-pageTopMargin));

            page->addChild(innerPage);
            
            if (bDebug) {
                auto l = LayerColor::create(Color4B(128, 128, 128, 128), pageSize.width, pageSize.height);
                page->addChild(l);
            }
            
            return page;
        }
        
        
        Node* FillTheBlanksScene::createWordBlock(string word)
        {
            auto block = WordBlock::create();
            block->initWord(word);
            
            // page scale에 맞춰서 단어 블록들도 scale해줍니다. 
            block->setScale(_pageScale);
            auto blockSize = block->getContentSize()*_pageScale;
            
            //auto pos = Point(trayInnerRect.origin.x+random(0.f, trayInnerSize.width-blockSize.width), random(blockSize.height * 2, trayInnerSize.height));
            auto pos = _randomPoints[randomAreasIndex++];
            
            block->setPosition(pos);
            
            block->onCheckTargetBegan = [](){
                GameSoundManager::getInstance()->playEffectSound(kPickEffectSound);
            };
            
            block->onCheckTargetEnd = [this, block, blockSize](){
                
                // 들어갈 수 있는 빈칸이 있나 찾아봅니다.
                WordBlock *newSlot = nullptr;
                float minDist = -1;
                
                for (auto slot : _slots) {
                    
                    if (slot->_pair==nullptr) {
                        // 슬롯과 블록의 부모노드가 다르기 때문에 원점을 world좌표계로 변환한 후 비교합니다.
                        auto sp = slot->convertToWorldSpace(Vec2::ZERO);
                        auto bp = block->convertToWorldSpace(Vec2::ZERO);
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
                if (newSlot && newSlot->_pair == nullptr && newSlot->_word == block->_word) {
                    
                    
                    // TODO: 위의 if 조건절 변화로 이 부분 동작하고 있지 않음, 처리해야 함
                    if (block->_pair) { // 원래 자리가 있었으면 비웁니다.
                        block->_pair->_pair = nullptr;
                    }
                    
                    newSlot->_pair = block;
                    block->_pair = newSlot;
                    
                    // position은 부모좌표계 기준이므로, slot의 위치를 block의 부모좌표계로 변환해서 세팅합니다.
                    auto newBP = _gameNode->convertToNodeSpace(newSlot->getParent()->convertToWorldSpace(newSlot->getPosition()));
                    block->setPosition(newBP);
                    block->setSnapped(true);
                    
                    // block 과 slot이 모두 WordBlock이므로, 정답이 들어갔는지를 확인하려면 _pair간의 _word가 같은지를 비교하면 됩니다.
                    // 같은 단어가 여러군데에 나올 수도 있을 것 같아서 이렇게 처리했습니다.
                    
                } else{
                    auto blockCenter = block->getPosition() + Vec2(blockSize.width/2, -blockSize.height/2);
                    if (trayOuterRect.containsPoint(blockCenter)) {  // tray로 돌아갔으면 거기 놔둡니다.
                        if (block->_pair) {
                            block->_pair->_pair = nullptr;
                            block->_pair = nullptr;
                        }
                        
                        block->setSnapped(false);
                    
                    
                    } else { // 아니면 원래 자리로 돌려보냅니다.
                        block->returnToOrigin();
                    }
                }
                
                writePlayLog();
                if (isSolved())
                    onSolve();
            };
            
            return block;
        }
        
        
        
#pragma - custom methods
        
        void FillTheBlanksScene::initData()
        {
            std::vector<std::string> rawData;
            
            for (int i = 0; i < _comprehensionScene->problemSet.size(); i++)
            {
                if (i == _comprehensionScene->getCurrentProblem())
                {
                    rawData = _comprehensionScene->problemSet[i].second;
                    break;
                }
            }

            _problemData = FillTheBlanks::ProblemData::parse(rawData);
        }

        
//        void FillTheBlanksScene::initData()
//        {
//            
//            
//            std::string dataPath = "ComprehensionTest/FillTheBlanks/ComprehensionTest - Fill the blanks.tsv";
//            std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
//            problemData = FillTheBlanks::ProblemData::parse(data);
//        }
        
        void FillTheBlanksScene::drawFixedResources()
        {
            auto tray = Sprite::create(kPrefixPath + "comprehention_filltheblank_tray.png");
            tray->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            tray->setPosition(getContentSize().width / 2, -300.f);
            addChild(tray);
            
        }
        
        bool FillTheBlanksScene::isSolved()
        {
            for (auto slot : _slots) {
                if (slot->_pair == nullptr)
                    return false;
            }
            return true;
        }
        
        void FillTheBlanksScene::onSolve()
        {
            GameSoundManager::getInstance()->stopBGM();
            GameSoundManager::getInstance()->stopAllEffects();
            
            if (_comprehensionScene)
                _comprehensionScene->onSolve();
        }
        
        void FillTheBlanksScene::createRandomAreas()
        {
            _randomPoints.clear();
            
            float xSize = kTrayArrangeSize.width / 4;
            float ySize = kTrayArrangeSize.height / 3;
            
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _randomPoints.push_back(Vec2(kTrayArrangeRect.origin.x + (xSize * i) + random(-30.f, 30.f), kTrayArrangeRect.origin.y + 150.f + (ySize * j) + random(-10.f, 10.f)));
                }
            }
            random_shuffle(_randomPoints.begin(), _randomPoints.end());
        }
        
        void FillTheBlanksScene::writePlayLog()
        {
            stringstream userAnswer;
            stringstream correctAnswer;
            
            userAnswer << "[";
            correctAnswer << "[";
            
            auto paren = [](const string& s) {
                return string("{") + s + string("}");
            };
            
            for (WordBlock* slot : _slots) {
                userAnswer << (slot->_pair ? paren(slot->_pair->_word) : "None") << ", ";
                correctAnswer << paren(slot->_word) << ", ";
            }
            
            userAnswer << "]";
            correctAnswer << "]";
            
            StrictLogManager::shared()->game_Peek_Answer("ComprehensionTest", makeWorkPath(), userAnswer.str(), correctAnswer.str());
        }
        
        string FillTheBlanksScene::makeWorkPath()
        {
            stringstream ss;
            ss << "ComprehensionTest";
            ss << "/" << _comprehensionScene->getBookName();
            ss << "/" << "filltheblanks";
            ss << "-" << _comprehensionScene->getCurrentProblem();
            return ss.str();
        }
    }
}
