//
//  LRDividedTypeReorderingScene.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 14/08/2018.
//

#include "LRDividedTypeReorderingScene.hpp"
#include "Managers/StrictLogManager.h"

namespace
{
    const bool bDebug = false;
    
    const string kLetterArray[7] = {"1", "2", "3", "4", "5", "6", "7"};
    
    const Size kGameSize = Size(2560.f, 1800.f);
    
    //        const Size kTrayInnerSize = Size(1800.f, 600.f);
    //        const Size kTrayOuterSize = Size(2188.f, 752.f);
    const Size kTrayInnerSize = Size(1800.f, 450.f);
    const Size kTrayArrangeSize = Size(2000.f, 500.f);
    const Size kTrayOuterSize = Size(2188.f, 612.f);
    const Rect kTrayInnerRect = Rect((kGameSize.width-kTrayInnerSize.width) / 2, 0.f, kTrayInnerSize.width, kTrayInnerSize.height);
    const Rect kTrayArrangeRect = Rect((kGameSize.width-kTrayArrangeSize.width) / 2, 0.f, kTrayArrangeSize.width, kTrayArrangeSize.height);
    const Rect kTrayOuterRect = Rect((kGameSize.width-kTrayOuterSize.width) / 2, 0.f, kTrayOuterSize.width, kTrayOuterSize.height);
    
    const float kTextSlotSpaceY = 20.f;
    
    const float kImageMinDistance = 300.f;
    const float kTextMinDistance = 100.f;
    
    const string kLetterFont = FONT_MUKTA_BOLD;
    const float kLetterCorrectionX = -80.f;
    const Color4B kLetterColor = Color4B(56, 56, 56, 255);
    
    const float kDownArrowCorrectionX = -90.f;
    const float kDownArrowCorrectionY = -77.f;
    
    const float kRightArrowCorrectionX = -90.f;
    const float kRightrrowCorrectionY = -77.f;
    
    const string kPrefixPath = "lrcomprehension/common/";
    const string kDownArrowImagePath = "comprehensive_arrow_down.png";
    const string kRightArrowImagePath = "xprize_comprehention_quiz.png";
}

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

bool LRDividedTypeReorderingScene::init()
{
    if (!Layer::init()) return false;
    return true;
}

Layer* LRDividedTypeReorderingScene::createLayer(GameType gameType, LRComprehensionScene *parent)
{
    auto layer = LRDividedTypeReorderingScene::create();
    layer->_currentType = gameType;
    layer->_comprehensionScene = parent;
    return layer;
}

void LRDividedTypeReorderingScene::onEnter()
{
    Layer::onEnter();
    
    initData();
    //determineItemType();
    createFixedResources();
    
    _gameNode = Node::create();
    _gameNode->setContentSize(kGameSize);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gameNode->setPosition(Vec2(getContentSize().width / 2, 0.f));
    addChild(_gameNode);
    
    string directionContent = LanguageManager::getInstance()->isEnglish() ? "ƒæÅÙæ¥ô¢ ·UUUUô âãè RUUUU× ×ð¢ Ú¹ð¢Ð" : "Yaweke matukio katika mpangilio ulio sahihi."; // Place the events in the correct sequence. // घटनाओं को सही क्रम में रखें।
    _comprehensionScene->drawQuestionTitleWithSoundButton(directionContent, _problem->audio, _gameNode);
    
    drawBlocksByGameType();
    
    if (bDebug)
    {
        LayerColor* lc1;
        
        lc1 = LayerColor::create(Color4B(255, 0, 0, 100));
        lc1->setContentSize(kTrayOuterSize);
        //                lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        //                lc1->ignoreAnchorPointForPosition(false);
        lc1->setPosition(kTrayOuterRect.origin.x, kTrayOuterRect.origin.y);
        _gameNode->addChild(lc1);
        
        lc1 = LayerColor::create(Color4B(0, 255, 0, 100));
        lc1->setContentSize(kTrayInnerSize);
        //                lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        //                lc1->ignoreAnchorPointForPosition(false);
        lc1->setPosition(kTrayInnerRect.origin.x, kTrayInnerRect.origin.y);
        _gameNode->addChild(lc1);
        
        lc1 = LayerColor::create(Color4B(0, 0, 255, 100));
        lc1->setContentSize(kTrayArrangeSize);
        //                lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        //                lc1->ignoreAnchorPointForPosition(false);
        lc1->setPosition(kTrayArrangeRect.origin.x, kTrayArrangeRect.origin.y);
        _gameNode->addChild(lc1);
    }
}

