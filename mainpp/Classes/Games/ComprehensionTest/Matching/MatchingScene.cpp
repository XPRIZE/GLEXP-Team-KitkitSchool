//
//  MatchingScene.cpp
//  enumaXprize
//
//  Created by timewalker on 05/12/2016.
//
//

#include "MatchingScene.hpp"
#include "ImageObject.hpp"
#include "TextObject.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"

#define GRAY_1  Color4B(100, 100, 100, 255)
#define GRAY_2  Color4B(130, 130, 130, 255)
#define GRAY_3  Color4B(160, 160, 160, 255)
#define GRAY_4  Color4B(190, 190, 190, 255)
#define GRAY_5  Color4B(220, 220, 220, 255)
#define GRAY_6  Color4B(250, 250, 250, 255)

namespace MatchingSceneSpace
{
    const bool bDebug = false;
    const Size gameSize = Size(2560, 1800);
    const Size imageFrameSize = Size(510, 374);

    const int _kThicknessValue = 9;
    const float kItemGapY = 50.f;
    const float kGapBetweenLeftRight = 250.f;
    const float kWrapperLayerMarginX = 200.f;
    const float kWrapperLayerMarginY = 250.f;
    const float kHeightCorrection = 100.f;
    
    const int kMaxStringLenthForCorrectX = 6;
    
    bool bPlayLineEffectSound = false;
}

using namespace MatchingSceneSpace;

namespace ComprehensionTest
{
    namespace Matching
    {
        ProblemData* _problemData;
        
        MatchingScene::MatchingScene()
        :_bDrag(false)
        {
            
        }
            
        bool MatchingScene::init()
        {
            if (!Layer::init())
            {
                return false;
            }
            return true;
        }

        Layer* MatchingScene::createLayer(ComprehensionScene *parent)
        {
            auto layer = MatchingScene::create();
            layer->_comprehensionScene = parent;
            return layer;
        }
        
        void MatchingScene::onEnter()
        {
            Layer::onEnter();
            
            auto winSize = getContentSize();
            
            // DesignResolution에서 FixedHeight로 세팅해뒀기 때문에 height는 언제나 1800입니다만, width는 바뀔 수 있기 때문에
            // 화면 해상도 변화에 상관없이 고정 해상도를 사용하기 위해 _gameNode를 만들고 그 안에서만 작업합니다.
            _gameNode = Node::create();
            _gameNode->setContentSize(gameSize);
            _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _gameNode->setPosition(Vec2(winSize.width / 2, 0.f));
            addChild(_gameNode);
            
            initData();
            
            createFixedResources();
            string directionContent = LanguageManager::getInstance()->isEnglish() ? "Draw a line to match." : "Chora mstari baina ya jozi.";
            _comprehensionScene->drawQuestionTitle(directionContent, _gameNode);
            
            createLeftLayer();
            createRightLayer();
            
            for (int i = 0; i < _problemData->leftValue.size(); i++)
            {
                auto eachLeftValue = _problemData->leftValue[i];
                auto eachRightValue = _problemData->rightValue[i];
                
                // Left
                if (_problemData->leftType == "image")
                {
                    auto obj = ImageObject::create();
                    obj->location = ObjectLocation::LeftSide;
                    obj->setImage(eachLeftValue.second);
                    obj->id = eachLeftValue.first;
                    obj->drawDot(DotDirection::Right);
                    obj->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    auto pos = Point(_leftLayer->getPosition().x - kGapBetweenLeftRight, _leftLayer->getPosition().y + _leftLayer->getContentSize().height / 2);
                    pos.y -= imageFrameSize.height * i + kItemGapY * i + kHeightCorrection;
                    obj->setPosition(pos);
                    _gameNode->addChild(obj);
                    _imageObjectVector.push_back(obj);
                }
                else
                {
                    // When it is a long string, correct postion x for spacing
                    auto gap = (eachLeftValue.second.length() > kMaxStringLenthForCorrectX ? 90.f : 0.f);
                    
                    auto obj = TextObject::create();
                    obj->location = ObjectLocation::LeftSide;
                    obj->setText(eachLeftValue.second);
                    obj->id = eachLeftValue.first;
                    obj->drawDot(DotDirection::Right);
                    obj->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    auto pos = Point(_leftLayer->getPosition().x - kGapBetweenLeftRight + gap, _leftLayer->getPosition().y + _leftLayer->getContentSize().height / 2);
                    pos.y -= imageFrameSize.height * i + kItemGapY * i + kHeightCorrection;
                    obj->setPosition(pos);
                    _gameNode->addChild(obj);
                    _textObjectVector.push_back(obj);
                }
                
                // Right
                if (_problemData->rightType == "image")
                {
                    auto obj = ImageObject::create();
                    obj->location = ObjectLocation::RightSide;
                    obj->setImage(eachRightValue.second);
                    obj->id = eachRightValue.first;
                    obj->drawDot(DotDirection::Left);
                    obj->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    auto pos = Point(_rightLayer->getPosition().x + kGapBetweenLeftRight, _rightLayer->getPosition().y + _rightLayer->getContentSize().height / 2);
                    pos.y -= imageFrameSize.height * i + kItemGapY * i + kHeightCorrection;
                    obj->setPosition(pos);
                    _gameNode->addChild(obj);
                    _imageObjectVector.push_back(obj);
                }
                else
                {
                    // When it is a long string, correct postion x for spacing
                    auto gap = (eachRightValue.second.length() > kMaxStringLenthForCorrectX ? 90.f : 0.f);
                    
                    auto obj = TextObject::create();
                    obj->location = ObjectLocation::RightSide;
                    obj->setText(eachRightValue.second);
                    obj->id = eachRightValue.first;
                    obj->drawDot(DotDirection::Left);
                    obj->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    auto pos = Point(_rightLayer->getPosition().x + kGapBetweenLeftRight - gap, _rightLayer->getPosition().y + _rightLayer->getContentSize().height / 2);
                    pos.y -= imageFrameSize.height * i + kItemGapY * i + kHeightCorrection;
                    obj->setPosition(pos);
                    _gameNode->addChild(obj);
                    _textObjectVector.push_back(obj);
                }
            }
            
            _drawNodeLine = DrawNode::create();
            _drawNodeCircle = DrawNode::create();
            addChild(_drawNodeLine);
            addChild(_drawNodeCircle);
            
            _eventListener = EventListenerTouchOneByOne::create();
            _eventListener->setEnabled(true);
            _eventListener->setSwallowTouches(true);
            _eventListener->onTouchBegan = CC_CALLBACK_2(MatchingScene::onTouchBegan, this);
            _eventListener->onTouchMoved = CC_CALLBACK_2(MatchingScene::onTouchMoved, this);
            _eventListener->onTouchEnded = CC_CALLBACK_2(MatchingScene::onTouchEnded, this);
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListener, this);
            
