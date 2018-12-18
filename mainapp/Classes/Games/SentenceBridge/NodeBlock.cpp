//
//  NodeBlock.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//


#include "NodeBlock.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeBlock::init()
{
	if (!Node::init()) return false;
	return true;
}

void NodeBlock::removeObjects()
{
	_spriteBlock->removeAllChildren();

	_spriteShadhow = nullptr;
	_spriteBlock = nullptr;
	_labelLetter = nullptr;

	onTouchBegan = nullptr;
	onTouchMoved = nullptr;
	onTouchEnded = nullptr;
    onTouchEnded_cancel = nullptr;

	removeAllChildren();
}

void NodeBlock::createBlock(string strLetter)
{
	/*
	if (LanguageManager::getInstance()->getCurrentLanguageTag() == "sw-TZ")
	{
		if (strLetter == "C")
		{
			strLetter = "Ch";
		}
		else if (strLetter == "c")
		{
			strLetter = "ch";
		}
	}
	*/

	_strLetter = strLetter;
	_spriteShadhow = nullptr;
	_spriteBlock = nullptr;
	_labelLetter = nullptr;

	onTouchBegan = nullptr;
	onTouchMoved = nullptr;
	onTouchEnded = nullptr;
    onTouchEnded_cancel = nullptr;
    
    _alpha = 1.0f;

	//setSoundFileName();

	string strSpriteBlock = "block-general.png";
	string strSpriteShadhow = "block-general-shadow.png";
	if (!isalpha(_strLetter[0]))
	{
		strSpriteBlock = "blocks-etc.png";
		strSpriteShadhow = "blocks-etc-shadow.png";
	}

	_spriteShadhow = Sprite::create(resourcePath + strSpriteShadhow);
	_spriteShadhow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteShadhow->setPosition(Vec2::ZERO);
	_spriteShadhow->setOpacity(255 * 0.2);
	addChild(_spriteShadhow);

	_spriteBlock = Sprite::create(resourcePath + strSpriteBlock);
	_spriteBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_spriteBlock->setPosition(Vec2::ZERO);
	addChild(_spriteBlock);

	Size blockSize = _spriteBlock->getContentSize();

	_labelLetter = TodoUtil::createLabel(_strLetter, fontSize, Size::ZERO, fontName, Color4B(245, 246, 211, 255), TextHAlignment::CENTER);
	_labelLetter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_labelLetter->setPosition(blockSize.width/2, blockSize.height/2 + 25);
	_spriteBlock->addChild(_labelLetter);

    if (isupper(_strLetter[0]))
    {
        _labelLetter->setColor(Color3B(255, 204, 23));
        _labelLetter->setScaleX(0.8);
    }
    
	if (_strLetter.size() > 1)
	{
		_labelLetter->setScaleX(0.75);
	}


	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_alpha > 0.9f)
        {
            auto pos = convertToNodeSpace(T->getLocation());

            Rect rrr = _spriteBlock->getBoundingBox();
            if (_spriteBlock->getBoundingBox().containsPoint(pos)) {
                if (onTouchBegan) {
                    if (onTouchBegan()) {
                        _spriteBlock->setGlobalZOrder(10);
                        _labelLetter->setGlobalZOrder(11);

                        //_spriteBlock->runAction(MoveTo::create(0.05f, Vec2(-10, 10)));
                        _spriteBlock->setPosition(-10, 10);
                        playLetterSound();
                        return true;
                    }
                }
                return false;
            }
            return false;
        }
	};

	listener->onTouchMoved = [this](Touch* touch, Event* event) {
		//auto P = getParent();
		auto pl = convertToNodeSpace(touch->getPreviousLocation());
		auto cl = convertToNodeSpace(touch->getLocation());
		auto delta = cl - pl;
		_spriteBlock->setPosition(_spriteBlock->getPosition() + delta);
		if (onTouchMoved) onTouchMoved(touch);

	};

	listener->onTouchEnded = [this](Touch* touch, Event* E) {
		bool bShowAnim = true;

		if (_spriteBlock->getPosition().distance(Vec2::ZERO) >= 200) {
			if (onTouchEnded) {
				bShowAnim = !onTouchEnded(touch);
			}
		}
        else {
            if (onTouchEnded_cancel) {
                onTouchEnded_cancel(touch);
            }
        }
		if (bShowAnim) {
			_spriteBlock->runAction(
				Sequence::create(
					MoveTo::create(0.1f, Vec2::ZERO),
					CallFunc::create([this]() {
						_spriteBlock->setGlobalZOrder(0);
						_labelLetter->setGlobalZOrder(0);
					}),
					nullptr
				)
			);
		}
		else {
			/*
			_spriteBlock->setPosition(Vec2::ZERO);
			_spriteBlock->setGlobalZOrder(0);
			_labelLetter->setGlobalZOrder(0);
			*/
		}
	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void NodeBlock::changeLetter(string strLetter)
{
	/*
	if (LanguageManager::getInstance()->getCurrentLanguageTag() == "sw-TZ")
	{
		if (strLetter == "C")
		{
			strLetter = "Ch";
		}
		else if (strLetter == "c")
		{
			strLetter = "ch";
		}
	}
	*/

	_strLetter = strLetter;
	_labelLetter->setString(_strLetter);
    
    if (isupper(_strLetter[0]))
    {
        _labelLetter->setColor(Color3B(255, 204, 23));
        _labelLetter->setScaleX(0.8);
    }
    else
    {
        _labelLetter->setColor(Color3B(245, 246, 211));
        _labelLetter->setScaleX(1);
    }
}

void NodeBlock::blockMoveToWorldPosition(float duration, Vec2 worldPos)
{
	Vec2 nodePos = convertToNodeSpace(worldPos);
	_spriteBlock->runAction(MoveTo::create(duration, nodePos));
}

void NodeBlock::blockResetPosition()
{
	_spriteBlock->setPosition(Vec2::ZERO);
	_spriteBlock->setGlobalZOrder(0);
	_labelLetter->setGlobalZOrder(0);

	/*
	_spriteBlock->runAction(
		Sequence::create(
			MoveTo::create(0.1f, Vec2::ZERO),
			CallFunc::create([this]() {
		_spriteBlock->setGlobalZOrder(0);
		_labelLetter->setGlobalZOrder(0);
	}),
			nullptr
		)
	);
	*/
}

Vec2 NodeBlock::getBlockWorldPosition()
{
	return _spriteBlock->convertToWorldSpaceAR(Vec2::ZERO);
}

void NodeBlock::showAlphabet(bool bHasAlphabet)
{
    if (!isalpha(_strLetter[0]))
    {
        return;
    }
    
    _alpha = 1.0f;
    if (!bHasAlphabet)
    {
        _alpha = 0.15f;
    }
    
    _spriteBlock->setOpacity(255*_alpha);
    _labelLetter->setOpacity(255*_alpha);
}
/*
void NodeBlock::setSoundFileName()
{
	_strSoundFileName = "";
	if (_strLetter == " ") _strSoundFileName = "space";
	else if (_strLetter == "'") _strSoundFileName = "yes";
	else if (_strLetter == "?") _strSoundFileName = "question";
	else if (_strLetter == ".") _strSoundFileName = "dot";
	else _strSoundFileName = _strLetter;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_strSoundFileName += ".mp3";
#else
	_strSoundFileName += ".m4a";
#endif
}
*/

void NodeBlock::playLetterSound()
{
	//GameSoundManager::getInstance()->playEffectSound("sentencebridge/sound/" + _strSoundFileName);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/matrix_clickblock.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/matrix_clickblock.m4a");
#endif
}
END_NS_SENTENCEBRIDGE;