void LRDividedTypeReorderingScene::createImageSlots()
{
    Size imageSize = ImageBlock::wordBlockSize;
    
    auto wrapperNode = Node::create();
    wrapperNode->setContentSize(Size((imageSize.width + 100.f) * _itemVector.size(), imageSize.height));
    wrapperNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    wrapperNode->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height / 2 + 150.f);
    
    for (int i = 0; i < _itemVector.size(); i++)
    {
        auto slot = ImageBlock::create();
        slot->initAsSlot(_itemVector[i]);
        slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slot->setPosition(slot->getContentSize().width / 2 + (slot->getContentSize().width + 100.f) * i, slot->getContentSize().height / 2);
        wrapperNode->addChild(slot);
        _imageSlots.push_back(slot);
        
        if (i > 0)
        {
            auto rightArrow = Sprite::create(kPrefixPath + kRightArrowImagePath);
            rightArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            rightArrow->setPosition((slot->getContentSize().width + 100.f) * i - 45.f, slot->getContentSize().height / 2);
            wrapperNode->addChild(rightArrow);
        }
    }
    
    _gameNode->addChild(wrapperNode);
}

void LRDividedTypeReorderingScene::createTextSlots(float wrapperNodeCorrectionY, float scaleFactor)
{
    Size textSize = TextBlock::wordBlockSize;
    
    auto wrapperNode = Node::create();
    wrapperNode->setContentSize(Size(textSize.width, (textSize.height + kTextSlotSpaceY) * _itemVector.size()));
    wrapperNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    wrapperNode->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height + wrapperNodeCorrectionY);
    wrapperNode->setScale(scaleFactor);
    
    for (int i = 0; i < _itemVector.size(); i++)
    {
        auto slot = TextBlock::create();
        slot->initAsSlot(_itemVector[i]);
        slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slot->setPosition(slot->getContentSize().width / 2, wrapperNode->getContentSize().height - (slot->getContentSize().height + kTextSlotSpaceY) * i);
        
        auto letterLabel = TodoUtil::createLabel(kLetterArray[i], 70, Size::ZERO,  kLetterFont, kLetterColor);
        letterLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        letterLabel->setPosition(kLetterCorrectionX, slot->getContentSize().height / 2);
        slot->addChild(letterLabel);
        
        wrapperNode->addChild(slot);
        
        if (i < _itemVector.size() - 1)
        {
            auto downArrow = Sprite::create(kPrefixPath + kDownArrowImagePath);
            downArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            downArrow->setPosition(kDownArrowCorrectionX, slot->getContentSize().height / 2 + kDownArrowCorrectionY);
            slot->addChild(downArrow);
        }
        
        _textSlots.push_back(slot);
    }
    
    if (bDebug)
    {
        auto lc1 = LayerColor::create(Color4B(0, 0, 255, 100));
        lc1->setContentSize(Size(textSize.width * _itemVector.size(), textSize.height));
        lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lc1->setIgnoreAnchorPointForPosition(false);
        lc1->setPosition(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height / 2 + 150.f);
        _gameNode->addChild(lc1);
    }
    
    _gameNode->addChild(wrapperNode);
}