            _drawNodeLine->setLineWidth(_kThicknessValue);

        }
        
        void MatchingScene::initData()
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
            
            _problemData = ProblemData::parse(rawData);
        }
        
        void MatchingScene::createLeftLayer()
        {
            _leftLayer = LayerColor::create(Color4B(100, 0, 0, bDebug ? 100 : 0));
            _leftLayer->setContentSize(Size(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height));
            _leftLayer->ignoreAnchorPointForPosition(false);
            _leftLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _leftLayer->setPosition(_gameNode->getContentSize() / 2);
            _gameNode->addChild(_leftLayer);
            
            auto itemSize = _problemData->leftValue.size();
            _leftLayer = LayerColor::create(Color4B(100, 100, 100, bDebug ? 100 : 0));
            _leftLayer->setContentSize(Size(imageFrameSize.width, imageFrameSize.height * itemSize + kItemGapY * (itemSize - 1)));
            _leftLayer->ignoreAnchorPointForPosition(false);
            _leftLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _leftLayer->setPosition(_gameNode->getContentSize().width / 2 - kWrapperLayerMarginX, _gameNode->getContentSize().height / 2 - kWrapperLayerMarginY);
            _gameNode->addChild(_leftLayer);
        }
        
        void MatchingScene::createRightLayer()
        {
            _rightLayer = LayerColor::create(Color4B(0, 100, 0, bDebug ? 100 : 0));
            _rightLayer->setContentSize(Size(_gameNode->getContentSize().width / 2, _gameNode->getContentSize().height));
            _rightLayer->ignoreAnchorPointForPosition(false);
            _rightLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _rightLayer->setPosition(_gameNode->getContentSize() / 2);
            _gameNode->addChild(_rightLayer);
            
            auto itemSize = _problemData->leftValue.size();
            _rightLayer = LayerColor::create(Color4B(100, 100, 100, bDebug ? 100 : 0));
            _rightLayer->setContentSize(Size(imageFrameSize.width, imageFrameSize.height * itemSize + kItemGapY * (itemSize - 1)));
            _rightLayer->ignoreAnchorPointForPosition(false);
            _rightLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _rightLayer->setPosition(_gameNode->getContentSize().width / 2 + kWrapperLayerMarginX, _gameNode->getContentSize().height / 2 - kWrapperLayerMarginY);
            _gameNode->addChild(_rightLayer);
        }
        
        void MatchingScene::createFixedResources()
        {
            
        }
        
        bool MatchingScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
        {
            if (_bDrag) return false;
            
            for (int i = 0; i < _imageObjectVector.size(); i++)
            {
                if (_imageObjectVector[i]->isTouchedIn(touch->getLocation()))
                {
                    if (_imageObjectVector[i]->isClosed) return false;
                    
                    _bDrag = true;
                    _selectedId = _imageObjectVector[i]->id;
                    _selectedLocation = _imageObjectVector[i]->location;
                    _selectedStarPoint = _imageObjectVector[i]->getDotPosition();
                    return true;
                }
            }

            for (int i = 0; i < _textObjectVector.size(); i++)
            {
                if (_textObjectVector[i]->isTouchedIn(touch->getLocation()))
                {
                    if (_textObjectVector[i]->isClosed) return false;
                    
                    _bDrag = true;
                    _selectedId = _textObjectVector[i]->id;
                    _selectedLocation = _textObjectVector[i]->location;
                    _selectedStarPoint = _textObjectVector[i]->getDotPosition();
                    return true;
                }
            }
            return false;
        }

        void MatchingScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
        {
            if (bPlayLineEffectSound == false)
            {
                GameSoundManager::getInstance()->stopAllEffects();
                GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/Matching/Sounds/LineStart.m4a");
            }
            bPlayLineEffectSound = true;
            
            _drawNodeLine->clear();
            _drawNodeLine->setLineWidth(_kThicknessValue);
            _drawNodeCircle->clear();
            _drawNodeLine->drawLine(_selectedStarPoint, touch->getLocation(), Color4F(Color4B(242, 190, 10, 255)));
            _drawNodeCircle->drawSolidCircle(_selectedStarPoint, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
            _drawNodeCircle->drawSolidCircle(touch->getLocation(), 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
        }

        void MatchingScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
        {
            bPlayLineEffectSound = false;
            
            for (int i = 0; i < _imageObjectVector.size(); i++)
            {
                if (_imageObjectVector[i]->isTouchedIn(touch->getLocation()))
                {
                    if (_selectedId == _imageObjectVector[i]->id && _selectedLocation == _imageObjectVector[i]->location)
                    {
                        canceled();
                        return;
                    }
                    
                    if (_selectedId == _imageObjectVector[i]->id && _selectedLocation != _imageObjectVector[i]->location)
                    {
                        matched(_imageObjectVector[i]->getDotPosition());
                        return;
                    }
                }
            }
            
            for (int i = 0; i < _textObjectVector.size(); i++)
            {
                if (_textObjectVector[i]->isTouchedIn(touch->getLocation()))
                {
                    if (_selectedId == _textObjectVector[i]->id && _selectedLocation == _textObjectVector[i]->location)
                    {
                        canceled();
                        return;
                    }
                    
                    if (_selectedId == _textObjectVector[i]->id && _selectedLocation != _textObjectVector[i]->location)
                    {
                        matched(_textObjectVector[i]->getDotPosition());
                        return;
                    }
                }
            }
            
            
            _bDrag = false;
            _drawNodeLine->clear();
            _drawNodeCircle->clear();
        }

        void MatchingScene::canceled()
        {
            CCLOG("[MatchingScene::canceled]");
            GameSoundManager::getInstance()->stopAllEffects();
            GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/Matching/Sounds/LineBack.m4a");
            _bDrag = false;
            _drawNodeLine->clear();
            _drawNodeCircle->clear();
        }

        void MatchingScene::matched(Vec2 endPoint)
        {
            CCLOG("[MatchingScene::matched]");
            GameSoundManager::getInstance()->stopAllEffects();
            GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/Matching/Sounds/Boom.m4a");
            auto matchedLine = DrawNode::create();
            matchedLine->setLineWidth(_kThicknessValue);
            matchedLine->drawLine(_selectedStarPoint, endPoint, Color4F(Color4B(242, 190, 10, 255)));
            addChild(matchedLine);
            
            auto matchedCircle = DrawNode::create();
            matchedCircle->setLineWidth(_kThicknessValue);
            matchedCircle->drawSolidCircle(_selectedStarPoint, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
            matchedCircle->drawSolidCircle(endPoint, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
            addChild(matchedCircle);
            
            for (int i = 0; i < _imageObjectVector.size(); i++)
            {
                if (_selectedId == _imageObjectVector[i]->id)
                    _imageObjectVector[i]->isClosed = true;
            }

            for (int i = 0; i < _textObjectVector.size(); i++)
            {
                if (_selectedId == _textObjectVector[i]->id)
                    _textObjectVector[i]->isClosed = true;
            }
            
            _bDrag = false;
            _drawNodeLine->clear();
            _drawNodeCircle->clear();
            
            if (isSolved())
            {
                onSolve();
            }
        }

        bool MatchingScene::isSolved()
        {
            for (int i = 0; i < _imageObjectVector.size(); i++)
            {
                if (_imageObjectVector[i]->isClosed == false)
                    return false;
            }
            
            for (int i = 0; i < _textObjectVector.size(); i++)
            {
                if (_textObjectVector[i]->isClosed == false)
                    return false;
            }
            
            return true;
        }

        void MatchingScene::onSolve()
        {
            if (_comprehensionScene) {
                _comprehensionScene->onSolve();
            }
        }

    }  // namespace Matching
}  // namespace ComprehensionTest