Node* LRDividedTypeReorderingScene::createImageBlock(string imageFile)
{
    auto block = ImageBlock::create();
    block->initAsBlock(imageFile);
    block->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto blockSize = block->getContentSize();
    auto pos = Point(kTrayInnerRect.origin.x+random(0.f, kTrayInnerSize.width), random(0.f, kTrayInnerSize.height));
    block->setPosition(pos);
    
    block->onCheckTarget = [this, block, blockSize](){
        
        // 들어갈 수 있는 빈칸이 있나 찾아봅니다.
        ImageBlock *newSlot = nullptr;
        float minDist = -1;
        
        for (auto slot : _imageSlots)
        {
            // 슬롯과 블록의 부모노드가 다르기 때문에 원점을 world좌표계로 변환한 후 비교합니다.
            auto sp = slot->convertToWorldSpace(Vec2::ZERO);
            auto bp = block->convertToWorldSpace(Vec2::ZERO);
            auto dist = sp.distance(bp);
            if (dist < kImageMinDistance) // 들어갈 수 있는 슬롯 중에서 가장 가까운 것을 고릅니다.
            {
                if (minDist < 0 || minDist > dist)
                {
                    minDist = dist;
                    newSlot = slot;
                }
            }
        }
        
        if (newSlot != nullptr && newSlot->_pair == nullptr && newSlot->getSolutionValue() == block->getValue())
        {
            newSlot->_pair = block;
            block->_pair = newSlot;
            
            // position은 부모좌표계 기준이므로, slot의 위치를 block의 부모좌표계로 변환해서 세팅합니다.
            auto newBP = _gameNode->convertToNodeSpace(newSlot->getParent()->convertToWorldSpace(newSlot->getPosition()));
            block->setPosition(newBP);
        }
        else
        {
            auto blockCenter = block->getPosition();
            if (kTrayOuterRect.containsPoint(blockCenter)) // tray로 돌아갔으면 거기 놔둡니다.
            {
                if (block->_pair)
                {
                    block->_pair->_pair = nullptr;
                    block->_pair = nullptr;
                }
                block->setSnapped(false);
                
            }
            else // 아니면 원래 자리로 돌려보냅니다.
            {
                block->returnToOrigin();
            }
        }
        
        writePlayLog();
        if (isSolved())
            onSolve();
    };
    
    return block;
}

Node* LRDividedTypeReorderingScene::createTextBlock(int index, string text, float scaleFactor)
{
    auto block = TextBlock::create();
    block->initAsBlock(text);
    block->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto blockSize = block->getContentSize();
    
    
    float tx = kTrayArrangeRect.origin.x + (kTrayArrangeRect.size.width/2) + random(-200.f, 200.f);
    //CCLOG("%d", (int)((int)(kTrayArrangeRect.size.height) / _itemVector.size()) * index);
    float ty = 100.f + (kTrayArrangeRect.size.height /_itemVector.size()) * index + random(0.f, 10.f);
    auto pos = Point(tx, ty);
    
    //auto pos = Point(kTrayInnerRect.origin.x+random(0.f + blockSize.width / 2, kTrayInnerSize.width - blockSize.width / 2), random(0.f, kTrayInnerSize.height));
    
    block->setScale(scaleFactor);
    block->setPosition(pos);
    
    // (s) Touch Callback
    block->onCheckTarget = [this, block, blockSize](){
        
        // 들어갈 수 있는 빈칸이 있나 찾아봅니다.
        TextBlock* newSlot = nullptr;
        float minDist = -1;
        
        for (auto slot : _textSlots)
        {
            // 슬롯과 블록의 부모노드가 다르기 때문에 원점을 world좌표계로 변환한 후 비교합니다.
            auto sp = slot->convertToWorldSpace(Vec2::ZERO);
            auto bp = block->convertToWorldSpace(Vec2::ZERO);
            auto dist = sp.distance(bp);
            if (dist < kTextMinDistance) // 들어갈 수 있는 슬롯 중에서 가장 가까운 것을 고릅니다.
            {
                if (minDist < 0 || minDist > dist)
                {
                    minDist = dist;
                    newSlot = slot;
                }
            }
        }
        
        if (newSlot != nullptr && newSlot->_pair == nullptr && newSlot->getSolutionValue() == block->getValue())
        {
            newSlot->_pair = block;
            block->_pair = newSlot;
            
            // position은 부모좌표계 기준이므로, slot의 위치를 block의 부모좌표계로 변환해서 세팅합니다.
            auto newBP = _gameNode->convertToNodeSpace(newSlot->getParent()->convertToWorldSpace(newSlot->getPosition()));
            block->setPosition(newBP);
        }
        else
        {
            auto blockCenter = block->getPosition();
            if (kTrayOuterRect.containsPoint(blockCenter)) // tray로 돌아갔으면 거기 놔둡니다.
            {
                if (block->_pair)
                {
                    block->_pair->_pair = nullptr;
                    block->_pair = nullptr;
                }
                block->setSnapped(false);
                
            }
            else // 아니면 원래 자리로 돌려보냅니다.
            {
                block->returnToOrigin();
            }
        }
        
        if (isSolved())
            onSolve();
    };
    // (e) Touch Callback
    
    return block;
}

void LRDividedTypeReorderingScene::createFixedResources()
{
    auto tray = Sprite::create("lrcomprehension/common/comprehention_filltheblank_tray.png");
    tray->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    tray->setPosition(getContentSize().width / 2, -100.f);
    addChild(tray);
}

bool LRDividedTypeReorderingScene::isSolved()
{
    if (_currentType == GameType::Image)
    {
        for (int i = 0; i < _imageSlots.size(); i++)
        {
            if (_imageSlots[i]->_pair == nullptr)
                return false;
        }
    }
    else
    {
        for (int i = 0; i < _textSlots.size(); i++)
        {
            if (_textSlots[i]->_pair == nullptr)
                return false;
        }
    }
    return true;
}

void LRDividedTypeReorderingScene::onSolve()
{
    if (_comprehensionScene)
    {
        _comprehensionScene->onSolve();
    }
}

void LRDividedTypeReorderingScene::determineItemType()
{
    //_currentType = _itemVector[0].find(".png") != std::string::npos ? GameType::Image : GameType::Text;
}

void LRDividedTypeReorderingScene::drawBlocksByGameType()
{
    if (_currentType == GameType::Image)
    {
        createImageSlots();
        for (auto imagePath : _itemVector) { _gameNode->addChild(createImageBlock(imagePath)); }
    }
    else
    {
        // 기본적인 디자인을 가정으로 들어갈 수 있는 최대 텍스트 아이템 갯수
        int maxItemSize = 4;
        
        float wrapperNodeCorrectionY;
        float scaleFactor;
        
        if (_itemVector.size() > maxItemSize)
        {
            wrapperNodeCorrectionY = -470.f;
            scaleFactor = (float)maxItemSize / (float)_itemVector.size();
        }
        else
        {
            wrapperNodeCorrectionY = -500.f;
            scaleFactor = 1.f;
        }
        
        // 최대 아이템 갯수를 기반으로, 아래의 위의 보정값을 파라미터로 전달함
        createTextSlots(wrapperNodeCorrectionY, scaleFactor);
        int index = 0;
        for (auto text : _itemVector)
        {
            _gameNode->addChild(createTextBlock(index, text, scaleFactor));
            index++;
        }
    }
}

void LRDividedTypeReorderingScene::initData()
{
    _itemVector.clear();
    
    for (int i = 0; i < _comprehensionScene->problemSet.size(); i++)
    {
        if (i == _comprehensionScene->getCurrentProblem())
        {
            _problem = _comprehensionScene->problemSet[i];
            break;
        }
    }
    
    if (TodoUtil::trim(_problem->option1).empty() == false) _itemVector.push_back(_problem->option1);
    if (TodoUtil::trim(_problem->option2).empty() == false) _itemVector.push_back(_problem->option2);
    if (TodoUtil::trim(_problem->option3).empty() == false) _itemVector.push_back(_problem->option3);
    if (TodoUtil::trim(_problem->option4).empty() == false) _itemVector.push_back(_problem->option4);
    
    
    if (_itemVector.size() <= 0)
    {
        NativeAlert::show("Error", "There are no items", "OK");
        return;
    }
}

void LRDividedTypeReorderingScene::writePlayLog()
{
    stringstream userAnswer;
    stringstream correctAnswer;
    userAnswer << "[";
    correctAnswer << "[";

    auto paren = [](const string& s) {
        return string("{") + s + string("}");
    };

    if (_currentType == GameType::Image) {
        for (int i = 0; i < _imageSlots.size(); i++) {
            userAnswer << (_imageSlots[i]->_pair ? paren(_imageSlots[i]->_pair->getValue()) : "None") << ", ";
            correctAnswer << paren(_imageSlots[i]->getSolutionValue()) << ", ";
        }
    }
    else {
        for (int i = 0; i < _textSlots.size(); i++) {
            userAnswer << (_textSlots[i]->_pair ? paren(_textSlots[i]->_pair->getValue()) : "None") << ", ";
            correctAnswer << paren(_textSlots[i]->getSolutionValue()) << ", ";
        }
    }

    userAnswer << "]";
    correctAnswer << "]";

    StrictLogManager::shared()->game_Peek_Answer("ComprehensionTest", makeWorkPath(), userAnswer.str(), correctAnswer.str());
}

string LRDividedTypeReorderingScene::makeWorkPath()
{
    stringstream ss;
    ss << "LRComprehension";
    ss << "/" << _comprehensionScene->getBookName();
    ss << "/" << "ordering";
    ss << "-" << _comprehensionScene->getCurrentProblem();
    return ss.str();
}

END_NS_LR_DIVIDED_TYPE_REORDERING;
